#include <stdio.h>
#include <time.h>
#pragma warning(disable:4996)

void hanoi(int n, char first, char second, char third, int* cnt) {

	if (n == 1) {
		(*cnt)++;
	}
	else {
		hanoi(n - 1, first, third, second, cnt);
		(*cnt)++;
		hanoi(n - 1, second, first, third, cnt);
	}
}

int main() {
	int n, cnt = 0;
		cnt = 0;
		printf("Put the number of disks:");
		scanf("%d", &n);
		
		hanoi(n, 'A', 'B', 'C', &cnt);
		//printf("The number of moves needed is %d\n", cnt);
		printf("For n = %d, Number of executions: %d\n", n, cnt);
	return 0;
}