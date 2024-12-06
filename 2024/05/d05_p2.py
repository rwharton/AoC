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

    return fail_reqs


def reorder(seq, idx_old, idx_new):
    """
    Move seq index idx_old to idx_new 
    and adjust accordingly
    """
    if (idx_old >= len(seq)) or (idx_new >= len(seq)):
        print("Invalid index!")
        return 0

    oseq = []
    if idx_new == idx_old:
        oseq = seq
    elif idx_old > idx_new:
        oseq = seq[slice(0,idx_new)] + [seq[idx_old]] +\
               seq[slice(idx_new,idx_old)] + seq[slice(idx_old+1, len(seq))]
    else:
        oseq = seq[slice(0,idx_old)] + seq[slice(idx_old+1,idx_new+1)] +\
               [seq[idx_old]] + seq[slice(idx_new+1, len(seq))]

    return oseq


def fix_seq(seq, reqs):
    fails = check_seq(seq, reqs)
    count = 0 
    while( len(fails) and count < 5):
        x = fails[0]
        # Get dict of seq vals and indices (d[val] = idx)  
        d = { nn:ii for ii,nn in enumerate(seq) }
        new_seq = reorder(seq, d[x[0]], d[x[1]])
        fails = check_seq(new_seq, reqs)
        seq = new_seq
    return seq


def solve():
    #infile = 'test.txt'
    infile = 'input_d05.txt'
    reqs, seqs = parse_input(infile)
    
    mseq = []
    for iseq in seqs:
        if len(check_seq(iseq, reqs)) == 0:
            continue
        else: pass

        fseq = fix_seq(iseq, reqs)
        mseq.append( fseq[len(fseq)//2] )


    ans = np.sum( mseq ) 
    print(f"Part2 = {ans}")

    return 
                
