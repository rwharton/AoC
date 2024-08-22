#include <stdio.h>
#include <stdlib.h>


// TEST
//static const int time[] = {7, 15, 30};
//static const int dist[] = {9, 40, 200};

static const int time[] = {44, 80, 65, 72};
static const int dist[] = {208, 1581, 1050, 1102};


int btime_to_dist( int btime, int race_time ){
    // calculate the distance travlled for a given 
    // button press time and total race time.
    // for each unit time of button press, the 
    // boat's velocity increases one unit 
    // so for 5 ms, the speed will be 5 mm/ms
    int vel, dist;
    vel = btime;
    dist = vel * (race_time - btime);
    return dist;
}

int count_ways_to_win( int race_time, int best_dist ) {
    int nn, dist, bt;
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
    int ii, nn;
    int n_races = 4;
    int ans;
    
    ans = 1;
    for(ii=0; ii<n_races; ii++){
        nn = count_ways_to_win( time[ii], dist[ii] );
        printf("%d to beat dist=%d in time=%d\n", nn, dist[ii], time[ii]);
        ans *= nn;
    }

    printf("answer = %d\n", ans);

    return 0;
}

