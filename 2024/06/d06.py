import numpy as np

class Walker:
    def __init__(self, row, col, grid):
        self.grid = grid
        self.loc = (row, col)
        self.path = [ self.loc ]
        self.ddx = 0
        #dirs = [0, 1, 2, 3] #= [up, right, down, left]
        self.row_add = [-1,  0, +1,  0]
        self.col_add = [ 0, +1,  0, -1]
        self.usteps = 1 
        self.state = [self.loc[0], self.loc[1], self.ddx]
        self.pstates = [ self.state ]
        self.closed = 0

    def check_and_step(self):
        rnext = self.loc[0] + self.row_add[self.ddx]
        cnext = self.loc[1] + self.col_add[self.ddx]
        vnext = self.grid[rnext, cnext]
        #print(f"Next: ({rnext}, {cnext}, {self.ddx}) = {vnext}")
        retval = -1
        if vnext == '0':
            retval = -1
        elif vnext == '#':
            self.ddx = (self.ddx + 1) % 4
            #print(" Uh oh! Turning!")
            retval = 0
        else:
            self.loc = (rnext, cnext)
            self.path.append( self.loc )
            self.usteps = len( set( self.path ) )
            pstate = [rnext, cnext, self.ddx]
            # closed loop?
            if pstate in self.pstates:
                retval = 2
            else:
                retval = 1
            self.pstates.append( pstate )
        return retval

    def walk(self):
        status = 0
        while( status > -1 ):
            status = self.check_and_step()
            if status == 2:
                self.closed = 1
                break
        return

    def __str__(self):
        row, col = self.loc
        return f"Walker({row},{col})"
    
    def __repr__(self):
        row, col = self.loc
        return f"Walker({row},{col})"


def read_input(infile):
    """
    Read the map stored in infile.  
    
    We will pad the border with '0', to 
    make finding when the guard exits a 
    bit easier
    """
    padchar = '0'
    lines = []
    with open(infile, 'r') as f:
        for line in f:
            ll = list(line.strip())
            if len(ll) > 0:
                lines.append( [padchar] + ll + [padchar] )
    # add padding 
    N = len(lines[0])
    lines = [[padchar] * N] + lines + [[padchar] * N]

    marr = np.array(lines)

    return marr


def solve_p1():
    infile = 'test.txt'
    #infile = 'input_d06.txt'
    grid = read_input(infile)
    xx = np.where( grid == '^')
    row = xx[0][0]
    col = xx[1][0]

    W = Walker(row, col, grid)
    W.walk()
    ans = W.usteps

    print(f"Part 1 = {ans}")
    return

def solve_p2():
    #infile = 'test.txt'
    infile = 'input_d06.txt'
    grid = read_input(infile)
    print(grid.size)
    xx = np.where( grid == '^')
    row = xx[0][0]
    col = xx[1][0]

    # Run once to get path
    W = Walker(row, col, grid)
    W.walk()
    path = W.path
    pset = set(path)
    Np = len(pset)

    count = 0 
    for ii, (rr, cc) in enumerate(pset) :
        print(f"Grid {ii} / {Np}")
        grid[rr, cc] = '#'
        W = Walker(row, col, grid)
        W.walk()
        if W.closed:
            print("  -> closed!")
            count += W.closed
        grid[rr, cc] = '.'

    print(f"Part 2 = {count}")
    return 
    
