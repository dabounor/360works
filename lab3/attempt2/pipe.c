#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

char *HOME = "/home/dara";
char *name[128];
int n, m;
int stdcp;
char *paths[256];
char args[128];

void main(int argc, char *argv[], char *env[])
{
	int isPipe, pipeNum;
	char *head[128];
	char *tail[128];
	tokenizePath(getenv("PATH"));
	///for(int i = 0; i < m; i++)
	//	printf("%s\n", paths[i]);
	while(1){
		isPipe = 0;
		stdcp = dup(1);
		printf("Please enter a command:\n");
		fgets(args, 128, stdin);
		//name[0] =  "./a.out";
		tokenize(args);
		name[n - 1][strlen(name[n - 1]) - 1] = '\0';
		checkPipe(&isPipe, &pipeNum, head, tail);	
		//for(int i = 0; i < n; i++)
		//	printf("%s\n", name[i]);
		if(!isPipe)
		{
			activate(name, n, env);
		}
		else
		{
			doPipe(head, tail, n, pipeNum, env);
		}
	}
}
	
void doPipe(char *head[], char *tail[], int n, int pipeNum, char *env[])
{
	int pd[2], pid;
	//printf("%s\t%s\n", head[0], tail[0]);
	pipe(pd); // creates a PIPE
	pid = fork(); // fork a child (to share the PIPE)
	if (pid)
	{ // parent as pipe WRITER
		close(pd[0]); // WRITER MUST close pd[0]
		close(1); // close 1
		dup(pd[1]); // replace 1 with pd[1]
		close(pd[1]); // close pd[1]
		activatePipe(head, pipeNum, env); // change image to cmd1
	}
	else
	{ // child as pipe READER
		close(pd[1]); // READER MUST close pd[1]
		close(0);
		dup(pd[0]); // replace 0 with pd[0]
		close(pd[0]); // close pd[0]
		activatePipe(tail, n - pipeNum, env); // change image to cmd2
		exit(0);
	}
}

void checkPipe(int *isPipe, int *pipeNum, char *head[], char *tail[])
{
	for(int i = 0; i < n; i++){
		if(strcmp(name[i], "|") == 0){
			*isPipe = 1;
			*pipeNum = i;
			break;
		}
	}
	if (*isPipe)
	{
		for(int i = 0; i < *pipeNum; i++)
			head[i] =  name[i];
		for(int i = *pipeNum + 1; i < n; i++)
			tail[i - *pipeNum - 1] = name[i];
		head[*pipeNum] = NULL;
		tail[n - *pipeNum - 1] = NULL;
	}
}

void searchPath(char cmd[])
{
	for(int i = 0; i < m; i++)
		{
			strcpy(cmd, paths[i]);
			strcat(cmd, "/");
			strcat(cmd, name[0]);
			if(!access(cmd, F_OK))
			{
				//printf("%s\n", cmd);
				break;
			}
			
		}
}

void findCarrot()
{
	if(strcmp(name[n - 2], "<") == 0 || strcmp(name[n - 2], ">") == 0 || strcmp(name[n - 2], ">>") == 0)
		{
			if(strcmp(name[n - 2], "<") == 0)
			{
				FILE *infile = fopen(name[n - 1], "r");
				char buf[256];
				while(!feof(infile))
				{
					char *s;
					n -= 2;
					fgets(buf, 256, infile);
					s = strtok(buf, " ");
					while(s){
						name[n] = s;
						s = strtok(0, " ");
						n++;
					}

					name[n] = NULL;

				}
			}
			else if(strcmp(name[n - 2], ">") == 0)
			{
				close(1);
				open(name[n - 1], O_WRONLY|O_CREAT, 0644);
				name[n - 2] = NULL;

			}
			else if(strcmp(name[n - 2], ">>") == 0)
			{
				close(1);
				open(name[n - 1], O_WRONLY|O_CREAT|O_APPEND, 0644);
				name[n - 2] = NULL;
			}
		}
}

void activatePipe(char *pName[], int pN, char *pEnv[])
{
	char cmd[128];	
	searchPath(cmd);
	int status;
	if(pN > 2){
		findCarrot();	
	}
	if (strcmp("cd", pName[0]) == 0){
		if (pName[1])
		{ 
			chdir(pName[1]);
		}
		else
		{
			chdir("/home/dara");
		}
	}
	if (strcmp("exit", pName[0]) == 0){
		exit(0);
	}
	else
	{
		if(fork() == 0)
		{
			
			//printf("%s\n", cmd);
			//for (int a = 0; a<pN; ++a)
			//	printf("\t%s\n", pName[a]);
			execve(cmd, pName, pEnv);
			exit(0);
		}
		else
		{
			
			wait(&status);
			printf("child's exit status code: %d\n", status);
			
		}
	}
}

void activate(char *name[], int n, char *env[])
{
	char cmd[128];	
	searchPath(cmd);
	int status;
	if(n > 2){
		findCarrot();	
	}
	if (strcmp("cd", name[0]) == 0){
		if (name[1])
		{ 
			chdir(name[1]);
		}
		else
		{
			chdir("/home/dara");
		}
	}
	if (strcmp("exit", name[0]) == 0){
		exit(0);
	}
	else
	{
		if(fork() == 0)
		{
			
			//printf("%s\n", cmd);
			//for (int a = 0; a<n; ++a)
			//	printf("\t%s\n", name[a]);
			execve(cmd, name, env);
			exit(0);
		}
		else
		{
			
			wait(&status);
			close(1);
			dup2(stdcp, 1);
			printf("child's exit status code: %d\n", status);
			
		}
	}
}

int tokenizePath(char *pathname)
{
        char *s;
        int i;
        m = 0;

        s = strtok(pathname, ":");
        while(s){
                paths[m] = s;
                s = strtok(0, ":");
                m++;
        }

        //for (i = 0; i < n; i++){
        //      printf("token[%d] = %s\n", i, name[i]);
        //}
}

int tokenize(char *pathname)
{
        char *s;
        int i;
        n = 0;

        s = strtok(pathname, " ");
        while(s){
                name[n] = s;
                s = strtok(0, " ");
                n++;
        }

	name[n] = NULL;

        //for (i = 0; i < n; i++){
        //      printf("token[%d] = %s\n", i, name[i]);
        //}
}
