#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_ARGS 255


void divideString(char *cmd, char *delim, char **argv);

int main()
{
	while (1) {
		char *cmd = NULL; char *delim = " \n"; size_t n = 0;
		int i; char **argv = NULL; pid_t pid = getpid();

		printf("> ");
		if (getline(&cmd, &n, stdin) == -1)
			return -1;

		if ((argv = (char **)malloc(sizeof(char *) * MAX_ARGS)) == NULL) {
			perror("Could not allocate memory");
			exit(EXIT_FAILURE);
		}
		divideString(cmd, delim, argv);
		free(cmd);

		pid = fork();
		if (pid == 0)
			if (execv(argv[0], argv) == -1) {
				perror("Could not execute command");
				exit(EXIT_FAILURE);
			}
		wait(&pid);

		for (i = 0; argv[i] != NULL; i++)
			free(argv[i]);
		free(argv);
	}

	return 0;
}

void divideString(char *cmd, char *delim, char **argv) {
	char *token	= NULL;												/* One Ring */
	int8_t i = 0;
	char *cmd_cpy = strdup(cmd);

	token = strtok(cmd_cpy, delim);									/* To Rule  */
	for (i = 0; token; i++) {
		argv[i] = strdup(token);
		token = strtok(NULL, delim);								/* Them All */
	}
	argv[i] = NULL;
	free(cmd_cpy);
}
