#include <string.h>
#include <stdio.h>

void mkdir(char *pathname)
{
	NODE *curPtr = NULL;
	if(pathname[0] == '/')
	{
		curPtr = root;
		char *s, *f;
		s = strtok(pathname, "/"); // first call to strtok()
		f = strtok(0, '/');
		while(f){
			if(curPtr->type == 'D'){
				curPtr = curPtr->childPtr;
			}
			else
			{
				printf("path invalid, not a directory");
			}
			while(strcomp(curPtr->name, s) != 0)
			{
				if(curPtr->siblingPtr != NULL)
				{
					curPtr = curPtr->siblingPtr;
				}
				else
				{
					printf("pathname invalid");
					return;
				}
			}
			strcpy(s, f);
			f = strtok(0, "/"); // call strtok() until it returns NULL
		}
		NODE *newNode = (NODE *)malloc(sizeof(NODE));
		newNode->parentPtr = curPtr;
		newNode->siblingPtr = NULL;
		newNode->childPtr = NULL;
		newNode.type = 'D';
		strcpy(newNode.name, s);
	}	
	else{
		curPtr = cwd;
		char *s, *f;
		s = strtok(pathname, "/"); // first call to strtok()
		f = strtok(0, '/');
		while(f){
			if(curPtr->type == 'D'){
				curPtr = curPtr->childPtr;
			}
			else
			{
				printf("path invalid, not a directory");
			}
			while(strcomp(curPtr->name, s) != 0)
			{
				if(curPtr->siblingPtr != NULL)
				{
					curPtr = curPtr->siblingPtr;
				}
				else
				{
					printf("pathname invalid");
					return;
				}
			}
			strcpy(s, f);
			f = strtok(0, "/"); // call strtok() until it returns NULL
		}
		NODE *newNode = (NODE *)malloc(sizeof(NODE));
		newNode->parentPtr = curPtr;
		newNode->siblingPtr = NULL;
		newNode->childPtr = NULL;
		newNode.type = 'D';
		strcpy(newNode.name, s);
	}
}

void initialize()
{
	root->parentPtr = NULL;
	root.name = '/';
	root.type = 'D';
	root->childPtr = NULL;
	root->siblingPtr = NULL;
	cwd = root;
}

int tokenize(char *pathname)
{
 char *s;
 s = strtok(pathname, "/"); // first call to strtok()
 while(s){
 printf(“%s “, s);
 s = strtok(0, "/"); // call strtok() until it returns NULL
 }
}

typedef struct node {
	char name[64];
	char type;
	struct node *childPtr;
	struct node *siblingPtr;
	struct node *parentPtr;	
}NODE;

char dname[64], bname[64]; 

NODE *root = (NODE *)malloc(sizeof(NODE)), *cwd;

char line[128], command[16], pathname[64];

char *cmd[] = {"mkdir", "rmdir", "ls", "cd", "pwd", "creat", "rm",
 "reload", "save", "menu", "quit", NULL};

int findCmd(char *command)
{
 int i = 0;
 while(cmd[i]){
 if (!strcmp(command, cmd[i]))
 return i; // found command: return index i
 i++;
 }
 return -1; // not found: return -1
}

int main()
{
 int index;
 initialize(); //initialize root node of the file system tree
 while(1){
 printf("input a commad line : ");
 fgets(line,128,stdin);
 line[strlen(line)-1] = 0;
 sscanf(line, “%s %s”, command, pathname);
 index = fidnCmd(command);
 switch(index){
 case 0 : mkdir(pathname); break;
 case 1 : rmdir(pathname); break;
 case 2 : ls(pathname); break;
 case 3 : cd(pathname); break;
 case 4 : pwd(pathname); break;
 case 5 : creat(pathname); break;
 case 6 : rm(pathname); break;
 case 7 : reload(pathname); break;
 case 8 : save(pathname); break;
 case 9 : menu(pathname); break;
 case 10 : quit(pathname); break;
 default: printf(“invalid command %s\n”, command);
}
