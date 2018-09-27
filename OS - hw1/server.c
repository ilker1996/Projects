#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include <poll.h>

#define PIPE(fd) socketpair(AF_UNIX, SOCK_STREAM, PF_UNIX, fd)

#define READ 0
#define WRITE 1

typedef struct coordinate {
	int x;
	int y;
} coordinate;

typedef struct element{ // MAP DATA STRUCTURE 
	char character;
	int energy;
	pid_t pid;
} element;

typedef struct ph_message{
	coordinate move_request;
}message;

typedef struct server_message {
	coordinate pos;
	coordinate adv_pos;
	int object_count;
	coordinate object_pos[4];
}server_message;

server_message* send_prey(server_message* sent,int pos_x,int pos_y,int hunter_count,coordinate arr_hunter[hunter_count],int width,int height,element map[height][width]);

server_message* send_hunter(server_message* sent,int pos_x,int pos_y,int prey_count,coordinate arr_prey[prey_count],int width,int height,element map[height][width]);

//PRINT MAP
void print(int width,int height,element map[height][width]){
	int i,j;
	printf("+");
	for( i = 0; i < width; i++)
	{
		printf("-");
	}
	printf("+\n");
	for( j = 0; j < height; j++)
	{	
		printf("|");
		for(i = 0; i < width; i++)
		{	
			printf("%c",map[j][i].character);
		}
		printf("|\n");
	}
	printf("+");
	for( i = 0; i < width; i++)
	{
		printf("-");
	}
	printf("+\n");
}

