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


stb_global_conflict = find_program('tests/STB_GLOBAL_CONFLICT/tester.sh')
    test('stb_global_conflict', stb_global_conflict)
