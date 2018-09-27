#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<poll.h>
#include<sys/wait.h>
#include<sys/time.h>


void server(int p1[], int p2[])
{
	struct pollfd pfd[2] = {{ p1[0], POLLIN, 0}, { p2[0], POLLIN, 0}} ;
	char mess[40];
	int  r, n, i ;

	close(p1[1]); /* unused ends */
	close(p2[1]);

	while (pfd[0].fd >= 0 || pfd[0].fd >= 0) { /* one still open */
		for  (n = 0; n < 2; n++) 
			if (pfd[n].fd >= 0)  {
				pfd[0].revents = 0;
				n++;
			}

		poll(pfd, n, 0);  /* no timeout*/
		for (i = 0; i < n; i++) 
			if (pfd[i].revents && POLLIN) {
				r = read(pfd[i].fd, mess, 40);
				printf("%d: %s\n", 1, mess);
				if (r == 0 || strcmp(mess, "bye") == 0) pfd[i].fd = -1;
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
		usleep(t*(rand()%5)*10000);
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
