#include <stdio.h>
/*
    * Add all header files
    *  */

#define MAX_VERTICES 100

int point[MAX_VERTICES]; // Array for storing every point, referred by the number in this array
int type[MAX_VERTICES];  // Array for storing the type of corresponding point in point array
int explored[MAX_VERTICES] = {0};  // Array for storing how many times the bot has explored corresponding point
                    // in the point array

int coordinates[MAX_VERTICES][2];    //Storing coordinates of corresponding points in point array
int point_count = 0;   // Number of distinct points encountered
// int direction;  // Var to store current direction of movement of bot, maybe enum could be used

enum direction {
    east, north, west, south
};

enum direction dir = north;


void map(void *arg) {
    
}
