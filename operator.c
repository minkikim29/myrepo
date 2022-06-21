// Chapter04 연산자의 이해 //


/*산술 연산자 사용*/

#include<stdio.h>

int main()

{
	int a, b = 5, c = 3;

	a = b + c;
	printf("%d + %d = %d \n", b, c, a);

	a = b - c;
	printf("%d - %d = %d \n", b, c, a);

	a = b * c;
	printf("%d * %d = %d \n", b, c, a);

	a = b / c;
	printf("%d / %d  = %d \n", b, c, a);

	a = b % c;
	printf("%d %% %d= %d \n", b, c, a);

}

//////////////////////////////////////////////////////////////////////////

/*연산자 우선순위와 강제 형 변환*/

#include<stdio.h>

int main()
{
	int a = 2, b = 3, c = 4;
	int r1, mok, namugi;
	float r2;

	r1 = a + b - c;
	printf("%d + %d - %d = %d \n", a, b, c, r1);

	r1 = a + b * c;
	printf("%d + %d * %d = %d \n", a, b, c, r1);

	r2 = a * b / (float)c;				//정수 c를 강제로 형 변환한 후에 연산해야 제대로 된 결과값 출력
	printf("%d * %d / %d = %f \n", a, b, c, r2);

	mok = c / b;
	printf("%d / %d 의 몫은 %d \n", c, b, mok);

	namugi = c % b;
	printf("%d %% %d 의 나머지는 %d \n", c, b, namugi);
}

///////////////////////////////////////////////////////////////////////////

/*대입 연산자와 증감 연산자 */

#include<stdio.h>

int main()
{
	int a = 10;

	a++;						// a=a+1과 동일
	printf(" a++ ==> %d\n", a);

	a--;						// a=a-1과 동일
	printf(" a-- ==> %d\n", a);

	a+=5;						// a=a+5와 동일
	printf(" a+=5 ==> %d\n", a);

	a-=5;						// a=a-5와 동일
	printf(" a-=5 ==> %d\n", a);

	a*=5;						// a=a*5와 동일
	printf(" a*=5 ==> %d\n", a);

	a/=5;						// a=a/5와 동일
	printf(" a/=5 ==> %d\n", a);

	a%=5;						// a=a&5와 동일
	printf(" a%%=5 ==> %d\n", a);

}

/////////////////////////////////////////////////////////////////////////////

/*관계 연산자의 사용*/

#include<stdio.h>

int main()
{
	int a = 100, b = 200;

	printf("%d == %d 는 %d 이다.\n", a, b, a == b);		// 같다
	printf("%d != %d 는 %d 이다.\n", a, b, a != b);		// 같지 않다
	printf("%d > %d 는 %d 이다.\n", a, b, a > b);
	printf("%d < %d 는 %d 이다.\n", a, b, a < b);
	printf("%d >= %d 는 %d 이다.\n", a, b, a >= b);
	printf("%d <= %d 는 %d 이다.\n", a, b, a <= b);

	printf("%d = %d 는 %d 이다.\n", a, b, a = b);		// 관계 연산자X 대입 연산자O
}

/////////////////////////////////////////////////////////////////////////////

/*논리 연산자 사용(1)*/

#include<stdio.h>

int main()
{
	int a = 99;

	printf(" AND 연산 : %d\n", (a >= 100) && (a <= 200));	// 둘다 참이어야 함
	printf(" OR 연산 : %d\n", (a >= 100) || (a <= 200));	// 둘 중 하나만 참이어도 됨
	printf(" NOT 연산 : %d\n", !(a >= 100) );		// 참이면 거짓, 거짓이면 참
}

/*논리 연산자 사용(2)*/

#include<stdio.h>

int main()
{
	int a = 100,b = -200;

	printf("상수의 AND 연산: %d\n", a && b);
	printf("상수의 OR 연산: %d\n", a || b);
	printf("상수의 NOT 연산: %d\n", !a);
}

/////////////////////////////////////////////////////////////////////////////

/*비트 논리곱 연산자 사용*/

