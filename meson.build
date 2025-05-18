project('prometheus', 'c')
subdir('src')
bin = executable('prometheus', 'src/main.c', link_with: linklib)

run_target(
    'run',
    command: bin
)
