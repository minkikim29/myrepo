// Chapter11 파일 입출력 //

/*파일을 이용한 입력(1)*/

#include<stdio.h>

void main()
{
	char s[20];
	FILE* rfp;

	fopen_s(&rfp, "C:\\temp\\data1.txt", "r");

	fgets(s, 20, rfp);

	printf("파일에서 읽은 문자열 :");
	puts(s);

	fclose(rfp);
}

/*파일을 이용한 입력(2)*/

#include<stdio.h>
#include<string.h>

void main()
{
	char str[200];
	FILE* rfp;
	int i,ch;
	fopen_s(&rfp, "C:\\windows\\win.ini", "r");


	ch = strlen(str);

	for (;;)
	{
		fgets(str, 200, rfp);
			if (feof(rfp))
				break;

			printf("%s",str);

	}


	fclose(rfp);
}

/*파일을 이용한 입력(3)*/

#include<stdio.h>

void main()
{
	char s[20];

	FILE* rfp;
	int hap = 0;
	int i,in;

	fopen_s(&rfp, "C:\\temp\\data2.txt", "r");

	for (i = 0; i < 5; i++)
	{
		fscanf_s(rfp, "%d", &in);
		hap = hap + in;
	}

	printf("합계 ==> %d\n", hap);

	fclose(rfp);
}

/////////////////////////////////////////////////////////////////////////////

/*파일을 이용한 출력(1)*/

#include<stdio.h>

void main()
{
	char s[20];
	FILE* wfp;

	fopen_s(&wfp, "C:\\temp\\data3.txt", "w");
	printf("문자열을 입력(최대 19자) : ");
	gets_s(s, 20);

	fputs(s, wfp);

	fclose(wfp);
}

/*파일을 이용한 출력(2)*/

#include<stdio.h>

void main()
{
	char str[200];
	int i;

	FILE* rfp;
	FILE* wfp;

	fopen_s(&rfp, "C:\\windows\\win.ini", "r");
	fopen_s(&wfp, "C:\\temp\\data5.txt", "w");

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
	int in, i;

	fopen_s(&wfp, "C:\\temp\\data7.txt", "w");

	for (i = 0; i < 5; i++)
	{
		printf(" 숫자 %d :", i + 1);
		scanf_s("%d", &in);
		hap = hap + in;
	}

	fprintf(wfp, "합계 ==> : %d\n", hap);

	fclose(wfp);
}

/////////////////////////////////////////////////////////////////////////////

연습 문제

/*구구단을 파일에 출력*/

#include<stdio.h>

void main()
{
	FILE* wfp;
	int i, k;

	fopen_s(&wfp, "C:\\temp\\gugu.txt","w");

	for (i = 2; i <= 9; i++)
		fprintf(wfp, "# 제%d단#", i);

	fprintf(wfp,"\n\n");

	for (i = 1; i <= 9; i++)
	{
		for (k = 2; k <= 9; k++)
		{
			fprintf(wfp, "%2dX%2d=%2d", k, i, k * i);
		}
		fprintf(wfp,"\n");
	}

	fclose(wfp);
}

/*파일에서 읽어온 문자열을 파일에 거꾸로 출력*/

#include<stdio.h>
#include<string.h>

void main()
{
	char in[200], out[200];
	int i, size;
	FILE* rfp, * wfp;

	fopen_s(&rfp, "C:\\temp\\in.txt", "r");
	fopen_s(&wfp, "C:\\temp\\out.txt", "w");

	while (1)
	{
		fgets(in, 200, rfp);

		if (feof(rfp))
			break;

		size = strlen(in);
		for (i = size - 1; i >= 0; i--)
			out[size - 1-i] = in[i - 1];

		out[size - 1] = '\0';
		fputs(out, wfp);
		fputs("\n", wfp);

	}

	fclose(rfp);
	fclose(wfp);
}
