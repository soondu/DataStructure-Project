#include <stdio.h>
#include <stdlib.h>
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

void pop(Stack** top) {
	if (*top == NULL) return;
	Stack* tmp = *top;
	*top = tmp->link;
	free(tmp);
}

void printStack(Stack** top) {
	if (*top == NULL) return;
	Stack* current = *top;
	printf("HELLO?\n");
	while (current->link) {
		printf("%c->", current->data.key);
		current = current->link;
	}
	printf("%c\n", current->data.key);
}

int check(Stack** top) {
	Stack* current = *top;
	Stack* previous = NULL;
	while (*top != NULL) {
		current = *top;
		previous = NULL;
		if (current->data.key == ')') {
			while (current->data.key != '(') {
				previous = current;
				current = current->link;
				if (current == NULL) {
					return 0;
				}
			}
			previous->link = current->link;
			pop(&(*top));
			free(current);
		}
		else if (current->data.key == ']') {
			while (current->data.key != '[') {
				previous = current;
				current = current->link;
				if (current == NULL) {
					return 0;
				}
			}
			previous->link = current->link;
			pop(&(*top));
			free(current);
		}
		else return 0;
	}
	return 1;
}

int main() {
	char str[21];
	Stack* top = NULL;
	scanf("%s", str);

	for (int i = 0; i < strlen(str); i++) {
		Element item = { str[i] };
		push(&top, item);
	}
	printf("%d", check(&top));

	while (top!=NULL) {
		pop(&top);
	}

	return 0;
}