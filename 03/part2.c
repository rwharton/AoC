#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 256

int str_to_int(char *numstr){
    int val;
    char *pend;
    val = (int) strtol(numstr, &pend, 10); 
        if(*pend != '\0'){
            printf("Invalid.  Trailing = %s\n", pend);
            return -1;
        }
    return val;
    }


int find_numbers(char *line, int *iarr, int *varr){
    char *ptok;
    char *ppend;
    char tmp[MAX_LEN];
    char delim[] = ".!@#%^&*()-+=:;\"\'<>,?|/\\{}[]$";
    int idx, nn;

    strcpy(tmp, line);

    ptok = strtok(tmp, delim);
    nn = 0; 
    while( ptok != NULL ){  
        idx = (ptok - tmp);
        iarr[nn] = idx;
        varr[nn] = str_to_int(ptok);
        //printf("ptok = %s at %d\n", ptok, idx);
        ptok = strtok(NULL, delim);
        nn += 1;
    }

    return nn;
}

int find_gears(char *line, int *iarr, char varr[MAX_LEN][100]){
    char *ptok;
    char tmp[MAX_LEN];
    char delim[] = ".0123456789!@#%^&()-+=:;\"\'<>,?|/\\{}[]$";
    int idx, nn;

    strcpy(tmp, line);

    ptok = strtok(tmp, delim);
    nn = 0;
    while( ptok != NULL ){  
        idx = (ptok - tmp);
        iarr[nn] = idx;
        //varr[nn] = ptok;
        strcpy(varr[nn], ptok);
        //printf("ptok = %s at %d\n", ptok, idx);
        ptok = strtok(NULL, delim);
        nn += 1; 
    }
    return nn;
}

int find_symbols(char *line, int *iarr, char varr[MAX_LEN][100]){
    char *ptok;
    char tmp[MAX_LEN];
    char delim[] = ".0123456789";
    int idx, nn;

    strcpy(tmp, line);

    ptok = strtok(tmp, delim);
    nn = 0;
    while( ptok != NULL ){  
        idx = (ptok - tmp);
        iarr[nn] = idx;
        //varr[nn] = ptok;
        strcpy(varr[nn], ptok);
        //printf("ptok = %s at %d\n", ptok, idx);
        ptok = strtok(NULL, delim);
        nn += 1; 
    }
    return nn;
}

int ndigits( int num ){
    // calc number of digits in number
    int ii, d, ndigits;

    if ( num == 0 ){
        return 1;
    }
    
    ndigits = 0; 
    d = num;
    while( d > 0 ) {
        d /= 10;
        ndigits += 1;
    }
    return ndigits;
}

void get_adjacents(int *nvals, int *nidx, int *sidx, int n_num, 
                   int n_sym,  int *hits) {
    // check for adjacent symbols 
    // this is technically different for cases where symbols are on 
    // current line or above/below, but the current line won't have 
    // any symbols in the spot occupied by numbers
    int ii, jj, nn;
    int nstart, nstop;
    
    // need nums and symbols 
    //printf("%d %d\n", n_num, n_sym);
    if( (n_num > 0) && (n_sym > 0) ){
        // loop over numbers
        for(ii=0; ii<n_num; ii++){
            // Get index range of current number
            nstart = nidx[ii];
            nstop  = nstart + ndigits( nvals[ii] ) - 1;
            //printf("val: %d, nstart = %d, nstop = %d\n", nvals[ii], nstart, nstop);
            for(jj=0; jj<n_sym; jj++){
                if( (sidx[jj] >= nstart - 1) && (sidx[jj] <= nstop + 1) ){
                    //printf("sidx[%d] = %d, nstart = %d, nstop = %d\n", 
                    //       jj, sidx[jj], nstart, nstop);
                    hits[ii] += 1;
                }
            }
        }
    }
}

void get_adjacent_nums(int *nvals, int *nidx, int *sidx, int n_num, 
                       int n_sym,  int *hits, int *pows) {
    // check for adjacent numbers to gears
    int ii, jj, nn;
    int nstart, nstop;
    
    // need nums and symbols 
    //printf("%d %d\n", n_num, n_sym);
    if( (n_num > 0) && (n_sym > 0) ){
        // loop over symbols
        for(ii=0; ii<n_sym; ii++){
            for(jj=0; jj<n_num; jj++){
                // Get index range of current number
                nstart = nidx[jj];
                nstop  = nstart + ndigits( nvals[jj] ) - 1;
                if( (sidx[ii] >= nstart - 1) && (sidx[ii] <= nstop + 1) ){
                    hits[ii] += 1;
                    pows[ii] *= nvals[jj];
                    //printf("%d, %d, %d,  ", nvals[jj], hits[ii], pows[ii]);
                }
            }
        }
    }
}

