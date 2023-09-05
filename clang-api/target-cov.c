int main() {
  int a = 4;
  
  switch(a){
  	case 1: a += 1;
	case 2: a *= 2;
	case 3: a -= 3; printf("hello world\n");
	default: a = 0;
  };

	return 0;
}
