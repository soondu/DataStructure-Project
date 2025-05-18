#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#pragma warning(disable:4996)

typedef struct element {
	char key;
}Element;

typedef struct stack {
	Element data;
	struct stack* link;
}Stack;

void push(Stack** top, Element item) {
	Stack* tmp = (Stack*)malloc(sizeof(Stack));
	tmp->data = item;
	tmp->link = *top;
	*top = tmp;
}

Element pop(Stack** top) {
	Stack* tmp = *top;
	Element item = tmp->data;
	*top = tmp->link;
	free(tmp);
	return item;
}

void printStack(Stack* top) {
	if (top == NULL) return;
	Stack* current = top;

	while (current->link != NULL) {
		printf("%c->", current->data.key);
		current = current->link;
	}
	printf("%c\n", current->data.key);
}

void check(Stack* top, char str[]) {
	printStack(top);
	Stack* current = top;
	Stack* previous = NULL;
	previous = current;
	current = current->link;
	previous->link = current->link;
	printStack(top);

}

int main() {
	char str[10000];
	Stack* top = NULL;
	scanf("%s", str);

	for (int i = 0; i < strlen(str); i++) {
		Element item = { str[i] };
		push(&top, item);
	}//문자열을 stack에 담는다

	check(top, str);

	return 0;
}