int main(){
	char width_str[12],height_str[12];
	int i,j,k,row,column,lifepoint;
	int width,height;
	int num_obstacle,num_hunter,num_prey,real_num_hunter,real_num_prey;
	
	//SCAN WIDTH AND HEIGHT
	scanf("%d %d",&width,&height);
	
	sprintf(width_str,"%d",width);
	sprintf(height_str,"%d",height);
	
	element grid[height][width];

	
	//MAKE GRID EMPTY
	for( j = 0; j < height; j++)
	{	
		for(i = 0; i < width; i++)
		{	
			grid[j][i].character = ' ';
			grid[j][i].energy = 0;
			grid[j][i].pid = 0;
		}
	}
	
	//SCAN OBSTACLES
	scanf("%d",&num_obstacle);
	for(i = 0; i < num_obstacle ; i++)
	{
		scanf("%d %d",&row,&column);
		grid[row][column].character = 'X';
	}

	//SCAN HUNTERS
	scanf("%d",&num_hunter);
	coordinate hunters[num_hunter];
	for(i = 0; i < num_hunter ; i++)
	{
		scanf("%d %d %d",&row,&column,&lifepoint);
		hunters[i].x = row;
		hunters[i].y = column;
		grid[row][column].character = 'H';
		grid[row][column].energy = lifepoint;
	}

	//SCAN PREYS
	scanf("%d",&num_prey);
	coordinate preys[num_prey];
	for(i = 0; i < num_prey ; i++)
	{
		scanf("%d %d %d",&row,&column,&lifepoint);
		preys[i].x = row;
		preys[i].y = column;
		grid[row][column].character = 'P';
		grid[row][column].energy = lifepoint;
	}
	real_num_hunter = num_hunter;
	real_num_prey = num_prey;

	//PRINT INITIAL MAP
	print(width,height,grid);
	
	server_message send;
	message take;
	while(real_num_prey != 0 && real_num_hunter != 0)
	{

		for(i = 0; i < height  ;i++)
		{	
			for(j = 0;j < width;j++)
			{	
				
				if(grid[i][j].character == 'H'){
					int nfd[2];
					PIPE(nfd);

					pid_t cpid = fork();
					if(cpid == 0){
						dup2(nfd[WRITE],0);	// USE PIPE nfd[1] END FOR THE CHILD
						dup2(nfd[WRITE],1);
						close(nfd[READ]);
						char* const arr[] = {"./hunter",width_str,height_str,NULL};
						execv("./hunter",arr);
					}
					else{
						
						send_hunter(&send,i,j,num_prey,preys,width,height,grid);
						close(nfd[WRITE]);
						write(nfd[READ],&send,sizeof(server_message));	// USE PIPE nfd[0] END FOR THE PARENT
						waitpid(cpid,NULL,0);
						grid[i][j].pid = cpid;
						read(nfd[READ],&take,sizeof(message));
						//fprintf(stderr, "HUNTER REQUEST : %d %d\n",take.move_request.x,take.move_request.y);
						if(send.pos.x != take.move_request.x || send.pos.y != take.move_request.y)
						{	

							//HUNTER CAUGHT PREY
							if(grid[take.move_request.x][take.move_request.y].character == 'P')
							{	
								grid[take.move_request.x][take.move_request.y].character = 'H';
								grid[take.move_request.x][take.move_request.y].energy = grid[send.pos.x][send.pos.y].energy + grid[take.move_request.x][take.move_request.y].energy - 1;
								grid[take.move_request.x][take.move_request.y].pid = grid[send.pos.x][send.pos.y].pid;
								

								for(k = 0 ; k < num_prey ; k++)
								{
									if(take.move_request.x == preys[k].x && take.move_request.y == preys[k].y)
									{
										//INACTIVATE THE PREY IN PREYS ARRAY
										real_num_prey--;
										if(real_num_prey == 0)
										{
											kill(grid[send.pos.x][send.pos.y].pid,SIGKILL);	// KILL DIED PREY PROCESS
											grid[send.pos.x][send.pos.y].character = ' ';
											grid[send.pos.x][send.pos.y].energy = 0;
											grid[send.pos.x][send.pos.y].pid = 0;
											
											fflush(stdout);	// FLUSH STDOUT FOR PRINTING MAP
											print(width,height,grid);
											kill_allchildren(width,height,grid);  // KILL ALL PROCESSES EXCEPT SERVER
											return 0;	//  CLOSE SERVER
										}
										preys[k].x = -1;
										preys[k].y = -1;
									}
								}
								for(k = 0 ; k < num_hunter ; k++)
								{
									if(send.pos.x == hunters[k].x && send.pos.y == hunters[k].y)
									{
										//CHANGE THE HUNTER LOCATON IN HUNTERS ARRAY
										hunters[k].x = take.move_request.x;
										hunters[k].y = take.move_request.y;
									}
								}


							}
							else
							{	

								grid[take.move_request.x][take.move_request.y].character = 'H';
								grid[take.move_request.x][take.move_request.y].energy = grid[send.pos.x][send.pos.y].energy - 1;
								grid[take.move_request.x][take.move_request.y].pid = grid[send.pos.x][send.pos.y].pid;

											//THE HUNTER DIED FROM 0 ENERGY
								if(grid[take.move_request.x][take.move_request.y].energy <= 0)
								{	
									kill(grid[take.move_request.x][take.move_request.y].pid,SIGKILL);	// KILL DIED HUNTER PROCESS
									grid[take.move_request.x][take.move_request.y].energy = 0;
									grid[take.move_request.x][take.move_request.y].character = ' ';
									grid[take.move_request.x][take.move_request.y].pid = 0;
									real_num_hunter--;
									if(real_num_hunter == 0)
									{	
									
										grid[send.pos.x][send.pos.y].pid = 0;
										grid[send.pos.x][send.pos.y].character = ' ';
										grid[send.pos.x][send.pos.y].energy = 0;
										
										fflush(stdout);	//FLUSH STDOUT FOR PRINTING MAP
										print(width,height,grid);
										kill_allchildren(width,height,grid);	// KILL ALL PROCESSES EXCEPT SERVER

										return 0; // CLOSE  SERVER
									}
								}

								for(k = 0; k < num_hunter ; k++)
								{
									if(send.pos.x == hunters[k].x && send.pos.y == hunters[k].y)
									{	
									
										//THE HUNTER DIED FROM 0 ENERGY
										if(grid[take.move_request.x][take.move_request.y].energy == 0)
										{
											
											//INACTIVATE THE HUNTER LOCATION IN HUNTERS ARRAY
											hunters[k].x = -1;
											hunters[k].y = -1;
										}
											//HUNTER DID NOT DIE
										else
										{
											//CHANGE THE HUNTER LOCATION IN HUNTERS ARRAY
											hunters[k].x = take.move_request.x;
											hunters[k].y = take.move_request.y;
										}
									}

								}

							}
						
							grid[send.pos.x][send.pos.y].pid = 0;
							grid[send.pos.x][send.pos.y].character = ' ';
							grid[send.pos.x][send.pos.y].energy = 0;

							print(width,height,grid);

						}
					}
				}
				else if(grid[i][j].character == 'P'){
					int nfd[2];
					PIPE(nfd);
					pid_t cpid = fork();
					if(cpid == 0){
						dup2(nfd[WRITE],0);	// USE PIPE nfd[1] END FOR CHILD
						dup2(nfd[WRITE],1);
						close(nfd[READ]);
						char* const arr[] = {"./hunter",width_str,height_str,NULL};
						execv("./prey",arr);
					}
					else{
						send_prey(&send,i,j,num_hunter,hunters,width,height,grid);
						close(nfd[WRITE]); 
						write(nfd[READ],&send,sizeof(server_message)); // USE PIPE nfd[0] END FOR THE PARENT
						waitpid(cpid,NULL,0);
						grid[i][j].pid = cpid;
						read(nfd[READ],&take,sizeof(message));
						//fprintf(stderr, "PREY REQUEST : %d %d \n",take.move_request.x,take.move_request.y);
						if(send.pos.x != take.move_request.x || send.pos.y != take.move_request.y)
						{		
								for(k = 0 ; k < num_prey ; k++)
								{
									if(send.pos.x == preys[k].x && send.pos.y == preys[k].y)
									{
										//CHANGE PREY LOCATION IN PREYS ARRAY
										preys[k].x = take.move_request.x;
										preys[k].y = take.move_request.y;
									}
								}
								grid[take.move_request.x][take.move_request.y].character = 'P';
								grid[take.move_request.x][take.move_request.y].energy = grid[send.pos.x][send.pos.y].energy;
								grid[take.move_request.x][take.move_request.y].pid = grid[send.pos.x][send.pos.y].pid;
								grid[send.pos.x][send.pos.y].pid = 0;
								grid[send.pos.x][send.pos.y].character = ' ';
								grid[send.pos.x][send.pos.y].energy = 0;
								print(width,height,grid);

						}
					}
				}
			}
		}

	}
	kill_allchildren(width,height,grid); 	// KILL ALL PROCESSES EXCEPT SERVER
	fflush(stdout);
	return 0;	// CLOSE SERVER
}


