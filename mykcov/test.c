#include <stdio.h>
#include <stdlib.h>

int foo() {
	printf("hello foo\n");
	return 1;
}

int main(int argc, char* argv[]) {
	int a = argc;

	if ( a == 1 ) {
		a = 2;
	} else if ( a == 3 ) {
		a = 4;
	} else {
		a = 100;
	}

	printf("%d\n", a);
	
	return 0;
}
