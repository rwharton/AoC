import numpy as np

def solve(infile):
    dat = np.loadtxt(infile)
    l1, l2 = dat.T

    sort_l1 = np.sort(l1)
    sort_l2 = np.sort(l2)

    dist = np.abs(sort_l1 - sort_l2)
    
    ans = np.sum(dist)

    return ans


infile = 'p1_input.txt'
ans = solve(infile)

print("Part1: %d"  %ans)
