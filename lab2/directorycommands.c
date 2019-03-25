#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>

typedef struct node{
	char name[64];
	char type;
	struct node *child, *sibling, *parent;
}NODE;

NODE *root, *cwd, *start;
char line[128];
char command[16], pathname[64];
char dname[64], bname[64];

char *name[128];
int n;

int tokenize(char *pathname)
{
	char *s;
	int i;
	n = 0;
	
	s = strtok(pathname, "/");
	while(s){
		name[n] = s;
		s = strtok(0, "/");
		n++;
	}

	//for (i = 0; i < n; i++){
	//	printf("token[%d] = %s\n", i, name[i]);
	//}
}

NODE *search_child(NODE *parent, char *name)
{
	NODE *cp;

	cp = parent->child;
	
	if(strcmp(name, "..") == 0 && parent->parent)
	{
		return parent->parent;
	}
	
	if(cp == 0)
		return 0;

	while (cp){
		//printf("%s\n", cp->name);
		if (strcmp(cp->name, name) == 0){
			return cp;
		}
		cp = cp->sibling;
	}
	return 0;
}

NODE *path2node(char *pathname)
{
	int i;
	NODE *p;
	
	start = root;
	if (pathname[0] != '/')
		//printf("relative path\n");
		start = cwd;
	
	tokenize(pathname);

	if (n == 0)
		return start;

	p = start;
	for (i = 0; i < n; i++){
		p = search_child(p, name[i]);
		if (p == 0){
			printf("can't find %s\n", name[i]);
			return 0;
		}
	}
	return p;
}

void initialize()
{
	root = (NODE *)malloc(sizeof(NODE));
	root->parent = NULL;
	strcpy(root->name, "/");
	root->type = 'D';
	root->child = NULL;
	root->sibling = NULL;
	cwd = root;
}

int dbname(char *pathname)
{
 char temp[128]; // dirname(), basename() destroy original pathname
 strcpy(temp, pathname);
 strcpy(dname, dirname(temp));
 strcpy(temp, pathname);
 strcpy(bname, basename(temp));
}

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

void mkdir(char *pathname)
{
	dbname(pathname);
	//printf("dname = %s bname = %s\n", dname, bname);
	NODE *newPtr = (NODE *)malloc(sizeof(NODE));
	strcpy(newPtr->name, bname);
	newPtr->type = 'D';
	newPtr->child = NULL;
	newPtr->sibling = NULL;
	if(dname[0] == '.') { memmove(dname, dname+1, strlen(dname));; }
	NODE *temp = path2node(dname);	
	newPtr->parent = temp;
	if(temp->child){
		NODE *temp2 = temp->child;
		while(temp2->sibling){
			if(strcmp(temp2->name, bname) == 0 && temp2->type == 'D')
			{
				printf("directory already exists\n");
				return;
			}
			temp2 = temp2->sibling;
		}
		temp2->sibling = newPtr;
	}
	else{
		temp->child = newPtr;
	}
}

void ls(char *pathname)
{
	NODE *temp = path2node(pathname);
	temp = temp->child;
	while(temp)
	{
		printf("[%c %s]", temp->type, temp->name);
		temp = temp->sibling;
	}
	printf("\n");	
}

void cd(char *pathname)
{
	if(path2node(pathname))
	{
		NODE *temp = path2node(pathname);
		if(temp->type == 'D'){
			cwd = path2node(pathname);
			return;
		}
	}
	printf("invalid cd\n");
}

void rmdir(char *pathname)
{	
	dbname(pathname);
	if(dname[0] == '.') { memmove(dname, dname+1, strlen(dname)); }
	NODE *temp = path2node(dname);
	if(temp->child){
		if(strcmp(temp->child->name, bname) == 0 && !temp->child->child)
		{
			NODE *rmNode = temp->child;
			temp->child = rmNode->sibling;
			free(rmNode);
			return;
		}
	}
	temp = temp->child;
	if(temp)
	{
		
		NODE *rmNode = temp->sibling;
		while(rmNode){
			if(strcmp(rmNode->name, bname) == 0 && rmNode->type == 'D' && rmNode->child == NULL)
			{
				temp->sibling = rmNode->sibling;
				free(rmNode);
				return;
			}
			else
			{
				temp = rmNode;
				rmNode = rmNode->sibling;
			}
		}
	}
	printf("cannot rmdir\n");
}

void pwd(NODE *recNode)
{
	if(recNode){
		pwd(recNode->parent);
		if(strcmp(recNode->name, "/") == 0)
		{
			printf("/");
			return;
		}
		printf("%s/", recNode->name);
	}
	
}

