#include "do_not_submit.h"
#include <pthread.h>
#include <semaphore.h>

struct location{
	int ID;
	int x;
	int y;
	char identity;
};

bool isExit = 0;
int finish_time;
time_t start_time,end_time;// START TIME OF PROGRAM
pthread_mutex_t mutex_array [GRIDSIZE][GRIDSIZE] = PTHREAD_MUTEX_INITIALIZER; // mutex for cells
pthread_cond_t sleep_condition = PTHREAD_COND_INITIALIZER;	// condition variable for sleep
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;	// Mutex for condition variable


bool isStucked(struct location* temp)
{
	int i,j;
	int x = temp -> x;
	int y = temp -> y;
	for(i = -1; i <= 1;i++)
	{
		for(j = -1 ; j <= 1;j++)
		{
			if( !( i==0 && j==0) && (x+i >= 0) && (y+j >= 0) && (x+i < GRIDSIZE) && (y+j < GRIDSIZE))	// EDGE CASES
			{
				pthread_mutex_lock(&mutex_array[x+i][y+j]);	// LOCK CELL AND LOOK AT IT
				if(lookCharAt(x+i,y+j) == '-')	//FOOD FOUNDED
				{	
					pthread_mutex_unlock(&mutex_array[x+i][y+j]);	// RELEASE FOOD CELL AFTER LOOK AT IT
					return false;

				}
				pthread_mutex_unlock(&mutex_array[x+i][y+j]);		//FOOD NOT FOUNDED RELEASE CELL
					
			}
			
		}
	}
	return true;
}


void tired_ant(struct location* temp)
{
	int i,j;
	int valid_x,valid_y;
	int x = temp -> x;
	int y = temp -> y;
	char identity = temp -> identity;
	while(true)
	{	
		if(isStucked(temp))
			return;
		valid_x = x + (rand() % 3 - 1);
		valid_y = y + (rand() % 3 - 1);
		if((valid_x != x || valid_y != y) && (valid_x >= 0) && (valid_y >= 0) && (valid_x < GRIDSIZE) && (valid_y < GRIDSIZE))
		{	
			pthread_mutex_lock(&mutex_array[valid_x][valid_y]); // LOCK CELL AND LOOK AT IT
			if(lookCharAt(valid_x,valid_y) == '-')
			{	
				putCharTo(valid_x,valid_y,'1');
				temp -> x = valid_x;
				temp -> y = valid_y;
				temp -> identity = '1';
				pthread_mutex_unlock(&mutex_array[valid_x][valid_y]);	//EMPTY CELL FOUNDED, RELASE CELL

				pthread_mutex_lock(&mutex_array[x][y]);	// LOCK AND MAKE YOUR OWN CELL EMPTY
				putCharTo(x,y,'-');
				pthread_mutex_unlock(&mutex_array[x][y]);	// RELEASE YOUR OWN CELL
				return;
			}
			pthread_mutex_unlock(&mutex_array[valid_x][valid_y]);		// CELL IS NOT EMPTY,RELEASE CELL
		}
	}
}

void become_tired_ant(struct location* temp)
{
	int i,j;
	int valid_x,valid_y;
	int x = temp -> x;
	int y = temp -> y;
	char identity = temp -> identity;
	while(true)
	{	
		if(isStucked(temp))
			return;
		valid_x = x + (rand() % 3 - 1);
		valid_y = y + (rand() % 3 - 1);
		if((valid_x != x || valid_y != y) && (valid_x >= 0) && (valid_y >= 0) && (valid_x < GRIDSIZE) && (valid_y < GRIDSIZE))
		{	
			pthread_mutex_lock(&mutex_array[valid_x][valid_y]); // LOCK CELL AND LOOK AT IT
			if(lookCharAt(valid_x,valid_y) == '-')
			{	
				putCharTo(valid_x,valid_y,'1');
				temp -> x = valid_x;
				temp -> y = valid_y;
				temp -> identity = 't';
				pthread_mutex_unlock(&mutex_array[valid_x][valid_y]);	//EMPTY CELL FOUNDED, RELASE CELL

				pthread_mutex_lock(&mutex_array[x][y]);	// LOCK AND MAKE YOUR OWN CELL EMPTY
				putCharTo(x,y,'o');
				pthread_mutex_unlock(&mutex_array[x][y]);	// RELEASE YOUR OWN CELL
				return;
			}
			pthread_mutex_unlock(&mutex_array[valid_x][valid_y]);		// CELL IS NOT EMPTY,RELEASE CELL
		}
	}
}

