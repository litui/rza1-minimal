/*
 * SDCard Driver for Renesas RSPI
 * Used https://github.com/adafruit/Adafruit_CircuitPython_SD/blob/main/adafruit_sdcard.py (MIT license) as a rough guide
 * alongside SPI and SD cards lecture notes by Chistinel Ababei https://www.dejazzer.com/ee379/lecture_notes/lec12_sd_card.pdf
*/

#include <stdio.h>
#include "sdcard.h"
#include "board.h"
#include "gpio.h"
#include "devdrv_rspi.h"

#define SDCARD_COMMAND_BYTES 6U
#define SDCARD_OUT_BUFFER_SIZE 512U /* 32 byte increments for cache alignment */
#define SDCARD_IN_BUFFER_SIZE 512U /* 32 byte increments for cache alignment */
#define SDCARD_RESPONSE_TIMEOUT 200U

#define SDCARD_FILL            0xffU
#define SDCARD_IDLE_STATE      0b00000001U
#define SDCARD_ERASE_RESET     0b00000010U
#define SDCARD_ILLEGAL_CMD     0b00000100U
#define SDCARD_CRC_ERROR       0b00001000U
#define SDCARD_ERASE_SEQ_ERROR 0b00010000U
#define SDCARD_ADDRESS_ERROR   0b00100000U
#define SDCARD_PARAM_ERROR     0b01000000U

#define SDCARD_CMD0_RETURN_BYTES  1
#define SDCARD_CMD8_RETURN_BYTES  5
#define SDCARD_CMD41_RETURN_BYTES 5
#define SDCARD_CMD55_RETURN_BYTES 1
#define SDCARD_CMD58_RETURN_BYTES 5

/* Aligning to 32 byte for cache alignment */
static uint8_t sdcard_input_buffer[SDCARD_IN_BUFFER_SIZE] __attribute__((section ("BSS_SDCARD_BUFFER")));

typedef struct sdcard_state {
    bool spi_initialized;
    bool card_inserted;
    uint16_t addressing;
} sdcard_state_t;

static sdcard_state_t state = {
    .spi_initialized = false,
    .card_inserted = false,
    .addressing = 512
};

static void send_fill(uint32_t bytes);
static void cd_high();
static void cd_low();
static uint8_t send_cmd(uint8_t cmd, uint32_t arg, uint8_t *receive_buffer, size_t length);
static uint8_t get_crc7(uint8_t *buffer, size_t length);
static bool send_init_v1();
static bool send_init_v2();
static void wait_for_ready();

void sdcard_init()
{
    if (!state.spi_initialized) {
        printf("Initializing SDCard SPI Bus\n");

        /* Initialize SDCard-related GPIO */
        init_gpio_as_alt(R_SDCARD_CLK_PORT, R_SDCARD_CLK_PIN, R_SDCARD_CLK_MUX, false);
        init_gpio_as_alt(R_SDCARD_CMD_PORT, R_SDCARD_CMD_PIN, R_SDCARD_CMD_MUX, false);
        init_gpio_as_alt(R_SDCARD_DAT0_PORT, R_SDCARD_DAT0_PIN, R_SDCARD_DAT0_MUX, false);
        init_gpio_as_alt(R_SDCARD_DAT3_PORT, R_SDCARD_DAT3_PIN, R_SDCARD_DAT3_MUX, false);
        init_gpio_as_output(R_SDCARD_CD_PORT, R_SDCARD_CD_PIN);

        /* Set CD high */
        cd_high();

        /* Start up SPI channel */
        R_RSPI_Init(R_SDCARD_SPI_CHANNEL);

        state.spi_initialized = true;
        state.card_inserted = false;
        state.addressing = 512;
    }

    send_fill(74);

    /* CMD0 Reset (put card into SPI mode) */
    uint8_t result = send_cmd(0, 0, sdcard_input_buffer, SDCARD_CMD0_RETURN_BYTES);
    if (result == SDCARD_IDLE_STATE) {
        printf("Sent CMD0 successfully\n");
        state.card_inserted = true;
    } else {
        printf("Failed to initialize SD card driver\n");
        return;
    }

    /* CMD8 Check card version */
    result = send_cmd(8, 0x1aa, sdcard_input_buffer, SDCARD_CMD8_RETURN_BYTES);
    if (result == SDCARD_IDLE_STATE) {
        /* Only newer cards should make it here */
        send_init_v2();
    } else if (result == (SDCARD_IDLE_STATE | SDCARD_ILLEGAL_CMD)) {
        /* Older cards should make it here */
        send_init_v1();
    } else {
        printf("Failed to determine SD card version\n");
        return;
    }

    /* CMD9 get card-specific data */
    result = send_cmd(9, 0, sdcard_input_buffer, 17);
    if (result != 0) {
        printf("Failed to get number of sectors.\n");
    }
}

static bool send_init_v1()
{
    send_cmd(55, 0, sdcard_input_buffer, 5);
    if (send_cmd(41, 0, sdcard_input_buffer, 5) == 0)
        return true;
    return false;
}

