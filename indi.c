#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

//when receive CTRL+C signal, prints out message and exxits program
void sigint_handler(int sig){
	printf("Keyboard Interrupted\n");
	exit(EXIT_SUCCESS);
}

int main(void){
	void sigint_handler(int sig);//call function

	int pipefds[2];
	char input[500];
	char buff[500];

	//precaution for signal error
	if(signal(SIGINT, sigint_handler)==SIG_ERR){
		perror("signal");
		exit(EXIT_SUCCESS);
	}

	if(pipe(pipefds)==-1){	//precaution for pipe error
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	pipe(pipefds);

	pid_t pid = fork();

	if(pid==0){ //child process
	//displays message receive from parent on terminal
		for(int i=0;i<3;i++){
			sleep(1);

			close(pipefds[1]);
			read(pipefds[0], &buff, sizeof(buff));
			close(pipefds[0]);
			printf("Child received message: %s", buff);
		}
		exit(EXIT_SUCCESS);
	}
	else if(pid==-1){ //precaution for fork error
		perror("fork");
		exit(EXIT_SUCCESS);
	}
	else{ //parent process
	//asks user input and will fork into child process with pipe
		close(pipefds[0]);
		printf("Enter a message: ");
		fgets(input, sizeof(input), stdin);
		write(pipefds[1], &input, sizeof(input));
		close(pipefds[1]);
		wait(NULL);

	}

	printf("\nProcess is done. \n");
	return EXIT_SUCCESS;
}
