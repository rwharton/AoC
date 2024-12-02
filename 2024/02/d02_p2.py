import numpy as np
from functools import reduce

def check_valid( dd ):
    """
    Check if report list is "safe", that is
    if both the following are true:

        1) all increasing or decreasing
        2) adjacent vals differ by at least 
           1 and at most 3
    """
    ddx = np.diff(dd)
    
    # check all have same sign
    res1 = len( np.unique(np.sign(ddx)) ) == 1

    # check that min( abs(ddx) >= 1) and 
    # max(abs(ddx) <= 3)   
    res2 = ( np.min( np.abs(ddx) ) >= 1 ) and \
           ( np.max( np.abs(ddx) ) <= 3 )

    result = 0
    if (res1 and res2):
        result = 1

    return result


def check_fix_dumb( dd ):
    """
    what if being a dumb guy... was smart?
    """
    for ii in range(len(dd)):
        xlo = slice(0, ii, 1)
        xhi = slice(ii+1, len(dd), 1)
        dd_ii = np.hstack( (dd[xlo], dd[xhi]) )
        ret = check_valid(dd_ii)
        #print(dd_ii, ret)
        if ret:
            break
        else: pass
    return ret


def check_fix( dd ):
    ddx = np.diff(dd)
    ssx = np.sign(ddx)

    pidx = []

    s = np.array([ -1, 0, +1 ])
    scnt = np.array([ np.sum( ssx == sii) for sii in s ])

    # Signs all the same
    if max(scnt) == len(ssx):
        pass
    # only one outlier
    elif max(scnt) == len(ssx) - 1:
        xsgn = np.where( scnt == 1 )[0]
        xx = np.where( ssx == s[xsgn] )[0][0]
        pidx.append( (xx, xx+1) )
    # Multiple outliers... unfixable
    else:
        return 0

    for ii, adx in enumerate(np.abs(ddx)):
        if adx < 1:
            pidx.append( (ii, ii+1) )
        elif adx > 3:
            pidx.append( (ii, ii+1) )
        else: pass

    # See if there is a common index value
    #print(pidx)
    idx = reduce( np.intersect1d, pidx )
    if len(idx) == 0:
        return 0

    #print(idx)
   
    for ii in idx: 
        xlo = slice(0, ii, 1)
        xhi = slice(ii+1, len(dd), 1)
        #print(xlo, xhi)
        dd_ii = np.hstack( (dd[xlo], dd[xhi]) )
        res = check_valid( dd_ii )
        if res:
            return res

    return res

 
def solve():
    infile = 'input_d02_p2.txt'
    results = []
    with open(infile, 'r') as fin:
        for line in fin:
            cols = line.split()
            if len(cols) == 0:
                continue
            else: pass
            dd = np.array( cols ).astype('int')
            res = check_valid( dd )
            if res == 0:
                res = check_fix(dd)
                #res = check_fix_dumb(dd)
            results.append(res)

    ans = np.sum(results)
    print(f"P2 Answer = {ans}")
     
    return

            
    