int search_buff(char lbuff[3][MAX_LEN], 
                int *hit_vals, int *hit_idx){
    // This function will search for adjacent symbols 
    // for numbers in the line in lbuff[1] 
    // return number of hits
    int hits[MAX_LEN] = {0};
    int pows[MAX_LEN];
    int iarr[MAX_LEN];
    int varr[MAX_LEN];
    int siarr[MAX_LEN];
    char svarr[MAX_LEN][100];
    int n_num, n_sym;
    int ii, nn;
    
    // get gear info for row = 1
    n_sym = find_gears(lbuff[1], siarr, svarr);

    // initialize pows array
    for(ii=0; ii<n_sym; ii++){
        pows[ii] = 1;
    }
    
   
    // get numbers for each line and search for hits
    for(ii=0; ii<3; ii++){
        n_num = find_numbers(lbuff[ii], iarr, varr);
        // will add 1 to hits array at num index
        get_adjacent_nums(varr, iarr, siarr, n_num, n_sym, hits, pows);
    }

    // loop over hits array to find index, value, and number
    // of adjacency hits 
  
    //printf("hits: "); 
    nn = 0;
    for(ii=0; ii<n_sym; ii++){
        if( hits[ii] == 2){
            hit_idx[nn] = siarr[ii];
            hit_vals[nn] = pows[ii];
            //printf("%d, ", pows[ii]);
            //printf("%s-%d, ", svarr[ii], siarr[ii]);
            nn += 1;
        }
    }
    //printf("\n");

    return nn;
}

int sum_hit_vals(int *hit_vals, int nhits){
    int ii, sum;
    sum = 0;
    for(ii=0; ii<nhits; ii++){
        sum += hit_vals[ii];
    }
    //printf("sum: %d\n", sum);
    return sum;
}


void add_line_to_buff(char lbuff[3][MAX_LEN], char *line){
    // lbuff has three rows. want to add line to bottom
    // and shift everything else up
    int ii;
    // Move 1 to 0, 2 to 1
    for(ii=0; ii<2; ii++){
        strcpy(lbuff[ii], lbuff[ii+1]);
    }
    // Add line to 2
    strcpy(lbuff[2], line);
}

void print_line_buff(char linebuff[3][MAX_LEN]){
    int ii;
    for(ii=0; ii<3; ii++){
        printf("%d: %s\n", ii, linebuff[ii]);
    }
}



int main(int argc, char **argv){
    FILE* fp;
    char line[MAX_LEN];
    char linebuff[3][MAX_LEN];
    char fname[MAX_LEN] = "input.txt";
    char *ptok;
    int lnum, idx, nvals, snvals;
    int iarr[MAX_LEN];
    int varr[MAX_LEN];
    int siarr[MAX_LEN];
    char svarr[MAX_LEN][100];
    int ii;
    int hit_vals[MAX_LEN];    
    int hit_idx[MAX_LEN];    
    int num_hits;
    int sum;
   
     
    printf("fname = %s\n", fname);
    fp = fopen(fname, "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file %s\n", fname);
    }

    sum = 0;
    lnum = -1;
    while( fgets(line, MAX_LEN, fp) && (lnum < 100000) ){
        line[strcspn(line, "\n")] = 0;
        // add line to buffer
        //printf("lnum = %d\n", lnum);
        add_line_to_buff(linebuff, line); 
        //printf("\n");
        //print_line_buff(linebuff);

        // want current line in middle, so if line num = 0;
        if( lnum < 0 ){
            lnum += 1;
            continue;
        }

        num_hits = search_buff(linebuff, hit_vals, hit_idx);
        //printf("%d hits on line %d\n", num_hits, lnum);
        sum += sum_hit_vals(hit_vals, num_hits);

        lnum += 1;
    } 
    // the while loop ends when we are done reading file
    // but in that case we still need to do one more search
    // we also need to add a blank line to buffer
    add_line_to_buff(linebuff, "."); 
    //print_line_buff(linebuff);
    num_hits = search_buff(linebuff, hit_vals, hit_idx);
    //printf("%d hits on line %d\n", num_hits, lnum);
    sum += sum_hit_vals(hit_vals, num_hits);

    printf("sum = %d\n", sum);
    
    return 0;
}