//KILL ALL PROCESSES

kill_allchildren(int width,int height,element map[height][width])
{	
	int i,j;
	for(i = 0 ; i < height;i++)
	{
		for(j = 0 ; j < width;j++)
		{
			if(map[i][j].pid != 0)
			kill(map[i][j].pid,SIGKILL);
		}
	}
}


//FOR HUNTER SERVER MESSAGE

server_message* send_hunter(server_message* sent,int pos_x,int pos_y,int prey_count,coordinate arr_prey[prey_count],int width,int height,element map[height][width])
{
	int counter = 0,i,new_dist,adv_pos_x,adv_pos_y;
	int min_dist = INT_MAX;
	char right,left,up,down;
	right = (pos_y == width-1) ? ' ' : map[pos_x][pos_y+1].character;
	left =  (pos_y == 0 )?  ' ' : map[pos_x][pos_y - 1].character;
	up = (pos_x == 0) ? ' ' : map[pos_x -1][pos_y].character;
	down = (pos_x == height-1) ? ' ' : map[pos_x+1][pos_y].character;
	
	//RIGHT
	if(right == 'H' || right == 'X')
	{
		
		sent->object_pos[counter].x = pos_x ;
		sent->object_pos[counter].y = pos_y + 1;
		counter++;
	}
	//LEFT
	if(left == 'H' || left == 'X')
	{
		sent->object_pos[counter].x = pos_x;
		sent->object_pos[counter].y = pos_y - 1;
		counter++;
	}
	//DOWN
	if(down == 'H' || down == 'X')
	{
		sent->object_pos[counter].x = pos_x + 1 ;
		sent->object_pos[counter].y = pos_y ;
		counter++;
	}
	//UP
	if(up == 'H' || up == 'X')
	{
		sent->object_pos[counter].x = pos_x - 1;
		sent->object_pos[counter].y = pos_y;
		counter++;
	}


	for(i = 0; i < prey_count ; i ++)
	{	
		if(arr_prey[i].x  >= 0 && arr_prey[i].y >= 0)
		{
			new_dist = abs(arr_prey[i].x - pos_x) + abs(arr_prey[i].y - pos_y);
			if(new_dist < min_dist)
			{
				min_dist = new_dist;
				adv_pos_x = arr_prey[i].x;
				adv_pos_y = arr_prey[i].y;
			}
		}
	}
	sent -> adv_pos.x = adv_pos_x;
	sent -> adv_pos.y = adv_pos_y;
	sent -> pos.x = pos_x;
	sent -> pos.y = pos_y; 
	sent -> object_count = counter;
	return sent;

}

