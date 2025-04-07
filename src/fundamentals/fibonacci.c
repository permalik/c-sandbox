#include <stdio.h>

int recursive(int n) {
	if (n == 0) {
		return n;
	}
	if (n == 1) {
		return n;
	};
	return recursive(n - 1) + recursive(n - 2);
}

int fibonacci(void) {

	for (int i = 0; i < 10; i++) {
		printf("%d ", recursive(i));
	}
	printf("\n");
	return 0;
}
