#include<stdio.h>
enum direction {
east, north, west, south
};
void main() {
enum direction dir1 = north;
enum direction dir2 = south;
printf("%d\n",dir2 + 1);
} 
