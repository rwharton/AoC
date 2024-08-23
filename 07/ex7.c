#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

char * read_file_ma(char *fname, int *nlines);
void parse_line(char *line, char *hand, int *bet);
int str_to_int(char *numstr);
int hand_type( char *hand );
int card_char_to_num( char c );
void htype_to_string( int num, char * name );
int Compare_Hands( const void *a, const void *b);
void hand_to_int_arr( char *hand, int *hand_ints );

struct HandInfo {
    int bet;
    char hand[6];
    int hand_ints[5];
    int hand_type;
};

int Compare_Hands( const void *a, const void *b){
    // comparison works like this:
    //      retval < 0 :   a goes before b
    //      retval > 0 :   a goes after b
    //      retval = 0 :   a = b
    // lets order best first    
    // order by hand type and then by first diff digit
    // 
    // lower hand type is better but higher digit is better
    // this means we want  (a.hand_type - b.handtype)
    // but -(a.digit - b.digit)
    //
    int retval, ii;
    
    int ha = ((struct HandInfo *)a)->hand_type;
    int hb = ((struct HandInfo *)b)->hand_type;

    retval = (ha-hb);

    if(retval == 0){
        for(ii=0; ii<5; ii++){
            int va = ((struct HandInfo *)a)->hand_ints[ii];
            int vb = ((struct HandInfo *)b)->hand_ints[ii];
            retval = -(va-vb);
            if(retval !=0){
                break;
            }
        }
    }
    
    return -1*retval; 
}


int main( int argc, char **argv ){
    char fname[] = "input.txt";
    //char fname[] = "test4.txt";
    char line[] = "J6336 780\n";
    char *buff;
    int nlines, ii, jj;
    char *ptok;
    char hand[5];
    int bet, h_type;
    char hname[50];
    int hand_nums[5];
    long ans;

    // get buffer
    buff = read_file_ma(fname, &nlines);

    // set up arrays
    int bets[nlines];
    char hands[nlines][6];
    struct HandInfo hand_info_arr[nlines];

    // split on newline character and parse line
    ii = 0;
    for(ptok = strtok(buff, "\n"); ptok != NULL; ptok = strtok(NULL, "\n")){
        //printf("ptok = %s\n", ptok);
        parse_line(ptok, hand, &bet);
        strcpy(hands[ii], hand);
        bets[ii] = bet;
        ii++;
    }

    // Fill struct array  
    for(ii=0; ii<nlines; ii++){
        hand_info_arr[ii].bet = bets[ii];
        strcpy(hand_info_arr[ii].hand, hands[ii]);
        hand_info_arr[ii].hand_type = hand_type(hands[ii]);
        hand_to_int_arr(hands[ii], hand_nums);
        for(jj=0; jj<5; jj++){
            hand_info_arr[ii].hand_ints[jj] = hand_nums[jj];
        }
    }

    // sort 
    qsort(hand_info_arr, nlines, sizeof(struct HandInfo), Compare_Hands);

    /* 
    for(ii=0; ii<nlines; ii++){
        htype_to_string(hand_info_arr[ii].hand_type, hname);
        hand_to_int_arr(hand_info_arr[ii].hand, hand_nums);
        printf("%4d:  %03d  %s ", ii+1, hand_info_arr[ii].bet, hand_info_arr[ii].hand);
        for(jj=0; jj<5; jj++){
            printf(" %02d ", hand_info_arr[ii].hand_ints[jj] );
        }
        printf(" %s \n", hname);
    }
    */
    
    /*
    for(ii=0; ii<nlines; ii++){
        printf("%s %d\n", hand_info_arr[ii].hand, hand_info_arr[ii].bet);
    }
    */

    // loop over sorted list and calc answer
    ans = 0;
    for(ii=0; ii<nlines; ii++){
        // rank x bet
        ans +=  (hand_info_arr[ii].bet * (ii+1));
    }
    
    printf("ans = %ld\n", ans);
   

    // free arrays
    free(buff); 
    buff = 0;

    //h_type = hand_type("6A213");    
    //printf("h_type = %d\n", h_type);


    return 0;
}

int card_char_to_num( char c ){
    char str[] = "123456789TJQKA";
    int val;
    char *pc;
    
    pc = strchr(str, c);
    val = (pc - str) + 1;
    return val;
}