#include<stdio.h>

int main()
{
	printf("10&7 = %d\n", 10 & 7);
	printf("123&456 = %d\n", 123 & 456);
	printf("0xFFFF & 0000 = %d\n", 0xFFFF & 0000);
}

/*비트 논리합 연산자 사용*/

#include<stdio.h>

int main()

{
	printf("10|7 = %d\n", 10 | 7);
	printf("123|456 = %d\n", 123 | 456);
	printf("0xFFFF|0000 = %d\n", 0xFFFF | 0000);
}

/*비트 배타적 논리합 연산자 사용*/

#include<stdio.h>

int main()
{
	printf("10^7 = %d\n", 10 ^ 7);
	printf("123^456 = %d\n", 123 ^ 456);
	printf("0xFFFF^0000 = %d\n", 0xFFFF ^ 0000);
}

/*비트 부정 연산자 사용*/

#include<stdio.h>

int main()
{
	int a = 12345;

	printf("%d\n", ~a + 1); // 2의 보수를 구한다.
}

/////////////////////////////////////////////////////////////////////////////

/*비트 왼쪽 시프트 연산자 사용*/

#include<stdio.h>

int main()
{
	int a = 10;

	printf("%d 를 왼쪽 1회 시프트 하면 %d 이다\n", a, a << 1);
	printf("%d 를 왼쪽 2회 시프트 하면 %d 이다\n", a, a << 2);
	printf("%d 를 왼쪽 3회 시프트 하면 %d 이다\n", a, a << 3);
}

/*비트 오른쪽 시프트 연산자 사용*/

#include<stdio.h>

int main()
{
	int a = 10;

	printf("%d 를 오른쪽 1회 시프트 하면 %d 이다\n", a, a >> 1);
	printf("%d 를 오른쪽 2회 시프트 하면 %d 이다\n", a, a >> 2);
	printf("%d 를 오른쪽 3회 시프트 하면 %d 이다\n", a, a >> 3);
}

/////////////////////////////////////////////////////////////////////////////

연습 문제

/*입력된 두 실수의 산술 연산*/

#include<stdio.h>

int main()
{
	float a, b, r;

	printf("첫번째 계산할 값을 입력하세요 ==> ");
	scanf_s("%f", &a);
	printf("두번째 계산할 값을 입력하세요 ==> ");
	scanf_s("%f", &b);

	r = a + b;
	printf("%5.2f + %5.2f = %5.2f\n", a, b, r);

	r = a - b;
	printf("%5.2f - %5.2f = %5.2f\n", a, b, r);

	r = a * b;
	printf("%5.2f * %5.2f = %5.2f\n", a, b, r);

	r = a / b;
	printf("%5.2f / %5.2f = %5.2f\n", a, b, r);

	r = (int)a % (int)b;
	printf("%d %% %d = %d\n", (int)a, (int)b, (int)r);
}

/*동전 교환 프로그램*/

#include<stdio.h>

int main()
{
	int c500, c100, c50, c10, money;

	printf(" ## 교환할 돈은 얼마인가요? ## ");
	scanf_s("%d", &money);


	c500 = money / 500;
	money = money % 500;

	c100 = money / 100;
	money = money % 100;

	c50 = money / 50;
	money = money % 50;

	c10 = money / 10;
	money = money % 10;

	printf("오백원짜리 ==> %d\n", c500);
	printf("백원짜리   ==> %d\n", c100);
	printf("오십원짜리 ==> %d\n", c50);
	printf("십원짜리   ==> %d\n", c10);
	printf("잔돈       ==> %d\n", money);
}

/*윤년 계산 프로그램*/

#include<stdio.h>

int main()
{
	int year;

	printf("년도를 입력하세요. : ");
	scanf_s("%d", &year);

	if ((year % 4 == 0) && (year / 100 != 0))
	{
		printf("%d 년은 윤년입니다.\n", year);
	}
	else
	{
		printf("%d 년은 윤년이 아닙니다.\n", year);
	}
}
