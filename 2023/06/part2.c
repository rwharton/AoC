#include <stdio.h>
#include <stdlib.h>


uint64_t btime_to_dist( uint64_t btime, uint64_t race_time ){
    // calculate the distance travlled for a given 
    // button press time and total race time.
    // for each unit time of button press, the 
    // boat's velocity increases one unit 
    // so for 5 ms, the speed will be 5 mm/ms
    uint64_t dist;
    dist = btime * (race_time - btime);
    return dist;
}

uint64_t count_ways_to_win( uint64_t race_time, uint64_t best_dist ) {
    uint64_t nn, dist, bt;
    // cant go anywhere with 0 or race_time
    nn=0;
    for(bt=1; bt<race_time; bt++){
        dist = btime_to_dist( bt, race_time );
        if( dist > best_dist ){
            nn++;
        }
    }
    return nn;
}


int main( int argc, char **argv ){
    uint64_t ans;

    long time = 44806572;
    uint64_t dist = 208158110501102;
   
    ans = count_ways_to_win( time, dist );
    printf("answer = %llu\n", ans);

    return 0;
}

