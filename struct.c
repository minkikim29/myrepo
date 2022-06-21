// Chapter13 구조체, 공용체, 열거형 //


/*구조체의 사용*/

#include<stdio.h>
#include<string.h>

int main()
{
	struct bibim {
		int a;
		float b;
		char c;
		char d[5];
	};

	struct bibim b1;

	b1.a = 10;
	b1.b = 1.1f;
	b1.c = 'A';
	strcpy_s(b1.d, 5, "ABCD");

	printf("b1.a==>%d \n", b1.a);
	printf("b1.b==>%f \n", b1.b);
	printf("b1.c==>%c \n", b1.c);
	printf("b1.d==>%s \n", b1.d);
}

/*구조체를 사용하지 않은 예*/

#include<stdio.h>

int main()
{
	char name[10];
	int kor;
	int eng;
	float avg;

	printf("이름 : ");
	scanf_s("%s", name, 9);

	printf("국어 점수 : ");
	scanf_s("%d", &kor);

	printf("영어 점수 : ");
	scanf_s("%d", &eng);

	avg = (kor + eng) / 2.0f;

	printf("\n");

	printf("학생 이름 ==> %s\n", name);
	printf("국어 점수 ==> %d\n", kor);
	printf("영어 점수 ==> %d\n", eng);
	printf("평균 점수 ==> %5.1fn", avg);
}

/*구조체로 표현*/

#include<stdio.h>

int main()
{
	struct student {
		char name[10];
		int kor;
		int eng;
		float avg;
	};

	struct student s;

	printf("이름 : ");
	scanf_s("%s", s.name, 9);

	printf("국어 점수 : ");
	scanf_s("%d", &s.kor);

	printf("영어 점수 : ");
	scanf_s("%d", &s.eng);

	s.avg = (s.kor + s.eng) / 2.0f;

	printf("\n--구조체 활용--\n");
	printf("학생 이름 ==> %s\n", s.name);
	printf("국어 점수 ==> %d\n", s.kor);
	printf("영어 점수 ==> %d\n", s.eng);
	printf("평균 점수 ==> %f\n", s.avg);
}

/////////////////////////////////////////////////////////////////////////////

/*구조체 배열을 사용하지 않은 예*/

#include<stdio.h>
#include<string.h>

int main()
{
	char name[3][10];
	int kor[3];
	int eng[3];
	float avg[3];

	int i;

	strcpy_s(name[0], 9, "Kim");
	kor[0] = 90;
	eng[0] = 80;
	avg[0] = ((kor[0] + eng[0]) / 2.0f);

	strcpy_s(name[1], 9, "Lee");
	kor[1] = 70;
	eng[1] = 60;
	avg[1] = ((kor[1] + eng[1]) / 2.0f);

	strcpy_s(name[2], 9, "Park");
	kor[2] = 50;
	eng[2] = 40;
	avg[2] = ((kor[2] + eng[2]) / 2.0f);

	for (i = 0; i < 3; i++)
	{
		printf("학생 이름 ==> %s\n", name[i]);
		printf("국어 점수 ==> %d\n", kor[i]);
		printf("영어 점수 ==> %d\n", eng[i]);
		printf("평균 점수 ==> %5.1f\n", avg[i]);

		printf("\n");
	}

}

/*구조체 배열로 표현하기*/

#include<stdio.h>
#include<string.h>

int main()
{
	struct student {
		char name[10];
		int kor;
		int eng;
		float avg;
	};

	struct student s[3];

	int i;

	strcpy_s(s[0].name, 9, "Kim");
	s[0].kor = 90;
	s[0].eng = 80;
	s[0].avg = ((s[0].kor + s[0].eng) / 2.0f);

	strcpy_s(s[1].name, 9, "Lee");
	s[1].kor = 70;
	s[1].eng = 60;
	s[1].avg = ((s[1].kor + s[1].eng) / 2.0f);

	strcpy_s(s[2].name, 9, "Kim");
	s[2].kor = 50;
	s[2].eng = 40;
	s[2].avg = ((s[2].kor + s[2].eng) / 2.0f);

	printf("구조체 배열\n");
	for (i = 0; i < 3; i++)
	{
		printf("학생 이름 ==> %s\n", s[i].name);
		printf("국어 점수 ==> %d\n", s[i].kor);
		printf("영어 점수 ==> %d\n", s[i].eng);
		printf("평균 점수 ==> %5.1f\n", s[i].avg);

		printf("\n");
	}
}

/////////////////////////////////////////////////////////////////////////////

/* 구조체 포인터를 사용한 예*/
#include<stdio.h>

int main()
{
	struct student {
		char name[10];
		int kor;
		int eng;
		float avg;
	};

	struct student s;


	struct student* p;

	p = &s;

	printf("이름 입력 : ");
	scanf_s("%s", p->name, 10);

	printf("국어 점수 : ");
	scanf_s("%d", &p->kor);

	printf("영어 점수 : ");
	scanf_s("%d", &p->eng);

	p->avg = (p->kor + p->eng) / 2.0f;

	printf("\n--구조체 포인터 활용-\n");
	printf("학생 이름 ==> %s\n", p->name);
	printf("국어 점수 ==> %d\n", p->kor);
	printf("영어 점수 ==> %d\n", p->eng);
	printf("평균 점수 ==> %5.1f\n", p->avg);
}

/*공용체 사용*/

#include<stdio.h>