static bool send_init_v2()
{
    uint8_t dummy_buf[5];
    send_cmd(58, 0, sdcard_input_buffer, SDCARD_CMD58_RETURN_BYTES);
    send_cmd(55, 0, dummy_buf, SDCARD_CMD55_RETURN_BYTES);
    if (send_cmd(41, 0x40000000U, dummy_buf, SDCARD_CMD41_RETURN_BYTES) == 0) {
        uint8_t result = send_cmd(58, 0, sdcard_input_buffer, SDCARD_CMD58_RETURN_BYTES);
        if (result & 0x40) {
            state.addressing = 1;
            return false;
        }
        return true;
    }
    return false;
}

static void wait_for_ready()
{
    R_RSPI_Open(R_SDCARD_SPI_CHANNEL);
    for (uint32_t i = 0; i < SDCARD_RESPONSE_TIMEOUT; i++) {
        R_RSPI_ResetBuffer(R_SDCARD_SPI_CHANNEL);
        R_RSPI_Read(R_SDCARD_SPI_CHANNEL, sdcard_input_buffer, 1, RSPI_RXTRG_01_BYTE);
        if (sdcard_input_buffer[0] == SDCARD_FILL)
            break;
    }
    R_RSPI_Close(R_SDCARD_SPI_CHANNEL);
}


static void cd_high()
{
    set_gpio(R_SDCARD_CD_PORT, R_SDCARD_CD_PIN);
}

static void cd_low()
{
    clear_gpio(R_SDCARD_CD_PORT, R_SDCARD_CD_PIN);
}

static void send_fill(uint32_t bytes)
{
    R_RSPI_Open(R_SDCARD_SPI_CHANNEL);
    /* Iterations set to 74; should be at least 74 clock cycles  */
    for (uint32_t i = 0; i < bytes; i++) {
        uint8_t filler = SDCARD_FILL;
        /* Send each byte immediately */
        R_RSPI_ResetBuffer(R_SDCARD_SPI_CHANNEL);
        R_RSPI_Write(R_SDCARD_SPI_CHANNEL, &filler, 1, RSPI_TXTRG_00_BYTE);
        R_RSPI_WaitTransmitEnd(R_SDCARD_SPI_CHANNEL);
    }

    R_RSPI_Close(R_SDCARD_SPI_CHANNEL);
}

static uint8_t send_cmd(uint8_t cmd, uint32_t arg, uint8_t *receive_buffer, size_t length)
{
    uint8_t buffer[6];

    buffer[0] = 0x40 | cmd;
    buffer[1] = (arg >> 24) & 0xff;
    buffer[2] = (arg >> 16) & 0xff;
    buffer[3] = (arg >> 8) & 0xff;
    buffer[4] = arg & 0xff;
    buffer[5] = get_crc7(buffer, 5);

    printf("Sending cmd: %#04x, %#04x, %#04x, %#04x, %#04x, %#04x\n", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5]);

    wait_for_ready();

    R_RSPI_Open(R_SDCARD_SPI_CHANNEL);
    cd_low();

    R_RSPI_ResetBuffer(R_SDCARD_SPI_CHANNEL);
    R_RSPI_Write(R_SDCARD_SPI_CHANNEL, buffer, 6, RSPI_TXTRG_00_BYTE);
    R_RSPI_WaitTransmitEnd(R_SDCARD_SPI_CHANNEL);

    for (uint32_t i = 0; i < SDCARD_RESPONSE_TIMEOUT; i++) {
        R_RSPI_ResetBuffer(R_SDCARD_SPI_CHANNEL);
        /* Receive only one byte on the first go-round until we find a byte that is not 0xFF */
        R_RSPI_Read(R_SDCARD_SPI_CHANNEL, receive_buffer, 1, RSPI_RXTRG_01_BYTE);
        R_RSPI_WaitTransmitEnd(R_SDCARD_SPI_CHANNEL);
        if (!(receive_buffer[0] & 0x80)) {
            if (length > 1) {
                /* Receive remainder of buffer */
                R_RSPI_Read(R_SDCARD_SPI_CHANNEL, receive_buffer+1, length - 1, RSPI_RXTRG_01_BYTE);
            }
            break;
        }
    }

    cd_high();
    R_RSPI_Close(R_SDCARD_SPI_CHANNEL);
    printf("In buffer: %#04x\n", receive_buffer[0]);
    return receive_buffer[0];
}

/*
 * CRC7 algorithm via https://github.com/LonelyWolf/stm32/blob/master/stm32l-dosfs/sdcard.c 
 * licensed under public domain
 */
static uint8_t get_crc7(uint8_t *buffer, size_t length)
{
    const uint8_t g = 0x89;

    uint8_t crc = 0;
    for (uint8_t i = 0; i < length; i++) {
        crc ^= buffer[i];
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x80) crc ^= g;
            crc <<= 1;
        }
    }
    return crc | 1;
}