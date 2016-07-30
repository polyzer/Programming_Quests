#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>

typedef struct _BigNumber{
	unsigned long long length;
	unsigned int * number;
} BigNumber;

BigNumber *createBigNum(const char *Str);
BigNumber *addBigNums(const BigNumber *BN1, const BigNumber *BN2);
void printBigNum(BigNumber *BN);

int main (int argc, char **argv)
{
	BigNumber *BN1;
	BigNumber *BN2;
	BigNumber *BN3;

	setlocale(LC_ALL, "Russian");
	
	BN1 = createBigNum("0xFFFFFFFF");
	printBigNum(BN1);
	BN2 = createBigNum("1");
	printBigNum(BN2);
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

BigNumber *createBigNum(const char *cStr)
{
	// создаем новую строку 
	int StrLen;
	char tstr[11];
	char *Str;
	BigNumber * NewNum;
	unsigned long long i,k, rest;
	
	Str = (char *) malloc(strlen(cStr) + 1);
	strcpy(Str, cStr);
	Str[strlen(cStr)] = 0;
	
	StrLen = strlen(Str);
	//то, что мы вернем
	NewNum = (BigNumber *) malloc(sizeof(BigNumber));
	i = 0;

	// прворяем - 16 ричное ли число!
	if ((Str[1] == 'x' || Str[1] == 'X') && (Str[0] == '0') || isalpha(Str[0]))
	{
		if((Str[1] == 'x' || Str[1] == 'X') && (Str[0] == '0'))
		{
			Str[1] = '0';
			Str[0] = '0';
		}
		// выделяем память
		NewNum->number = (unsigned int *) malloc(sizeof(unsigned int) * (StrLen/8 + 1));
		// сколько букв не влезло в целое слово
		rest = StrLen % 8;
		NewNum->length = (StrLen/8) + 1;
		// заполняем слова!
		while(StrLen > 7)
		{
			for(k = 0; k < 8; k++)
			{
				tstr[7-k] = Str[StrLen - k - 1];
			}
			StrLen -= 8;
			tstr[k] = 0;
			NewNum->number[i] = strtoul(tstr, NULL, 16);
			i++;
		}
		if(rest == 2 && Str[0] == '0' && Str[0] == '0')
		{
			NewNum->length--;
		}else
		{
			for(k = 0; k < rest; k++)
			{
				tstr[k] = Str[k];
			}
			tstr[k] = 0;
			NewNum->number[i] = strtoul(tstr, NULL, 16);
		}
	}else
	{
		// выделяем память
		NewNum->number = (unsigned int *) malloc(sizeof(unsigned int) * (StrLen/10 + 1));
		// сколько букв не влезло в целое слово
		rest = StrLen % 10;
		NewNum->length = (StrLen/10) + 1;
		// заполняем слова!
		while(StrLen > 10)
		{
			for(k = 0; k < 10; k++)
			{
				tstr[9-k] = Str[StrLen - k - 1];
			}
			StrLen -= 10;
			tstr[k] = 0;
			// переводим в число и проверяем на прееполнение
			if((NewNum->number[i] = strtoul(tstr, NULL, 10)) && NewNum->number[i] == UINT_MAX)
			{
				tstr[0] = '0';
				NewNum->number[i] = strtoul(tstr, NULL, 10);
				StrLen+1;
			}
			i++;
		}
		for(k = 0; k < rest; k++)
		{
			tstr[k] = Str[k];
		}
		tstr[k] = 0;
		NewNum->number[i] = strtoul(tstr, NULL, 10);

	}

	return NewNum;
}

BigNumber *addBigNums(const BigNumber *BN1, const BigNumber *BN2)
{
	const BigNumber *Bigger, *Smaller;
	unsigned long long i, carry, rest;
	BigNumber *Res;
	i = 0;
	// то, с чем будем сравнивать
	Res = (BigNumber *) malloc(sizeof(BigNumber));
	if(BN1->length >= BN2->length){
		Bigger = BN1;
		Smaller = BN2;
	}
	else{
		Bigger = BN2;
		Smaller = BN1;
	}
	// складываем сначала оба числа
		for(i=0; i < Smaller->length; i++)
		{
			if(BN1->number[i] > INT_MAX && BN2->number[i] > INT_MAX)
			{
				carry = 1;
				// ТУТ МОЖЕТ БЫТЬ КОСЯК!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				Res->number[i] = (BN1->number[i] - INT_MAX) + (BN2->number[i] - INT_MAX)  + carry + 1;
			} else if(BN1->number[i] > INT_MAX && BN2->number[i] < INT_MAX)
			{
				// 
				if((BN1->number[i] - INT_MAX) + BN2->number[i] + carry > INT_MAX)
				{
					carry = 1;
					Res->number[i] = (BN1->number[i] - INT_MAX) + BN2->number[i] + carry;
				}else
				{
					carry = 0;
					Res->number[i] =  (BN1->number[i] - INT_MAX) + BN2->number[i] + carry;
				}
			} else if(BN1->number[i] < INT_MAX && BN2->number[i] > INT_MAX)
			{
				// если у нас второе число меньше половины
				if(BN1->number[i] + (BN2->number[i] - INT_MAX) + carry > INT_MAX)
				{
					carry = 1;
					Res->number[i] = BN1->number[i] + (BN2->number[i] - INT_MAX) + carry;
				}else
				{
					carry = 0;
					Res->number[i] = BN1->number[i] + (BN2->number[i] - INT_MAX) + carry;
				}
				// если у нас оба числа меньше половины
			} else
			{
				carry = 0;
				Res->number[i] = BN1->number[i] + (BN2->number[i] - INT_MAX) + carry;
			}
		}
		// теперь копируем одно число + перенос, если есть
		for(; i < Bigger->length; i++)
		{
			Res->number[i] += Res->number[i] + carry;
			carry = 0;
		}
		// i++ уже увеличено, и если есть перенос, 
		// при том, что мы уже все числа скопировали, то тогда создаем новый разряд
		// и устанавливаем значение Length!
		if(carry == 1)
		{
			Res->number[i] = 1;
			Res->length = i;
		}else
			Res->length = i-1;

	return Res;
}

void printBigNum(BigNumber *BN)
{
	unsigned long long i;
	i = BN->length-1;
	do {
		printf("%u", BN->number[i]);
	} while(i!= 0);
}

