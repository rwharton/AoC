#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 256

static const char names[10][16] = {"zero", "one", "two", "three",
                                   "four", "five", "six", "seven",
                                   "eight", "nine"};

int digit_to_str( int val, char *vname ){
	int ii, len;
	if( (val >= 0) && (val < 10) ){
		strcpy(vname, names[val]);
		len = strlen(names[val]);
		printf("%d\n", len);
	} else {
		len = 0;
	}
	return len;
}

int digit_to_strlen( int val ){
	int ii, len;
	if( (val >= 0) && (val < 10) ){
		len = strlen(names[val]);
	} else {
		len = 0;
	}
	return len;
}
	
int str_to_digit( char *vname ){
	int ii, digit;
	digit = -1;
	for(ii=0; ii<10; ii++){
		if( strcmp(vname, names[ii]) == 0 ){
			digit = ii;
			break;
		}
	}
	return digit;	
}

void search_strings( char *instr, int *varr, int *iarr ) {	
	int ii, nlen, index;
	int first_val, last_val;
	int first_idx, last_idx;	
	char *p;

	first_idx = -1;
	last_idx  = -1;
	first_val = -1;
	last_val  = -1;

	for(ii=0; ii<10; ii++) {
		char *ptmp = instr;
		p = strstr(ptmp, names[ii]);
		index = 0;
		while ( (ptmp != 0) && ((p = strstr(ptmp, names[ii])) != NULL)){
			nlen = digit_to_strlen( ii );
			index += (p - ptmp) ;
			//printf("found %d at index %d\n", ii, index);
			if( (first_idx < 0) || (index < first_idx) ){
				first_idx = index;
				first_val = ii;
			}
			if ((index > last_idx) && (index >= first_idx)) {
				last_idx = index;
				last_val = ii;
			}
			ptmp = p+nlen;
			index += nlen;
		}
	}

	varr[0] = first_val;
	varr[1] = last_val;
	iarr[0] = first_idx;
	iarr[1] = last_idx;
}


void search_ints( char *instr, int *varr, int *iarr ) {	
	int ii, index, dd, len;
	int first_val, last_val;
	int first_idx, last_idx;	
	char *p;
	
	first_idx = -1;
	last_idx  = -1;
	first_val = -1;
	last_val  = -1;

	len = strlen(instr);
	
	for(ii=0; ii<len; ii++){
		if( isdigit(instr[ii]) ){
			// turns char rep of digit to digit
			dd = instr[ii] - '0';
			index = ii;
			//printf("found %d at index %d\n", dd, index);
			if( (index < first_idx) || (first_idx < 0) ){
				first_val = dd;
				first_idx = ii;
			}
			if( (index > last_idx)){
				last_val = dd;	
				last_idx = ii;
			}
		}
	}

	varr[0] = first_val;
	varr[1] = last_val;
	iarr[0] = first_idx;
	iarr[1] = last_idx;
}

int search( char *instr ){
	int s_varr[2], i_varr[2];
	int s_iarr[2], i_iarr[2];
	int varr[4], iarr[4];
	int ii;
	int first_val, last_val;
	int first_idx, last_idx;
	int retval;

	search_strings(instr, s_varr, s_iarr);	
	search_ints(instr, i_varr, i_iarr);	

	for(ii=0; ii<2; ii++){
		varr[ii] = s_varr[ii];
		varr[ii+2] = i_varr[ii];
		iarr[ii] = s_iarr[ii];
		iarr[ii+2] = i_iarr[ii];
	}

	first_idx = -1;
	last_idx  = -1;
	first_val = -1;
	last_val  = -1;

	for(ii=0; ii<4; ii++){
		if( iarr[ii] < 0 ){
			continue;
		}

		if( (first_idx < 0) || (iarr[ii] < first_idx) ){
			first_idx = iarr[ii];
			first_val = varr[ii];
		}
		if( iarr[ii] >= last_idx ){
			last_idx = iarr[ii];
			last_val = varr[ii];
		}
		
	}

	if( (first_val < 0) && (last_val > 0) ){
		first_val = last_val;
	}
	
	if( (first_val > 0) && (last_val < 0) ){
		last_val = first_val;
	}
	
	if( (first_val < 0) && (last_val < 0) ){
		printf("string = %s\n", instr);
		printf("\n\n NEGATIVE VAL! \n\n");
		exit(1);
	}

	
	retval = 10 * first_val + last_val;
	//printf("string = %s\n", instr);
	//printf("  %d, %d -> %d\n", first_val, last_val, retval);

	return retval;

}

void parse_and_sum_lines(char *fname ){
	FILE* fp;
	char line[MAX_LEN];
	int sum;

	printf("fname = %s\n", fname);
	fp = fopen(fname, "r");
	if (fp == NULL) {
		perror("Failed: ");
	}	
	
	sum = 0;
	while(fgets(line, MAX_LEN, fp)){
		//printf("%s\n", line);
		// removes trailing white space 
		// strcspn counts the number of 
		// characters until hitting "\n" or 
		// reaching end '\0', which i guess 
		// is how you terminate a string
		line[strcspn(line, "\n")] = 0;
		//printf("%s\n", line);
		sum += search(line);
	}
	printf("sum = %d\n", sum);
		
}

int main(int argc, char **argv) {
	char fname[100] = "input.txt";
	char test[100];
	int ii, len, num;
	char *p1;
	char p2[100];
	char name[10];
	int varr_s[2], varr_i[2];
	int iarr_s[2], iarr_i[2];
	int retval;

	/*
	//print_lines(fname);
	strcpy(test, "lkrjlsz7mgv9525p1");
	//printf("len = %lu\n", strlen(test));
	printf("%s\n", test);

	search_strings(test, varr_s, iarr_s);
	printf("first str int = %d at %d\n", varr_s[0], iarr_s[0]);
	printf("last  str int = %d at %d\n", varr_s[1], iarr_s[1]);
	
	search_ints(test, varr_i, iarr_i);
	printf("first int int = %d at %d\n", varr_i[0], iarr_i[0]);
	printf("last  int int = %d at %d\n", varr_i[1], iarr_i[1]);
	*/

	/*
	retval = search(test);
	printf("%s\n", test);
	printf("res = %d\n", retval);
	*/

	parse_and_sum_lines(fname);


	return 0;
}
