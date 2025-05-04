project('prometheus', 'c')
subdir('src')
bin = executable('prom', 'src/main.c', link_with: linklib)

run_target(
    'run',
    command: bin
)
