import numpy as np
from tqdm import tqdm, trange
from itertools import combinations

def op(a, b, part=1):
    """
    return (a+b, a*b, a||b)

    where a||b concatenates the numbers
    so that 45||100 = 45100
    """
    if part == 1:
        return (a+b, a*b)
    else:
        return (a+b, a*b, int(f"{a}{b}"))


def check_combos(ans, nums, part=1):
    """
    can nums be made to = ans for some 
    combo of (mult, sum, concat)
    """
    N = len(nums)

    prev = np.array([nums[0]])
    cur = np.array([])
    
    for ii in range(1, N):
        cur = np.array([ op(pp, nums[ii], part=part)  for \
                         pp in prev ]).flatten()
        prev = cur
        #print(cur)

    retval = 0
    if ans in cur:
        retval = 1
    return retval
        

def parse_input(infile):
    """
    read data file

    list of tuples ( ans, np.array( nums ) 
    """
    data = []
    with open( infile, 'r' ) as fin:
        for line in fin:
            if ":" not in line:
                continue
            else:
                sans, snums = line.split(":")
                ans = int(sans)
                nums = np.array( list(map(int, snums.split())) )
                data.append( (ans, nums) )

    return data


def solve_p1():
    #infile = 'test.txt'
    infile = 'd07_input.txt'
    data = parse_input(infile)
    
    ans = 0
    for ii in trange(len(data)):
        val, seq = data[ii]
        if check_combos(val, seq, part=1):
            ans += val

    print(f"Part 1: {ans}")
    return 
    

def solve_p2():
    #infile = 'test.txt'
    infile = 'd07_input.txt'
    data = parse_input(infile)
    
    ans = 0
    for ii in trange(len(data)):
        val, seq = data[ii]
        if check_combos(val, seq, part=2):
            ans += val

    print(f"Part 2: {ans}")
    return 
    
