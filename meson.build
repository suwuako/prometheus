project('prometheus', 'c')
subdir('src')
bin = executable('prometheus', 'src/main.c', link_with: [ headerlib, objdumplib ])

run_target(
    'run',
    command: bin
)
