#include <stdio.h>

#ifndef _MALLOC_H_
#define malloc.h
#endif

struct branch {
    char **conditionList;
    int lineCnt;
    int thenCnt;
    int elseCnt;
};int main() {
	int a = 0;

	/*Before if statement*/
  if ( a == 1 ) {
		a = 2;
	}
  /*After if statement*/

	
	return 0;
}
