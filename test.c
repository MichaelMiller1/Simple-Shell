#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

void main() {
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	char input[1024];
	char path[1024];
	struct node *head = NULL;
	char *param[101];
	struct node *next;
	path[0] = '\0';
	char *ptr = NULL;
	while(1){
		//initial print; scan input
		printf("%s~$ ", cwd);
		fgets(input, 100, stdin);

		//formats input properly, removes trailing newline, checks to make sure string is not empty (segfault on empty string)
		if ((ptr = strchr(input, '\n')) != NULL){
			*ptr = '\0';
			if(input[0] == '\0'){
				printf("No input detected.\n");
				continue;
			}
		} else {
			printf("Input too long.\n");
			continue;
		}

		//breaks input string into tokens, incase of multiple token inputs
		ptr = strtok(input, " ");

		//cases possible for commands
		if (strcmp(ptr,"cd") == 0){
			ptr = strtok(NULL, " ");
			if (ptr != NULL){
				if (chdir(ptr) != 0){
					perror("ERROR:");
				}
			} else {
				printf("No directory specified.\n");
			}
			getcwd(cwd, sizeof(cwd));
		} else if (strcmp(ptr,"path") == 0){
			ptr = strtok(NULL, " ");
			if (ptr != NULL){
				if (strcmp(ptr,"+") == 0){
					//add elements to path
					ptr = strtok(NULL, " ");
					if (ptr == NULL){
						continue;
					}
					//if first element, don't add ':'
					if (path[0] != '\0'){
						strcat(path, ":");
						strcat(path, ptr);
					} else {
						strcat(path, ptr);
					}
				} else if (strcmp(ptr,"-") == 0){
					//remove elements from path
					ptr = strtok(NULL, " ");
					if (ptr == NULL){
						path[0] = '\0';
						continue;
					}
					//temp path to store new path
					char temppath[1024];
					temppath[0] = '\0';
					char *searchptr = NULL;
					searchptr = strtok(path, ":");
					//loop to remove duplicates
					while(searchptr != NULL){
						if (strcmp(searchptr, ptr) != 0){
							if (temppath[0] != '\0'){
								strcat(temppath, ":");
								strcat(temppath, searchptr);
							} else {
								strcat(temppath, searchptr);
							}
						}
						searchptr = strtok(NULL, ":");
					}
					//restore path
					strcpy(path, temppath);
				} else {
					printf("Invalid command parameter for path.\n");
				}
			} else {
				printf("%s\n", path);
			}
		} else if (strcmp(ptr, "quit") == 0){
			exit(0);
		} else {
			if(fork() == 0){
				//child

				//construct executable parameter list
				char *param[11] = {NULL};
				param[0] = ptr;
				for (int i = 1; i < 10; i++){
					ptr = strtok(NULL, " ");
					if (ptr != NULL){
						param[i] = ptr;
					} else {
						param[i] = NULL;
					}
				}
				if ((ptr = strtok(NULL, " ")) != NULL){
					printf("Too many parameters.\n");
					continue;
				}

				//ensures path is not empty
				char *pathptr = NULL;
				if ((pathptr = strtok(path, ":")) == NULL){
					printf("Path is empty.\n");
					exit(0);
				}


				//loop through path
				char *execpath = (char*)malloc(sizeof(char)*1024);
				while(pathptr != NULL){
					strcpy(execpath, pathptr);
					strcat(execpath, "/");
					strcat(execpath, param[0]);
					execv(execpath, param);
					pathptr = strtok(NULL, ":");
				}


				printf("Executable not found in path locations.\n");
				exit(0);
			} else {
				//parent
				wait(NULL);
			}
			/*
      // I/O REDIRECTION 
      struct node *temp = head;

			temp = head;
						while(temp){
							if (temp->next){
								//redirects, pipes
								//temp = temp->next;

								if(strcmp(temp->next->param[0],"<") == 0){
									//input redir
									if(fork() == 0){
										//child

										//failsafe for if nothing after redirect symbol
										if (!temp->next->next){
											printf("No file specified for input redirect.\n");
											break;
										}
										//open file for reading
										int fd = open(temp->next->next->param[0], O_RDONLY);
										if (fd == -1){
											perror("Error opening file");
											break;
										}
										//create copy of fd at stdin
										close(0);
										dup(fd);

										//ensures path is not empty
										char *pathptr = NULL;
										if ((pathptr = strtok(path, ":")) == NULL){
											printf("Path is empty.\n");
											exit(0);
										}

										//loop through path
										char *execpath = (char*)malloc(sizeof(char)*1024);
										while(pathptr != NULL){
											strcpy(execpath, pathptr);
											strcat(execpath, "/");
											strcat(execpath, temp->param[0]);
											execv(execpath, temp->param);
											pathptr = strtok(NULL, ":");
										}


										printf("Executable not found in path locations.\n");
										exit(0);
									} else {
										//parent
										if (temp->next->next){
											if (temp->next->next->next){
												temp = temp->next->next->next;
											} else {
												temp = NULL;
											}
										} else {
											temp = NULL;
										}
										wait(NULL);
									}
								//end of input redirection
								} else if (strcmp(temp->next->param[0],">") == 0){
									//output redir
									if(fork() == 0){
										//child
										if (!temp->next->next){
											printf("No file specified for output redirect.\n");
											break;
										}
										int fd = open(temp->next->next->param[0], O_WRONLY|O_CREAT, S_IRWXU);
										if (fd == -1){
											perror("Error opening file.");
											break;
										}
										close(1);
										dup(fd);


										//ensures path is not empty
										char *pathptr = NULL;
										if ((pathptr = strtok(path, ":")) == NULL){
											printf("Path is empty.\n");
											exit(0);
										}

										//loop through path
										char *execpath = (char*)malloc(sizeof(char)*1024);
										while(pathptr != NULL){
											strcpy(execpath, pathptr);
											strcat(execpath, "/");
											strcat(execpath, temp->param[0]);
											execv(execpath, temp->param);
											pathptr = strtok(NULL, ":");
										}


										printf("Executable not found in path locations.\n");
										exit(0);
									} else {
										//parent
										if (temp->next->next){
											if (temp->next->next->next){
												temp = temp->next->next->next;
											} else {
												temp = NULL;
											}
										} else {
											temp = NULL;
										}
										wait(NULL);
									}
								//end output redir 
                
                */
		}

	}//while loop end

}

