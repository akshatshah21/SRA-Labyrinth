#include <stdio.h>
#include <math.h>
/*
    * Add all header files
    *  
*/


#define LEFT 0
#define RIGHT 1
#define AHEAD 2
#define BACK 3
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
int start_flag = 0;
int start_count = 1;
int end_x = 50000,end_y = 50000; //initial end coordinates
int end_flag = 0;
int end_count = 0;

/*
enum direction {
    east, north, west, south
};
*/



// enum direction current_dir = north;
// enum direction future_dir = east;


struct Direction
{
    int dir;
    struct Direction* next;
    struct Direction* prev; 
};


/*
 *  dir:
 *  0 - NORTH
 *  1 - EAST
 *  2 - SOUTH
 *  3 - WEST 
 */


struct Direction directions[4];
struct Direction* direction;

int direction_explored[MAX_VERTICES][4] = {0} ; //  Parth ka Array
int NEXT_TURN = AHEAD;
int JUNCTION = 0;




/*Variables for line following and junction */
int digital_ls[3] = {0};

/*
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
*/

/*
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
*/




void follow_right(){
	if(digital_ls[RIGHT]==1){
		//turn right
		direction = (*direction).next;
		NEXT_TURN = RIGHT;
		direction_explored[encountered_pt[current_point]][(*direction).dir] = 2;
		// bot_stop(MCPWM_UNIT_0, MCPWM_TIMER_0);
		// vTaskDelay(500/portTICK_PERIOD_MS);
		// bot_spot_left(MCPWM_UNIT_0,MCPWM_TIMER_0,70,70);//left means right, since connections are opposite
		// vTaskDelay(30);	// Change delay values
	}
	else if(digital_ls[AHEAD]==1){
		//go straight
		NEXT_TURN = AHEAD;
		direction_explored[encountered_pt[current_point]][(*direction).dir] = 2;
		// bot_stop(MCPWM_UNIT_0, MCPWM_TIMER_0);
		// vTaskDelay(500/portTICK_PERIOD_MS);	
		// bot_forward(MCPWM_UNIT_0,MCPWM_TIMER_0,70,70);
		// vTaskDelay(50);
	}
	else if(digital_ls[LEFT]==1){
		//turn left
		direction = (*direction).prev;
		NEXT_TURN = LEFT;
		direction_explored[encountered_pt[current_point]][(*direction).dir] = 2;
		// bot_stop(MCPWM_UNIT_0, MCPWM_TIMER_0);
		// vTaskDelay(500/portTICK_PERIOD_MS);
		// bot_spot_right(MCPWM_UNIT_0,MCPWM_TIMER_0,70,70);// right means left, since connections are opposite
		// vTaskDelay(30);	// Change delay values
	}
	else{
		//180 turn  AT DEAD END
		direction = (*direction).prev;
		direction = (*direction).prev;
		NEXT_TURN = BACK;
		direction_explored[encountered_pt[current_point]][(*direction).dir] = 2;
		// while(digital_ls[RIGHT] != 1) {
		// 	bot_spot_left(MCPWM_UNIT_0, MCPWM_TIMER_0, 70, 70);//left means right
		// 	vTaskDelay(30);
		// }

	}

}


