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


def check_match(r, c, smat): 
    """
    Check possible X matches  
    centered on (r, c) of smat 
    """
    rmax, cmax = smat.shape
    
    # The r and c are the position of A
    # so we just need to ensure that we 
    # arent on an edge
    if (r in [0, rmax-1]) or (c in [0, cmax-1]):
        return 0
    else: pass

    match = 0
    # Now we need to check diagonal \
    rr = np.arange(r-1, r+2)
    cc = np.arange(c-1, c+2)
    if ''.join(smat[rr, cc].tolist()) in ['SAM', 'MAS']:
        # Now we need to check diagonal /
        if ''.join(smat[rr[::-1], cc].tolist()) in ['SAM', 'MAS']:
            match = 1
    
    return match 


def solve():
    infile = "input_d04.txt"
    #infile = "test.txt"
    smat = get_dat(infile)

    rr, cc = np.where( smat == "A" )

    ans = 0
    for ii in range(len(rr)):
        res = check_match(rr[ii], cc[ii], smat)
        ans += res

    print(f"Part2 = {ans}")

    return 
