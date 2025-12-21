project('prometheus', 'c')

deps = [
    'src/misc.c', 'src/parse_args.c',
    'src/dump_elf_header.c', 'src/dump_section_header.c',
    'src/dump_program_header.c'
    ]

bin = executable('dump', 'src/main.c', deps)

run_target(
    'run',
    command: bin
)

'''
run_target('run',
    command : 'scripts/run.sh')
'''
