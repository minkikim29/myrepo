// Chapter08 배열 //

/*변수값 여러 개를 선언하여 출력하는 예*/
#include<stdio.h>

int main()
{
	int a, b, c, d;

	int hap = 0;

	printf("1번째 숫자를 입력하세요:");
	scanf_s("%d", &a);

	printf("2번째 숫자를 입력하세요:");
	scanf_s("%d", &b);

	printf("3번째 숫자를 입력하세요:");
	scanf_s("%d", &c);

	printf("4번째 숫자를 입력하세요:");
	scanf_s("%d", &d);

	hap = a + b + c + d;

	printf("합계 ==> %d\n", a + b + c + d);
}

/*배열에 값을 선언하여 출력하는 예*/

#include<stdio.h>

int main()
{
	int aa[4];
	int hap;

	printf("1번째 숫자를 입력하세요 : ");
	scanf_s("%d", &aa[0]);
	printf("2번째 숫자를 입력하세요 : ");
	scanf_s("%d", &aa[1]);
	printf("3번째 숫자를 입력하세요 : ");
	scanf_s("%d", &aa[2]);
	printf("4번째 숫자를 입력하세요 : ");
	scanf_s("%d", &aa[3]);

	hap = aa[0] + aa[1] + aa[2] + aa[3];

	printf("합계 ==> %d\n", hap);
}

/////////////////////////////////////////////////////////////////////////////

/*for문으로 배열의 첨자를 활용하는 예*/

#include<stdio.h>

int main()
{
	int aa[4];

	int hap = 0;
	int i;

	for (i = 0; i <= 3; i++)
	{
		printf("%d번째 숫자를 입력하세요 :", i + 1);
		scanf_s("%d", &aa[i]);
		hap = hap + aa[i];
	}

	printf("합계 ==> %d", hap);
}

/////////////////////////////////////////////////////////////////////////////

/*배열의 초기화(1)*/

#include<stdio.h>

int main()
{
	int aa[4] = { 100,200,300,400 };
	int bb[] = { 100,200,300,400 };
	int cc[4] = { 100,200 };
	int dd[4] = { 0 };
	int i;

	for (i = 0; i <= 3; i++)
		printf("aa[%d]==>%d\t", i, aa[i]);
	printf("\n");
	for (i = 0; i <= 3; i++)
		printf("bb[%d]==>%d\t", i, bb[i]);
	printf("\n");
	for (i = 0; i <= 3; i++)
		printf("cc[%d]==>%d\t", i, cc[i]);
	printf("\n");
	for (i = 0; i <= 3; i++)
		printf("dd[%d]==>%d\t", i, dd[i]);
	printf("\n");
}

/*배열의 초기화(2)*/

#include<stdio.h>

int main()
{
	int aa[100], bb[100];
	int i;

	for (i = 0; i < 100; i++)
	{
		aa[i] = i * 2;
	}

	for (i = 0; i < 100; i++)

	{
		bb[i] = aa[99-i];
	}

	printf("bb[0]은 %d, bb[99]는 %d 입력됨\n",bb[0],bb[99]);
}

/////////////////////////////////////////////////////////////////////////////

/*배열의 크기를 계산하는 예*/

#include<stdio.h>

int main()
{
	int aa[] = { 10,20,30,40,50 };
	int count;

	count = sizeof(aa) / sizeof(int);

	printf("배열 aa[]의 요소의 개수는 %d 입니다.\n", count);
}

/////////////////////////////////////////////////////////////////////////////

/*문자열을 선언하고 출력하는 예*/

#include<stdio.h>

int main()
{
	char ss[8] = "Basic-C";
	int i;

	ss[5] = '#';

	for (i = 0; i <= 7; i++)
	{
		printf("ss[%d]==>%c\n", i, ss[i]);
	}

	printf("문자열 배열 ss는 ==> %s\n", ss);
}

/*문자열 순서를 뒤집어 출력*/

#include<stdio.h>

int main()
{
	char ss[5] = "abcd";
	char tt[5];
	int i;

	for (i = 0; i < 4; i++)
	{
		tt[i] = ss[3 - i];
	}

	tt[4] = '\0';

	printf("거꾸로 출력한 결과 ==> %s\n", tt);
}

/////////////////////////////////////////////////////////////////////////////

/*문자열 처리 함수 strlen()사용*/

#include<stdio.h>
#include<string.h>

int main()
{
	char ss[] = "XYZ";
	int len;

	len = strlen(ss);
	printf("문자열 \"%s\"의 길이 ==> %d\n",ss,len);
}

/*문자열 처리 함수 strcpy_s()사용*/

#include<stdio.h>
#include<string.h>

int main()
{
	char ss[4];

	strcpy_s(ss, 4, "XYZ");

	printf("문자열 ss의 내용 ==> %s\n", ss);
}

/*문자열 처리 함수 strcat_s 사용*/

#include<stdio.h>
#include<string.h>

int main()
{
	char ss[7] = "XYZ";

	strcat_s(ss, 7, "ABC");

	printf("이어진 문자열 ss의 내용 ==> %s\n", ss);
}

/*문자열 처리 함수 strcmp() 사용*/