//FOR PREY SERVER MESSAGE

server_message* send_prey(server_message* sent,int pos_x,int pos_y,int hunter_count,coordinate arr_hunter[hunter_count],int width,int height,element map[height][width])
{

	int counter = 0,i,new_dist,adv_pos_x = pos_x,adv_pos_y = pos_y;
	int min_dist = INT_MAX;
	
	char right,up,down,left;
	right = (pos_y == width-1) ? ' ' : map[pos_x][pos_y+1].character;
	left =  (pos_y == 0 )?  ' ' : map[pos_x][pos_y - 1].character;
	up = (pos_x == 0) ? ' ' : map[pos_x -1][pos_y].character;
	down = (pos_x == height-1) ? ' ' : map[pos_x+1][pos_y].character;
	
	//RIGHT
	if(right == 'X' || right == 'P')
	{	
		sent->object_pos[counter].x = pos_x ;
		sent->object_pos[counter].y = pos_y + 1;
		counter++;
	}
	//LEFT
	if(left == 'P' || left == 'X')
	{	
		sent->object_pos[counter].x = pos_x;
		sent->object_pos[counter].y = pos_y - 1;
		counter++;
	}
	//DOWN
	if(down == 'P' || down == 'X')
	{
		sent->object_pos[counter].x = pos_x + 1 ;
		sent->object_pos[counter].y = pos_y ;
		counter++;
	}
	//UP
	if(up == 'P' || up == 'X')
	{	
		sent->object_pos[counter].x = pos_x - 1;
		sent->object_pos[counter].y = pos_y;
		counter++;
	}


	for(i = 0; i < hunter_count ; i ++)
	{	
		if(arr_hunter[i].x >= 0 && arr_hunter[i].y >= 0)
		{
			new_dist = abs(arr_hunter[i].x - pos_x) + abs(arr_hunter[i].y - pos_y);
			if(new_dist < min_dist)
			{
				min_dist = new_dist;
				adv_pos_x = arr_hunter[i].x;
				adv_pos_y = arr_hunter[i].y;
			}
		}
	}
	sent->adv_pos.x = adv_pos_x;
	sent->adv_pos.y = adv_pos_y;
	sent->pos.x = pos_x;
	sent->pos.y = pos_y; 
	sent->object_count = counter;
	return sent;

}
