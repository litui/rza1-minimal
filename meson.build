project('rza1-minimal', 'c',
  version : '0.1',
  default_options : [
    'warning_level=2',
    'c_std=c11',
    # 'optimization=s',
    'debug=true',
    'werror=false'
  ]
)

# version = vcs_tag(
# 	command: ['git', 'describe', '--always', '--dirty', '--tags'],
# 	input: 'src/include/version.h.in',
# 	output: 'version.h',
# 	fallback: meson.project_version(),
# )

# Import the filesystem module
fs = import('fs')

cc_host = meson.get_compiler('c')

# Project wide flags
extended_warnings = [
	'-Warith-conversion',
	'-w24244', # 'conversion' conversion from 'type1' to 'type2', possible loss of data (integer version)
	'-Wbad-function-cast',
	# '-Wcast-align=strict',
	'-Wcast-function-type',
	# '-Wcast-qual',
	# '-Wconversion',
	'-Wdangling-else',
	'-Wdouble-promotion',
	'-Wduplicated-branches',
	'-w24754', # Conversion rules for arithmetic operations in a comparison mean that one branch cannot be executed.
	'-Wfloat-conversion',
	# '-Wformat-overflow=2',
	# '-Wformat-signedness',
	'-Wformat-truncation',
	'-Wformat=2',
	'-w24774', # ‘<function>’ : format string expected in argument <position> is not a string literal
	'-w24777', #‘<function>’ : format string ‘<format-string>’ requires an argument of type ‘<type>’, but variadic argument <position> has type ‘<type>’ 
	'-Wimplicit-fallthrough',
	'-Wmaybe-uninitialized',
	'-w24701', # Potentially uninitialized local variable 'name' used
	'-w24703', # Potentially uninitialized local pointer variable 'name' used
	'-Wmissing-attributes',
	'-Wmissing-braces',
	'-Wno-char-subscripts',
	'-Wnull-dereference',
	# '-Wpacked',
	'-Wredundant-decls',
	'-Wreturn-type',
	'-w24013', # 'function' undefined; assuming extern returning int
	'-Wsequence-point',
	'-Wshadow=local',
	'-w24456', # declaration of 'identifier' hides previous local declaration
	'-w24457', # declaration of 'identifier' hides function parameter
	# '-Wsign-conversion',
	# '-Wstack-protector',
	'-Wstrict-aliasing',
	'-Wstrict-overflow=3',
	'-Wstring-compare',
	'-Wstringop-overflow',
	'-Wunknown-pragmas', # MSVC's C4081 and it's a level 1 warning
	'-Wunsafe-loop-optimizations',
	'-Wunsuffixed-float-constants',
	'-Wunused-const-variable=2',
	'-w24189', # 'identifier' : local variable is initialized but not referenced
	'-Wunused-local-typedefs',
	'-Wunused',
	'-w24101', # 'identifier' : unreferenced local variable
	'-Wvla-parameter',
	'-Wvla',
]
add_project_arguments(
	cc_host.get_supported_arguments(extended_warnings),
	language: 'c',
)

subdir('src')

add_project_link_arguments('-Wl,--print-memory-usage', language: 'c')

# System binary utilities
size = find_program('size')
objcopy = find_program('objcopy')

firmware_target = get_option('firmware_target')

is_firmware_build = firmware_target != ''

if is_firmware_build
  base_name = 'main_@0@'.format(firmware_target).to_lower().underscorify()
  elf_file = executable(
    f'@base_name@_firmware',
    name_suffix: 'elf',
    dependencies: [core, native_rza1]
  )
  alias_target('elf', elf_file)

  # Firmware binary and hex files
  bin_file = custom_target(
    output: fs.replace_suffix(elf_file.name(), '.bin'),
    input: elf_file,
    command: [objcopy, ['-O', 'binary', '@INPUT@', '@OUTPUT@']],
    depends: elf_file,
    build_by_default: true,
  )
  alias_target('bin', bin_file)

  hex_file = custom_target(
    output: fs.replace_suffix(elf_file.name(), '.hex'),
    input: elf_file,
    command: [objcopy, ['-O', 'ihex', '@INPUT@', '@OUTPUT@']],
    depends: elf_file,
  )
  alias_target('hex', hex_file)

  # Firmware size report
  run_target(
    'size',
    command: [size, elf_file.full_path(), '-B'],
    depends: elf_file,
  )
endif

summary(
	{
		'Building Firmware': is_firmware_build,
	},
	bool_yn: true,
	section: 'RZA1 Minimal',
)
