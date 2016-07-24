#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define SON 10

char *createBigNum(const char *Str);
char *addBigNums(const char *BN1, const char *BN2);
void printBigNum(const char *BN);
char *mulBigNumOnInt(const char *BN1, int mul);

int main (int argc, char **argv)
{
	char *BN1;
	char *BN2;
	char *BN3;

	setlocale(LC_ALL, "Russian");
	
	BN1 = createBigNum("0xFFFFFFFF");
	BN2 = createBigNum("1");
	BN3 = addBigNums(BN2, BN1);
	printBigNum(BN3);

	free(BN1);
	BN1 = NULL;
	free(BN2);
	BN2 = NULL;
	free(BN3);
	BN3 = NULL;

	system("pause");
	return 0;
}

char *createBigNum(const char *Str)
{
	// создаем новую строку
	int i = 0; 
	int factor = 0, hexdecnum = 0, multiplier = 1;
	int StrLen = strlen(Str);
	char *NewNum, *tnum, *tnum1;
	
	if ((Str[1] == 'x' || Str[1] == 'X') && (Str[0] == '0'))
	{
		NewNum = (char *) malloc(sizeof(char) * 2);
		NewNum[0] = '0'; NewNum[1]= 0;
		for(i = (StrLen - 1); i >= 2; i--)
		{
			tnum = (char *) malloc(sizeof(char) * 3);
			if (Str[i] >= 65)
			{
				tnum[0] = Str[i] - 'A' + '0';
				tnum[1] = '1';
				tnum[2] = 0;
			}else
			{
				tnum[0] = Str[i];
				tnum[1] = 0;
			}
			tnum1 = mulBigNumOnInt(tnum, multiplier);
			free(tnum);
			NewNum = addBigNums(NewNum, tnum1);
			multiplier = multiplier * 16;
			free(tnum1);
		}
		return NewNum;

	}else
	{
		NewNum = (char *) malloc(sizeof(char) * (StrLen + 1));
		for(i = (StrLen - 1); i >= 0; i--)
		{
			NewNum[(StrLen - 1) - i] = Str[i];
		}
		NewNum[strlen(Str)] = '\0';
	}

	return NewNum;
}

char *mulBigNumOnInt(const char *BN1, int mul)
{
	int i;
	char *retNum, *tch;
	retNum = (char *) malloc(strlen(BN1) + 1);
	strcpy(retNum, BN1);
	for(i = 1; i < mul; i++)
	{
		tch = retNum;
		retNum = addBigNums(retNum, BN1);
		free(tch);
	}
	return retNum;
}

char *addBigNums(const char *num1, const char *num2)
{
	char *Res;
	const char *tp;
	int i;
	short Digit, Num;
	Digit = 0; 
	if(strlen(num1) > strlen(num2)){
		tp = num1;
	}
	else{
		tp = num2;
	}
	Res = (char *) malloc(sizeof(char) * (strlen(tp)+2));
	i = 0, Num = 0;
	// складываем числа
	while(num1[i] != 0 && num2[i] != 0)
	{
		// складываем 2 числа
		Num = num1[i] + num2[i] - 96; // 96 == 2*'0'
		// записываем в текущий разряд
		Res[i] = Digit + (Num % SON) + '0';
		// перенос
		Digit = Num / SON;
		i++;
	}
	// теперь записываем оставшуюся часть числа!
	while(tp[i] != 0)
	{
		// если есть с чем складывать!
		if(Digit > 0)
		{
			//Num = tp[i] - '0'; // 98 == 2*'0'
			Num = Digit + (tp[i] - '0');
			Res[i] = (Num % SON) + '0';		
			Digit = Num / SON;
		}else
			Res[i] = tp[i];
		i++;
	}
	if(Digit > 0)
	{
		Res[i] = Digit + '0';
		Res[i+1] = 0;
	}else
		Res[i] = 0;

	return Res;
}

void printBigNum(const char *BN)
{
	int i;
	for(i = (strlen(BN) - 1); i >= 0; i--)
	{
		printf("%c", BN[i]);
	}
	printf("\n");
}

