/*
	BRAINDUCK
	an interpreter for brainfuck

	goals-
	easily portable
	easily mod-able- maybe turn it into a translator/compiler?
	-*readable source code*- edit- depracated idea

	implementation specs-
	do nothing on EOF input
	the data line "wraps around"
	the data line" is 4096 bytes long, see MEMSIZE, easily changable
	can go up to 256 loops "deep", see STACKSIZE, easily changable
*/
#include <stdio.h>

#define MEMSIZE 4096
#define STACKSIZE 256

//global vars- whats the point of passing em around?
int	stack[STACKSIZE];
int*	sp;
char	data[MEMSIZE];
char*	dataptr;
FILE* 	in;

char get_ins(FILE* fptr){
	/*get instruction
	  return it*/
	  return fgetc(fptr);
}

void inc_data_ptr(){
	++dataptr;
	dataptr=(dataptr-data)%MEMSIZE+data;//wrap around
}
void dec_data_ptr(){
	--dataptr;
	dataptr=(dataptr-data)%MEMSIZE+data;//wrap around
}


void inc_data(){
	++*dataptr;
}
void dec_data() {
	--*dataptr;
}

void print_data(){
	putchar(*dataptr);
}
void read_data(){
	*dataptr=fgetc(stdin);
}

void start_loop(){
	if(*dataptr==0){
		int level=0;
		do{
			switch(fgetc(in)){
				case '[': level++;break;
				case ']': level--;break;
			}
		}while(level>=0 && !feof(in));
		return;
	}
	ungetc('[',in);
	*(sp--)=ftell(in);
	fgetc(in);
}
void end_loop(){
	fseek(in, *(++sp), SEEK_SET);

}

int main(int argc, char *argv[])
{
	{//argc, argv check
		if (argc<2){
			printf("please enter file name\n");
			return 1;
		}if (argc>2){
			puts("too many args\n");
			return 1;
	}	}

	{//open input file
		in=fopen(argv[1],"r");
		if(in==NULL){
			printf("cant open %s!\n",argv[1]);
			 return 1;
	}	}
	{//init global things
		for (int i=0;i<STACKSIZE;i++) stack[i]=0;
		sp = &stack[STACKSIZE-1];
		for (int i=0;i<MEMSIZE	;i++) data[i]=0;
		dataptr = data;
	}
	char ins;
	while (feof(in)==0){
		ins=get_ins(in);
		switch(ins){
			case '>':	inc_data_ptr(); break;
			case '<':	dec_data_ptr(); break;
			case '+':	inc_data();	break;
			case '-':	dec_data();	break;
			case '.':	print_data();	break;
			case ',':	read_data();	break;
			case '[':	start_loop();	break;
			case ']':	end_loop();	break;
	}	}
	putchar('\n');
	fclose(in);
	return 0;
	/**/
}