void check_neighbours_with_food(struct location* temp){
	int i,j;
	int x = temp -> x;
	int y = temp -> y;
	int valid_y,valid_x;
	for(i = -1; i <= 1;i++)
	{
		for(j = -1 ; j <= 1;j++)
		{
			if( ( i != 0 || j != 0) && (x+i >= 0) && (y+j >= 0) && (x+i < GRIDSIZE) && (y+j < GRIDSIZE))	// EDGE CASES
			{
				pthread_mutex_lock(&mutex_array[x+i][y+j]);	// LOCK CELL AND LOOK AT IT
				if(lookCharAt(x+i,y+j) == 'o')	//FOOD FOUNDED
				{	
					pthread_mutex_unlock(&mutex_array[x+i][y+j]);	// RELEASE FOOD CELL AFTER LOOK AT IT
					become_tired_ant(temp);
					return;

				}
				pthread_mutex_unlock(&mutex_array[x+i][y+j]);		//FOOD NOT FOUNDED RELEASE CELL
					
			}
			
		}
	}
	while(true)
	{	
		if(isStucked(temp))
			return;
		valid_x = x + (rand() % 3 - 1);
		valid_y = y + (rand() % 3 - 1);
		if((valid_x != x || valid_y != y) && (valid_x >= 0) && (valid_y >= 0) && (valid_x < GRIDSIZE) && (valid_y < GRIDSIZE))
		{	
			pthread_mutex_lock(&mutex_array[valid_x][valid_y]); // LOCK CELL AND LOOK AT IT
			if(lookCharAt(valid_x,valid_y) == '-')
			{	
				
				putCharTo(valid_x,valid_y,'P');
				temp -> x = valid_x;
				temp -> y = valid_y;
				pthread_mutex_unlock(&mutex_array[valid_x][valid_y]);	//EMPTY CELL FOUNDED, RELASE CELL

				pthread_mutex_lock(&mutex_array[x][y]);	// LOCK AND MAKE YOUR OWN CELL EMPTY
				putCharTo(x,y,'-');
				pthread_mutex_unlock(&mutex_array[x][y]);	// RELEASE YOUR OWN CELL
				return;
			}
			pthread_mutex_unlock(&mutex_array[valid_x][valid_y]);		// CELL IS NOT EMPTY,RELEASE CELL
		}
	}
}

void check_neighbours_normal(struct location* temp) // HELPER FUNCTION FOR CHECKING NEIGHBOURS
{	
	int i,j;
	int valid_x,valid_y;
	int x = temp -> x;
	int y = temp -> y;
	char identity = temp -> identity;
	for(i = -1; i <= 1;i++)
	{
		for(j = -1 ; j <= 1;j++)
		{
			if( !( i==0 && j==0) && (x+i >= 0) && (y+j >= 0) && (x+i < GRIDSIZE) && (y+j < GRIDSIZE))	// EDGE CASES
			{
				pthread_mutex_lock(&mutex_array[x+i][y+j]);	// LOCK CELL AND LOOK AT IT
				if(lookCharAt(x+i,y+j) == 'o')
				{	
					putCharTo(x+i,y+j,'P');
					temp -> x = x + i;
					temp -> y = y + j;
					temp -> identity = 'P';
					pthread_mutex_unlock(&mutex_array[x+i][y+j]);	//FOOD FOUNDED, RELASE CELL

					pthread_mutex_lock(&mutex_array[x][y]);	// LOCK AND MAKE YOUR OWN CELL EMPTY
					putCharTo(x,y,'-');
					pthread_mutex_unlock(&mutex_array[x][y]);	// RELEASE YOUR OWN CELL
					return;
				}
				pthread_mutex_unlock(&mutex_array[x+i][y+j]);	// FOOD NOT FOUNDED, RELEASE CELL
			}
			
		}
	}
	while(true)
	{	
		if(isStucked(temp))
			return;
		valid_x = x + (rand() % 3 - 1);
		valid_y = y + (rand() % 3 - 1);
		if((valid_x != x || valid_y != y) && (valid_x >= 0) && (valid_y >= 0) && (valid_x < GRIDSIZE) && (valid_y < GRIDSIZE))
		{	
			pthread_mutex_lock(&mutex_array[valid_x][valid_y]); // LOCK CELL AND LOOK AT IT
			if(lookCharAt(valid_x,valid_y) == '-')
			{	
				
				putCharTo(valid_x,valid_y,'1');
				temp -> x = valid_x;
				temp -> y = valid_y;
				pthread_mutex_unlock(&mutex_array[valid_x][valid_y]);	//EMPTY CELL FOUNDED, RELASE CELL

				pthread_mutex_lock(&mutex_array[x][y]);	// LOCK AND MAKE YOUR OWN CELL EMPTY
				putCharTo(x,y,'-');
				pthread_mutex_unlock(&mutex_array[x][y]);	// RELEASE YOUR OWN CELL
				return;
			}
			pthread_mutex_unlock(&mutex_array[valid_x][valid_y]);		// CELL IS NOT EMPTY,RELEASE CELL
		}
	}
}


