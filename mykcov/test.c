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

	for (int i=0; (i<10) ? (printf("true\n"),1):(printf("false"),0); i++) {
		printf("hello world\n");
	}
	
	return 0;
}
