#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int foo() {
	printf("hello foo\n");
	return 1;
}

int main(int argc, char* argv[]) {
	int a = argc;
	int b = argc;

	if ( a == 1 || a == 234) {
		a += 8;
	} else if ( a++ == 3 ) {
		printf("a++\n");
	} else if ( ++b == 3 ) {
		printf("++b\n");
	} else {
		a = 100;
	}

	printf("%d %d\n", a, b);
	
	return 0;
}
