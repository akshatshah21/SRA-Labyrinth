/*
 *  A linked list for keeping track of the direction of the bot
 */

/*
 * USAGE:
 *          Make a directions array and initialize it using the two for loops given below
 *          
 *          Initially make a pointer as:        struct Direction* direction = directions[0];
 * 
 *          FOR LEFT TURN:
 *                          direction = (*direction).prev;
 *          FOR RIGHT TURN:
 *                          direction = (*direction).next
 * 
 *          TO KNOW CURRENT DIRECTION:
 *                          current_dir = (*direction).dir;
 *                          switch((*direction).dir);
 *                  
 * */

#include<stdio.h>
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

struct Direction directions[4];  // THIS IS NOT HOW A LINKED LIST IS MADE. BUT OKAY

void main()
{
    // Initialize dir member of direction array
    for(int i=0;i<4;i++)
    {
        directions[i].dir = i;
    }

    // Creating the links
    for(int i=0;i<4;i++)
    {
        if(i!=3)    directions[i].next = &directions[i+1];
        else    directions[i].next = &directions[0];
        if(i != 0)  directions[i].prev = &directions[i-1];
        else    directions[i].prev = &directions[3];
    }

    struct Direction* trav;

    // Traversing the linked list in the forward direction | Taking RIGHT turn
    printf("Traversing forward, initial direction NORTH, 0 | Going right: \n");
    trav = &directions[0];   // Initially pointing to first direction, dir = 0
    for(int i=0;i<10;i++)
    {
        printf("%d\n", (*trav).dir);
        trav = (*trav).next;
    }

    // Traversing linked list in the backward direction | Taking LEFT turn
    printf("Traversing backward, initial direction SOUTH, 3 | Going left:\n");
    trav = &directions[3];   // Initially pointing to last direction, dir = 3
    for(int i=0;i<10;i++)
    {
        printf("%d\n", (*trav).dir);
        trav = (*trav).prev;
    }
}