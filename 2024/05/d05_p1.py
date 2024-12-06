import numpy as np

def parse_input(infile):
    """
    Need to read lines that are XX|YY as (XX, YY) 
    where this is to be read as the requirement that 
    XX occur before YY

    The sequences are lines of comma separated numbers

    We can identify the rules as lines with | and the 
    sequences as lines with ","
    """
    reqs = []
    seqs = []

    with open(infile, 'r') as f:
        for line in f:
            if "|" in line:
                rnums = list(map(int, line.split('|')))
                reqs.append(rnums)
            elif "," in line:
                snums = list(map(int, line.split(',')))
                seqs.append(snums)
            else:
                continue
  
    return reqs, seqs


def check_seq(seq, reqs):
    """
    check if sequence is in right order.
    """
    # Get dict of seq vals and indices (d[val] = idx)  
    d = { nn:ii for ii,nn in enumerate(seq) }

    fail_reqs = []
    for rr in reqs:
        li = d.get(rr[0])
        ri = d.get(rr[1])
        if (li is None) or (ri is None):
            continue
        else:
            pass
        # if condition fails
        if not li < ri:
            fail_reqs.append( rr )

    good = 0
    if len(fail_reqs) == 0:
        good = 1

    return good
               


def solve():
    #infile = 'test.txt'
    infile = 'input_d05.txt'
    reqs, seqs = parse_input(infile)
    
    mseq = []
    for iseq in seqs:
        if check_seq(iseq, reqs):
            mseq.append( iseq[ len(iseq)//2 ] )

    ans = np.sum( mseq ) 
    print(f"Part1 = {ans}")

    return 
                
