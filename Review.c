// Ch09~12 Review //

//배열과 포인터//

/*스택의 구현*/

#include<stdio.h>

int main()
{
	int select = 9;
	char carName = 'A';
	char stack[5];
	int top=0;

	while (select != 3)
	{
		printf("<1>자동차 넣기 <2>자동차 빼기 <3>끝 : ");
		scanf_s("%d", &select);

		switch (select)
		{
		case 1:
			if (top >= 5)
			{
				printf("자동차를 넣을 수 없습니다.\n");
			}
			else
			{
				stack[top] = carName++;
				printf(" %c 자동차가 터널 안으로 들어갑니다.\n", stack[top]);
				top++;
			}
			break;
		case 2:
			if (top <= 0)
			{
				printf("자동차를 뺄 수 없습니다.\n");
			}
			else
			{
				top--;
				carName--;
				printf(" %c 자동차가 터널 안에서 빠져나옵니다.\n", stack[top]);
				stack[top] = ' ';
			}
			break;
		case 3:
			printf("프로그램 끝. 터널안의 자동차는 %d대 입니다.\n", top);
			break;
		default :
			printf("잘못 입력하셨습니다.");
		}
	}
}

/*정수형 배열의 메모리 할당*/

#include<stdio.h>

int main()
{
	int aa[3] = { 10,20,30 };

	printf("&aa[0]은 %d, aa+0은 %d \n", &aa[0], aa + 0);
	printf("&aa[1]은 %d, aa+1은 %d \n", &aa[1], aa + 1);
	printf("&aa[2]은 %d, aa+2은 %d \n", &aa[2], aa + 2);
}

/*일반 변수와 포인터 변수의 관계*/

#include<stdio.h>

int main()
{
	char ch;
	char* p;

	ch = 'A';
	p = &ch;

	printf("ch가 가지고 있는 값: ch ==> %c \n", ch);
	printf("ch의 주소(address): &ch ==> %d \n", &ch);
	printf("p가 가지고 있는 값 : p ==> %d \n", p);
	printf("p가 가리키는 곳의 실제 값 ==> %d\n", *p);
}

/*문자열 배열과 포인터의 관계*/

#include<stdio.h>

int main()
{
	char s[8]="Basic-C";
	int i;
	char* p;

	p = s;

	for (i = sizeof(s); i >= 0; i--)
	{
		printf("%c", *(p + i));
	}
}

/*포인터를 이용해 문자열을 거꾸로 출력*/

#include<stdio.h>
#include<string.h>

int main()
{
	char ss[100];
	char* p;
	int i;
	int size = 0;

	printf("문자열을 입력하세요 :");
	gets_s(ss, 100);

	p = ss;

	size = strlen(ss);

	printf("거꾸로 출력된 문자열 : ");

	for (i = size - 1; i >= 0; i--)
	{
		printf("%c", *(p + i));
	}

	printf("\n");
}

/*포인터를 이용한 두값의 교환*/

#include<stdio.h>

int main()
{
	int a, b;
	int tmp;
	int* p, * s;

	printf("a의 값을 입력하세요 : ");
	scanf_s("%d", &a);

	printf("b의 값을 입력하세요 : ");
	scanf_s("%d", &b);

	p = &a;
	s = &b;

	tmp = *p;
	*p = *s;
	*s = tmp;

	printf("바뀐 a값은 %d, b값은 %d\n", a, b);
}

/*포인터를 이용한 배열의 정렬*/

#include<stdio.h>

