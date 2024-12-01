#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
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

int str_to_numarr(char *str, int *numarr){
    char *p;
    char tmp[100];
    int nn;
    
    strcpy(tmp, str);

    nn = 0;
    p = strtok(tmp, " ");
    while( p != NULL ){
        numarr[nn] = str_to_int(p);
        p = strtok(NULL, " ");
        nn += 1;
    }

    return nn;
}

void parse_line(char *line, int *winarr, int *numarr, 
                int *nwin, int *nnum) {
    char *p1, *p2, *p3;
    int len1, len2; 
    int istart1, istart2;
    char nstr1[100] = " ";
    char nstr2[100] = " ";
    
    // Find ranges of sub strings for numbers 
    p1 = strchr(line, ':');
    p2 = strchr(line, '|');
    p3 = strchr(line, '\0');
    
    istart1 = p1 - line + 1;
    istart2 = p2 - line + 1;

    len1 = p2 - p1 - 1;
    len2 = p3 - p2 - 1;

    strncpy(nstr1, line + istart1, len1);
    strncpy(nstr2, line + istart2, len2);

    //printf("string1 = %s\n", nstr1);
    //printf("string2 = %s\n", nstr2);

    *nwin = str_to_numarr(nstr1, winarr);
    *nnum = str_to_numarr(nstr2, numarr);
}


int count_matches(int *winarr, int wlen, int *numarr, int nlen){
    int count;
    int ii, jj;
    
    count = 0;
    for(ii=0; ii<wlen; ii++){
        for(jj=0; jj<nlen; jj++){
            if( numarr[jj] == winarr[ii] ){
                count += 1;
            }
        }
    }

    return count;
}

int count_to_score( int count ){
    int score, ii;
    if( count == 0 ){
        return 0;
    } 
    score = 1;
    for(ii=1; ii<count; ii++){
        score *= 2;
    } 
    return score;
}

void count_cards( int *sarr, int slen ){
    unsigned long long carr[slen];
    unsigned long long sum;
    int ii, jj;

    // you start with one of each card
    for(ii=0; ii<slen; ii++){
        carr[ii] = 1;
    }

    // now we add in how many we won
    for(ii=0; ii<slen; ii++){
        //printf("ii = %d\n", ii);
        if( sarr[ii] == 0 ){
            continue;
        }
        for(jj=1; jj<sarr[ii]+1; jj++){
            if( (ii+jj) < slen ){
                carr[ii+jj] += carr[ii]; 
            } else {
                break;
            }
        }
    }

    // print
    sum = 0;
    for(ii=0; ii<slen; ii++){
        sum += carr[ii];
        //printf("%d:  sarr = %d, carr = %llu\n", ii, sarr[ii], carr[ii]);
    }
    printf("sum = %llu\n", sum);
    
}


int main(int argc, char **argv){
    FILE *fp;
    char fname[] = "input.txt";
    //char fname[] = "test.txt";
    char line[MAX_LEN];
    int winarr[100];
    int numarr[100];
    int nwin, nnum, count, score, sum;
    int ii, nlen;
    int sarr[256];
    
    printf("fname = %s\n", fname);
    fp = fopen(fname, "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file %s\n", fname);
    }

    sum = 0;
    ii = 0;
    while( fgets(line, MAX_LEN, fp) ){
        line[strcspn(line, "\n")] = 0;
        parse_line(line, winarr, numarr, &nwin, &nnum);
        count = count_matches(winarr, nwin, numarr, nnum);
        // no more score, dummy
        //score = count_to_score(count);
        sarr[ii] = count;
        ii += 1;
    }

    nlen = ii; 
    count_cards(sarr, nlen);    

    //printf("sum = %d\n", sum);

    return 0;
}