int main()
{
	union student {
		int tot;
		char grade;
	};

	union student u;

	u.tot = 300;
	u.grade = 'A';

	printf("\n--공용체 활용--\n");
	printf("총점 ==> %d\n", u.tot);
	printf("등급 ==> %c\n", u.grade);
}

/*열거형의 사용*/

#include<stdio.h>

int main()
{
	enum week{sun,mon,tue,wed,thu,fri,sat};

	enum week ww;

	ww = sat;

	if (ww == sun)
		printf("오늘은 일요일입니다.\n\n", ww);
	else
		printf("오늘은 일요일이 아닙니다.\n\n", ww);
}

/////////////////////////////////////////////////////////////////////////////

연습 문제
/*구조체 포인터를 활용한 학생관리*/

#include<stdio.h>
#include<malloc.h>

int main()
{
	struct student {
		char name[10];
		int age;
	};

	struct student* s;

	int i, cnt;

	printf("입력할 학생 수 : ");
	scanf_s("%d", &cnt);

	s = (struct student*)malloc(sizeof(struct student) * cnt);

	for (i = 0; i < cnt; i++)
	{
		printf("이름과 나이 입력 : ");
		scanf_s("%s %d", s[i].name, 9, &s[i].age);
	}

	printf("\n\n--학생 명단--\n");
	for (i = 0; i < cnt; i++)
		printf("이름:%s, 나이:%d \n", s[i].name, s[i]. age);

	free(s);
}

/*구조체와 공용체의 혼합*/

#include<stdio.h>

int main()
{
	typedef struct _person {
		char name[10];
		union _id {
			char phone[15];
			char jumin[15];
		}id;
	}person;

	person p1;

	printf("이름 --> ");
	scanf_s("%s", p1.name,10);
	printf("전화번호 또는 주민번호 --> ");
	scanf_s("%s", p1.id.jumin,15);

	printf("\n");

	printf("--구조체/공용체 혼합 활용--\n");
	printf("이름 ==> %s\n", p1.name);
	printf("전화번호/주민번호 ==> %s\n", p1.id.phone);
}

/*열거형을 활용한 월 이름 출력*/

#include<stdio.h>

int main()
{
	enum month {
		January=1,February,March,April,
		May,June,July,August,September,
		October,November,December
	};

	enum month mm;

	printf("월 입력 : ");
	scanf_s("%d", &mm);

	switch (mm)
	{
	case January: printf("%d월은 January 입니다.", mm); break;
	case February: printf("%d월은 February 입니다.", mm); break;
	case March: printf("%d월은 March 입니다.", mm); break;
	case April: printf("%d월은 April 입니다.", mm); break;
	case May: printf("%d월은 May 입니다.", mm); break;
	case June: printf("%d월은 June 입니다.", mm); break;
	case July: printf("%d월은 July 입니다.", mm); break;
	case August: printf("%d월은 August 입니다.", mm); break;
	case September: printf("%d월은 September 입니다.", mm); break;
	case October: printf("%d월은 October 입니다.", mm); break;
	case November: printf("%d월은 November 입니다.", mm); break;
	case December : printf("%d월은 December 입니다.", mm); break;
	}

	printf("\n\n");
}

// Chpater14 //

/*함수를 파일별로 분리하는 예*/

#include"func.h"

void main()
{
	func1();
	func2();
}

#include<stdio.h>

void func1();
void func2();

#include"func.h"

void func1()
{
	printf("func1() 함수를 실행합니다.\n");
}

#include"func.h"

void func2()
{
	printf("func2() 함수를 실행합니다.\n");
}

/*#define문의 사용*/

#include<stdio.h>

#define PI 3.1415926535
#define STR "원의 면적을 계산했습니다.\n"
#define END_MSG printf("프로그램이 종료되었습니다. \n\n")

void main()
{
	printf("반지름이 10인 원의 면적은 ==> %10.5f\n", 10 * 10 * PI);

	printf(STR);

	END_MSG;
}

/*static 예약어의 사용*/

#include<stdio.h>

void myfunc();

void main()
{
	myfunc();
	myfunc();
}

void myfunc()
{
	static int a = 0;

	a = a + 100;
	printf("a의 값 ==> %d\n", a);
}

/*extern 예약어의 사용(1)*/

#include<stdio.h>

void func();

int a;

void main()
{
	a = 100;

	func();
}

#include<stdio.h>

extern int a;

void func()
{
	printf("extern int a 값==> %d\n", a);
}

/*extern 예약어 사용(2)*/

#include<stdio.h>

void exchange();

int a, b;

int main()
{
	printf("a의 값을 입력 : ");
	scanf_s("%d", &a);
	printf("b의 값을 입력 : ");
	scanf_s("%d", &b);

	exchange();

	printf("\n바뀐 값 a는 %d, b는 %d\n", a, b);
}

extern int a, b;

void exchange()
{
	int tmp;

	tmp = a;
	a = b;
	b = tmp;
}

/*static 예약어를 활용한 구구단 출력*/

#include<stdio.h>

void gugu()
{
	static int dan = 1;
	int i;

	dan++;

	printf("\n\n**%d단**\n",dan);
	for (i = 0; i <= 9; i++)
	{
		printf("%2d X %2d = %2d \n", dan, i, dan * i);
	}

}

int main()
{
	int i;

	for (i = 0; i < 8; i++)
		gugu();
}
