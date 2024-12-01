import numpy as np
from collections import Counter

def solve(infile):
    dat = np.loadtxt(infile).astype('int')
    l1, l2 = dat.T
    
    c2 = Counter(l2)

    ans = 0
    for ll in l1:
        ans += (ll * c2[ll])
        #print(ll, c2[ll])

    return ans


infile = 'p1_input.txt'
ans = solve(infile)

print("Part2: %d"  %ans)
