//example-kcov.c

#include <stdio.h>
#include "example-kcov.h"

#define FOO(X)					\
  do {						\
	if (X > 100)				\
		printf("foo\n");		\
	else					\
		printf("bar\n");		\
  } while (0)

int static_var = 1;

int f1(int a) {
	static_var++;
	//if-else
	if( a == 1) { // 2
		return 2;
	} else {
		return 4;
	}
}

int main() {
	int a = 0;		
	
	//if-else if-else
	if( a==1 || a==3) { // 4
		a =  2;
	} else if ( a == 2){ // 2
		a = 1;
	} else {
		a = 4;
	}	
	int i;
	//for	
	for(i = 0 ; i < 10 ; i++ ) { // 2
		a += i;
	}
	
	//while
	while( a < 100 ) { // 2
		a += a;
	}
	
	//do-while
	do {
		switch(a) { // 3
			case 100: 
				a += 10;
				break;
			case 200: 
				a = a == 3 ? 1 : 2;// ?: operator // 2
				a += 20;
				break;
			default:
				a += 1;
		}
	} while( a == 0 ); // 2

	if ( a>1 ? a-1 :a ) { // 4
		a=150;
	}

	switch(a) { // 3
		case 150:
			a+=20;
			break;
		case 250:
			a+=30;
			break;
	}

	FOO(a); // 2
	f2(a);
}

/*
$ ./PrintBranches example-kcov.c
function: f2
        If      ID: 0   Line: 4         Col: 2          Filename: ./example-kcov.h
function: f1
        If      ID: 1   Line: 19                Col: 2          Filename: example-kcov.c
function: main
        If      ID: 2   Line: 30                Col: 2          Filename: example-kcov.c
        If      ID: 3   Line: 32                Col: 9          Filename: example-kcov.c
        For     ID: 4   Line: 39                Col: 2          Filename: example-kcov.c
        While   ID: 5   Line: 44                Col: 2          Filename: example-kcov.c
        Do      ID: 6   Line: 49                Col: 2          Filename: example-kcov.c
        Case    ID: 7   Line: 51                Col: 4          Filename: example-kcov.c
        Case    ID: 8   Line: 54                Col: 4          Filename: example-kcov.c
        ?:      ID: 9   Line: 55                Col: 9          Filename: example-kcov.c
        Default ID: 10  Line: 58                Col: 4          Filename: example-kcov.c
        If      ID: 11  Line: 63                Col: 2          Filename: example-kcov.c
        ?:      ID: 12  Line: 63                Col: 7          Filename: example-kcov.c
        ImpDef. ID: 13  Line: 67                Col: 2          Filename: example-kcov.c
        Case    ID: 14  Line: 68                Col: 3          Filename: example-kcov.c
        Case    ID: 15  Line: 71                Col: 3          Filename: example-kcov.c
        Do      ID: 16  Line: 76                Col: 2          Filename: example-kcov.c
        If      ID: 17  Line: 76                Col: 2          Filename: example-kcov.c
Total number of branches: 30
*/
/* ImpDef. indicates an implicit default in a switch statement */ 
