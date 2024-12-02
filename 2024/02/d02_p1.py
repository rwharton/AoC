import numpy as np

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
    
 
def solve():
    infile = 'input_d02_p1.txt'
    results = []
    with open(infile, 'r') as fin:
        for line in fin:
            cols = line.split()
            if len(cols) == 0:
                continue
            else: pass
            dd = np.array( cols ).astype('int')
            res = check_valid( dd )
            results.append(res)

    ans = np.sum(results)
    print(f"P1 Answer = {ans}")
     
    return

            
    