#include<stdio.h>
#include<string.h>

int main()
{
	char ss[] = "XYZ";
	char tt[] = "xyz";
	int r;

	r = strcmp(ss, tt);

	printf("두 문자열의 비교결과 %d\n", r);
}

/////////////////////////////////////////////////////////////////////////////

/*문자열 입출력 함수 gets_s, puts사용*/

#include<stdio.h>
#include<string.h>

int main()
{
	char ss[20];
	char tt[20];
	int r1, r2;

	puts("첫 번째 문자열을 입력하세요 : ");
	gets_s(ss,20);

	puts("두 번째 문자열을 입력하세요 : ");
	gets_s(tt, 20);

	r1 = strlen(ss);
	r2 = strlen(tt);

	printf("첫 번째 문자열의 길이 ==> %d\n", r1);
	printf("두 번째 문자열의 길이 ==> %d\n", r2);

	if (strcmp(ss, tt) == 0)
		puts("두 문자열의 내용이 같습니다.\n");
	else
		puts("두 문자열의 내용이 다릅니다.\n");
}

/////////////////////////////////////////////////////////////////////////////

/*2차원 배열의 사용(1)*/

#include<stdio.h>

int main()
{
	int aa[3][4];

	aa[0][0] = 1; aa[0][1] = 2; aa[0][2] = 3; aa[0][3] = 4;
	aa[1][0] = 5; aa[1][1] = 6; aa[1][2] = 7; aa[1][3] = 8;
	aa[2][0] = 9; aa[2][1] = 10; aa[2][2] = 11; aa[2][3] = 12;

	printf("aa[0][0]부터 aa[2][3]까지 출력 \n");

	printf("%3d %3d %3d %3d\n", aa[0][0], aa[0][1], aa[0][2], aa[0][3]);
	printf("%3d %3d %3d %3d\n", aa[1][0], aa[1][1], aa[1][2], aa[1][3]);
	printf("%3d %3d %3d %3d\n", aa[2][0], aa[2][1], aa[2][2], aa[2][3]);
}

/*2차원 배열의 사용(2)*/

#include<stdio.h>

int main()
{
	int aa[3][4];
	int i, k;
	int val = 1;

	for (i = 0; i <= 2; i++)
	{
		for (k = 0; k <= 3; k++)
		{
			aa[i][k] = val;
			val++;
		}
	}

	printf("aa[0][0]부터 aa[3][4]까지 출력 \n");
	for (i = 0; i <= 2; i++)
	{
		for (k = 0; k <= 3; k++)
		{
			printf("%3d", aa[i][k]);
		}
		printf("\n");
	}
}

/////////////////////////////////////////////////////////////////////////////

연습 문제

/*입력된 문자열을 거꾸로 출력*/

#include<stdio.h>
#include<string.h>

int main()
{
	char ss[100];
	char tt[100];
	int count, i;

	printf("문자열을 입력하세요 : ");
	scanf_s("%s", ss, 100);


	count = strlen(ss);

	for (i = 0; i < count; i++)
	{
		tt[i] = ss[count - (i + 1)];

	}

	tt[count] = '\0';

	printf("내용을 거꾸로 출력 ==> %s\n", tt);
}

/*대문자와 소문자의 변환*/

#include<stdio.h>
#include<string.h>

int main()
{
	char ss[100];
	char tt[100];
	int i, k;
	int diff = 'a' - 'A';
	int r1;

	printf("문자 입력 : ");
	scanf_s("%s", ss, 100);

	r1 = strlen(ss);

	for (i = 0; i < r1; i++)
	{
		if (ss[i] >= 'A' && ss[i] <= 'Z')
			tt[i] = ss[i] + diff;
		if (ss[i] >= 'a' && ss[i] <= 'z')
			tt[i] = ss[i] - diff;
		else
			tt[i] = ss[i];
	}
	tt[r1] = '\0';
	printf("변환된 문자 => %s\n", tt);
}

/*대문자와 소문자의 변환*/

#include<stdio.h>
#include<string.h>

int main()
{
	int r1;
	int i;
	char in[100];
	char out[100];
	int diff = 'a' - 'A';

	printf("문자 입력 : ");
	scanf_s("%s", in, 100);

	r1 = strlen(in);

	for (i = 0; i < r1; i++)
	{
		if (in[i] >= 'A' && in[i] <= 'Z')
			out[i] = in[i] + diff;
		else if (in[i] >= 'a' && in[i] <= 'z')
			out[i] = in[i] - diff;
		else
			out[i] = in[i];
	}
	out[r1] = '\0';

	printf("변환된 문자 ==> %s", out);
}

/*구구단의 결과를 2차원 배열에 저장*/

#include<stdio.h>

int main()
{
	int dan[9][9];
	int i, k;

	for (i = 0; i < 9; i++)
		for (k = 0; k < 9; k++)
			dan[i][k] = (i + 1) * (k + 1);

	for (i = 0; i < 9; i++)
	{
		for (k = 0; k < 9; k++)
		{
			printf("%dX%d=%2d  ", k + 1, i + 1, dan[i][k]);
		}
		printf("\n");
	}
}
