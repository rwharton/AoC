import numpy as np
import re

def parse_mult(mstr):
    """
    get mult of digits in mul(d1,d2)
    """
    d = np.array(mstr.strip('mul\(\)').split(',')).astype('int')
    return np.multiply(*d)


def find_dos_and_donts(d):
    do_iter = re.finditer("do\(\)", d)
    dont_iter = re.finditer("don\'t\(\)", d)
    
    do_idx = []
    for mm in do_iter:
        do_idx.append(mm.span()[1])

    dont_idx = []
    for nn in dont_iter:
        dont_idx.append(nn.span()[1])

    do_idx = np.array(do_idx)
    dont_idx = np.array(dont_idx)

    do_mat = np.vstack( (do_idx, np.ones(len(do_idx), dtype='int')) ).T
    dont_mat = np.vstack( (dont_idx, np.zeros(len(dont_idx), dtype='int')) ).T
    
    mat = np.vstack( (do_mat, dont_mat) )

    xx_sort = np.argsort( mat[:, 0] )

    smat = mat[xx_sort]

    return smat


def get_state(idx, smat, start=1):
    """
    using smat (indices, on/off), find if we are 
    on (1) or off (0) at idx
    
    starting state given by start
    """
    state = start
    xx = np.where( smat[:, 0] < idx )[0]
    if len(xx) == 0:
        pass
    else:
        state = smat[xx[-1], 1]

    return state
    

def solve():
    #infile = 'test.txt'
    infile = 'input_d03_p2.txt'

    with open(infile, "r") as f:
        d = f.read()
        # Get dos and donts
        smat = find_dos_and_donts(d)     

        # get iterable of "mul(num,num)" matches
        miter = re.finditer("mul\(\d+,\d+\)", d)

        ans = 0
        for mm in miter:
            idx = mm.span()[1]
            gg = mm.group()
            state = get_state(idx, smat)
            print(f"{gg}, {idx}, {state}")
            if state:
                ans += parse_mult(gg)
    print(f"Part 2: {ans}")
    return

    
