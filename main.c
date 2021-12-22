/* Written by Andrzej Pregowski 22.12.2021
*/
#include<stdio.h>
#include<stdlib.h>

//start of stack data structure
#define MAXSTACKSIZE 1024
int stack[MAXSTACKSIZE];
int *pstack=stack;

#define isFull() (pstack==(stack+MAXSTACKSIZE))
#define isEmpty() (pstack==stack)
#define peek()	*(pstack-1)
#define pop() 	*(--pstack)
#define push(a) *(pstack++)=a	

//end of stack data structure

char* parseFile(char* filename);
void executeProgram(char* program);
int main(int argc,char** argv){
	setvbuf(stdout, NULL, _IONBF, 0);
	if(argc==1){
		fprintf(stderr,"Program to run not specified\n");
		exit(1);
	}
	char* program=parseFile(argv[1]);
	if(program==NULL){
		fprintf(stderr,"Program does not exist/cannot be loaded \n");
		exit(1);
	}
	executeProgram(program);
	return 0;
}



char* parseFile(char* filename){
	FILE* file=fopen(filename,"r");
	if(file==NULL){
		return NULL;
	}
	char* program=(char*) calloc(8,sizeof(char));
	if(program==NULL) return NULL;
	size_t program_size=8;
	size_t memory_usage=0;
	int c;
	while((c=getc(file))!=EOF){
		//TODO : do not load comments
		if(memory_usage==program_size-1){
			program_size*=2;
			program=(char*) realloc(program,sizeof(char)*program_size);
			if(program==NULL) return NULL;
		}	
		program[memory_usage]=c;
		++memory_usage;

	}	
	program[memory_usage]='\0';
	fclose(file);	
	return program;


}
void executeProgram(char* program){
	char* memory=(char*) calloc(30000,sizeof(char));
	char* p=memory+14999;
	//note: pointer will be at the position somewhat in the middle becouse why not (it should start on the infinite row)
	size_t ip=0;
	while(program[ip]!='\0'){
		int c=program[ip];
		switch(c){
			case '+':
				(*p)++;
				break;
			case '-':
				(*p)--;
				break;
			case '.':
				putchar(*p);
				break;
			case ',':
				*p=getchar();
				break;
			case '>':
				p++;
				break;
			case '<':
				p--;
				break;
			case ']':
				if(isEmpty()){
					fprintf(stderr,"Stack is empty\n");
					return;
				}
				if(*p){
					ip=pop()-1;
				}
				else{
					pop();
				}
				break;
			case '[':
				if(isFull()){
					fprintf(stderr,"Stack is full\n");
					return;}
				if(*p){push(ip);}
				else{
					size_t count=1;
					while(count){
						++ip;
						if(program[ip]==']'){--count;}
						if(program[ip]=='['){++count;}
					}
						
				}
				break;
			default:
				break;	
		}
		++ip;
	}
}
