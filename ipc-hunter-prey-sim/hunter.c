#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/types.h>

#define READ 0
#define WRITE 1


typedef struct coordinate {
	int x;
	int y;
} coordinate;


typedef struct ph_message {
	coordinate move_request;
} message;

typedef struct server_message {
	coordinate pos;
	coordinate adv_pos;
	int object_count;
	coordinate object_pos[4];
}server_message;


int main(int argc,char *argv[])
{
	
	int i,index;
	int width = atoi(argv[1]),height = atoi(argv[2]);
	int x = 0, y = 0;
	int requested_x,requested_y;
	int man_dist,dummy_dist;
	coordinate right;
	coordinate left;
	coordinate down;
	coordinate up;
	int isRight,isLeft,isUp,isDown;
	
	server_message take;
	message send;
	while(1)
	{	
		isRight = isLeft = isUp = isDown = 1;
		index = 0;
		read(READ,&take,sizeof(server_message)); // READ THE SERVER MESSAGE
		//fprintf(stderr, "SERVER SENT: %d ,%d ,%d ,%d ,%d\n",take.pos.x,take.pos.y,take.adv_pos.x,take.adv_pos.y,take.object_count);
		requested_x = x = take.pos.x;
		requested_y = y = take.pos.y;
		right.y = (y == width - 1 ? y : y + 1);
		right.x = x;
		left.y = (y == 0 ? 0: y -1);
		left.x = x;
		down.x = (x == height - 1 ? x : x + 1);
		down.y = y;
		up.x = (x == 0 ? 0: x -1);
		up.y = y;
		coordinate hostile_objects[4];
		for(i = 0; i < take.object_count ; i++)
		{
			if(right.x == take.object_pos[i].x && right.y == take.object_pos[i].y){
				isRight = 0;
			}
			if(down.x == take.object_pos[i].x && down.y == take.object_pos[i].y){
				isDown = 0;
			}
			if(up.x == take.object_pos[i].x && up.y == take.object_pos[i].y){
				isUp = 0;
			}
			if(left.x == take.object_pos[i].x && left.y == take.object_pos[i].y){
				isLeft = 0;
			}
		}
		if(isRight)
		{
			hostile_objects[index] = right;
			index++;
		}
		if(isLeft)
		{
			hostile_objects[index] = left;
			index++;
		}
		if(isUp)
		{
			hostile_objects[index] = up;
			index++;
		}
		if(isDown)
		{	
			hostile_objects[index] = down;
			index++;
		}
		man_dist = abs(take.adv_pos.x - x) + abs(take.adv_pos.y - y);
		for(i = 0; i < 4 - take.object_count; i++)
		{	
			dummy_dist = abs(take.adv_pos.x - hostile_objects[i].x) + abs(take.adv_pos.y - hostile_objects[i].y);
			//CALCULATING MANHATTAN DISTANCE --> (if man_dist == 0 -> PREY CAUGHT)
			if(dummy_dist < man_dist)
			{	

				requested_x = hostile_objects[i].x;
				requested_y = hostile_objects[i].y;
				man_dist = dummy_dist;
			}
		}
		send.move_request.x = requested_x;
		send.move_request.y = requested_y;
		write(WRITE,&send,sizeof(message)); // SEND REQUEST TO THE SERVER
		usleep(10000*(1+rand()%9));	// SLEEEP
		exit(0);	//QUIT
	}
}