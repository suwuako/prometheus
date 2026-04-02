#!/bin/python3

import subprocess
import os
import sys

linker_bin = sys.argv[1]

def test_prom(prom_path: str, bin_path: str, cwd: str):
    result = subprocess.run(
        [prom_path, bin_path],
        capture_output=True, text=True,
        cwd=cwd
    )
    if result.returncode == 0:
        print(f"failed to identify {bin_path} is not ET_REL")
        print(result.stdout)
        print(result.stderr)
        exit(1)


def main():
    cwd = os.path.dirname(os.path.abspath(__file__))

    result = subprocess.run(
        ["make", "all", "-C", cwd],
        capture_output = True, text = True
    )

    if result.returncode != 0:
        print("make failed:", result.stderr)
        sys.exit(1)

    # checking prom against binaries

    for i in range(1,4):
        test_prom(linker_bin, f"program{i}.out", cwd)

    print("pass")


main()
