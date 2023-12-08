#define _GNU_SOURCE		/* needed for getline in c89??? */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void displayPrompt();

size_t max_size = 64;
int running = 1;

char *hostname;
char *username;

void interruptHandler(int signum) {
	running = 0;
}

int main()
{
	int i;
	size_t n = 10;
	pid_t pid = getpid();

	char *command = NULL;
	char *new_command[10];

	signal(SIGINT, interruptHandler);			/* goes to the function when ctrl+c is pressed				*/

	hostname = (char *)malloc(max_size);		/* would sometimes crash other times not if not for malloc	*/
	if (gethostname(hostname, max_size) == -1)
		printf("Couldn't get hostname");
	if ((username = getlogin()) == NULL)
		printf("Couldn't get username");

	while (running) {
		fflush(NULL);							/* flush all data streams, because it's probally good (idrk tho) */

		if (pid == 0) {							/* if (child_process) */
			execve(new_command[0], new_command, NULL);
			return 0;
		} else {
			displayPrompt();

			if (getline(&command, &n, stdin) == -1)
				interruptHandler(0);

			new_command[0] = strtok(command, " ");								/* break up command				*/
			for (i = 1; (new_command[i] = strtok(NULL, " ")); i++)				/* put in char *new_command[]	*/
				new_command[i][strcspn(new_command[i], "\n")] = '\0';			/* remove newline from EOL		*/

			pid = fork();
			wait(NULL);
		}
	}
	free(hostname);
	return 0;
}

void displayPrompt() {
	char *cwd;

	if ((cwd = getcwd(NULL, 128)) == NULL)
		printf("Couldn't get current directory");

	printf("%s@%s %s$ ", hostname, username, cwd);
}
