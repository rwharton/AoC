#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 256

int parse_line(char *carr){
	size_t len;
	int ii, dd, count; 
	int first_int, last_int, ret_val;
	
	// get size of carr
	len = strlen(carr);

	// loop over char to get first 
	// and last digit
	first_int = -1;
	last_int = -1;
	count = 0;
	for(ii=0; ii<len; ii++){
		if( isdigit(carr[ii]) ){
			// turns char rep of digit to digit
			dd = carr[ii] - '0';
			if(first_int < 0){
				first_int = dd;
			} else {
				last_int = dd;
			}
			count++;
		}
	}
	if( (count==1) && (last_int == -1) ){
		last_int = first_int;
	}
	//printf("digits:  %d, %d\n", first_int, last_int);

	ret_val = 10 * first_int + last_int; 
	return ret_val;
}

void print_lines(char *fname ){
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
		// removes trailing white space 
		// strcspn counts the number of 
		// characters until hitting "\n" or 
		// reaching end '\0', which i guess 
		// is how you terminate a string
		line[strcspn(line, "\n")] = 0;
		//printf("%s\n", line);
		sum += parse_line(line);
	}
	printf("sum = %d\n", sum);
		
}

int main(int argc, char **argv) {
	char fname[100] = "input.txt";
	print_lines(fname);

	return 0;
}