void *ant_thread(void* location){
	struct location *temp_coord = (struct location*) location;
	char identity = temp_coord -> identity;
    int x = temp_coord -> x;
    int y = temp_coord -> y;
    int ID = temp_coord -> ID;
	while(true)
	{	
    	if(isExit)	// FINISH PROGRAM GIVEN AMOUNT OF TIME
    	{	
    		free(temp_coord);
    		temp_coord = NULL;
    		pthread_exit(NULL);
    	}
		// OPERATIONS
		if(identity == '1')
		{
			while(getSleeperN() > ID && !isExit)
			{	
				if(lookCharAt(x,y) != 'S')
				{											// IF NEWLY SLEEPED PUT CHAR 'S'
					pthread_mutex_lock(&mutex_array[x][y]);		// LOOKS ITS OWN CELL so JUST LOCK FOR WRITING
					putCharTo(x,y,'S');
					pthread_mutex_unlock(&mutex_array[x][y]);
				}
				pthread_mutex_lock(&mutex);
				pthread_cond_wait(&sleep_condition,&mutex);		// WAIT ON SLEEP
				pthread_mutex_unlock(&mutex);		// UNLOCK CONDITION MUTEX
			}
			if(lookCharAt(x,y) != '1'){	// MAKE SLEEPY ANT GREAT(NORMAL) AGAIN!!
            	pthread_mutex_lock(&mutex_array[x][y]); 	
            	putCharTo(x,y,'1');
				pthread_mutex_unlock(&mutex_array[x][y]);
			}
			if(isExit)	// FINISH PROGRAM GIVEN AMOUNT OF TIME
    		{	
    			free(temp_coord);
    			temp_coord = NULL;
    			pthread_exit(NULL);
    		}
            check_neighbours_normal(temp_coord);
            identity = temp_coord -> identity;
       		x = temp_coord -> x;
        	y = temp_coord -> y;
		}
		else if(identity == 't')
		{
			while(getSleeperN() > ID && !isExit)
			{	
				if(lookCharAt(x,y) != 'S')
				{									// IF NEWLY SLEEPED PUT CHAR 'S'
					pthread_mutex_lock(&mutex_array[x][y]);		// LOOKS ITS OWN CELL so JUST LOCK FOR WRITING
					putCharTo(x,y,'S');
					pthread_mutex_unlock(&mutex_array[x][y]);
				}
				pthread_mutex_lock(&mutex);
				pthread_cond_wait(&sleep_condition,&mutex);
				pthread_mutex_unlock(&mutex);
			}
			if(lookCharAt(x,y) != '1'){		// MAKE SLEEPY ANT GREAT(NORMAL) AGAIN!!
            	pthread_mutex_lock(&mutex_array[x][y]); 	
            	putCharTo(x,y,'1');
				pthread_mutex_unlock(&mutex_array[x][y]);
			}
			if(isExit)	// FINISH PROGRAM GIVEN AMOUNT OF TIME
    		{	
    			free(temp_coord);
    			temp_coord = NULL;
    			pthread_exit(NULL);
    		}
			tired_ant(temp_coord);
            identity = temp_coord -> identity;
       		x = temp_coord -> x;
        	y = temp_coord -> y;	
		}
        else if(identity == 'P')	// FOOD CARRYING ANT
        {	
        	while(getSleeperN() > ID && !isExit)
			{
				if(lookCharAt(x,y) != '$')
				{									// IF NEWLY SLEEPED PUT CHAR '$'
					pthread_mutex_lock(&mutex_array[x][y]);		// LOOKS ITS OWN CELL so JUST LOCK FOR WRITING
					putCharTo(x,y,'$');
					pthread_mutex_unlock(&mutex_array[x][y]);
				}
				pthread_mutex_lock(&mutex);
				pthread_cond_wait(&sleep_condition,&mutex);
				pthread_mutex_unlock(&mutex);

			}
			if(isExit)	// FINISH PROGRAM GIVEN AMOUNT OF TIME
    		{	
    			free(temp_coord);
    			temp_coord = NULL;
    			pthread_exit(NULL);
    		}
			if(lookCharAt(x,y) != 'P'){	// MAKE SLEEPY ANT GREAT(NORMAL) AGAIN!!
            	pthread_mutex_lock(&mutex_array[x][y]); 	
            	putCharTo(x,y,'P');
				pthread_mutex_unlock(&mutex_array[x][y]);
			}
            check_neighbours_with_food(temp_coord);
            identity = temp_coord -> identity;
       		x = temp_coord -> x;
        	y = temp_coord -> y;
        }
		usleep(getDelay() * 1000 + (rand() % 10)*1000);	// DELAY BETWEEN 50 MS AND 59 MS
	}
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    
    time(&start_time);

    int  ant_count = atoi(argv[1]);	// NUMBER OF ANTS
    int  food_count = atoi(argv[2]);// NUMBER OF FOODS
    finish_time = atoi(argv[3]); // FINISH TIME

    struct location* delete[ant_count];	// DELETE ANT STRUCTS WITH THIS ARRAY

    pthread_t ants[ant_count];	// DECLARE THREAD TIDS

    int i,j;
    for (i = 0; i < GRIDSIZE; i++) {		// PUT EMPTY CELLS
        for (j = 0; j < GRIDSIZE; j++) {
            putCharTo(i, j, '-');
        }
    }
    int a,b;
    for (i = 0; i < food_count; i++){			// PUT FOODS ON THE GRID
        do {
            a = rand() % GRIDSIZE;
            b = rand() % GRIDSIZE;
        }while (lookCharAt(a,b) != '-');
        putCharTo(a, b, 'o');
    }
    for (i = 0; i < ant_count; i++) {		// PUT ANTS ON THE GRID
        do {
            a = rand() % GRIDSIZE;
            b = rand() % GRIDSIZE;
        }while (lookCharAt(a,b)!= '-');
        putCharTo(a, b, '1');
        
        delete[i] = malloc(sizeof(struct location));
        
        delete[i] -> x = a;
        delete[i] -> y = b;
        delete[i] -> ID = i;
        delete[i] -> identity = '1';
        
        pthread_create(&ants[i],NULL,ant_thread,(void *) delete[i]);
    }

    // you have to have following command to initialize ncurses.
    startCurses();
    
    // You can use following loop in your program. But pay attention to 
    // the function calls, they do not have any access control, you 
    // have to ensure that.
    char c;
    while (TRUE) {
    	time(&end_time);
    	if(difftime(end_time,start_time) >= finish_time)	// FINISH PROGRAM GIVEN AMOUNT OF TIME
    	{	
			break;
    	}
        drawWindow();
        
        c = 0;
        c = getch();

        if (c == 'q' || c == ESC) break;
        if (c == '+') {
            setDelay(getDelay()+10);
        }
        if (c == '-') {
            setDelay(getDelay()-10);
        }
        if (c == '*') {
        	if(getSleeperN() < ant_count)
            	setSleeperN(getSleeperN()+1);
        }
        if (c == '/') {
            setSleeperN(getSleeperN()-1);
            pthread_cond_broadcast(&sleep_condition); //MAKE BROADCAST
            
        }
        usleep(DRAWDELAY);
        
        // each ant thread have to sleep with code similar to this
        //usleep(getDelay() * 1000 + (rand() % 5000));
    }
    
    // do not forget freeing the resources you get
    endCurses();
	
	isExit = true;
	pthread_cond_broadcast(&sleep_condition);	// RELASE WAITING THREADS ON SLEEP
	
    
    


    for(i = 0 ; i < ant_count ; i++){	// WAIT FOR THREAD TO END
    	pthread_join(ants[i],NULL);
    }
    for(i = 0 ; i < ant_count; i++){	// MAKE NULL ANT STRUCT POINTERS
    	delete[i] = NULL;
    }
 	return 0;
}

