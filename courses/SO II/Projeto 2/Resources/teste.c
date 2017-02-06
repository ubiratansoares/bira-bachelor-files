#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sched.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>


//#define MODE_NORMAL 0
//#define MODE_REALTIME 1
//#define MODE_RR 2

#define MAX_FORKS 10

typedef struct {
	struct sched_param sched_param;
	int type;
} child_param;

int main() {
	int current_scheduler;
	int child_count;
	int i;
	struct timeval start_time, end_time;
	char command[255];
	int prio;
	//int mode = MODE_NORMAL; //all realtime, all normal, mixed
	pid_t pid;
	child_param child[MAX_FORKS];
	//realtime_policy.sched_priority = sched_get_priority_max(SCHED_FIFO) - 1;
	printf("Max prio: %d\n", sched_get_priority_max(SCHED_RR));
	printf("Select the scheduler policy(0 = Normal, 1 = FIFO, 2 = Round-Robin): ");
	scanf("%d", &current_scheduler);

	printf("How many childs? (1-%d): ", MAX_FORKS);
	scanf("%d", &child_count);

	for (i=0;i<child_count;i++) {
		printf("Enter priority for child %d: ", i);
		scanf("%d", &prio);
		child[i].sched_param.sched_priority = prio;
		printf("Enter process type (0 = IO bound, 1 = CPU bound, 2 = mixed): ");
		scanf("%d", &(child[i].type));
	}


	gettimeofday(&start_time, NULL);
	for (i=0;i<child_count;i++) {
		if ((pid = fork())) {
			//parent process
			//printf("P, %d, %d, tempo\n", i, pid);
			sched_setscheduler(pid, current_scheduler, &(child[i].sched_param));
		}
		else {
			//child process
			sleep(1); //wait for priority change
			switch (child[i].type) {
				case 0:
					sprintf(command, "./iobound.sh %d", getpid());
					system(command); //IO Bound
					break;
				case 1:
					system("./cpubound.sh"); //CPU Bound
					break;
				case 2:
					system("./mixed.sh"); //mixed
					break;
			}
			gettimeofday(&end_time, NULL);
			printf("%d, %lf\n", getpid(), (difftime(end_time.tv_sec, start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec)/1000000.0) - 1);
			//printf("cabou\n");
			return 0;
		}
	}
	for (i=0;i<child_count;i++) {
		wait(NULL);
	}
	return 0;
}

