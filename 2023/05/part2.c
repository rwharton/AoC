#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 256
#define MAP_NUM 7

// 0: sts, 1: s2f, 2: f2w, 3: w2l, 4: l2t, 5: t2h, 6: h2l 
static const char map_str[MAP_NUM][30] = {
                     "seed-to-soil", "soil-to-fertilizer", "fertilizer-to-water", 
                     "water-to-light", "light-to-temperature", 
                     "temperature-to-humidity", "humidity-to-location"};
static const int map_len[MAP_NUM] = {12, 18, 19, 14, 20, 23, 20};

long min_long( long *arr, int len){
    long cur_min;
    int ii = 0;

    cur_min = 0;
    for(ii=0; ii<len; ii++){
        if( (arr[ii] < cur_min) || (ii == 0) ){
            cur_min = arr[ii];
        }
    }
    return cur_min;
}


long str_to_long(char *numstr){
    long val;
    char *pend;
    val = strtol(numstr, &pend, 10);
        if(*pend != '\0'){
            printf("Invalid.  Trailing = %s\n", pend);
            return -1;
        }
    return val;
}

int get_seeds( char *line, long *seed_arr ){
    char *p;
    int nn;
    char tmp[MAX_LEN];

    strcpy(tmp, line);
    
    p = strtok(tmp, " ");
    nn = -1;
    while( p != NULL ){
        if( nn > -1 ){
            seed_arr[nn] = str_to_long( p );
        }
        nn += 1;
        p = strtok(NULL, " ");
    }
    return nn;
}

void get_map_row( char *line, long *row ){
    char *p;
    int ii;
    char tmp[MAX_LEN];
    
    strcpy(tmp, line);
    p = strtok(tmp, " ");
    ii = 0;
    while(p != NULL) {
        row[ii] = str_to_long(p);   
        ii += 1;
        p = strtok(NULL, " ");
    }
}

int check_line_for_map( char *line ){
    int ii, retval;
    retval = -1;
    for(ii=0; ii<MAP_NUM; ii++){
        if( strncmp(line, map_str[ii], map_len[ii]) == 0 ){
            retval = ii;
            break; 
        }
    }   
    return retval;
}

int check_for_blank_line( char *line ){
    int ii, retval;
    retval = 1;
    for(ii=0; ii<strlen(line); ii++){
        if( isspace(line[ii]) != 0 ){
            retval = 0;
            break;
        }
    }
    return retval;
}


long check_map_entry(long val, long dest_start, long src_start, long range){
    // if val is in range, give corresponding dest value
    // else, return -1
    long ret_val;
    if( (val >= src_start) && (val < src_start + range) ){
        ret_val = (val - src_start) + dest_start;
    }
    else{
        ret_val = -1;
    }
    return ret_val;
}


long traverse_one_map(long val, long map_arr[MAX_LEN][3], int midx, int mlen){
    // apply map (starting at map index midx for len mlen) to val
    // map will return val if none of the ranges apply
    long ret_val;
    int ii;
    for(ii=0; ii<mlen; ii++){
        ret_val = check_map_entry(val, map_arr[midx+ii][0], 
                                       map_arr[midx+ii][1], 
                                       map_arr[midx+ii][2]);
        if( ret_val != -1 ){
            break;
        }
    }
    if(ret_val == -1) {
        ret_val = val;
    }
    return ret_val;
}


void parse_file(char *fname, long *seed_arr, long map_arr[256][3], 
                int *marr, int *midx, int *mlens, int *nseeds){
    char line[MAX_LEN];
    int lnum, mnum;
    int ii, jj, kk, mm;
    //int nseeds;
    //int marr[7], midx[7], mlens[7];
    int marr_size;

    FILE *fp;

    fp = fopen(fname, "r");
    if( fp == NULL ){
        fprintf(stderr, "Failed to open file %s\n", fname);
    }

    lnum = 0;
    mnum = -1;
    mm = -1;
    ii = 0;
    while( fgets(line, MAX_LEN, fp) && (lnum > -1)) {
        line[strcspn(line, "\n")] = 0;
        //printf("line: %s\n", line);

        // check if this is the seed line
        if( strncmp(line, "seeds:", 6) == 0 ){
            *nseeds = get_seeds(line, seed_arr);
        
        } else if ( (mnum = check_line_for_map( line )) > -1 ){
            //printf("line: %s, mnum = %d\n", line, mnum);
            mm++;
            marr[mm] = mnum;
            midx[mm] = ii;
            mlens[mm] = 0;
            //printf(" midx[%d] = %d\n", mm, ii);
        } else if ( check_for_blank_line(line) != 1 ){
            //printf("ii = %d\n", ii);
            get_map_row(line, map_arr[ii]); 
            //printf("mnum = %d, ii = %d\n", marr[mm], ii);
            ii++;
            mlens[mm] += 1;
        } else {
            ;
        }

        lnum += 1;
    }
    marr_size = ii;

    /*
    for(jj=0; jj<*nseeds; jj++){
        printf("%d:  %ld\n", jj, seed_arr[jj]);
    }   

    for(jj=0; jj<mm; jj++){
        printf("marr[%d] = %d, midx[%d] = %d, mlens[%d] = %d\n", 
               jj, marr[jj], jj, midx[jj], jj, mlens[jj]);
    }
  
    kk = 0; 
    for(jj=0; jj<marr_size; jj++){
        if( midx[kk] == jj ) {
            printf("%d\n", jj);
            printf("---  %d   (%d elements) ---\n", marr[kk], mlens[kk]);
            kk++;
        }
        printf("%16ld  %16ld  %16ld\n", map_arr[jj][0], map_arr[jj][1], map_arr[jj][2]);
    }
    */
        
}

int main(int argc, char **argv){
    char fname[] = "input2.txt";
    //char fname[] = "test.txt";
    long seed_arr[100];
    long loc_arr[100];
    long map_arr[MAX_LEN][3];
    int marr[7], midx[7], mlens[7];
    long mi, mi_new, min_loc;
    int nseeds, ii, jj;
    long seed_start, seed_range, ss;
    long final_min, min_seed, min_start;

    parse_file(fname, seed_arr, map_arr, marr, midx, mlens, &nseeds);

    printf("fname = %s\n", fname);

    final_min = -1;
    min_seed = -1;
    min_start = -1;
    for(ii=0; ii < nseeds/2; ii++){
        seed_start = seed_arr[2 * ii];
        seed_range = seed_arr[2 * ii + 1];
        printf("start = %ld\n", seed_start);
        for(ss=seed_start; ss < seed_start + seed_range; ss += 1){
            mi = ss;
            if( ((ss-seed_start) % 10000000) == 0 ){
                ;//printf("%ld, min = %ld\n", (ss-seed_start)/10000000, final_min);
            }
            for(jj=0; jj<7; jj++){
                mi_new = traverse_one_map(mi, map_arr, midx[jj], mlens[jj]); 
                mi = mi_new;
            }
            if( (mi_new < final_min) || (final_min == -1) ){
                final_min = mi_new;
                min_seed = ss;
                min_start = seed_start;
            }
        }
    }

    printf("lowest final val = %ld\n", final_min);
    printf("            seed = %ld\n", min_seed);
    printf("      seed start = %ld\n", min_start);

    return 0;

}
