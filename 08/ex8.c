#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * read_file_ma(char *fname, int *nlines);


int main( int argc, char **argv ){
    char fname[] = "input.txt";
    char *buff; 
    int nsteps, ii;
    
    // get buffer
    buff = read_file_ma(fname, &nsteps);
    printf("nsteps = %d\n", nsteps);


    // free arrays
    free(buff);
    buff = 0;

    return 0;
}




char * read_file_ma( char *fname, int *nsteps){
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

    // count number of steps (via "=")
    (*nsteps) = 0;
    for(ii=0; ii<nread; ii++){
        if( buff[ii] == '=' ){
            (*nsteps) += 1;
        }
    }

    return buff;

}
