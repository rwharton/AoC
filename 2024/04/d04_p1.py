import numpy as np

def get_dat(infile):
    d = np.loadtxt(infile, dtype='str')
    lsize = np.array([ len(dd) for dd in d ])
    usize = np.unique(lsize)
    if len(usize) > 1:
        print("Size mismatch!")
        print(usize)
        return 0
    else:
        pass

    smat = np.empty( (len(d), usize[0]), dtype='str' )
    for ii, drow in enumerate(d):
        smat[ii] = np.array( list(drow) )

    return smat


def get_idx(r, c, rmax, cmax, nlen=4, otype=0):
    """
    Get indices (row, col) for VALID search regions 
    centered on (r,c).

    otype:

        0: right
        1: left 
        2: down
        3: up
        4: diag up right
        5: diag down right
        6: diag up left
        7: diag down left

    """
    rr = None
    cc = None
    
    # right
    if otype == 0:
        if c + (nlen-1) < cmax:
            rr = np.ones(nlen, dtype='int') * r
            cc = np.arange(c, c + nlen, dtype='int')
        else: pass

    # left
    elif otype == 1:
        if c - (nlen-1) >= 0:
            rr = np.ones(nlen, dtype='int') * r
            cc = np.arange(c, c - nlen, -1, dtype='int')
        else: pass

    # down
    elif otype == 2:
        if r + (nlen-1) < rmax:
            rr = np.arange(r, r + nlen, dtype='int') 
            cc = np.ones(nlen, dtype='int') * c
            
    # up
    elif otype == 3:
        if r - (nlen-1) >= 0:
            rr = np.arange(r, r - nlen, -1, dtype='int') 
            cc = np.ones(nlen, dtype='int') * c

    # diag up right
    elif otype == 4:
        if  (r - (nlen-1) >=0) and (c + (nlen-1) < cmax):
            rr = np.arange(r, r - nlen, -1, dtype='int') 
            cc = np.arange(c, c + nlen, dtype='int')

    # diag down right
    elif otype == 5:
        if (r + (nlen-1) < rmax) and (c + (nlen-1) < cmax):
            rr = np.arange(r, r + nlen, dtype='int') 
            cc = np.arange(c, c + nlen, dtype='int')
            
    # diag up left
    elif otype == 6:
        if  (r - (nlen-1) >=0) and (c - (nlen-1) >= 0):
            rr = np.arange(r, r - nlen, -1, dtype='int') 
            cc = np.arange(c, c - nlen, -1, dtype='int')

    # diag down left
    elif otype == 7:
        if  (r + (nlen-1) < rmax) and (c - (nlen-1) >= 0):
            rr = np.arange(r, r + nlen, dtype='int') 
            cc = np.arange(c, c - nlen, -1, dtype='int')

    else:
        print(f"Invalid {otype=}")
            
    return rr, cc 


def check_match(r, c, smat, mstr): 
    """
    Check possible matches for mstr 
    centered on (r, c) of smat 
    """
    rmax, cmax = smat.shape
    nlen = len(mstr)
    
    match = 0
    for ii in range(8):
        rr, cc = get_idx(r, c, rmax, cmax, nlen=nlen, otype=ii)
        if rr is None:
            continue
        else: pass
        
        str_ii = ''.join(smat[rr, cc].tolist())
        if str_ii == mstr:
            #print(f"{ii=}")
            match += 1
    
    return match 


def solve():
    infile = "input_d04.txt"
    #infile = "test.txt"
    smat = get_dat(infile)

    mstr = "XMAS"
    rr, cc = np.where( smat == mstr[0] )

    ans = 0
    for ii in range(len(rr)):
        res = check_match(rr[ii], cc[ii], smat, mstr)
        ans += res

    print(f"Part1 = {ans}")

    return 