int junction_decide() {
	if((digital_ls[LEFT] == 0) && (digital_ls[RIGHT] == 1) && (digital_ls[AHEAD] == 0)){
		// Right only
		direction_explored[encountered_pt[current_point]][((*direction).next).dir] = 1;
		direction_explored[encountered_pt[current_point]][((*direction).next.next).dir] = 2;
		return 1;
	}
	else if((digital_ls[LEFT] == 1) && (digital_ls[RIGHT] == 0) && (digital_ls[AHEAD] == 0)) {
		// Left only
		direction_explored[encountered_pt[current_point]][((*direction).prev).dir] = 1;
		direction_explored[encountered_pt[current_point]][((*direction).next.next).dir] = 2;
		return 1;
	}
	else if(((digital_ls[LEFT] == 0) && (digital_ls[RIGHT] == 1) && digital_ls[AHEAD] == 1)){
		// Right and ahead
		direction_explored[encountered_pt[current_point]][((*direction).next).dir] = 1;
		direction_explored[encountered_pt[current_point]][(*direction).dir] = 1;
		direction_explored[encountered_pt[current_point]][((*direction).next.next).dir] = 2;
		return 2;
	}
	else if(((digital_ls[LEFT] == 1) && (digital_ls[RIGHT] == 0) && digital_ls[AHEAD] == 1)) {
		// Left and ahead
		direction_explored[encountered_pt[current_point]][((*direction).prev).dir] = 1;
		direction_explored[encountered_pt[current_point]][(*direction).dir] = 1;
		direction_explored[encountered_pt[current_point]][((*direction).next.next).dir] = 2;
		return 2;
	}
	else if(((digital_ls[LEFT] == 1) && (digital_ls[RIGHT] == 1) && digital_ls[AHEAD] == 0)) {
		// Left and Right
		direction_explored[encountered_pt[current_point]][((*direction).prev).dir] = 1;
		direction_explored[encountered_pt[current_point]][((*direction).next).dir] = 1;
		direction_explored[encountered_pt[current_point]][((*direction).next.next).dir] = 2;
		return 2;
	}
	else if((digital_ls[LEFT] == 0) && (digital_ls[RIGHT] == 0) && digital_ls[AHEAD] == 0){
		// Dead end
		direction_explored[encountered_pt[current_point]][(((*direction).next).next).dir] = 1;
		//direction_explored[encountered_pt[current_point]][((*direction).next.next).dir] = 2;
		return 0;
	}
	else if(((digital_ls[LEFT] == 1) && (digital_ls[RIGHT] == 1) && digital_ls[AHEAD] == 1)){
		//5cm to and fro
		while(round(countToDistance())!= 5){
			bot_forward(MCPWM_UNIT_0,MCPWM_TIMER_0,65,65);
			end_flag = 0;
			if(digital_ls[LEFT]<3500 && digital_ls[RIGHT]<3500){
				break;
			}
			end_flag = 1;
		}
		while(round(countToDistance())!= 5){
			bot_backward(MCPWM_UNIT_0,MCPWM_TIMER_0,65,65);
		}
		if(end_flag==1){
			end_count++;
			direction_explored[encountered_pt[current_point]][(((*direction).next).next).dir] = 1;
			//direction_explored[encountered_pt[current_point]][((*direction).next.next).dir] = 2;
			return 0;
		}
		else 
			// + junction
			direction_explored[encountered_pt[current_point]][(*direction).dir] = 1;
			direction_explored[encountered_pt[current_point]][((*direction).prev).dir] = 1;
			direction_explored[encountered_pt[current_point]][((*direction).next).dir] = 1;
			direction_explored[encountered_pt[current_point]][((*direction).next.next).dir] = 2;
			return 3;
	}
	else{
		return -1;
	}	
}


void dont_follow_right(){
	explored_f();
	if(explored[encountered_pt[current_point]] - 1 < type[encountered_pt[current_point]]) {
		//traverse parth ka array
		//if 1 is found ,go in that direction by taking appropriate turn
		int parth_flag = 0;
		for(int i=0;i<4;i++) {
			if(direction_explored[encountered_pt[current_point]][i] == 1) {
				
				direction_explored[encountered_pt[current_point]][i] = 2;
				parth_flag = 1;
				// Take turn here
				switch(i - (*direction).dir){
					case 0:
						//go straight
						NEXT_TURN = AHEAD;
						break;
					case 2:
						//turn 180degrees
						NEXT_TURN = BACK;
						break;
					case -2:
						//turn 180degrees
						NEXT_TURN = BACK;
						break;
					case 1:
						//turn left
						NEXT_TURN = LEFT;
						break;
					case -3:
						//turn left
						NEXT_TURN = LEFT;
						break;
					case -1:
						//turn right
						NEXT_TURN = RIGHT;
						break;
					case 3:
						//turn right
						NEXT_TURN = RIGHT;
						break;
					default:
						printf("error in switch_dir()\n");
				}
			}
		}
		//check this condition
		if(parth_flag==0){
			follow_right();
		}
	}
	else {
		follow_right();
	}
}


