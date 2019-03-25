#include <stdio.h>

typedef unsigned int u32;

char *ctable = "0123456789ABCDEF";
int  BASE = 10; 

int rpu(u32 x)
{  
    char c;
    if (x){
       c = ctable[x % BASE];
       rpu(x / BASE);
       putchar(c);
    }
}

int printu(u32 x){
   BASE = 10;
   (x==0)? putchar('0') : rpu(x);
   putchar(' ');
}

int printd(int x){
	BASE = 10;
	if(x < 0){
		putchar('-');
		rpu(-x);
	}
	else{
		printu(x);
	}
}

int printx(u32 x){
	putchar('0');
	putchar('x');
	char c;
	BASE = 16;
	if (x){
		c = ctable[x % BASE];
		rpu(x / BASE);
		putchar(c);
	}

}

int printo(u32 x){
	putchar('0');
	char c;
	BASE = 8;
	if (x){
		c = ctable[x % BASE];
		rpu(x / BASE);
		putchar(c);
	}

}

int prints(char *x){
	int i = 0;
	while(x[i] != '\0'){
		putchar(x[i]);
		i++;
	}
}

int myprintf(char *fmt, ...){
	char x;
	int *ip = (int *) &fmt;
	for(int i = 0; fmt[i] != '\0'; i++){
		x = fmt[i];
		if (x == '%'){
			i++;
			x = fmt[i];
			ip++;
			switch(x){
				case 'c' :
				putchar((char)*ip);
				break;
				case 's' :
				prints((char *)*ip);
				break; 
				case 'u' :
				printu((u32)*ip);
				break;
				case 'd' :
				printd((int)*ip);
				break;
				case 'o' :
				printo((u32)*ip);
				break;
				case 'x' : 
				printx((u32)*ip);
				break;
				default :
				ip--;
				putchar('%');
				putchar(x);
			}
		}
		else{
			putchar(x);
		}	
	}
}

int main(int argc, char *argv[], char *env[]){
	myprintf("argc = %d\n", argc);
	for(int i = 0; i < argc; i++){
		myprintf("argv[%d] = %s\n", i, argv[i]);
	}
	myprintf("cha=%c string=%s      dec=%d hex=%x oct=%o neg=%d\n", 'A', "this is a test", 100,    100,   100,  -100);
}
