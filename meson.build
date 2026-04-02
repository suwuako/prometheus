project('prometheus', 'c')

deps = [
    'src/misc.c', 'src/parse_args.c',
    'src/dump_elf_header.c', 'src/dump_section_header.c',
    'src/dump_program_header.c', 'src/hashmap.c',
    'src/dump_symbol_table.c', 'src/objmeta.c',
    'src/link.c', 'src/linkedlist.c',
    ]

bin = executable('prom', 'src/main.c', deps)

run_target(
    'run',
    command: bin
)


'''
tests (yuck!)
'''
test(
    'stb_global_conflict', 
    find_program('python3'), 
    args : [
        files('tests/STB_GLOBAL_CONFLICT/tester.py'), bin
    ],
    depends: [bin],
    workdir: meson.project_source_root() / 'tests/STB_GLOBAL_CONFLICT'
)

test(
    'stb_global_noconflict', 
    find_program('python3'), 
    args : [
        files('tests/STB_GLOBAL_NOCONFLICT/tester.py'), bin
    ],
    depends: [bin],
    workdir: meson.project_source_root() / 'tests/STB_GLOBAL_NOCONFLICT'
)
