#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/select.h>
#include<sys/wait.h>
#include<sys/time.h>


void server(int p1[], int p2[])
{
	fd_set readset;
	char mess[40];
	int m, r;
	int open[2] = {1,1}; /* if pipe is still open */

	close(p1[1]); /* unused ends */
	close(p2[1]);

	m = ((p1[0] > p2[0]) ? p1[0] : p2[0]) + 1; /* maximum of file descriptors */

	while (open[0] || open[1]) {
		FD_ZERO(&readset);
		if (open[0]) FD_SET(p1[0],&readset);
		if (open[1]) FD_SET(p2[0],&readset);

		select(m, &readset, NULL,NULL,NULL);  /* no wset, eset, timeout */
		if (FD_ISSET(p1[0], &readset)) {
			r = read(p1[0], mess, 40);
			printf("%d: %s\n", 1, mess);
			if (r == 0 || strcmp(mess, "bye") == 0) open[0] = 0;
		}
		if (FD_ISSET(p2[0], &readset)) {
			r = read(p2[0], mess, 40);
			printf("%d: %s\n", 2, mess);
			if ( r == 0 || strcmp(mess, "bye") == 0) open[1] = 0;
		}

	}

}

int child(int p[], int t, char *mess)
{
	int i;
	
	struct timeval now;
	gettimeofday(&now, NULL);
	srand(now.tv_sec + now.tv_usec);
	close(p[0]);
	for (i = 0; i < 200; i += t) {
		usleep(t*(rand()%10)*10000);
		write(p[1], mess, strlen(mess)+1);
	}
	usleep(100000);
	write(p[1],"bye",4);
	close(p[1]);
	return 0;
}


int main() {
	int pipe1[2];
	int pipe2[2];
	int w;

	pipe(pipe1);
	pipe(pipe2);
 	if (fork()) {
		if (fork()) {
			server(pipe1, pipe2);
			wait(&w);
			wait(&w);
		} else {
			return child(pipe1,10,"trololo");
		}
	} else {
			return child(pipe2,5,"lalala");
	}

	return 0;
}
