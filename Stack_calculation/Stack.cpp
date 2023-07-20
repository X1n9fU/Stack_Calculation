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

//괄호 검사 프로그램
int check_matching(char* in) {
	LinkedStackType s;
	char ch, open_ch;
	int i, n = strlen(in);
	init(&s);

	for (i = 0; i < n; i++) {
		ch = in[i];
		switch (ch) {
		case'(':	case'[':	case'{':
			push(&s, ch);
			break;
		case')':	case']':	case'}':
			if (is_empty(&s)) return 0; 
			else {
				open_ch = pop(&s);
				if ((open_ch == '(' && ch != ')') ||
					(open_ch == '[' && ch != ']') ||
					(open_ch == '{' && ch != '}')) {
					return 0;
				}
				break;
			}
		}
		
	}
	if (!is_empty(&s)) return 0;
	return 1; //올바름
}
//식이 올바른지 검사
int check_right(char* in) {
	LinkedStackType s;
	int count = 0;
	char ch;
	char data,temp=' ';
	int i, n = strlen(in);
	init(&s);

	for (i = 0; i < n; i++) {
		ch = in[i];
		switch (ch) {
		case '(': case')': case' ':
			break;
		case '+': case'-': case'*': case'/': case'.':
			push(&s, ch);
			count++;
			break;
		default:
			if (isdigit(ch) == 0) //연산자 외의 문자열
				return 0;
			else {
				push(&s, ch);
				count++;
			}
		}
	}
	for (i = 0; i < count; i++) {
		data = pop(&s);
		//printf("%c\n", data);
		switch (data) {
		case '+': case'-': case'*': case'/': case'.':
			if (i == 0 || i == n - 1) {
				return 0;
			}
			else if (isdigit(peek(&s)) != 0 && isdigit(temp) != 0) {
				//연산자 앞 뒤로 숫자이면
				break;
			}
			else
				//연산자 앞 뒤로 숫자 외 다른 문자가 하나라도 있으면?
				return 0;
		default:
			temp = data;
			break;
		}
	}
	return 1;
}

void push_s(char* arr, element data, int i) {
	arr[i] = data;
}

// 중위식을 후위식으로 바꾸기
int prec(char op) {
	switch (op) {
	case'(': case')': return 0;
	case'+': case'-': return 1;
	case '*':case '/': return 2;
	}
	return -1;
}

char* infix_to_postfix(char* exp) {
	int i = 0, j = 0, c = 0, k = 0, count=0;
	double n = 0;
	char ch, top_op;
	int len = strlen(exp);
	char* min = NULL;
	min= (char*)calloc(100,sizeof(min));
	char* postfix = NULL;
	postfix = (char*)calloc(100,sizeof(char));
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
			i = c-1;
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

//중위식에서 전위식
element *reserve(char *exp) {
	char temp,i;
	int len = strlen(exp);
	char* arr=NULL;
	arr = (char*)calloc(len, sizeof(char));
	for (i = 0; i < len; i++) {
		push_s(arr,exp[i],i);
	}
	for (i = 0; i < len / 2; i++) {
		temp = arr[i];
		push_s(arr, arr[len - 1 - i], i);
		arr[len - 1 - i] = temp;
	}
	push_s(arr, '\0', len);
	return arr;
}

char* infix_to_prefix(char* exp) {
	int i, j = 0,k;
	char ch, top_op,temp='\0';
	int len = strlen(exp);
	int count = 0,c=0;
	LinkedStackType s,point;
	init(&s);
	init(&point);
	char* min = (char*)malloc(sizeof(char));
	char* prefix = (char*)malloc(sizeof(char));
	exp = reserve(exp); //뒤집어서 진행한다

	for (i = 0; i < len; i++) {
		ch = exp[i];
		switch (ch) {
		case '+': case'-': case'*': case'/':
			while (!is_empty(&s) && (prec(ch) <= prec(peek(&s)))) {
				push_s(prefix, pop(&s), j++);
				push_s(prefix, ' ', j++);
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
				push_s(prefix, min[o], j++);
			push_s(prefix, ' ', j++);
			i = c - 1;
			break;
		case ')': // 왼쪽 괄호
			push(&point, ch);
			c++;
			push(&s, ch);
			break;
		case '(': // 오른쪽 괄호
			push(&point, ch);
			c++;
			top_op = pop(&s);
			// 왼쪽 괄호를 만날때까지 출력
			while (top_op != ')') {
				push_s(prefix, top_op, j++);
				push_s(prefix, ' ', j++);
				top_op = pop(&s);
			}
			break;
		default: // 피연산자
			push_s(prefix, ch, j++);
			push_s(prefix, ' ', j++);
			push(&point, ch);
			c++;
			break;
		}
	}
	while (!is_empty(&s)){// 스택에 저장된 연산자들 출력
		push_s(prefix, pop(&s), j++);
		push_s(prefix, ' ', j++);
		count++;
	}
	
	push_s(prefix, '\0', j++);
	prefix = reserve(prefix);

	return prefix;
}

//후위식 계산하는 함수
double cal_postfix(char* exp) {
	double op1, op2, value;
	int j,i = 0,p = strlen(exp),count=2;
	char ch,c_ch;
	char min[10] = { 0 };
	char lf[100];
	LinkedStackType s;
	init(&s);
	
	for (i = 0; i < p; i++) {
		ch = exp[i];
		//printf("%c", ch);
		if (ch != '+' && ch != '-' && ch != '*' && ch != '/') {
			push(&s, ch);
		}
		else {
			//print_stack(&s);
			//printf("%c",pop(&s)); 
			pop(&s);//공백 꺼내주기
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
			//printf("%f", value);
			if (count == 2) {
				op2 = value;
				count--;
				i -= 2;
			}
			else if (count == 1) {
				op1 = value;
				count--;
			}
			if (count==0){
				push(&s, ' ');
				//printf("op1 %f op2 %f", op1, op2);
				//printf("%c", ch);
				switch (ch) {
				case'+':
					value = op1 + op2;
					//printf("value=%f", value);
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
	char* infix;
	infix=(char*)calloc(100,sizeof(char));
	printf("중위식을 입력하세요:");
	scanf("%s", infix);

	if (check_matching(infix) && check_right(infix)) {
		printf("전위식: %s\n", infix_to_prefix(infix));
		printf("후위식: %s\n", infix_to_postfix(infix));
		printf("계산결과: %f\n", cal_postfix(infix_to_postfix(infix)));
	}
	if (check_matching(infix) == 0)
		printf("괄호문제");
	else if (check_right(infix) == 0)
		printf("식 문제");

	return 0;
	
}