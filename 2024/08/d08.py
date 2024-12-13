import numpy as np
from collections import defaultdict
from itertools import combinations

def parse_input(infile):
    """
    read input file

    return default dict of list of coords 
    of each alphanumeric char and the size 
    of the grid
    """
    rows = cols = 0
    ants = defaultdict(list)
    with open(infile, 'r') as fin:
        for ii, line in enumerate(fin):
            if len(line) > 1:
                rows += 1
                cols = len(line) - 1
                
            for jj, c in enumerate(line):
                if c.isalnum(): 
                    ants[c].append( (ii, jj) )

    grid_size = (rows, cols)

    return ants, grid_size


def check_valid(cc, gridsize):
    """
    check if coordinate cc is actually 
    on the grid with size gridsize = (rows, cols)
    """
    valid = 1
    x, y = cc
    mx, my = gridsize
    if (x < 0) or (y < 0) or (x >= mx) or (y >= my):
        valid = 0
    return valid


def get_antinode_pair( cc1, cc2 ):
    """
    Get the two antinodes for antennas at 
    coordinates cc1 =(row, col) and cc2
    """
    x1, y1 = cc1
    x2, y2 = cc2

    acc1 = ( 2 * x2 - x1, 2 * y2 - y1 )
    acc2 = ( 2 * x1 - x2, 2 * y1 - y2 )

    return acc1, acc2


def get_antinodes( cclist, gridsize ):
    """
    Calculate the antinodes for all pairs 
    of coordinates in the list of tuples cclist 
    """
    nlist = []
    for idx1, idx2 in combinations( range(len(cclist)), 2):
        cc1 = cclist[idx1]
        cc2 = cclist[idx2] 
        accs = get_antinode_pair( cc1, cc2 )
        for acc in accs:
            if check_valid(acc, gridsize):
                nlist.append(acc)
    return nlist


def get_reduced_slope( cc1, cc2 ):  
    """
    get slope (reduce fraction as nec)

    return (m, n) so slope = m/n
    """
    x1, y1 = cc1
    x2, y2 = cc2

    m = x2 - x1
    n = y2 - y1 

    g = np.gcd(m, n)
    if np.abs(g) > 0:
        m = m // g
        n = n // g

    return m, n


def nodes_along_line( cc0, m, n, gridsize ):
    """
    Find the points on the line with slope 
    m/n passing through cc0  
    """
    nlist = []
    for sgn in [-1, + 1]:
        for ii in range( max(gridsize) ):
            cc_ii = ( cc0[0] + sgn * m * ii, 
                      cc0[1] + sgn * n * ii )
            if check_valid(cc_ii, gridsize):
                nlist.append( cc_ii )
            else:
                break
    nlist = [ nn for nn in set(nlist) ]
    return nlist 
             

def get_all_antinodes( cclist, gridsize ):
    """
    Calculate the antinodes along line of all pairs
    of coordinates in the list of tuples cclist 
    """
    nlist = []
    for idx1, idx2 in combinations( range(len(cclist)), 2):
        cc1 = cclist[idx1]
        cc2 = cclist[idx2] 
        m, n = get_reduced_slope( cc1, cc2 )
        nlist += nodes_along_line( cc1, m, n, gridsize )
    return nlist


def solve_p1():
    #infile = "test.txt"
    infile = "d08_input.txt"
     
    ants, gridsize = parse_input(infile)
    nlist = []
     
    # loop over antenna names 
    for kk in ants.keys():
        ccs = ants[kk]
        nlist += get_antinodes( ccs, gridsize )

    ulist = [ cc for cc in set( nlist ) ] 

    print(f"Part 1: {len(ulist)}")

    return 


def solve_p2():
    #infile = "test.txt"
    infile = "d08_input.txt"
     
    ants, gridsize = parse_input(infile)
    nlist = []
     
    # loop over antenna names 
    for kk in ants.keys():
        ccs = ants[kk]
        nlist += get_all_antinodes( ccs, gridsize )

    ulist = [ cc for cc in set( nlist ) ] 

    print(f"Part 2: {len(ulist)}")

    return 
