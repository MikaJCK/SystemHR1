#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//simple struct to hold data gotten from a file.
// notable though that the linked list can be gone through backwards or forwards meaninig that next
//or previous are never null. Thusly the structures that goe through the list are buil on
// the idea that the node that stops while loops is the first node
// it also has a reference to linecnode which is ment to house more of the line should it go
// over 255 characTers
typedef struct LineStartNode{
	char line[255];
	struct LineCNode *linecontinued;
	struct LineStartNode *next, *previous;
	
}*LSnode;

typedef struct LineCNode{
	char line[255];
	struct LineCNode *linecontinued;
}*LCnode;



//makes a piece of string that is lenght of 255
char* MakeAString(char *fullstring, int position){
	if((char *string=malloc(sizeof(char)*256))==NULL){
		fprintf(stderr,"Malloc failed\n");
		exit(1);
	}
	for(int i=0;i<255;i++){
		
		string[i]=fullstring[i+position];
		if(fullstring[i+position]=='\n' ){
			break;
		}
	}
	return string;
}

LSnode MakeNode(char* string, int lenght){
	if((LSnode newNode=malloc(sizeof(LSnode)))==NULL){
		fprintf(stderr,"Malloc failed\n");
		exit(1);
	}
	LCnode LCproxy=NULL, LCnew;
	char *proxyChar;
	int proxy=lenght;
	int position=0;
	if(lenght>=255){
		strcpy(proxyChar,MakeAString(string, position));
		strcpy(newNode->line,proxyChar);
		free(proxyChar);
		while(proxy>=0){
			proxy-=255;
			position+=255;
			if((LCnew=malloc(sizeof(LCnode)))==NULL){
				fprintf(stderr,"Malloc failed\n");
				exit(1);		

			}
			strcpy(proxyChar,MakeAString(string, position));
			strcpy(LCnew->line,proxyChar);
			free(proxyChar);
			
			if(LCproxy==NULL){
				LCproxy=LCnew;
				newNode->linecontinued=LCproxy;		
			}else{
				LCproxy->linecontinued=LCnew;
				LCproxy=LCnew;
			}
		}
	}else{
		strcpy(newNode->line, string);
		newNode->linecontinued=NULL;
	}
	return newNode;
}


void ReadFromFile(char *fileName, LSnode firstNode){
	if((char *x=malloc(sizeof(char)*255+1))==NULL){
		fprintf(stderr,"Malloc failed\n");
		exit(1);
	}
	FILE *file;
	if((file=fopen(fileName, "r"))==NULL){
		fprintf(stderr,"error: cannot open file '%s'\n",fileName);
		exit(1);
	}
	LSnode proxyNode, currentNode=firstNode;
	firstNode->next=currentNode;
	firstNode->previous=currentNode;
	int lenght=0, proxy;
	while(1){
		proxy=fgetc(file);
		if(lenght>=strlen(x)){
			x=realloc(x,strlen(x)+sizeof(char)*255);
		}
		x[lenght]=proxy;
		if(proxy=='\n'){
			proxyNode=MakeNode(x,lenght);
			currentNode->next=proxyNode;
			currentNode=proxyNode;
			proxyNode->next=firstNode;
			firstNode->previous=proxyNode;
			free(x);
			if((x=malloc(sizeof(char)*255+1))==NULL){
				fprintf(stderr,stderr,"Malloc failed\n");
				exit(1);
			}
			lenght=0;
		}else if(proxy==EOF){
			proxyNode=MakeNode(x,lenght);
			currentNode->next=proxyNode;
			currentNode=proxyNode;
			proxyNode->next=firstNode;
			firstNode->previous=proxyNode;
			break;
		}else{
		
			lenght++;
		}
	}
	fclose(file);
}

void FreeList(LSnode first){
	LSnode LSproxy=first->next;
	LCnode LCproxy, secondLCP;
	while(LSproxy!=first){
		LCproxy=LSproxy->linecontinued;
		if(LCproxy!=NULL){
			while(LCproxy!=NULL){
				secondLCP=LCproxy->linecontinued;
				free(LCproxy);
				LCproxy=secondLCP;
			}
		}
		free(first);
		first=LSproxy;
		LSproxy=first->next;
	}
	
}

void WriteToFile(char *fileName, LSnode firstNode){
	FILE *file;
	if((char *String=malloc(sizeof(char)*255))==NULL){
		fprintf(stderr,"Malloc failed\n");
		exit(1);
	}
	int size=1;
	LSnode Lsproxy=firstNode->previous;
	LCnode Lcproxy;
	if((file=fopen(fileName, "w"))==NULL){
		fprintf(stderr,"error: cannot open file '%s'\n",fileName);
		exit(1);
	}
	while(Lsproxy!=firstNode){
		strcat(String,Lsproxy->line);
		if(Lsproxy->linecontinued!=NULL){
			Lcproxy=Lsproxy->linecontinued;
			
			size++;
			while(Lcproxy!=NULL){
				String=realloc(String,sizeof(char)*255*size);
				strcat(String, Lcproxy->line);
				Lcproxy=Lcproxy->linecontinued;
				size++;
			}
		}
		fprintf(file, "%s",String);
	}
	fclose(file);
	free(String);
	FreeList(firstNode);
}



int main(int argc, char *argv[]){
	char *filename1,*filename2;
	strcpy(filename2,argv[2]);
	strcpy(filename1,argv[1]);
	if(strcmp(filename1,filename2)){
		//error text exit
		exit(1);
	}
	if((LSnode firstNode=malloc(sizeof(LSnode)))==NULL){
		fprintf(stderr,"Malloc failed\n");
		exit(1);
	}
	if(argc>3){
		fprintf(stderr,"usage: reverse <input> <output>\n");
		exit(1);
	}
	ReadFromFile(filename1, firstNode);
	WriteToFile(filename2,firstNode);
}



