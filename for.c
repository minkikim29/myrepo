// Chapter06 반복문 //

/*기본 for문 사용*/

#include<stdio.h>

int main()
{
	int i;

	for (i = 0; i < 5; i++)
	{
		printf("안녕하세요 for문을 공부중입니다.\n");
	}
}

/////////////////////////////////////////////////////////////////////////////

/*for문과 블록 사용 예*/

#include<stdio.h>

int main()
{
	int i;

	for (i = 0; i < 3; i++)				// 블록 사용
	{
		printf("안녕하세요? \n");
		printf("\\또 안녕하세요?\\ \n");
	}

	printf("\n\n");

	for (i = 0; i < 3; i++)				// 블록 사용X
		printf("안녕하세요? \n");
		printf("\\또 안녕하세요?\\ \n");

}

/////////////////////////////////////////////////////////////////////////////

/*for문 사용 예(1)*/

#include<stdio.h>

int main()
{
	int i;

	for (i = 5; i > 0; i--)
	{
		printf("%d : 안녕하세요 for문을 공부중입니다.\n",i);
	}
}

/*for문 사용 예(2)*/

#include<stdio.h>

int main()
{
	int i;

	for (i = 1; i <= 5; i++)
	{
		printf("%d\n", i);
	}
}

/////////////////////////////////////////////////////////////////////////////

/*for문을 활용하지 않고 합계 구하기*/

#include<stdio.h>

int main()
{
	int hap;

	hap = 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10;


	printf(" 1에서 10까지의 합 : %d\n", hap);
}

/////////////////////////////////////////////////////////////////////////////

/*for문을 활용하여 합계 구하기(1)*/

#include<stdio.h>

int main()
{
	int hap=0;
	int i;

	for (i = 1; i <= 10; i++)
	{
		hap = hap + i;
	}

	printf(" 1에서 10까지의 합 : %d \n", hap);
}

/*for문을 활용하여 합계 구하기(2)*/

#include<stdio.h>

int main()
{
	int hap = 0;
	int i;

	for (i = 501; i <= 1000; i += 2)
	{
		hap = hap + i;
	}

	printf("500에서 1000사이의 홀수의 합은 : %d\n",hap);
}

/*for문을 활용하여 합계 구하기(3)*/

#include<stdio.h>

int main()
{
	int hap = 0;		// 0으로 초기화 해주지 않으면 쓰레기 값이 들어감
	int i;
	int num;

	printf("값 입력 : ");
	scanf_s("%d", &num);

	for (i = 1; i <= num; i++)
	{
		hap = hap + i;
	}

	printf("1부터 %d까지의 합은 %d\n", num, hap);
}

/*for문을 활용하여 합계 구하기(4)*/

#include<stdio.h>

int main()
{
	int hap = 0;
	int i;
	int num1, num2, num3;

	printf("시작값, 끝값, 증감값 입력 : ");
	scanf_s("%d %d %d", &num1, &num2, &num3);

	for (i = num1; i <= num2; i =i+ num3)
	{
		hap = hap + i;
	}

	printf("%d부터 %d까지 %d씩 증가한 값의 합은 %d \n", num1, num2, num3, hap);
}

/////////////////////////////////////////////////////////////////////////////

/*for문을 사용한 구구단*/

#include<stdio.h>

int main()
{
	int dan;
	int i;

	printf("몇 단 입니까? ");
	scanf_s("%d", &dan);

	for (i = 1; i <= 9; i++)
	{
		printf("%d X %d = %d\n", dan, i, dan * i);
	}
}

/////////////////////////////////////////////////////////////////////////////

/*중첩 for문의 사용(1)*/

#include<stdio.h>

int main()
{
	int i, k;

	for (i = 0; i < 3; i++)
	{
		for (k = 0; k < 2; k++)
		{
			printf("중첩 for문입니다. <i값 : %d, k값 : %d>\n", i, k);
		}
	}
}

/*중첩 for문의 사용(2)*/

#include<stdio.h>

int main()
{
	int i, k;

	for (i = 2; i <= 9; i++)
	{
		printf("## 제 %d 단 ##\n", i);
		for (k = 1; k <= 9; k++)
		{

			printf("%d X %d = %d\n", i, k, i * k);
		}
		printf("\n");
	}
}

/*중첩 for문의 사용(3)*/

#include<stdio.h>

int main()
{
	int i, k;

	for (i = 1; i <= 9; i++)
	{
		for (k = 2; k <= 9; k++)
		{

			printf("%2dX%2d =%2d ", k, i, i * k);
		}
		printf("\n");
	}
}

/////////////////////////////////////////////////////////////////////////////

/*다양한 for문의 활용(1)*/

#include<stdio.h>

int main()
{
	int i, k;

	for (i = 1, k = 1; i <= 9; i++, k++)
	{
		printf("%d X %d = %d \n", i, k, i * k);
	}
}

/*다양한 for문의 활용(2)*/

#include<stdio.h>

int main()
{
	int i;
	i = 0;
	for (; ;) // 무한루프
	{
		printf("%d \n", i);
		i++;
	}
}

/*다양한 for문의 활용(3)*/

#include<stdio.h>

int main()
{
	int a, b;
	for (; ; )
	{
		printf("더할 두수 입력(멈추려면 Ctrl + C): ");
		scanf_s("%d %d", &a, &b);

		printf("%d + %d = %d \n", a, b, a + b);
	}
}

/////////////////////////////////////////////////////////////////////////////

연습 문제

/*구구단 출력 프로그램*/

#include<stdio.h>

int main()
{
	int dan, i,k;

	for (i = 2; i <= 9; i++)
		printf("#제 %d 단# ",i);
		printf("\n\n");

		for (i = 1; i <= 9; i++)
		{
			for (k = 2; k <= 9; k++)
			{
				printf("%2dX%2d = %2d", k, i, k * i);
			}
			printf("\n");
		}

}

/*아스키 코드표 출력 프로그램*/

#include<stdio.h>

int main()
{
	int i;

	for (i = 0; i < 128; i++)
	{
		if (i % 16 == 0)
		{
			printf("------------------\n");
			printf("10진수 16진수 문자\n");
			printf("------------------\n");

		}
		printf("%5d %5x %5c\n", i, i, i);
	}
}

/*입력한 문자를 거꾸로 출력*/

#include<stdio.h>
#include<string.h>

int main()
{
	int r1, r2;
	char str[100];
	printf("영문자 및 숫자를 입력 <100자 이하> : ");
	scanf_s("%s", str, 100);

	printf("입력한 문자열 ==> %s\n", str);
	printf("변환된 문자열 ==> ");

	r2 = strlen(str);

	for (r1 = r2 ; r1 >= 0; r1--)
	{
		printf("%c", str[r1]);
	}

	printf("\n");
}
