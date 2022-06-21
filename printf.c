// Chapter03 print()와 데이터 형식 //

/*print() 함수 사용*/

#include<stdio.h>

int main(
{
	printf("100+100");
	printf("\n");
	printf("%d", 100 + 100);
	printf("\n");
}

/////////////////////////////////////////////////////////////////////////////

/*서식을 사용한 출력(1)*/

#include<stdio.h>

int main()
{
	printf("%d / %d = %f", 100, 200, 0.5);
	printf("\n");
}

/*서식을 사용한 출력(2)*/

#include<stdio.h>

int main()
{
	printf("%d / %d = %f\n", 100, 200, 0.5);
	printf("%c %c\n", 'a', 'K');
	printf("%s %s\n", "안녕", "c 언어");
}

/////////////////////////////////////////////////////////////////////////////

/*다양한 서식의 활용(1)*/

#include<stdio.h>

int main()
{
	printf("%d\n", 123);			// 정수형 서식
	printf("%5d\n", 123);
	printf("%05d\n", 123);

	printf("%f\n", 123.45);			// 실수형 서식
	printf("%7.1f\n", 123.45);
	printf("%7.3f\n", 123.45);

	printf("%s\n", "Basic-C");		// 문자열형 서식
	printf("%10s\n", "Basic-C");
}

/*다양한 서식의 활용(2)*/

#include<stdio.h>

int main()
{
	printf("\n줄 바꿈\n 연습 \n");
	printf("\t탭키\t연습 \n");
	printf("이것을\r덮어씁니다. \n");
	printf("\a\a\a삐소리 3번 \n");
	printf("글자가 \"강조\"되는 효과 \n");
	printf("\\\\\\ 역슬래시 세개 출력");
}

/////////////////////////////////////////////////////////////////////////////

/*변수에 변수를 대입하는(1)*/

#include<stdio.h>

int main()
{
	int a, b;
	float c, d;

	a = 100;
	b = a;

	c = 111.1f;
	d = c;

	printf("a,b의 값 ==> %d, %d \n", a, b);
	printf("c,d의 값 ==> %5.1f, %5.1f \n", c, d);
}

/*변수에 변수를 대입하는(2)*/

#include<stdio.h>

int main()
{
	int a, b, c, d;

	a = 100 + 100;
	b = a + 100;
	c = a + b - 100;
	d = a + b + c;

	printf("a,b,c,d의 값 ==> %d, %d, %d, %d \n", a, b, c, d);

	a, b, c, d = 100;
	printf("a,b,c,d의 값 ==> %d, %d, %d, %d \n", a, b, c, d);

	a = 100;
	a = a + 200;
	printf("a의 값 ==> &d \n", a);
}

/////////////////////////////////////////////////////////////////////////////

/*소숫점이 없는 정수형의 사용*/

#include<stdio.h>

int main()
{
	int a = 100, b = 200;
	float result;

	result = a / b;			// 정수형 변수로 선언된 변수로 계산하여 실수로 된 변수에 대입하여
							// 값이 정확히 나오지 않는다.

	printf("%f \n", result);
}

/*소숫점이 있는 실수형의 사용*/

#include<stdio.h>

int main()
{
	float a = 0.1234567890123456789;
	double b = 0.1234567890123456789012345;

	printf("%30.25f \n", a);
	printf("%30.25lf \n", b);
}

/////////////////////////////////////////////////////////////////////////////

/*문자형 변수 사용(1)*/

#include<stdio.h>

int main()
{
	char a, b, c;

	a = 'A';


	printf(" %c\n", a);
	printf(" %d\n", a);

	b = 'a';
	c = b + 5;
	printf(" %c\n", b);
	printf(" %c\n", c);

	c = 90;
	printf(" %c\n", c);
}

/*문자형 변수 사용(2)*/

#include<stdio.h>

int main()
{
	int a, b;
	char c, d;

	a = 0x41;
	b = 0x50;

	printf("%c\n", b);

	c = a;
	printf("%c\n", c);

	d = '#';
	printf("%c의 ASCII값은 %d 입니다.\n", d, d);
}

/////////////////////////////////////////////////////////////////////////////

/*문자열 형식 사용(1)*/

#include<stdio.h>
#include<string.h>

int main()
{
	char str1[10];
	char str2[10];
	char str3[10] = "CookBook";

	strcpy_s(str1, "Basic-C");
	strcpy_s(str2, str3);

	printf("str1 ==> %s \n", str1);
	printf("str2 ==> %s \n", str2);
	printf("str3 ==> %s \n", str3);

}

/*문자열 형식 사용(2)*/

#include<stdio.h>

int main()
{
	char str[11] = "0123456789";

	printf("str ==> %s \n", str);

	str[0] = 'I';
	str[1] = 'T';
	str[2] = 'C';
	str[3] = 'o';
	str[4] = 'o';
	str[5] = 'k';
	str[6] = '\0';

	printf("str ==> %s \n", str);
	printf("str[7] ==> %c \n", str[7]);
	printf("str[50] ==> %c \n", str[50]);
}

/////////////////////////////////////////////////////////////////////////////

연습 문제

/*정수형을 출력하는 프로그램*/

#include<stdio.h>

int main()
{
	int a;

	printf("정수를 입력하세요 : ");
	scanf_s("%d", &a);

	printf("10진수 ==> %d\n", a);
	printf("16진수 ==> %x\n", a);
	printf("8진수 ==> %o\n", a);
}


/*입력하는 정수의 진수 결정*/

#include<stdio.h>

int main()
{
	int type, a;

	printf("입력진수 결정 <1>10 <2>16 <3>8 : ");
	scanf_s("%d", &type);

	printf("값 입력 : ");

	if (type == 1)
	{
		scanf_s("%d", &a);
	}
	if (type == 2)
	{
		scanf_s("%x", &a);
	}
	if (type == 3)
	{
		scanf_s("%o", &a);
	}

	printf("10진수 ==> %d\n", a);
	printf("16진수 ==> %x\n", a);
	printf("8진수 ==> %o\n", a);

}

/*데이터 형의 크기 확인*/

#include<stdio.h>

int main()
{
	printf("int 형의 크기              ==> %d\n", sizeof(int));
	printf("unsigned in 형의 크기      ==> %d\n", sizeof(unsigned int));
	printf("short 형의 크기            ==> %d\n", sizeof(short));
	printf("unsigned short 형의 크기   ==> %d\n", sizeof(unsigned short));
	printf("long int 형의 크기         ==> %d\n", sizeof(long int));
	printf("unsigned long int 형의 크기==> %d\n", sizeof(unsigned long int));
	printf("float 형의 크기            ==> %d\n", sizeof(float));
	printf("double 형의 크기           ==> %d\n", sizeof(double));
	printf("long double 형의 크기      ==> %d\n", sizeof(long double));
	printf("char 형의 크기             ==> %d\n", sizeof(char));
	printf("unsigned char 형의 크기    ==> %d\n", sizeof(unsigned char));

}

/*입력된 문자열을 거꾸로 출력*/

#include<stdio.h>

int main()
{
	char str[10];
	int i;

	printf("문자열을 입력 ==> ");
	scanf_s("%s", str, sizeof(str));

	for (i = sizeof(str)-1; i >= 0; i--)
	{
		printf("%c", str[i]);
	}
}
