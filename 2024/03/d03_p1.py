import numpy as np
import re

def parse_mult(mstr):
    """
    get mult of digits in mul(d1,d2)
    """
    d = np.array(mstr.strip('mul\(\)').split(',')).astype('int')
    return np.multiply(*d)


def solve():
    with open("input_d03_p1.txt", "r") as f:
        d = f.read()
        # get list of "mul(num,num)" matches
        mlist = re.findall("mul\(\d+,\d+\)", d)
        ans = 0
        for mm in mlist:
            ans += parse_mult(mm)
    print(f"Part 1: {ans}")
    return

    
