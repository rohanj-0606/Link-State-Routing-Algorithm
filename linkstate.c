#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//globally defined variables which will be used in all the functions
int col=1;
int row=0,i,j;
int matrix[100][100];
int path[20],dist[20];

//function to accept the matrix from user and print it
void matrix_func()
{


FILE *files;
int ch;
char ff[30];

printf("\nInput: ");
scanf("%s",ff);
//opening of file in reading mode
files=fopen(ff,"r");

//checking of number of rows and columns in the inputted file matrix. 
do
{
ch = fgetc(files);
if(ch=='\n')
row++;
if (ch==' ' && row==0) 
 { 
	col++; 
 }
} while(ch != EOF);
ch = fgetc(files);

printf("\n");
fclose(files);

files=fopen(ff,"r");
//scanning of user matrix
for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
			{	
 			fscanf(files, "%d", &matrix[i][j]);
			}
	}
//printing of matrix
 for(i=0;i<row;i++)
    {
        for(j=0;j<col;j++)
        {
            printf("%d \t",matrix[i][j]);
        }
        printf("\n");
    }

printf("\n");
}

//function to calculate shortest path from source to destination
void path_calc(int start_node,int dest_node)
{
	int right_path[col],count=1;
	int final_node = dest_node;	
	right_path[0]=dest_node;
	//initializing previous node as same node for each router and saving previous node of each visited router
	while(path[dest_node]!=dest_node)
	{
		right_path[count]=path[dest_node];
		count++;
		int temp = path[dest_node];
		dest_node=temp;
	}
	//printing shortest path from source node to destination node.
	printf("\n\nPath between %d and %d is %d",start_node,final_node,start_node);
	for(i=count-1;i>=0;i--)
		printf("-%d",right_path[i]);
	printf(" with cost %d\n\n",dist[final_node]);
	
}
//function to remove the router and change the matrix accordingly
void change_topology(int rem_node)
{
	for(i=0;i<col;i++)
	{
		for(j=0;j<col;j++)
		{
			//Setting rows and columns value as -1 for the router to be removed
			if(i==rem_node || j==rem_node)
				matrix[i][j]=-1;
		}
	}
}

//function which performs dijsktra’s alorithm
void dij(int start_node, int print_flag)
{	
	int v_node[col-1],temp_cost=0;
	//Initializing visited nodes, distance array and path
	for(i=0;i<col;i++)
	{
		v_node[i]=0;
		dist[i]=-1;
		path[i]=i;
	}
	//Setting the distance(cost) for source node as 0
	dist[start_node]=0;
	//Copying the values for routers that have direct link to source node from matrix array.
	for(i=0;i<col;i++)
	{
		dist[i]=matrix[start_node][i];
	}
	
	int next_node=0;
	//If start node is 0 then the next should be 1
	if(start_node==0)
		next_node=1;
	//Find the node with the least cost to start_node
	for(j=0;j<col;j++)
	{
		for(i=0;i<col;i++)
		{
			//If node is start_node or a visited node then skip the iteration.		
			if(i==start_node || v_node[i]==1)
			{
				//printf("nothing %d",i);
			}
			else
			{	
				if(dist[i]!=-1)
				{	
					if(dist[next_node]>dist[i] || dist[next_node]==-1 || v_node[next_node]==1)
					{
						next_node = i;
						
					}
				}
			}
		}
		v_node[next_node]=1;
	

		//Update the dist array based on comaprison with cost from next_node
		for(i=0;i<col;i++)
		{
			if(matrix[next_node][i]!=-1)
			{
				if(i==start_node || v_node[i]==1)
				{
				}
				else
				{		
					temp_cost = dist[next_node]+matrix[next_node][i];
					if(dist[i]>temp_cost || dist[i]==-1)
					{
						dist[i]=temp_cost;
						//Setting previous node as the next node if cost is less through next_node
						path[i]=next_node;
					
					}
			}
		}
	}
	}
	if(print_flag==1)
	{
	//printing destination node and shortest distance from source node.
		printf("\nNode\tInterface");
		for(i=0;i<col;i++)
		{
			printf("\n%d",i);
			if(i==start_node)
				printf("\t-");
			else
				printf("\t%d",dist[i]);
		}
	}	
}
//function to find shortes path from source to destination
void router_shortest_path()
{
	int temp_cost=0, final_cost[20];
	int l=0,m=0;
	for(l=0;l<col;l++)
	{
		dij(l,0);
		//calculating shortest path from one router to all other router
		for(m=0;m<col;m++)
			{temp_cost=temp_cost+dist[m];}
		final_cost[l]=temp_cost;
		temp_cost=0;
	}
	int least_cost_router=0;
	for(l=0;l<col;l++)
	{
		if(final_cost[least_cost_router]>=final_cost[l])
			least_cost_router=l;
	}
	printf("The router with shortest path to all other routers is %d with total cost %d\n\n",least_cost_router,final_cost[least_cost_router]);
}
  
int main()
{
int k, matrix_flag=0;
int startnode,destnode,remnode;
do
{
//printing menu to user
printf("\n");
printf("\nCS542- Simulator for Link State Routing Algorithm:\n");
printf("\n1. Create Network Topology\n2. Build a connection table\n3. Find shortest path\n4. Modify a topology\n5. Best router for broadcast\n6. EXIT\n");
printf("Enter your option: ");
scanf("%d",&k);
switch(k)
{
case 1:
	matrix_func();
	matrix_flag=1;
	break;
case 2:
	if(matrix_flag==0)
	{
	//if user have not entered source file then user has to selection option 1 and input source file before proceeding to the code
		printf("\nPlease select option 1 and set the network topology");
		break;	
	}
	printf("\nEnter a source node: ");
	scanf("%d", &startnode);
	//calling dijkstra’s algorithm
	dij(startnode,1);
	break;
case 3:
	printf("\nSource router is %d",startnode);
	printf("\nEnter a destination node: ");
	scanf("%d", &destnode);
	//calculating shortest path from source to destination
	path_calc(startnode,destnode);
	break;
case 4:
	printf("\nSelect the router to be removed: ");
	scanf("%d", &remnode);
	//calling function to remove the entered router
	change_topology(remnode);
	//caling dijkstra’s algorithm again
	dij(startnode,1);
	//calcuating shortest past
	path_calc(startnode,destnode);
	break;
case 5:
	//calculating shortest path from one router all other router
	router_shortest_path();
	break;
case 6:
	printf("EXITING -> GOOD BYE\n\n");
	//exiting
	return(0);
	break;
default :
printf("BYE BYE\n\n");
}
}while(k!=0);
return 0;

}
