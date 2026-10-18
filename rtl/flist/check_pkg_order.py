#!/usr/bin/env python3
"""Check that, in a .f file, every package is compiled before its first use."""
import os, re, sys
root = os.environ.get("TITAN_ROOT", os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
seen, bad = set(), 0
files = []
def read(f):
    for line in open(f):
        line = line.strip().replace("${TITAN_ROOT}", root)
        if line.startswith("-f "):
            read(line[3:].strip())
        elif line.endswith(".sv") and not line.startswith("//"):
            files.append(line)
for f in sys.argv[1:]:
    read(f)
pkgs = {}
for p in files:
    t = open(p, errors="ignore").read()
    for m in re.findall(r"^\s*package\s+(\w+)", t, re.M): pkgs[m] = p
for p in files:
    t = re.sub(r"//[^\n]*", "", open(p, errors="ignore").read())
    t = re.sub(r"^\s*`define[^\n]*", "", t, flags=re.M)   # macro bodies expand later
    used = set(re.findall(r"\b(\w+)::", t)) | set(re.findall(r"\bimport\s+(\w+)\s*::", t))
    own = set(re.findall(r"^\s*package\s+(\w+)", t, re.M))
    for u in sorted((used & set(pkgs)) - own - seen):
        print(f"ORDER: {os.path.relpath(p, root)} uses {u} before it is compiled ({os.path.relpath(pkgs[u], root)})"); bad += 1
    seen |= own
print(f"{len(files)} files, {len(pkgs)} packages, {bad} ordering problems")
sys.exit(1 if bad else 0)
