#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 256

void reset_buffer(char *buffer, int blen){
    int ii;
    for(ii=0; ii<blen; ii++){
        buffer[ii] = '\0';
    }   
}

int str_to_uint( char *str, int nlen ){
    int retval, slen, dlen, dfac;
    int ii, jj, N;
    int darr[100];
   
    if( nlen > 0 ){
       slen = nlen;
    } else { 
        slen = strlen(str);
    }
    ii = 0;
    jj = 0;
    dlen = 0;
    for(ii=0; ii<slen; ii++){
        //printf("%d %c\n", ii, str[ii]);
        if( isdigit(str[ii]) ){
            darr[jj] = str[ii] - '0'; 
            jj += 1;
        } else if( str[ii] == ' ') {
            // remove preceding non numbers
            // but break at trailing
            if( jj == 0 ){
                continue;
            } else {
                break;
            }
        } else {
            printf("Invalid character detected: %c\n", str[ii]);
            printf("   str: %s\n", str);
            return -1;
        }
    }
    dlen = jj;
    //printf("dlen = %d\n", dlen);
    retval = darr[dlen-1];
    ii=dlen-2;
    dfac=10;
    while(ii >= 0){
        //printf("darr[%d] = %d\n", ii, darr[ii]);
        retval += (darr[ii] * dfac); 
        dfac *= 10;
        ii -= 1;
    }

    return retval;
}


int find_all_char(char *sarr, char *delim, int *iarr){
    int ii, slen, nn;
    slen = strlen(sarr);
    nn = 0;
    for(ii=0; ii<slen; ii++){
        if( sarr[ii] == delim[0] ){
            iarr[nn] = ii;
            nn +=1;
        }
    }
    return nn;
}

void count_balls2(char *res, int slen, int *carr){
    char tmp[100];
    char numstr[16];
    char *btok, *pmatch;
    char *cmatch;
    char colors[3][8] = {"red", "green", "blue"};
    int ii;
    int num;
    int ctmp[3] = {0, 0, 0};
    
    // make temp copy so we dont ruin orig
    strncpy(tmp, res, slen);
    
    // split by comma
    // --- THIS IS THE PROBLEM --- 
    // If just one color, there is no comma
    btok = strtok(tmp, " ");
   
    num = -1; 
    while ( (btok != NULL) ) {
        if( isdigit(btok[0]) ){
            // is it a number
            num = str_to_uint(btok, 0);
        } else {
            // if not, it should be a color name
            for(ii=0; ii<3; ii++){
                pmatch = strstr(btok, colors[ii]);
                if(pmatch != NULL){
                    ctmp[ii] = num;
                }
            }
        num = -1;
        }
        btok = strtok(NULL, " ");
    }

    // fill output array
    //printf("carr = ");
    for(ii=0; ii<3; ii++){
        carr[ii] = ctmp[ii];
        //printf("%d ", carr[ii]);
    }   
    //printf("\n");
}
    

void count_balls(char *res, int slen, int *carr){
    char tmp[100];
    char numstr[16];
    char *btok;
    char *cmatch;
    char colors[3][8] = {"red", "green", "blue"};
    int cidx, count;
    int ii;
    int ctmp[3] = {0, 0, 0};
    
    // make temp copy so we dont ruin orig
    strncpy(tmp, res, slen);
    
    // split by comma
    // --- THIS IS THE PROBLEM --- 
    // If just one color, there is no comma
    btok = strtok(tmp, ",");

    cidx = 0;
    while (btok != NULL) {
        for(ii=0; ii<3; ii++){
            count = 0;
            cmatch = strstr(btok, colors[ii]);
            printf("cmatch = %s\n", cmatch);
            if (cmatch != NULL) {
                cidx = cmatch - btok - 1;
                strncpy(numstr, btok, cidx);
                count = str_to_uint(numstr, cidx);
                printf("count = %d, numstr = %s\n", count, numstr);
                if(count > ctmp[ii]){
                    ctmp[ii] = count;
                }
            }
        }
        btok = strtok(NULL, ",");
    }

    // fill output array
    printf("carr = ");
    for(ii=0; ii<3; ii++){
        carr[ii] = ctmp[ii];
        printf("%d ", carr[ii]);
    }   
    printf("\n");
}
    

