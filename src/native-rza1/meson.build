native_rza1_includes = include_directories(
    'common/inc/iobitmasks',
    'common/inc/iodefines',
    'common/inc',
    'common/src/drivers/bsc/inc',
    'common/src/drivers/intc/inc',
    'common/src/drivers/ostm/inc',
    'common/src/drivers/scif_uart/inc',
    'common',
    '.',
)

native_rza1_generic_sources = files(
    'asm/initsect.S',
    'asm/irq.S',
    'asm/irqfiq_handler.S',
    'asm/l1_cache_init.S',
    'asm/l1_cache_operation.S',
    'asm/reset_handler.S',
    'asm/ttb_init.S',
    'asm/vbar_init.S',
    'asm/vector_mirrortable.S',
    'asm/vector_table.S',
    'asm/vfp_init_asm.S',
    'common/src/common_settings/cache.c',
    'common/src/common_settings/l2_cache_init.c',
    'common/src/common_settings/resetprg.c', # Contains SystemInit
    'common/src/common_settings/rza_io_regrw.c',
    'common/src/drivers/bsc/bsc_driver/bsc.c',
    'common/src/drivers/bsc/userdef/bsc_userdef.c',
    'common/src/drivers/intc/intc_driver/intc.c',
    'common/src/drivers/intc/intc_driver/intc_handler.c',
    'common/src/drivers/intc/userdef/intc_userdef.c',
    'common/src/drivers/ostm/ostm_driver/ostm.c',
    'common/src/drivers/ostm/userdef/ostm_userdef.c',
    'common/src/drivers/scif_uart/scif_uart_driver/scif_uart.c',
    'common/src/drivers/scif_uart/userdef/scif_uart_userdef.c',
    'common/src/sections.c',
    'gpio.c',
)

native_rza1_gr_peach_sources = files(
    'gr-peach/peripheral_init_basic.c',
    'gr-peach/port_init.c',
    'gr-peach/stb_init.c',
    'gr-peach/siochar.c',
)

native_rza1_gr_lychee_sources = files()

firmware_target = get_option('firmware_target')
if firmware_target == 'gr-peach'
    native_rza1_linker_file = 'gr-peach.ld'
    native_rza1_sources = [
        native_rza1_generic_sources,
        native_rza1_gr_peach_sources
    ]
elif firmware_target == 'gr-lychee'
    native_rza1_linker_file = 'gr-lychee.ld'
    native_rza1_sources = [
        native_rza1_generic_sources,
        native_rza1_gr_lychee_sources
    ]
endif

native_rza1_args = [
    '-mcpu=cortex-a9',
    '-mfpu=neon',
    '-mthumb',
]

native_rza1_link_args = [
    '-mthumb',
    '-lc',
    '-lrdimon',
    '-nostartfiles',
    '--specs=rdimon.specs',
    '--entry=start',
	'-L@0@'.format(meson.current_source_dir()),
	'-T@0@'.format(native_rza1_linker_file),
]

native_rza1 = declare_dependency(
    include_directories: native_rza1_includes,
    sources: native_rza1_sources,
    compile_args: native_rza1_args,
    link_args: native_rza1_link_args
)