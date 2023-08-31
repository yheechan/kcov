#include <stdio.h>

int temp() {
	printf("hello world");
	return 1;
}

int main() {
	int a = 5;
	
	/*Before if statement*/
  if (a > 5){
		a += 1;
	}
	else{
		a -= 1;
	}
  /*After if statement*/

	return 0;
}