void creat(char *pathname)
{
	dbname(pathname);
	//printf("dname = %s bname = %s\n", dname, bname);
	NODE *newPtr = (NODE *)malloc(sizeof(NODE));
	strcpy(newPtr->name, bname);
	newPtr->type = 'F';
	newPtr->child = NULL;
	newPtr->sibling = NULL;
	if(dname[0] == '.') { memmove(dname, dname+1, strlen(dname));; }
	NODE *temp = path2node(dname);	
	newPtr->parent = temp;
	if(temp->child){
		NODE *temp2 = temp->child;
		while(temp2->sibling){
			if(strcmp(temp2->name, bname) == 0 && temp2->type == 'F')
			{
				printf("file already exists\n");
				return;
			}
			temp2 = temp2->sibling;
		}
		temp2->sibling = newPtr;
	}
	else{
		temp->child = newPtr;
	}
}

void rm(char *pathname)
{	
	dbname(pathname);
	if(dname[0] == '.') { memmove(dname, dname+1, strlen(dname)); }
	NODE *temp = path2node(dname);
	if(temp->child){
		if(strcmp(temp->child->name, bname) == 0)
		{
			NODE *rmNode = temp->child;
			temp->child = rmNode->sibling;
			free(rmNode);
			return;
		}
	}
	temp = temp->child;
	if(temp)
	{
		
		NODE *rmNode = temp->sibling;
		while(rmNode){
			if(strcmp(rmNode->name, bname) == 0 && rmNode->type == 'F')
			{
				temp->sibling = rmNode->sibling;
				free(rmNode);
				return;
			}
			else
			{
				temp = rmNode;
				rmNode = rmNode->sibling;
			}
		}
	}
	printf("cannot rm\n");
}

void menu(char *pathname)
{
	printf("==========commands=========\n");
	printf("mkdir rmdir ls cd pwd creat\n rm reload save menu quit\n");
	printf("===========================\n");
}

void quit(char *pathname)
{	
	
	FILE *fp = fopen("myfile", "w+"); // fopen a FILE stream for WRITE
	save(root, fp, ""); 
	fclose(fp); // close FILE stream when done
	exit(0);
}

void save(NODE *recNode, FILE *opened, char *pathname)
{
	if(recNode)
	{
		if(opened)
		{
			if(recNode->parent)
			{
				char s[128];
				strcpy(s, pathname);	
				strcat(s, recNode->name);
				fprintf(opened, "%c %s\n", recNode->type, s);
				strcat(s, "/");
				save(recNode->child, opened, s);
				save(recNode->sibling, opened, pathname);
			}
			else
			{
				fprintf(opened, "%c %s\n", recNode->type, "/");
				save(recNode->child, opened, "/");
			}
			
		}
	}
}

void reload(FILE *opened)
{
	if(opened)
	{
		root->child = NULL;
		char c;
		char s[128];
		char space;
		fscanf(opened, "%c%c%s\n", &c, &space, s);
		while(!feof(opened))
		{
			fscanf(opened, "%c%c%s\n", &c, &space, s);
			if(c == 'D')
			{
				mkdir(s);	
			}	
			else
			{
				creat(s);
			}
		}
	}
}

int main()
{
	int index;
	FILE *fp;
	initialize(); //initialize root node of the file system tree
	while(1){
		strcpy(pathname, "");
		printf("input a commad line : ");
		fgets(line,128,stdin);
		line[strlen(line)-1] = 0;
		sscanf(line, "%s %s", command, pathname);
		//printf("this is pathname: %s\n", pathname);
		index = findCmd(command);
		switch(index){
			case 0 : mkdir(pathname); break;
			case 1 : rmdir(pathname); break;
			case 2 : ls(pathname); break;
			case 3 : cd(pathname); break;
			case 4 : pwd(cwd);
				 printf("\n");
				 break;
			case 5 : creat(pathname); break;
			case 6 : rm(pathname); break;
			case 7 : fp = fopen("myfile", "r");
				 reload(fp);
				 fclose(fp);
				 break;
			case 8 : fp = fopen("myfile", "w+"); // fopen a FILE stream for WRITE
				 save(root, fp, ""); 
				 fclose(fp); // close FILE stream when done
				 break;
			case 9 : menu(pathname); break;
			case 10 : quit(pathname); break;
			default: printf("invalid command %s\n", command);
		}
	}
	return 0;
}

