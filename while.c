// While문과 흐름 제어 //


/*for문을 while문으로 바꾸는 예(1)*/

#include<stdio.h>

int main()
{
	int i;
	i = 0;

	while (i < 5)
	{
		printf("while문을 공부합니다.\n");
		i++;
	}
}

/*for문을 while문으로 바꾸는 예(2)*/

#include<stdio.h>

int main()
{
	int hap=0;
	int i;
	i = 1;

	while (i <= 10 ) {
		hap = hap + 1;
		i++;
	}

	printf("1에서 10까지의 합 : %d", hap);

}

/////////////////////////////////////////////////////////////////////////////

/*while으로 무한루프 만들기*/

#include<stdio.h>

int main()
{
	int a, b;

	while (1)
	{
		printf("더할 두 수 입력(멈추려면 Ctrl+C)");
		scanf_s("%d %d", &a, &b);

		printf("%d + %d = %d\n",a,b,a+b);

	}
}

/*무한루프를 활용한 계산기*/

#include<stdio.h>

int main()
{
	int a, b;
	char ch;

	while (1)
	{
		printf("계산할 두 수를 입력(멈추려면 Ctrl+C):");
		scanf_s("%d %d", &a, &b);

		printf("계산할 연산자를 입력하세요 : ");
		scanf_s(" %c", &ch, 1);

		switch (ch)
		
		{
		case'+':
			printf("%d + %d = %d\n", a, b, a + b);
			break;
		case'-':
			printf("%d - %d = %d\n", a, b, a - b);
			break;
		case'*':
			printf("%d * %d = %d\n", a, b, a * b);
			break;
		case'/':
			printf("%d / %d = %f\n", a, b, a / (float)b);
			break;
		case'%':
			printf("%d %% %d = %d\n", a, b, a % b);
			break;
		default:
			printf("연산자를 잘못 입력했습니다.\n");
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

/*do~while문의 사용(1)*/

#include<stdio.h>

int main()
{
	int a = 100;

	while (a == 200)								// 조건식을 먼저 판단
	{
		printf("while문 내부에 들어 왔습니다.\n");
	}
	do
	{
		printf("do~while문 내부에 들어 왔습니다.\n");

	} while (a == 200);								// 내용 실행 후 조건 판단
}

/*do~while문의 사용(2)*/

#include<stdio.h>

int main()
{

	int menu;
	do {
		printf("\n 손님 주문하시겠습니까? \n");
		printf("<1>카페라떼 <2>카푸치노 <3>아메리카노 <4>그만시킬래요 ==> ");
		scanf_s("%d", &menu);

		switch (menu)
		{
		case 1:printf("#카페라떼 주문하셨습니다.\n"); break;
		case 2:printf("#카푸치노 주문하셨습니다.\n"); break;
		case 3:printf("#아메리카노 주문하셨습니다.\n"); break;
		case 4:printf("#주문하신 커피 준비하겠습니다.\n"); break;
		default:printf("잘못 주문하셨습니다.\n");

		}
	} while (menu != 4);
}

/////////////////////////////////////////////////////////////////////////////

/*break문의 사용(1)*/

#include<stdio.h>

int main()
{
	int i;
	for (i = 1; i <= 100; i++)
	{
		printf("for문을 %d회 실행했습니다.\n", i);
		break;
	}

	printf("for문을 종료했습니다.\n");
}

/*break문의 사용(2)*/

#include<stdio.h>

int main()
{
	int a, b;

	while (1)
	{
		printf("더할 두 수 입력(멈추려면 0을 입력):");
		scanf_s("%d %d", &a, &b);

		if (a == 0)
			break;

		printf("%d + %d = %d \n", a, b, a + b);

	}

	printf("0을 입력해서 for문을 탈출했습니다.\n");
}

/*break문의 사용(2)*/

#include<stdio.h>

int main()
{
	int a, b;

	while (1)					// 무한 루프
	{
		printf("더할 두 수 입력 (멈추려면 0을 입력) :");
		scanf_s("%d %d", &a, &b);

		if (a == 0)
			break;

		printf("%d + %d = %d \n", a, b, a + b);
	}

	printf("0을 입력해서 for문을 탈출 했습니다.\n");

}

/*break문의 사용(3)*/

#include<stdio.h>

int main()
{
	int hap = 0;
	int i;

	for (i = 1; i <= 100; i++)
	{
		hap = hap + i;

		if (hap >= 1000)
			break;
	}

	printf("1~100의 합에서 최초로 1000이 넘는 위치는? %d \n", i);
}

/////////////////////////////////////////////////////////////////////////////

/*continue문의 사용*/

#include<stdio.h>

int main()
{
	int hap = 0;

	int i;

	for (i = 1; i <= 100; i++)
	{
		if (i % 3 == 0)
			continue;

		hap += i;
	}

	printf("1~100까지의 합(3의 배수 제외): %d\n", hap);
}

/*goto문의 사용*/

#include<stdio.h>

int main()
{
	int hap = 0;
	int i;

	for (i = 1; i <= 100; i++)
	{
		hap += i;

		if (hap > 2000)
			goto mygoto;
	}

mygoto:
	printf("1부터 %d까지 합하면 2000이 넘어요.\n", i);
}

/*return문의 사용*/

#include<stdio.h>

int main()
{
	int hap = 0;
	int i;

	for (i = 1; i <= 100; i++)
		hap += i;

	printf("1부터 100까지의 합은 %d 입니다.\n", hap);
	return 0;

	printf("프로그램의 끝입니다.");
}

/////////////////////////////////////////////////////////////////////////////

연습 문제

/*원하는 배수의 합계를 구하는 계산기*/

#include<stdio.h>

int main()
{
	int start, end,i;
	int bs;
	int hap = 0;

	printf("합계의 시작값 ==> ");
	scanf_s("%d", &start);

	printf("합계의 끝값==> ");
	scanf_s("%d", &end);

	printf("배수 ==>");
	scanf_s("%d", &bs);

	for (i = start; i <= end; i++)
	{
		if (i % bs==0)
			hap += i;

	}

	printf("%d부터 %d까지의 %d배수의 합계 ==> %d\n",start,end,bs,hap);

}

/*입력한 문자열의 종류 구분하기*/

#include<stdio.h>

int main()
{
	char str[100];
	char ch;

	int up = 0, low = 0, nu = 0;
	int i;

	printf("문자열을 입력<100자 이내> : ");
	scanf_s("%s", str, 100);

	i = 0;

	do {
		ch = str[i];

		if (ch >= 'A' && ch <= 'Z')
			up++;
		if (ch >= 'a' && ch <= 'z')
			low++;
		if (ch >= '0' && ch <= '9')
			nu++;

			i++;
	} while (ch != '\0');

	printf("대문자%d개, 소문자%d개, 숫자%d개\n",up,low,nu);
}

/*입력한 문자열의 종류 구분하기*/

#include<stdio.h>

int main()
{
	char str[100];
	char ch;

	int up = 0, low = 0, nu = 0;
	int i;

	printf("문자열을 입력<100자 이내> : ");
	scanf_s("%s", str, 100);

	i = 0;


	do {
		ch = str[i];
		if (ch >= 'A' && ch <= 'Z')
			up++;
		if (ch >= 'a' && ch <= 'z')
			low++;
		if (ch >= '0' && ch <= '9')
			nu++;
		i++;
	} while (ch != '\0');

		printf("대문자%d개, 소문자%d개, 숫자%d개\n", up, low, nu);
}


