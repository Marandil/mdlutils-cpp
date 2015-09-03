#!/usr/bin/env python3

# Generates 3 * TESTS random ranges along with their expected contents

TESTS=500

import random

print("\t\tpythonRRT = {")

def gen(minse, maxse, mino, maxo):
    s = random.randrange(minse, maxse)
    e = random.randrange(minse, maxse)
    o = random.randrange(mino, maxo)
    if o == 0: return gen(minse, maxse, mino, maxo)
    return "\t\t\t\t\t\t{{" + str(s) + ", " + str(e) + ", " + str(o) + "}, {" + ", ".join(str(x) for x in range(s,e,o)) + "}}"
    # {{1,1,1}, {}}

cases = (
        [gen(-2**12, 2**12, -2**10, 2**10) for _ in range(TESTS)] +
        [gen(-2**10, 2**10, -2**9, 2**9) for _ in range(TESTS)] +
        [gen(-2**15, 2**15, -2**12, 2**12) for _ in range(TESTS)]
        )

print(",\n".join(cases))
print("\t\t\t\t};")