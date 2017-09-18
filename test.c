#include <stdio.h>

typedef enum {A_plus, A_minus, A_time, A_div} binop;

int main(void) {
	printf("%d %d %d %d \n", A_plus, A_minus, A_time, A_div);

	return 0;
}
