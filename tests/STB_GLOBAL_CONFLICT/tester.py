#!/bin/python3

import subprocess
import os
import sys

linker_bin = sys.argv[1]

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
    result = subprocess.run(
        [linker_bin, "one.o", "two.o"],
        capture_output=True, text=True,
        cwd=cwd
    )
    if result.returncode == 0:
        print("failed to correctly identify conflicting STB_GLOBALS")
        print(result.stdout)
        print(result.stderr)

        exit(1)
    print("pass")


main()
