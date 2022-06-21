// Chapter05 조건문 //

/*기본 if문 사용 (1)*/

#include<stdio.h>

int main()
{
	int a = 99;

	if (a < 100)
		printf("100보다 작군요.\n");
}

/*기본 if문 사용(2)*/

#include<stdio.h>

int main()
{
	int a = 200;

	if (a < 100)
		printf("100보다 작군요..\n");
		printf("거짓이므로 이 문장은 안보이겠죠?\n");

		printf("프로그램 끝\n");
}

/*기본 if문 사용(3)*/

#include<stdio.h>

int main()
{
	int a = 200;

	if (a < 100)
	{
		printf("100보다 작군요..\n");
		printf("거짓이므로 앞의 문장은 안보이겠죠?\n");
	}

	printf("프로그램 끝! \n");
}

/////////////////////////////////////////////////////////////////////////////

/*if~else문 사용*/

#include<stdio.h>

int main()
{
	int a = 200;

	if (a < 100)
		printf("100보다 작군요.\n");
	else
		printf("100보다 크군요.\n");
}

/////////////////////////////////////////////////////////////////////////////

/*블록을 활용한 if~else문 사용(1)*/

#include<stdio.h>

int main()
{
	int a = 200;

	if (a < 100)
	{
		printf("100보다 작군요..\n");
		printf("참이면 이 문장도 보이겠죠?");
	}
	else
	{
		printf("100보다 크군요..\n");
		printf("거짓이면 이 문장도 보이겠죠?\n");
	}

	printf("프로그램 끝!\n");
}

/*블록을 활용한 if~else문 사용(2)*/

#include<stdio.h>

int main()
{
	int a;

	printf("정수를 입력하세요:");
	scanf_s("%d", &a);

	if (a % 2 == 0)
	{
		printf("짝수를 입력했군요..\n");
	}
	else {
		printf("홀수를 입력했군요..\n");
	}
}

/////////////////////////////////////////////////////////////////////////////

/*중첩 if문 사용(1)*/

#include<stdio.h>

int main()
{
	int a = 75;

	if (a > 50)
	{
		if (a < 100)
		{
			printf("50보다 크고 100보다 작군요.\n");
		}
		else
		{
			printf("와~~100보다 크군요..\n");
		}
	}

	else
	{
		printf("50보다 작군요.\n");
	}
}

/*중첩 if문 사용(2)*/

#include<stdio.h>

int main()
{
	int a;

	printf("점수를 입력하세요:");
	scanf_s("%d", &a);

	if (a >= 90)
		printf("A");
	else
		if (a >= 80)
			printf("B");
		else
			if (a >= 70)
				printf("C");
			else
				if (a >= 60)
					printf("D");
				else
					printf("F");

	printf(" 학점 입니다.\n");
}

/////////////////////////////////////////////////////////////////////////////

/*switch~case문 사용(1)*/

#include<stdio.h>

int main()
{
	int a;

	printf("1~4 중에 선택하세요:");
	scanf_s("%d", &a);

	switch (a)
	{
	case 1:
		printf("1을 선택했다\n");
		break;
	case 2:
		printf("2를 선택했다\n");
		break;
	case 3:
		printf("3을 선택했다\n");
		break;
	case 4:
		printf("4를 선택했다\n");
		break;
	default:
		printf("이상한걸 선택했다.\n");
	}
}

/*switch~case문 사용(2)*/

#include<stdio.h>

int main()
{
	int year;

	printf("출생년도를 입력하세요: ");
	scanf_s("%d", &year);

	switch (year % 12)
	{
	case 0:printf("원숭이띠\n");	break;
	case 1:printf("닭띠\n");		break;
	case 2:printf("개띠\n");		break;
	case 3:printf("돼지띠\n");		break;
	case 4:printf("쥐띠\n");		break;
	case 5:printf("소띠\n");		break;
	case 6:printf("호랑이띠\n");	break;
	case 7:printf("토끼띠\n");		break;
	case 8:printf("용띠\n");		break;
	case 9:printf("뱀띠\n");		break;
	case 10:printf("말띠\n");		break;
	case 11:printf("양띠\n");		break;
	}

}

/////////////////////////////////////////////////////////////////////////////

연습 문제

/*단순 if문을 활용한 간단한 계산기*/

#include<stdio.h>

int main()
{
	int a1, a2;
	char ch;

	printf("첫 번째 수를 입력하세요 : ");
	scanf_s("%d" ,& a1);

	printf("계산할 연산자를 입력하세요 : ");
	scanf_s(" %c", &ch, 1);

	printf("두 번째 수를 입력하세요 : ");
	scanf_s("%d", &a2);

	if (ch == '+')
	{
		printf("%d + %d = %d\n", a1, a2, a1 + a2);
	}
	if (ch == '-')
	{
		printf("%d - %d = %d\n", a1, a2, a1 - a2);
	}
	if (ch == '*')
	{
		printf("%d * %d = %d\n", a1, a2, a1 * a2);
	}
	if (ch == '/')
	{
		printf("%d / %d = %f\n", a1, a2, a1 / (float)a2);
	}
	if (ch == '%')
	{
		printf("%d %% %d = %d\n", a1, a2, a1 % a2);
	}
}

/*중복 if문을 활용한 간단한 계산기*/

#include<stdio.h>

int main()
{
	int a1, a2;
	char ch;

	printf("첫 번째 수를 입력하세요 : ");
	scanf_s("%d", &a1);

	printf("계산할 연산자를 입력하세요 : ");
	scanf_s(" %c", &ch, 1);

	printf("두 번째 수를 입력하세요 : ");
	scanf_s("%d", &a2);

	if (ch == '+')
		printf("%d + %d = %d\n", a1, a2, a1 + a2);
	else if (ch == '-')
		printf("%d - %d = %d\n", a1, a2, a1 - a2);
	else if (ch == '*')
		printf("%d * %d = %d\n", a1, a2, a1 * a2);
	else if (ch == '/')
		printf("%d / %d = %f\n", a1, a2, a1 / (float)a2);
	else if (ch == '%')
		printf("%d %% %d = %d\n", a1, a2, a1 % a2);
	else
		printf("연산자를 잘못입력했습니다.\n");
}

/*switch~case문을 활용한 간단한 계산기*/

#include<stdio.h>

int main()
{
	int a1, a2;
	char ch;

	printf("수식을 한줄로 띄어쓰기로 입력하세요 : ");
	scanf_s("%d %c %d", &a1, &ch,1,&a2);

	switch (ch)
	{
	case '+':
		printf("%d + %d = %d\n", a1, a2, a1 + a2);
		break;
	case '-':
		printf("%d - %d = %d\n", a1, a2, a1 - a2);
		break;
	case '*':
		printf("%d * %d = %d\n", a1, a2, a1 * a2);
		break;
	case '/':
		printf("%d / %d = %f\n", a1, a2, a1 / (float)a2);
		break;
	case '%':
		printf("%d %% %d = %d\n", a1, a2, a1 % a2);
		break;
	default:
		printf("연산자를 잘못 입력했습니다.");
	}
}