int main()
{
	int s[10] = { 1,0,3,2,5,4,7,6,9,8 };
	int* p;
	int i, k, tmp;

	p = s;

	printf("정렬 전 배열 s ==> ");
	for (i = 0; i < 10; i++)
	{
		printf("%d ", *(p + i));
	}

	printf("\n");

	for (i = 0; i < 9; i++)
	{
		for (k = i+1; k < 10; k++)
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
	for (i = 0; i < 10; i++)
	{
		printf("%d ", *(p + i));
	}

	printf("\n");
}

// Ch10 함수 //

/*함수를 사용*/

#include<stdio.h>

int coffee_machine(int button)
{
	printf("# 1.(자동으로) 뜨거운 물을 준비한다\n");
	printf("# 2.(자동으로) 종이컵을 준비한다.\n");

	switch (button)
	{
	case 1:printf("# 3.(자동으로) 보통커피를 탄다.\n"); break;
	case 2:printf("# 3.(자동으로) 설탕커피를 탄다.\n"); break;
	case 3:printf("# 3.(자동으로) 블랙커피를 탄다.\n"); break;
	default:printf("# 3.(자동으로) 아무거나 탄다.\n"); break;
	}

	printf("# 4.(자동으로) 물을 붓는다.\n");
	printf("# 5.(자동으로) 스푼으로 저어서 녹인다.\n\n");

	return 0;
}

int main()
{
	int coffee;
	int ret;

	printf("어떤 커피를 드릴까요? (1:보통, 2:설탕, 3:블랙)");
	scanf_s("%d", &coffee);

	ret = coffee_machine(coffee);

	printf("손님 커피 여기 있습니다.");
}

/*본격적인 함수 사용*/

#include<stdio.h>

int calc(int v1, int v2, char op)
{
	int result;

	switch (op)
	{
	case 1:result = v1 + v2; break;
	case 2:result = v1 - v2; break;
	case 3:result = v1 * v2; break;
	case 4:result = v1 / v2; break;
	}

	return result;
}

int main()
{
	int res;
	int oper, a, b;

	printf("계산 입력(1:+,2:-,3:*,4:/) :");
	scanf_s("%d", &oper);

	printf("계산할 두 숫자를 입력 :");
	scanf_s("%d %d", &a, &b);

	res = calc(a, b, oper);

	printf("계산 결과는 : %d\n", res);
}

/*반환값 유무에 따른 함수 비교*/

#include<stdio.h>

void func1()
{
	printf("void 형 함수는 돌려줄게 없음.\n");
}

int func2()
{
	return 100;
}

int main()
{
	int a;

	func1();

	a = func2();
	printf("int형 함수에서 돌려준 값 ==> %d\n", a);
}

/*매개변수 전달 방법 비교*/

#include<stdio.h>

void func1(char a, char b)
{
	int imsi;

	imsi = a;
	a = b;
	b = imsi;
}

void func2(char* a, char* b)
{
	int imsi;

	imsi = *a;
	*a = *b;
	*b = imsi;
}

void main()
{
	char x = 'A', y = 'Z';

	printf("원래 값     :x=&c,y=%c\n", x, y);

	func1(x, y);
	printf("값을 전달한 후 :x=%c,y=%c\n", x, y);

	func2(&x, &y);
	printf("주소를 전달한 후: x=%c,y=%c\n", x, y);
}

/*함수를 이용한 대소문자 변환 프로그램*/

#include<stdio.h>

char upper(char ch)
{
	return ch - ('a' - 'A');
}
char lower(char ch)
{
	return ch + ('a' - 'A');
}

int main()
{
	char in[100];
	char out[100];
	int i = 0;
	char ch;

	printf("문자열을 입력 : ");
	gets_s(in,100);

	do {
		ch = in[i];
		if (ch >= 'A' && ch <= 'Z')
		{
			out[i] = lower(ch);
		}
		else if (ch >= 'a' && ch <= 'z')
		{
			out[i] = upper(ch);
		}
		else
		{
			out[i] = ch;
		}

		i++;
	} while (ch != '\0');

	printf("변환된 문자열 : %s", out);
}

// Ch11 파일 입출력 //

/*파일을 이용한 입력(1)*/

#include<stdio.h>

void main()
{
	char s[20];
	FILE* rfp;

	fopen_s(&rfp, "C:\\temp\\in.txt", "r");
	fgets(s, 20, rfp);

	printf("파일에서 읽은 문자열 : ");
	puts(s);

	fclose(rfp);
}

/*파일을 이용한 입력(2)*/

#include<stdio.h>

void main()
{
	char str[200];
	FILE* rfp;

	fopen_s(&rfp, "C:\\windows\\in.ini", "r");

	for (;;)
	{
		fgets(str, 200, rfp);

		if (feof(rfp))
			break;

		printf("%s", str);
	}

	fclose(rfp);
}

/*파일을 이용한 출력(1)*/

#include<stdio.h>

void main()
{
	char str[200];
	FILE* rfp;
	FILE* wfp;

	fopen_s(&rfp, "C:\\windows\\win.ini","r");
	fopen_s(&wfp, "C:\\temp\\test.txt", "w");

	for (;;)
	{
		fgets(str, 200, rfp);

		if (feof(rfp))
			break;

		fputs(str, wfp);
	}

	fclose(rfp);
	fclose(wfp);

}

/*파일을 이용한 출력(3)*/

#include<stdio.h>

void main()
{
	FILE* wfp;
	int hap = 0;
	int i, in;

	fopen_s(&wfp, "c:\\temp\\data7.txt", "w");

	for (i = 0; i < 5; i++)
	{
		printf(" 숫자%d : ", i + 1);
		scanf_s("%d", &in);
		hap = hap + in;
	}

	fprintf(wfp, "합계 ==> %d\n", hap);

	fclose(wfp);
}

/*구구단을 파일에 출력*/

#include<stdio.h>

int main()
{
	FILE* wfp;
	int i, k;

	fopen_s(&wfp, "c:\\temp\\gugu.txt", "w");

	for (i = 2; i <= 9; i++)
		fprintf(wfp, " #제%d단#", i);

	fprintf(wfp, "\n\n");

	for (i = 1; i <= 9; i++)
	{
		for (k = 2; k <= 9; k++)
		{
			fprintf(wfp, "%2dX%2d=%2d", k, i, k * i);
		}

		fprintf(wfp, "\n");
	}

	fclose(wfp);
}

/*파일에서 읽어온 문자열을 파일에 거꾸로 출력*/

#include<stdio.h>
#include<string.h>

int main()
{
	char str1[200];
	char str2[200];
	FILE* rfp;
	FILE* wfp;
	int i, k,size;

	fopen_s(&rfp, "C:\\temp\\in.txt", "r");
	fopen_s(&wfp, "C:\\temp\\out.txt", "w");

	while (1)
	{
		fgets(str1, 200, rfp);

		if (feof(rfp))
			break;

		size = strlen(str1);
		for (i = size - 1; i > 0; i--)
			str2[size - i - 1] = str1[i - 1];
		str2[size - 1] = '\0';
		fputs(str2, wfp);

		fputs("\n", wfp);
	}

	fclose(rfp);
	fclose(wfp);
}

/*파일에서 읽어온 문자열을 파일에 거꾸로 출력*/

#include<stdio.h>
#include<string.h>

int main()
{
	char str1[200];
	char str2[200];
	FILE* rfp;
	FILE* wfp;
	int i, k,size;

	fopen_s(&rfp, "C:\\temp\\in.txt", "r");
	fopen_s(&wfp, "C:\\temp\\out.txt", "w");

	while (1)
	{
		fgets(str1, 200, rfp);

		if (feof(rfp))
			break;

		size = strlen(str1);
		for (i = size - 1; i > 0; i--)
			str2[size - i - 1] = str1[i - 1];
		str2[size - 1] = '\0';
		fputs(str2, wfp);

		fputs("\n", wfp);
	}

	fclose(rfp);
	fclose(wfp);
}

// Ch12 고급 포인터 //

/*포인터를 사용하여 정수 합계*/

#include<stdio.h>

int main()
{
	int aa[3];
	int* p;
	int i, hap = 0;

	for (i = 0; i < 3; i++)
	{
		printf("%d번째 숫자",i+1);
		scanf_s("%d", &aa[i]);

	}

	p = aa;

	for (i = 0; i < 3; i++)
		hap = hap + *(p + i);

	printf("입력 숫자의 합 ==> %d", hap);
}

/*malloc()함수와 calloc()함수의 비교*/

#include<stdio.h>
#include<malloc.h>

int main()
{
	int* p;
	int* s;
	int i, j;

	printf("malloc() 함수 사용\n");
	p = (int*)malloc(sizeof(int) * 3);

	for (i = 0; i < 3; i++)
		printf("할당된 곳의 초기값 p[%d] ==> %d\n", i, p[i]);

	free(p);

	printf("\ncalloc() 함수 사용\n");

	s = (int*)calloc(sizeof(int), 3);

	for (j = 0; j < 3; j++)
		printf("할당된 곳의 초기값 s[%d] ==> %d\n", j, s[j]);

	free(s);
}

/*realloc() 함수 사용 예*/
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
		printf(" %d번째 숫자 : ", i);
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

/*2차원 배열의 사용*/
#include<stdio.h>

int main()
{
	char data[3][100];
	int i;

	for (i = 0; i < 3; i++)
	{
		printf(" %d 번째 문자열 : ", i + 1);
		gets_s(data[i], 99);
	}

	printf("\n--입력과 반대로 출력(이차원 배열)--\n");
	for (i = 2; i >= 0; i--)
	{
		printf(" %d :%s\n", i + 1, data[i]);
	}
}

/*포인터 배열의 사용 예*/

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

		strcpy_s(p[i], size + i, imsi);
	}

	printf("\n--입력과 반대로 출력(포인터) --\n");
	for (i = 2; i >= 0; i--)
	{
		printf(" %d :%s", i + 1, p[i]);
	}

	for (i = 0; i < 3; i++)
		free(p[i]);
}

/*여러 숫자 중 짝수만 더하기*/

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
		printf(" %d 번째 숫자 : ", i + 1);
		scanf_s("%d", *(p + i));
	}

	for (i = 0; i < cnt; i++)
	{
		if (p[i] % 2 == 0)
			hap = hap + p[i];
	}

	printf("입력한 짝수합 ==> %d\n", hap);

	free(p);
}


