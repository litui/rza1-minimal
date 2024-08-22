core_includes = include_directories('.')

core_sources = files(
    'main.c',
)

core_args = []

firmware_target = get_option('firmware_target')
if firmware_target == 'gr-peach'
    subdir('native-rza1')
elif firmware_target == 'gr-lychee'
    subdir('native-rza1')
endif

core = declare_dependency(
    compile_args: core_args,
    include_directories: core_includes,
    sources: [core_sources]
)