void hand_to_int_arr( char *hand, int *hand_ints ){
    int ii; 
    for(ii=0; ii<5; ii++){
        hand_ints[ii] = card_char_to_num(hand[ii]);
    }
}

void htype_to_string( int num, char * name ){
    char names[7][50] = { "Five of a kind", 
                          "Four of a kind", 
                          "Full House (3/2)",
                          "Three of a kind",  
                          "Two pair",
                          "One pair",
                          "High Card" };
    strcpy(name, names[num]);
}


int hand_type(char *hand){
    // classify string "hand" to be:
    //   0:  Five of a kind  (AAAAA),    (uu=1)
    //   1:  Four of a kind  (AAAAJ),    (uu=2)
    //   2:  Full House (3/2:  AAAJJ),   (uu=2)
    //   3:  Three of a kind   AAA56 ,   (uu=3)
    //   4:  Two Pair (2 pairs, AAJJx),  (uu=3)
    //   5:  One pair (1 pair, AA325),   (uu=4)
    //   6:  High Card (all diff, 23456) (uu=5)
    int count_arr[5] = {0, 0, 0, 0, 0};
    int uchar_arr[6]; 
    int uu, ii, jj, skip, max_c;
    int h_type;

    skip = 0;
    uu = 0;
    for(ii=0; ii<5; ii++){  
        //printf("ii = %d, char=%c\n", ii, hand[ii]);
        for(jj=0; jj<uu; jj++){
            //printf("  jj = %d\n", jj);
            if( hand[ii] == uchar_arr[jj] ){
                count_arr[jj] += 1;
                //printf("    uu = %d\n", uu);
                skip = 1;
                break;
            }
        }
        if(skip == 0){
            uchar_arr[uu] = hand[ii];
            count_arr[uu] += 1;
            uu += 1;
        } else {
            skip = 0;
        }
    } 

    max_c = 0;
    for(ii=0; ii<uu; ii++){
        if( count_arr[ii] > max_c ){
            max_c = count_arr[ii];
        }
    } 
   
    /* 
    for(ii=0; ii<uu; ii++){
        printf("carr[%d] = %d, uarr[%d] = %c\n", ii, count_arr[ii], 
                                                 ii, uchar_arr[ii]);
    }
    */
    //printf("max_c = %d\n", max_c);

    //printf("uu = %d\n", uu);

    if( uu == 1 ){
        h_type = 0;
    } else if( uu == 2 ){
        if( max_c == 4) {
            h_type = 1;
        } else {
            h_type = 2;
        }
    } else if( uu == 3 ){
        if( max_c == 3) {
            h_type = 3;
        } else {
            h_type = 4;
        }
    } else if( uu == 4 ){
        h_type = 5;
    } else {
        h_type = 6;
    }
    
    return h_type;  
}


void parse_line(char *line, char *hand, int *bet){
    char buff[6];
    char *p;
    // hand is first 5 characters
    // bet is number from index 6 on
    p = strncpy(buff, line, 5);
    buff[5] = '\0';
    strcpy(hand, buff);

    // clear buffer
    for(int ii=0; ii<6; ii++){
        buff[ii] = '\0';
    }

    p = strncpy(buff, line + 6, 4);
    buff[4] = '\0';
    (*bet) = str_to_int(buff);

}


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


char * read_file_ma( char *fname, int *nlines){
    FILE *fp;
    size_t size, nread;
    int ii;
    char *buff;
    
    // Open file
    fp = fopen(fname, "r");

    // Make sure its not empty
    if (fp == NULL){
        fprintf(stderr, "File cannot be opened\n");
        exit(EXIT_FAILURE);
    }

    // Go to end of file
    fseek(fp, 0L, SEEK_END);

    // Get size
    size = (size_t) ftell(fp);

    // allocate memory
    buff = malloc( size );

    // Go to start of file and read in
    rewind(fp);
    
    nread = fread(buff, sizeof(char), size, fp);

    // close file
    fclose(fp);

    // count number of newlines in buffer
    (*nlines) = 0;
    for(ii=0; ii<nread; ii++){
        if( buff[ii] == '\n' ){
            (*nlines) += 1;
        }
    }

    return buff;
}
