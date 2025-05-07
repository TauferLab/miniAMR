#!/usr/bin/env python3
import os
import sys
from itertools import zip_longest

def compare_byte_mismatches(path1, path2, chunk_size=1024*1024):
    """
    Compare two files byte-by-byte in streaming chunks.
    Returns (total_bytes_compared, num_mismatched_bytes).
    """
    total = 0
    mismatches = 0
    with open(path1, "rb") as f1, open(path2, "rb") as f2:
        while True:
            b1 = f1.read(chunk_size)
            b2 = f2.read(chunk_size)
            if not b1 and not b2:
                break
            for byte1, byte2 in zip_longest(b1, b2, fillvalue=0):
                total += 1
                if byte1 != byte2:
                    mismatches += 1
    return total, mismatches

def main(dir1, dir2):
    files1 = sorted(f for f in os.listdir(dir1) if f.endswith(".bin"))
    total_bytes = 0
    total_diffs = 0

    print(f"Comparing `{dir1}` → `{dir2}`")
    print("-" * 60)
    for fname in files1:
        path1 = os.path.join(dir1, fname)
        path2 = os.path.join(dir2, fname)
        if not os.path.exists(path2):
            print(f"[!] Skipping {fname}: not found in {dir2}")
            continue

        t, d = compare_byte_mismatches(path1, path2)
        total_bytes += t
        total_diffs += d
        pct = (d / t * 100) if t else 0
        print(f"{fname:30s}  {d:8d} differing bytes  ({pct:6.3f}%)")

    overall_pct = (total_diffs / total_bytes * 100) if total_bytes else 0
    print("-" * 60)
    print(f"{'OVERALL':30s}  {total_diffs:8d} differing bytes  ({overall_pct:6.3f}%)")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <nondeterminism_test_1> <nondeterminism_test_2>")
        sys.exit(1)
    main(sys.argv[1], sys.argv[2])
