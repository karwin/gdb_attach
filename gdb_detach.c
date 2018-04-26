#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <signal.h>
#include <pthread.h>

inline __attribute__((always_inline)) static int be_attached_check(void)
{
	char filename[128];
	char line[128];
	int pid = getpid();
	int ret=-1;
	FILE *fd = NULL;
	
	snprintf(filename,sizeof(filename), "/proc/%d/status", pid);

	if ((fd=fopen(filename, "r")) != NULL){
		while (fgets(line, sizeof(line), fd)){
			if (strncmp(line, "TracerPid", 9) == 0)	{
				printf("%s\n", line);
				int statue = atoi(line+10);
				if (statue != 0){
					printf("be attached !! kill %d", pid);
					fclose(fd);
					kill(pid, SIGKILL);
				}else{
					ret = 0;
				}
				break;
			}
		}
		fclose(fd);
	} else {
		printf("open fail...\n");
	}
	return ret;
}

int main(void)
{
	while(1){
		sleep(1);
		be_attached_check();
		printf("program is running\n");
	}
}
