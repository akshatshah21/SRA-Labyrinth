#include <stdio.h>
/*
    * Add all header files
    *  */
#define LEFT 0
#define RIGHT 1
#define AHEAD 2
#define MAX_VERTICES 100
/*Variables for mapping */
int point[MAX_VERTICES]; // Array for storing every point, referred by the number in this array
int type[MAX_VERTICES];  // Array for storing the type of corresponding point in point array
int explored[MAX_VERTICES] = {0};  // Array for storing how many times the bot has explored corresponding point
                  				   // in the point array
int current_point = 0;
int coordinates[MAX_VERTICES][2];    //Storing coordinates of corresponding points in point array
int point_count = 0;   // Number of distinct points encountered
// int direction;  // Var to store current direction of movement of bot, maybe enum could be used


enum direction {
    east, north, west, south
};

enum direction dir = north;


/*Variables for line following and junction */
int digital_ls[3] = {0};


int junction() {
	if((digital_ls[LEFT] == 0) && (digital_ls[RIGHT] == 1) && (digital_ls[AHEAD] == 0)){
		type[current_point] = 1;
	}

	else if((digital_ls[LEFT] == 1) && (digital_ls[RIGHT] == 0) && digital_ls[AHEAD] == 0) {
		type[current_point] = 2;
	}

	else if((digital_ls[LEFT] == 0) && (digital_ls[RIGHT] == 1) && digital_ls[AHEAD] == 1) {
		type[current_point] = 3;
	}

	else if((digital_ls[LEFT] == 1) && (digital_ls[RIGHT] == 0) && digital_ls[AHEAD] == 1) {
		type[current_point] = 4;
	}
}

void explored_f() {
	int current_x, current_y;
	switch(dir)
	{		
		case east:
			current_x = coordinates[prev_point][0] + //dist_x;
			current_y = coordinates[prev_point][1];
			break;
		case west:
			current_x = coordinates[prev_point][0] - //dist_x;
			current_y = coordinates[prev_point][1];
			break;
		case north:
			current_x = coordinates[prev_point][0];
			current_y = coordinates[prev_point][1] + //dist_y;
			break;
		case south:
			current_x = coordinates[prev_point][0];
			current_y = coordinates[prev_point][1] - //dist_y;
			break;
		default:
			printf"Error\n");
			break;
	}
	int i;
	bool new = true;
	for(i=0;i<point_count;i++) {
		if(current_x == coordinates[i][0] && current_y == coordinates[i][1]) {
			// Not a new point
			new = false;
			break;
		}
	}

	if(new){
		current_point = point_count;
		point_count++;
		point[current_point] = current_point;
		coordinates[current_point][0] = current_x;
		coordinates[current_point][1] = current_y;
		explored[current_point] ++;
		type[current_point] = junction();
		decide_dir();


	}




}



void map(void *arg) {
    point_count++;	//Start point
	point[point_count - 1] = 0;	// Adding start to point list
	explored[point_count-1] = 1;
	type[point_count-1] = 0;
	dir = north;	//Start direction
	coordinates[point_count-1][0] = 0;
	coordinates[point_count-1][1] = 0;

	while(junction() == -1) {
		printf("No junction\n");

	}

	//Junction detected
	calculate_coord();
	explored_f();
	coordinates[]

	// type[point_count] = junction();



}
