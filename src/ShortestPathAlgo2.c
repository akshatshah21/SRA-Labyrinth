#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#define V 9

int minDistance(int dist[], bool sptSet[])
{
    int min = INT_MAX, min_index;
    for(int v=0;v < V; v++)
    {
        if(sptSet[v] == false && dist[v] <= min)
        {
              min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

int printSolution(int dist[],int parent[], int n)
{
    printf("Vertex\t\tParent\t\tDist from src\n");
    for(int i=0;i<n; i++)
    {
        printf("%d\t\t%d\t\t%d\n", i, parent[i], dist[i]);
    }
}

int findPath(int parent[], int end)
{
    int path[V];
    int length = 0;
    int i;
    for(i=0;i<V;i++)
    {
        if(end == i)    break;
    }
    while(i != -1)
    {
        path[length] = parent[i];
        i = parent[i];
        length++;
    }

    printf("\n Path:");
    for(int i=0;i<length;i++)
    {
        printf("%d  ",path[i]);
    }
    printf("\n");
}

void dijkstra(int graph[V][V], int src)
{
    int dist[V];
    int parent[V];
    bool sptSet[V];
    for(int i=0;i<V;i++)
    {
        sptSet[i] = false;
        dist[i] = INT_MAX;
    }
    dist[src] = 0;
    parent[src] = -1;
    for(int i=0;i<V-1;i++)
    {
        int u = minDistance(dist, sptSet);
        sptSet[u] = true;
        for(int v=0;v<V;v++)
        {
            if(!sptSet[v] && graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v])
            {
                dist[v] = dist[u] + graph[u][v];
                parent[v] = u;
            }
        }

        // printSolution(dist, V); // Uncomment this if you want to see step by step solution
    }
    printSolution(dist,parent,V);
    findPath(parent, 8);
}



int main()
{
    int graph[V][V] = {{0, 4, 0, 0, 0, 0, 0, 8, 0}, 
                       {4, 0, 8, 0, 0, 0, 0, 11, 0}, 
                       {0, 8, 0, 7, 0, 4, 0, 0, 2}, 
                       {0, 0, 7, 0, 9, 14, 0, 0, 0}, 
                       {0, 0, 0, 9, 0, 10, 0, 0, 0}, 
                       {0, 0, 4, 14, 10, 0, 2, 0, 0}, 
                       {0, 0, 0, 0, 0, 2, 0, 1, 6}, 
                       {8, 11, 0, 0, 0, 0, 1, 0, 7}, 
                       {0, 0, 2, 0, 0, 0, 6, 7, 0} 
                      }; 
    
    dijkstra(graph, 0);
    return 0;
}