void parse_turn(char *line, int *carr){
    char gstr[256] =  {' '};
    char nstr[256] = {' '};
    int iarr[256];
    char *pg;
    int idx, nidx;
    int ii, nn, result;
    // find where first colon is
    nn = find_all_char(line, ":", iarr);
    if( nn == 0 ){
        printf("No \":\" found in %s\n", line); 
        exit(1);
    }
    // Only want first colon
    idx = iarr[0];
    // write new string
    for(ii=0; ii<idx; ii++){
        gstr[ii] = line[ii];
    }  

    // search for Game in this new string
    pg = strstr(gstr, "Game");
    if (pg == NULL) {
        printf("\"Game\" not found in %s\n", gstr);
        exit(1);
    }
    
    // get index
    nidx = pg - gstr;

    for(ii=0; ii<idx-nidx-5; ii++){
        nstr[ii] = gstr[ii + nidx + 5];
    }

    }


int get_game_num(char *gstr, int len){
    // gstr should be of the form "Game xxx"
    // where xxx is a number
    char tmp[256];
    char *ptok;
    int retval;
    strcpy(tmp, gstr);
    ptok = strtok(tmp, " ");
    ptok = strtok(NULL, " ");
    retval = str_to_uint(ptok, 0);
    return retval;
}



void print_buffer(char *buffer, int blen){
    int ii;
    for(ii=0; ii<blen;ii++){
        printf("%c", buffer[ii]);
    }
    printf("\n"); 
}

int main(void){
    FILE* fp;
    char line[MAX_LEN];
    char fname[MAX_LEN] = "input.txt";
    char test[256];
    char temp[256];
    int carr[3], marr[3];
    int ii, jj, nn;
    int gnum, sum, slen, pval;
    int rmax = 12; 
    int gmax = 13;
    int bmax = 14; 
    char buffer[256];

    printf("fname = %s\n", fname);
    fp = fopen(fname, "r");
    if (fp == NULL) {
        perror("Failed: ");
    }

    gnum = 0;
    sum = 0;
    pval = 0;
    while(fgets(line, MAX_LEN, fp)){
        line[strcspn(line, "\n")] = 0;
        //printf("line = %s\n", line);
        slen = strlen(line);
        nn = 0;
        for(ii=0; ii<3; ii++){
            marr[ii] = 0;
            //carr[ii] = 0;
        }
        for(ii=0; ii<slen; ii++){
            //printf("%c", line[ii]);
            if(line[ii] == ':'){
                // buffer contains game string with len nn 
                //printf(" buff = %s, nn = %d\n", buffer, nn);
                gnum = get_game_num(buffer, nn);
                //printf("game num = %d\n", gnum), 
                nn = 0;
            } else if(  (line[ii] == ';')  || (ii==slen-1)) {
                if(ii == slen-1){
                    buffer[nn] = line[ii];
                    nn += 1;
                } 
                // buffer contains one round of game
                count_balls2(buffer, nn, carr);
                //printf("  carr = %d, %d, %d\n", carr[0], carr[1], carr[2]);
                // check and update max array
                for(jj=0; jj<3; jj++){
                    if( carr[jj] > marr[jj] ){
                        marr[jj] = carr[jj];
                    }
                }
                // reset buffer index
                nn=0;
            } else {
                // keep filling
                buffer[nn] = line[ii];
                nn += 1; 
            }
        }
        //printf("  marr = %d, %d, %d\n", marr[0], marr[1], marr[2]);
        // After reading line, calc power 
        pval = marr[0] * marr[1] * marr[2];
        //printf("  pow = %d\n", pval);
        sum += pval;
        //printf("  sum = %d\n", sum);
        reset_buffer(buffer, 256);
        //printf("\n\n");
    }

    printf("sum of powers = %d\n", sum);

    return 0;
}
