#include <stdio.h>
/*
    * Add all header files
    *  */
#define LEFT 0
#define RIGHT 1
#define AHEAD 2
#define MAX_VERTICES 100
/*Variables for mapping */
int adj[MAX_VERTICES][4];	//Adjacency list
int point[MAX_VERTICES]; // Array for storing every unique point, referred by the number in this array
int encountered_pt[MAX_VERTICES*5]; //Array for storing every Encountered point in order of occurence
int type[MAX_VERTICES];  // Array for storing the type of corresponding point in point array
int explored[MAX_VERTICES] = {0};  // Array for storing how many times the bot has explored corresponding point
                  				   // in the point array
int current_point = 0;	// pointer to encountered_pt array
int prev_point = -1;
int coordinates[MAX_VERTICES][2];    //Storing coordinates of corresponding points in point array
int point_count = 0;   // Number of distinct points encountered | pointer to point array
// int direction;  // Var to store current direction of movement of bot, maybe enum could be used
int end_x = 50000,end_y = 50000; //initial end coordinates

enum direction {
    east, north, west, south
};

enum direction current_dir = north;
enum direction future_dir = east;


/*Variables for line following and junction */
int digital_ls[3] = {0};


void switch_dir() {
	switch(future_dir - current_dir) {
		case 0:
			//go straight
			current_dir = future_dir;
			break;
		case 2:
			//turn 180degrees
			current_dir = future_dir;
			break;
		case -2:
			//turn 180degrees
			current_dir = future_dir;
			break;
		case 1:
			//turn left
			current_dir = future_dir;
			break;
		case -3:
			//turn left
			current_dir = future_dir;
			break;
		case -1:
			//turn right
			current_dir = future_dir;
			break;
		case 3:
			//turn right
			current_dir = future_dir;
			break;
		default:
			printf("error in switch_dir()\n");
	}
}

void decide_dir(){
	switch(current_dir){
		case east:
			if(digital_ls[AHEAD]==1){
				future_dir = east;
				break;
			}
			else if(digital_ls[LEFT] == 1) {
				future_dir = north;
				break;
			}
			else if(digital_ls[RIGHT] == 1) {
				future_dir = south;
				break;
			}
			else {
				future_dir = west;
				break;
			}

		case north:
			if(digital_ls[RIGHT]==1){
				future_dir = east;
				break;
			}
			else if(digital_ls[AHEAD] == 1) {
				future_dir = north;
				break;
			}
			else if(digital_ls[LEFT] == 1) {
				future_dir = west;
				break;
			}
			else {
				future_dir = south;
				break;
			}

		case west:
			if(digital_ls[RIGHT]==1){
				future_dir = north;
				break;
			}
			else if(digital_ls[AHEAD] == 1) {
				future_dir = west;
				break;
			}
			else if(digital_ls[LEFT] == 1) {
				future_dir = south;
				break;
			}
			else {
				future_dir = east;
				break;
			}

		case south:
			if(digital_ls[LEFT]==1){
				future_dir = east;
				break;
			}
			else if(digital_ls[RIGHT] == 1) {
				future_dir = west;
				break;
			}
			else if(digital_ls[AHEAD] == 1) {
				future_dir = south;
				break;
			}
			else {
				future_dir = north;
				break;
			}	

				
	}
}