void explored_f() {
	int current_x, current_y;
	switch((*direction).dir)
	{		
		case 1:
			current_x = coordinates[prev_point][0] + round(countToDistance()); // Iska comm dekho
			current_y = coordinates[prev_point][1];
			break;
		case 3:
			current_x = coordinates[prev_point][0] - round(countToDistance());
			current_y = coordinates[prev_point][1];
			break;
		case 0:
			current_x = coordinates[prev_point][0];
			current_y = coordinates[prev_point][1] + round(countToDistance());
			break;
		case 2:
			current_x = coordinates[prev_point][0];
			current_y = coordinates[prev_point][1] - round(countToDistance());
			break;
		default:
			printf("Error in explored_f()\n");
			break;
	}
	int i;
	int new_f = 1;
	for(i=0;i<point_count;i++) {
		if((current_x == coordinates[i][0] && current_y == coordinates[i][1]) ) {
			// Not a new point
			new_f = 0;
			if(current_x==0 && current_y==0){
				start_flag = 1;
				start_count++;
			}
			break;
		}

	}
	current_point++;

	if(new_f == 1){
		point_count++;
		point[point_count-1] = point_count-1;
		coordinates[point_count-1][0] = current_x;
		coordinates[point_count-1][1] = current_y;
		explored[point_count-1] ++;
		encountered_pt[current_point] = point[point_count-1];
		type[point_count-1] = junction_decide();
		if(end_flag==1 && end_count==1) {
			end_x = current_x;
			end_y = current_y;
			end_flag = 0;
		}
		
		//prev_point = current_point;
	}

	else{
		explored[i]++;
		encountered_pt[current_point] = point[i];
		//prev_point = current_point;
		//traverse encountered_pt backward and look for point with explored value <= type value
		//for '<' from adjacency list get neibouring points and take different path


	}
	if (prev_point != -1)
	{
		int flag = 0;
		for (int j = 0; j < 4; j++)
		{
			if (adj[prev_point][j] == encountered_pt[current_point])
			{
				flag = 1;
				break;
			}
		}
		if (flag == 0)
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
	
	int flag = 0;
	for(int j=0; j<4; j++) {
		if(adj[encountered_pt[current_point]][j]==prev_point){
			flag = 1;
			break;
		}
	}
	if(flag == 0){
		for(int k=0;k<4;k++){
			if(adj[current_point][k]==-1){
				adj[encountered_pt[current_point]][k]=prev_point;
				break;
			}
		}
	}
	prev_point = encountered_pt[current_point];
	if(end_count<=1 && start_count<2){
		follow_right();
	}
	
}


void map(void *arg) {

	while(JUNCTION == 0) {
		printf("No junction\n");
	}
	// bot stop

	// bot_stop(MCPWM_UNIT_0, MCPWM_TIMER_0); // Line follow mai stop karo

	//Junction detected
	if(end_count<=1 && start_count<2){
		explored_f();
	}
	else{
		dont_follow_right();
	}
	JUNCTION = 0;
	// decide_dir();
	// switch_dir();
	// type[point_count] = junction();

}


void app_main()
{
	point_count++;	//Start point
	point[point_count - 1] = 0;	// Adding start to point list
	explored[point_count-1] = 1;
	type[point_count-1] = 0;
	// current_dir = north;	//Start direction


	direction = &directions[0];


	coordinates[point_count-1][0] = 0;
	coordinates[point_count-1][1] = 0;
	encountered_pt[current_point] = 0;
	direction_explored[0][0] = 2;
	prev_point = current_point;
	for(int j=0;j<MAX_VERTICES;j++){
		for(int k=0;k<4;k++){
			adj[j][k] = -1;
		}
	}

	// Initialize dir member of direction array
    for(int i=0;i<4;i++)
    {
        directions[i].dir = i;
    }

    // Creating the links
    for(int i=0; i<4; i++)
    {
        if(i!=3)    directions[i].next = &directions[i+1];
        else    directions[i].next = &directions[0];
        if(i != 0)  directions[i].prev = &directions[i-1];
        else    directions[i].prev = &directions[3];
    }



    // if(dry run) {
	// xTaskCreatePinnnedToCore(&map, "map", 4096, NULL, NULL, 1, NULL, 0);	// CHECK PARAMS
	// xTaskCreatePinnnedToCore(&line_follow, "line_follow", 4096, NULL, NULL, 1, NULL, 1);
	// }

	// else if(final run) {
	// xTaskCreatePinnedToCore(&dijkstra_task, "dijkstra_task", 4096, NULL, NULL, 1, NULL, 0);
	// xTaskCreatePinnedToCore(&line_follow, "line_follow", 4096, NULL, NULL, 1, NULL, 1);
	// }
}