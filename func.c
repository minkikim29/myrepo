// Chapter10 함수 //

/*직접 커피를 타는 과정*/

#include<stdio.h>

int main()
{
	int coffee;

	printf("어떤 커피 드릴까요? (1:보통, 2:설탕, 3:블랙)");
	scanf_s("%d", &coffee);

	printf("# 1. 뜨거운 물을 준비한다.\n");
	printf("# 2. 종이컵을 준비한다.\n");

	switch (coffee)
	{
	case 1:printf("# 3. 보통커피를 탄다\n"); break;
	case 2:printf("# 3. 설탕커피를 탄다\n"); break;
	case 3:printf("# 3. 블랙커피를 탄다\n"); break;
	default:printf("# 3. 아무거나 탄다\n"); break;
	}

	printf("# 4. 물을 붓는다\n");
	printf("# 5. 스푼으로 저어서 녹인다.\n\n");

	printf("손님~~ 커피 여기 있습니다.\n");
}

/*함수를 사용하여 변경*/

#include<stdio.h>

int coffee_machine(int button)
{
	printf("\n# 1.(자동으로) 뜨거운 물을 준비한다.");
	printf("\n# 2.(자동으로) 종이컵을 준비한다.\n");

	switch (button)
	{
	case 1:printf("# 3. (자동으로) 보통커피를 탄다.\n"); break;
	case 2:printf("# 3. (자동으로) 설탕커피를 탄다.\n"); break;
	case 3:printf("# 3. (자동으로) 블랙커피를 탄다.\n"); break;
	default: printf("# 3.(자동으로) 아무거나 탄다.\n"); break;
	}

	printf("# 4. (자동으로) 물을 붓는다\n");
	printf("# 5. (자동으로) 스푼으로 저어서 녹인다.\n\n");

	return 0;
}

int main()
{
	int coffee;
	int ret;

	printf("어떤 커피를 드릴까요? (1:보통, 2:설탕, 3:블랙)");
	scanf_s("%d", &coffee);

	ret = coffee_machine(coffee);

	printf("손님~ 커피 여기 있습니다.\n\n");
}

/*주문을 여러개 받도록 변경한 예*/

#include<stdio.h>

int coffee_machine(int button)
{
	printf("\n# 1.(자동으로) 뜨거운 물을 준비한다.");
	printf("\n# 2.(자동으로) 종이컵을 준비한다.\n");

	switch (button)
	{
	case 1:printf("# 3. (자동으로) 보통커피를 탄다.\n"); break;
	case 2:printf("# 3. (자동으로) 설탕커피를 탄다.\n"); break;
	case 3:printf("# 3. (자동으로) 블랙커피를 탄다.\n"); break;
	default: printf("# 3.(자동으로) 아무거나 탄다.\n"); break;
	}

	printf("# 4. (자동으로) 물을 붓는다\n");
	printf("# 5. (자동으로) 스푼으로 저어서 녹인다.\n\n");

	return 0;
}

int main()
{
	int coffee;
	int ret;

	printf("A님, 어떤 커피를 드릴까요? (1:보통, 2:설탕, 3:블랙)");
	scanf_s("%d", &coffee);
	ret = coffee_machine(coffee);
	printf("A님 커피 여기 있습니다.\n\n");

	printf("B님, 어떤 커피를 드릴까요? (1:보통, 2:설탕, 3:블랙)");
	scanf_s("%d", &coffee);
	ret = coffee_machine(coffee);
	printf("B님 커피 여기 있습니다.\n\n");

	printf("C님, 어떤 커피를 드릴까요? (1:보통, 2:설탕, 3:블랙)");
	scanf_s("%d", &coffee);
	ret = coffee_machine(coffee);
	printf("C님 커피 여기 있습니다.\n\n");
}

/////////////////////////////////////////////////////////////////////////////

/*본격적인 함수 사용 예(1)*/

#include<stdio.h>

int plus(int v1, int v2)
{
	int result;
	result = v1 + v2;
	return result;
}

int main()
{
	int hap;

	hap = plus(100, 200);

	printf("100과 200의 plus() 함수 결과는 : %d\n", hap);
}

/*본격적인 함수 사용(2)*/

#include<stdio.h>

int calc(int v1, int v2, int op)
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
	int oper, a, b;
	int ret;

	printf("계산 입력 (1:+, 2:-, 3:*, 4:/) ");
	scanf_s("%d", &oper);

	printf("계산할 두 수 입력 : ");
	scanf_s("%d %d", &a, &b);

	ret = calc(a, b, oper);

	printf("계산 결과는 %d\n", ret);
}

/////////////////////////////////////////////////////////////////////////////

/*지역변수와 전역변수의 구분*/

#include<stdio.h>

int a = 100;

void func1()
{
	int a=200;
	printf("func1()에서 a의 값 ==> %d\n", a);
}

int main()
{
	func1();
	printf("main() 에서 a의 값 ==> %d\n", a);
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

/////////////////////////////////////////////////////////////////////////////

/*매개변수 전달 방법(값으로 전달)*/

#include<stdio.h>

void func1(int a)
{
	a = a + 1;
	printf("전달받은 a ==> %d\n", a);
}

void main()
{
	int a = 10;

	func1(a);
	printf("func1() 실행 후의 a ==> %d\n", a);
}

/*매개변수 전달 방법(주소로 전달)*/

#include<stdio.h>

void func1(int* a)
{
	*a = *a + 1;
	printf("전달받은 a ==> %d\n", *a);
}

void main()
{
	int a = 10;

	func1(&a);
	printf("func1() 실행 후의 a ==> %d\n", a);
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

	printf("원래 값   : x=%c, y=%c\n", x, y);

	func1(x, y);

	printf("값을 전달한 후 : x=%c, y=%c\n", x, y);

	func2(&x, &y);

	printf("주소를 전달한 후 : x=%c, y=%c\n", x, y);
}

/////////////////////////////////////////////////////////////////////////////

연습 문제

/*함수를 이용한 구구단 프로그램*/

#include<stdio.h>

void gugu(int dan)
{
	int i;

	for (i = 1; i <= 9; i++)
	{
		printf("%2d X%2d = %2d\n", dan, i, dan * i);
	}
}

void main()
{
	int input;

	printf("출력하고 싶은 단을 입력 : ");
	scanf_s("%d", &input);

	gugu(input);
}

/*함수를 이용한 대소문자 변환 프로그램(my style)*/

#include<stdio.h>
#include<string.h>

void change(char in[100],char out[100])
{
	int i,ch;
	int diff = 'a' - 'A';
	ch = strlen(in);
	for (i = 0; i < ch; i++)
	{
		if (in[i] >= 'A' && in[i] <= 'Z')
		{
			out[i] =in[i] + diff;
		}
		else if (in[i] >= 'a' && in[i] <= 'z')
		{
			out[i] = in[i] - diff;
		}
		else
			out[i] = in[i];
	}

	out[ch] = '\0';
}

void main()
{
	char ss[100];
	char tt[100];
	printf("문자열을 입력하세요 : ");
	scanf_s("%s", ss, 100);

	change(ss, tt);
	printf("대소문자 변환 함수 : %s",tt);

}

/*함수를 이용한 대소문자 변환 프로그램(awnser)*/

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
	int i = 0;
	char ch;
	char in[100], out[100];

	printf("문자열을 입력<100자 이내> : ");
	scanf_s("%s", in, 100);
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
			out[i] = ch;
		i++;
	} while (ch != '\0');
	out[i] = '\0';
	printf("변환된 결과 ==> %s", out);
}
/////////////////////////////////////////////////////////////////////////////