int junction() {
	if((digital_ls[LEFT] == 0) && (digital_ls[RIGHT] == 1) && (digital_ls[AHEAD] == 0) || ((digital_ls[LEFT] == 1) && (digital_ls[RIGHT] == 0) && (digital_ls[AHEAD] == 0))){
		return 1;
	}
	else if(((digital_ls[LEFT] == 0) && (digital_ls[RIGHT] == 1) && digital_ls[AHEAD] == 1) || ((digital_ls[LEFT] == 1) && (digital_ls[RIGHT] == 0) && digital_ls[AHEAD] == 1) || ((digital_ls[LEFT] == 1) && (digital_ls[RIGHT] == 1) && digital_ls[AHEAD] == 0)){
		return 2;
	}
	else if((digital_ls[LEFT] == 0) && (digital_ls[RIGHT] == 0) && digital_ls[AHEAD] == 0)){
		return 0;
	}
	else if(((digital_ls[LEFT] == 1) && (digital_ls[RIGHT] == 1) && digital_ls[AHEAD] == 1)){
		//5cm to and fro
	}
	else{
		return -1;
	}	
	/*
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
	else if((digital_ls[LEFT] == 1) && (digital_ls[RIGHT] == 1) && digital_ls[AHEAD] == 1) {
		bool end = true;
		while(/* travelled 5cm to and fro /){
			if((digital_ls[LEFT] == 0) || (digital_ls[RIGHT] == 0) || digital_ls[AHEAD] == 0){
				end = false;
				break;
			}
		}
		if()
	}
	else if((digital_ls[LEFT] == 1) && (digital_ls[RIGHT] == 1) && digital_ls[AHEAD] == 0) {
		type[current_point] = 6;
	}
	else if((digital_ls[LEFT] == 0) && (digital_ls[RIGHT] == 0) && digital_ls[AHEAD] == 0) {
		type[current_point] = 7;
	}
	*/


}

void explored_f() {
	int current_x, current_y;
	switch(current_dir)
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
			printf("Error in explored_f()\n");
			break;
	}
	int i;
	bool new = true;
	for(i=0;i<point_count;i++) {
		if((current_x == coordinates[i][0] && current_y == coordinates[i][1]) || (current_x == end_x && current_y == end_y)) {
			// Not a new point
			new = false;
			break;
		}

	}
	current_point++;

	if(new){
		point_count++;
		point[point_count-1] = point_count-1;
		coordinates[point_count-1][0] = current_x;
		coordinates[point_count-1][1] = current_y;
		explored[point_count-1] ++;
		type[point_count-1] = junction();
		encountered_pt[current_point] = point[point_count-1];
		//prev_point = current_point;
	}

	else{
		explored[i]++;
		encountered_pt[current_point] = point[i];
		//prev_point = current_point;
		//traverse encountered_point backward and look for point with explored value <= type value
		//for '<' from adjacency list get neibouring points and take different path


	}
	if (prev_point != -1)
	{
		bool flag = 0;
		for (int j = 0; j < 4; j++)
		{
			if (adj[prev_point][j] == encountered_pt[current_point])
			{
				flag = 1;
				break;
			}
		}
		if (!flag)
		{
			for (int j = 0; j < 4; j++)
			{
				if (adj[prev_point][j] == -1)
				{
					adj[prev_point][j] = encountered_pt[current_point];
					break;
				}
			}
		}
	}
	bool flag = 0;
	for(int j=0;j<4,j++) {
		if(adj[encountered_pt[current_point]][j]==prev_point){
			flag = 1;
			break;
		}
	}
	if(!flag){
		for(int k=0;k<4;k++){
			if(adj[current_point][k]==-1){
				adj[encountered_pt[current_point]][k]=prev_point;
				break;
			}
		}
	}
	prev_point = current_point;
}


void map(void *arg) {

	while(junction() == -1) {
		printf("No junction\n");
	}
	// bot stop

	//Junction detected
	explored_f();
	decide_dir();
	switch_dir();
	// type[point_count] = junction();



}

void app_main(){
	point_count++;	//Start point
	point[point_count - 1] = 0;	// Adding start to point list
	explored[point_count-1] = 1;
	type[point_count-1] = 0;
	current_dir = north;	//Start direction
	coordinates[point_count-1][0] = 0;
	coordinates[point_count-1][1] = 0;
	encountered_pt[current_point] = 0;
	prev_point = current_point;
	for(int j=0;j<MAX_VERTICES;j++){
		for(int k=0;k<4;k++){
			adj[j][k] = -1;
		}
	}

	xTaskCreate(&map "map",/*    */);
}
