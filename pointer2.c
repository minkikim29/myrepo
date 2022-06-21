// Chapter12 고급 포인터 //

/*포인터를 사용하여 정수 합계 구하기*/

#include<stdio.h>

int main()
{
	int aa[3];
	int* p;
	int i, hap = 0;

	for (i = 0; i < 3; i++)
	{
		printf(" %d 번째 숫자 : ",i + 1);
		scanf_s("%d", &aa[i]);
	}

	p = aa;

	for (i = 0; i < 3; i++)
		hap = hap + *(p + i);

	printf("입력 숫자의 합 ==> %d\n", hap);
}

/*고정된 크기의 배열로 인한 메모리 낭비*/

#include<stdio.h>

int main()
{
	int aa[10000];
	int* p;
	int i, hap = 0;
	int cnt;

	printf("입력할 개수는 ? ");
	scanf_s("%d", &cnt);

	for (i = 0; i < cnt; i++)
	{
		printf("%d 번째 숫자 :", i + 1);
		scanf_s("%d", &aa[i]);
	}

	p = aa;

	for (i = 0; i < cnt; i++)
		hap = hap + *(p + i);

	printf("입력 숫자의 합 ==> %d\n", hap);
}

/////////////////////////////////////////////////////////////////////////////

/*malloc() 함수 사용*/

#include<stdio.h>
#include<malloc.h>

int main()
{
	int* p;
	int i, hap = 0;
	int cnt;

	printf("입력할 개수는 ? ");
	scanf_s("%d", &cnt);

	p = (int*)malloc(sizeof(int) * cnt);

	for (i = 0; i < cnt; i++)
	{
		printf(" %d 번째 숫자 : ",i+1);
		scanf_s("%d", p + i);
	}

	for (i = 0; i < cnt; i++)
		hap = hap + *(p + i);

	printf("입력 숫자의 합 ==> %d\n", hap);

	free(p);
}

/*malloc()함수와 calloc함수의 차이*/

#include<stdio.h>
#include<malloc.h>

int main()
{
	int* p, * s;
	int i, k;

	printf("malloc() 함수 사용 \n");
	p = (int*)malloc(sizeof(int) * 3);

	for (i = 0; i < 3; i++)
	{
		printf("할당된 곳의 값 p[%d] ==> %d \n", i, p[i]);
	}
	free(p);

	printf("\n");

	printf("calloc() 함수 사용 \n");
	s = (int*)calloc(sizeof(int), 3);

	for (k = 0; k < 3; k++)
	{
		printf("할당된 곳의 값 s[%d] ==> %d\n", k, s[k]);
	}

	free(s);

}

/*realloc() 함수 사용*/

#include<stdio.h>
#include<malloc.h>

int main()
{
	int* p;
	int i, hap = 0;
	int cnt = 0;
	int data;

	p = (int*)malloc(sizeof(int) * 1);
	printf(" 1 번째 숫자 : ");
	scanf_s("%d", &p[0]);
	cnt++;

	for (i = 2; ; i++)
	{
		printf(" %d 번째 숫자 : ", i);
		scanf_s("%d", &data);

		if (data != 0)
			p = (int*)realloc(p, sizeof(int) * i);
		else
			break;
		p[i - 1] = data;
		cnt++;
	}

	for (i = 0; i < cnt; i++)
		hap = hap + p[i];

	printf("입력 숫자 합 ==> %d\n", hap);

	free(p);
}

/////////////////////////////////////////////////////////////////////////////

/*2차원 배열의 사용*/

#include<stdio.h>

int main()
{
	char data[3][100];
	int i;

	for (i = 0; i <= 2; i++)
	{
		printf(" %d 번째 문자열 : ", i + 1);
		gets_s(data[i], 99);
	}

	printf("\n-- 입력과 반대로 출력(이차원 배열)--\n");
	for (i = 2; i >= 0; i--)
	{
		printf(" %d :%s\n", i + 1, data[i]);
	}
}

/*포인터 배열의 사용*/
#include<stdio.h>
#include<string.h>
#include<malloc.h>

int main()
{
	char* p[3];
	char imsi[100];
	int i, size;

	for (i = 0; i < 3; i++)
	{
		printf(" %d 번째 문자열 : ", i + 1);
		gets_s(imsi, 100);

		size = strlen(imsi);
		p[i] = (char*)malloc((sizeof(char) * size) + 1);

		strcpy_s(p[i], size + 1, imsi);
	}

	printf("\n--입력과 반대로 출력(포인터)--\n");

	for (i = 2; i >= 0; i--)
	{
		printf(" %d :%s\n", i + 1, p[i]);
	}

	for (i = 0; i < 3; i++)
		free(p[i]);
}

/////////////////////////////////////////////////////////////////////////////

연습 문제

/*여러 숫자 중 짝수만 더하기*/

#include<stdio.h>
#include<malloc.h>

int main()
{
	int* p;
	int i, cnt;
	int hap = 0;

	printf(" 입력할 개수는 ? ");
	scanf_s("%d", &cnt);
	p = (int*)malloc(sizeof(int) * cnt);

	for (i = 0; i < cnt; i++)
	{
		printf(" %d 번째 숫자는 : ", i + 1);
		scanf_s("%d", p + i);
	}

	for (i = 0; i < cnt; i++)
	{
		if(p[i]%2==0)
		hap = hap + p[i];
	}

	printf("입력한 숫자중에 짝수의 합은 ==> %d\n", hap);

	free(p);
}

/*입력한 문자열을 거꾸로 출력*/

#include<stdio.h>
#include<malloc.h>
#include<string.h>

int main()
{
	char* p[3];
	char imsi[100];
	int i, k, size;

	for (i = 0; i < 3; i++)
	{
		printf(" %d 번째 문자열 : ");
		scanf_s("%s", imsi, 100);

		size = strlen(imsi);

		p[i] = (char*)malloc((sizeof(char) * size + 1));

		strcpy_s(p[i], size + 1, imsi);

	}

	printf("\n--입력과 반대로 출력(포인터) : 글자 순서도 거꾸로 --\n");
	for (i = 2; i >= 0; i--)
	{
		size = strlen(p[i]);
		imsi[size] = '\0';
		for (k = size - 1; k >= 0; k--)
			imsi[size - k] = p[i][k];

		printf(" %d :%s\n", i + 1, imsi);
	}

	for (i = 0; i < 3; i++)
		free(p[i]);
}
