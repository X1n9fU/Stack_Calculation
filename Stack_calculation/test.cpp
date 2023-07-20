#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <math.h>

typedef char element;

typedef struct StackNode {
	element data;
	struct StackNode* link;
} StackNode;

typedef struct {
	StackNode* top;
} LinkedStackType;

// 초기화 함수
void init(LinkedStackType* s)
{
	s->top = NULL;
}
// 공백 상태 검출 함수
int is_empty(LinkedStackType* s)
{
	return (s->top == NULL);
}
// 포화 상태 검출 함수
int is_full(LinkedStackType* s)
{
	return 0;
}
// 삽입 함수
void push(LinkedStackType* s, element item)
{
	StackNode* temp = (StackNode*)malloc(sizeof(StackNode));
	if (temp == NULL) {
		printf("메모리 할당 에러");
		return;
	}
	temp->data = item;
	temp->link = s->top;
	s->top = temp;
}

element peek(LinkedStackType* s) {
	if (is_empty(s)) {
		fprintf(stderr, "스택이 비어있음\n");
		exit(1);
	}
	return s->top->data;
}

void print_stack(LinkedStackType* s)
{
	for (StackNode* p = s->top; p != NULL; p = p->link)
		printf("%c->", p->data);
	printf("NULL \n");
}
//삭제 함수
element pop(LinkedStackType* s)
{
	if (is_empty(s)) {
		fprintf(stderr, "스택이 비어있음\n");
		exit(1);
	}
	else {
		StackNode* temp = s->top;
		element data = temp->data;
		s->top = s->top->link;
		free(temp);
		return data;
	}
}
void push_s(char* arr, element data, int i) {
	arr[i] = data;
}

int prec(char op) {
	switch (op) {
	case'(': case')': return 0;
	case'+': case'-': return 1;
	case '*':case '/': return 2;
	}
	return -1;
}




element* reserve(const char* exp) {
	char temp, i;
	int len = strlen(exp);
	char* arr = NULL;
	arr = (char*)calloc(len, sizeof(char));
	for (i = 0; i < len; i++) {
		push_s(arr, exp[i], i);
	}
	for (i = 0; i < len / 2; i++) {
		temp = arr[i];
		push_s(arr, arr[len - 1 - i], i);
		arr[len - 1 - i] = temp;
	}

	return arr;
}




char* infix_to_postfix(const char* exp) {
	int i = 0, j = 0, c = 0, k = 0, count = 0;
	double n = 0;
	char ch, top_op;
	int len = strlen(exp);
	char* min = (char*)malloc(sizeof(min));
	char* postfix = NULL;
	postfix = (char*)malloc(sizeof(char));
	LinkedStackType s, point;
	init(&point);
	init(&s); //연산자

	for (i = 0; i < len; i++) {
		ch = exp[i];
		switch (ch) {
		case '+': case'-': case'*': case'/':
			while (!is_empty(&s) && (prec(ch) <= prec(peek(&s)))) {
				char op = pop(&s);
				push_s(postfix, op, j++);
				push_s(postfix, ' ', j++);
			}
			push(&s, ch);
			push(&point, ch);
			c++;
			break;
		case'.':
			while (isdigit(peek(&point)) != 0) {
				pop(&point);
				c--;
				j = j - 2;
				if (is_empty(&point) == 1)
					break;
			}
			k = 0;
			count = 0;
			while (1) {
				if (isdigit(exp[c]) != 0) {
					push_s(min, exp[c++], k++);
					count++;
				}
				else if (exp[c] == '.') {
					push_s(min, exp[c++], k++);
					count++;
				}
				else
					break;
			}
			for (int o = 0; o < count; o++)
				push_s(postfix, min[o], j++);
			push_s(postfix, ' ', j++);
			i = c - 1;
			break;
		case '(': // 왼쪽 괄호
			push(&point, ch);
			c++;
			push(&s, ch);
			break;
		case ')': // 오른쪽 괄호
			push(&point, ch);
			c++;
			top_op = pop(&s);
			// 왼쪽 괄호를 만날때까지 출력
			while (top_op != '(') {
				push_s(postfix, top_op, j++);
				push_s(postfix, ' ', j++);
				top_op = pop(&s);
			}
			break;
		default: // 피연산자
			push_s(postfix, ch, j++);
			push_s(postfix, ' ', j++);
			push(&point, ch);
			c++;
			break;
		}
	}
	while (!is_empty(&s)) {// 스택에 저장된 연산자들 출력
		push_s(postfix, pop(&s), j++);
		push_s(postfix, ' ', j++);
	}

	push_s(postfix, '\0', j++);
	return postfix;
}

double cal_postfix(char* exp) {
	double op1, op2, value;
	int j, i = 0, p = strlen(exp), count = 2;
	char ch, c_ch;
	char min[10] = { 0 };
	char lf[100];
	char* n = { 0 };
	LinkedStackType s;
	init(&s);

	for (i = 0; i < p; i++) {
		ch = exp[i];
		printf("%c", ch);
		if (ch != '+' && ch != '-' && ch != '*' && ch != '/') {
			push(&s, ch);
		}
		else {
			 //공백 꺼내주기
			printf("pop=%c", pop(&s));
			for (j = 0; j < 10; j++)
				min[j] = 0;
			j = 0;
			while (1) {
				c_ch = pop(&s);
				if (c_ch == ' ')
					break;
				if (j == 0) {
					min[0] = c_ch;
					j += 1;
				}
				else {
					int c = j;
					while (c > 0) {
						min[c] = min[c - 1];
						c--;
					}
					j++;
				}
				min[0] = c_ch;
				if (is_empty(&s))
					break;
			}
			value = atof(min);
			printf("%f", value);
			if (count == 2) {
				op2 = value;
				count--;
				i -= 2;
			}
			else if (count == 1) {
				op1 = value;
				count--;
			}
			if (count == 0) {
				printf("op1=%f op2=%f", op1, op2);
				printf("ch= %c\n", ch);
				push(&s, ' ');
				switch (ch) {
				case'+':
					value = op1 + op2;
					sprintf(lf, "%f", value);
					for (j = 0; j < strlen(lf); j++)
						push(&s, lf[j]);
					count = 2;
					break;
				case'-':
					value = op1 - op2;
					sprintf(lf, "%f", value);
					for (j = 0; j < strlen(lf); j++) {
						push(&s, lf[j]);
					}
					count = 2;
					break;
				case'*':
					value = op1 * op2;
					sprintf(lf, "%f", value);
					for (j = 0; j < strlen(lf); j++)
						push(&s, lf[j]);
					count = 2;
					break;
				case'/':
					value = op1 / op2;
					sprintf(lf, "%f", value);
					for (j = 0; j < strlen(lf); j++)
						push(&s, lf[j]);
					count = 2;
					break;
				}
			}
		}
	}
	pop(&s); //공백 꺼내주기
	for (j = 0; j < 10; j++)
		min[j] = 0;
	j = 0;
	while (1) {
		c_ch = pop(&s);
		if (c_ch == ' ')
			break;
		if (j == 0) {
			min[0] = c_ch;
			j += 1;
		}
		else {
			int c = j;
			while (c > 0) {
				min[c] = min[c - 1];
				c--;
			}
			j++;
		}
		min[0] = c_ch;
		if (is_empty(&s))
			break;
	}
	value = atof(min);
	return value;
}

int main(void) {
	char* infix = (char*)calloc(100, sizeof(char));

	gets_s(infix, 100);

	printf("%s",infix_to_postfix(infix));
	cal_postfix(infix_to_postfix(infix));
}