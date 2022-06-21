// Chapter09 배열과 포인터 //

/*스택의 구현(1)*/

#include<stdio.h>

int main()
{
	char stack[5];
	int top = 0;

	stack[top] = 'A';
	printf(" %c 자동차가 터널에 들어감\n", stack[top]);
	top++;

	stack[top] = 'B';
	printf(" %c 자동차가 터널에 들어감\n", stack[top]);
	top++;

	stack[top] = 'C';
	printf(" %c 자동차가 터널에 들어감\n", stack[top]);
	top++;

	printf("\n");

	top--;
	printf(" %c 자동차가 터널을 빠져나감.\n", stack[top]);
	stack[top] = ' ';

	top--;
	printf(" %c 자동차가 터널을 빠져나감.\n", stack[top]);
	stack[top] = ' ';

	top--;
	printf(" %c 자동차가 터널을 빠져나감.\n", stack[top]);
	stack[top] = ' ';
}

/*스택의 구현(2)*/

#include<stdio.h>

int main()
{
	int top = 0;
	int select = 9;
	char stack[5];
	char carName = 'A';

	while (select != 3)
	{
		printf("<1>자동차 넣기 <2>자동차 빼기 <3>프로그램 종료");
		scanf_s("%d", &select);

		switch (select)
		{
		case 1:
			if (top >= 5)
			{
				printf("더이상 들어갈 수 없습니다.\n");
			}
			else
			{
				stack[top] = carName++;
				printf(" %c자동차가 터널로 들어왔습니다.\n", stack[top]);
				top++;
			}
			break;
		case 2:
			if (top <= 0)
			{
				printf("더이상 나올 수 있는 차가 없습니다.\n");
			}
			else
			{
				top--;
				carName--;
				printf(" %c자동차가 터널에서 빠져나왔습니다.\n", stack[top]);
				stack[top] = '\0';
			}
			break;
		case 3:
			printf("프로그램 끝 터널안의 자동차는 %d대 입니다.\n",top);
			break;
		default :
			printf("잘못입력하셨습니다. 다시 입력하세요.\n");
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

/*변수의 주소 알아내기*/

#include<stdio.h>

int main()
{
	int a = 100;
	int b = 200;

	printf("변수 a의 주소는 %d 입니다.\n", &a);
	printf("변수 b의 주소는 %d 입니다.\n", &b);

}

/////////////////////////////////////////////////////////////////////////////

/*정수형 배열의 메모리 할당(1)*/

#include<stdio.h>

int main()
{
	int aa[3] = { 10,20,30 };

	printf("aa[0]의 값은 %d, 주소는 %d\n", aa[0], &aa[0]);
	printf("aa[0]의 값은 %d, 주소는 %d\n", aa[1], &aa[1]);
	printf("aa[0]의 값은 %d, 주소는 %d\n", aa[2], &aa[2]);
	printf("배열 이름 aa의 값(=주소)는 %d \n", aa);

}

/*정수형 배열의 메모리 할당(2)*/

#include<stdio.h>

int main()
{
	int aa[3] = { 10,20,30 };

	printf("&aa[0]은 %d, aa+0은 %d \n", &aa[0], aa + 0);
	printf("&aa[1]은 %d, aa+1은 %d \n", &aa[1], aa + 1);
	printf("&aa[2]은 %d, aa+2은 %d \n", &aa[2], aa + 2);

}

/////////////////////////////////////////////////////////////////////////////

/*일반 변수와 포인터 변수의 관계*/

#include<stdio.h>

int main()
{
	char ch;
	char* p;

	ch = 'A';
	p = &ch;

	printf("ch가 가지고 있는 값: ch ==> %c\n", ch);
	printf("ch의 주소(address): &ch ==> %d\n", &ch);
	printf("p가 가지고 있는 값: p ==> %d\n", p);
	printf("p가 가리키고 있는 곳의 실제 값: *p ==> %c\n", *p);

}

/////////////////////////////////////////////////////////////////////////////

/*포인터의 관계 이해하기*/

#include<stdio.h>

int main()
{
	char ch;
	char* p;
	char* q;

	ch = 'A';
	p = &ch;

	printf("p의 실제값 %c\n", *p);

	q = p;
	*q = 'Z';

	printf("ch가 가지고 있는 값:ch==> %c \n\n", ch);
}

/////////////////////////////////////////////////////////////////////////////

/*문자열 배열과 포인터의 관계(1)*/

#include<stdio.h>

int main()
{
	char s[8] = "Basic-C";
	char* p;

	p = s;

	printf("%s[3] ==> %s\n", &s[3]);
	printf("p+3 ==> %s\n\n", *p + 3);

	printf("s[3] ==> %c\n", s[3]);
	printf("*(p+3) ==> %c\n", *(p + 3));
}

/*문자열 배열과 포인터의 관계(2)*/

#include<stdio.h>

int main()
{
	char s[8] = "Basic-C";
	char* p;
	int i;

	p = s;

	for (i = sizeof(s) - 2; i >= 0; i--)
		printf("%c", *(p + i));

	printf("\n");
}

/////////////////////////////////////////////////////////////////////////////

연습 문제

/*포인터를 이용해 문자열을 거꾸로 출력*/

#include<stdio.h>
#include<string.h>

int main()
{
	char aa[100];
	char* p;


	int i, ch;

	printf("문자 입력 : ");
	scanf_s("%s", aa, 100);

	ch = strlen(aa);

	p = aa;

	printf("내용을 거꾸로 출력 ==> ");
	for (i = 0; i < ch; i++)
	{
		printf("%c", *(p + ch - (i + 1)));
	}

}

/*포인터를 이용한 두 값의 교환*/

#include<stdio.h>

int main()
{
	int a, b;

	int* p1;
	int* p2;

	int tmp = 0;

	printf("a 값 입력 : ");
	scanf_s("%d", &a);
	printf("b 값 입력 : ");
	scanf_s("%d", &b);

	p1 = &a;
	p2 = &b;

	tmp = *p1;
	*p1 = *p2;
	*p2 = tmp;


	printf("바뀐 a값은 %d, b는 %d\n",a,b);
}

/*포인터를 이용한 배열의 정렬*/

#include<stdio.h>

int main()
{
	int aa[10] = { 1,0,3,2,5,4,7,6,9,8 };
	int i, k;
	int tmp = 0;

	int* p;

	p = aa;

	printf("정렬 전 배열 s ==> ");
	for (i = 0; i <= 9; i++)
	{
		printf(" %d", *(p + i));
	}

	printf("\n");

	for (i = 0; i <= 9; i++)
	{
		for (k = i+1; k <= 9; k++)
		{
			if (*(p + i) > *(p + k))
			{
				tmp = *(p + i);
				*(p + i) = *(p + k);
				*(p + k) = tmp;
			}
		}
	}

	printf("정렬 후 배열 s ==> ");
	for (i = 0; i <= 9; i++)
	{
		printf(" %d", *(p + i));
	}

}
