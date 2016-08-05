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
void freeBN(BigNumber **);

int main (int argc, char **argv)
{
	BigNumber *BN1;
	BigNumber *BN2;
	BigNumber *BN3;
	int i;

	setlocale(LC_ALL, "Russian");
	
	i = 0;
	BN1 = createBigNum("0x3333333FDA751531247652394782398746BCB23765437825423478523485234");
	printBigNum(BN1);
	printf("\n");
	BN2 = createBigNum("0x4242AAAAAAAA48762348765FACBDDDA4325AA");
	printBigNum(BN2);
	printf("\n");
	BN3 = addBigNums(BN2, BN1);
	printBigNum(BN3);	
	for(i = 0; i < 100; i++)
	{
		BN3 = addBigNums(BN3, BN1);
	}

	printBigNum(BN3);
	freeBN(&BN1);
	freeBN(&BN2);
	freeBN(&BN3);
	
	system("pause");
	return 0;
}

void freeBN(BigNumber** BigNum)
{
	free((*BigNum)->number);
	free(*BigNum);
}

BigNumber *createBigNum(const char *cStr)
{
	// создаем новую строку 
	int StrLen;
	char tstr[11];
	char *Str;
	BigNumber * NewNum;
	unsigned long long i,k, rest;
	
	Str = (char *) malloc(strlen(cStr) - 1);
	strcpy(Str, cStr + 2);
	Str[strlen(cStr)] = 0;
	
	StrLen = strlen(Str);
	//то, что мы вернем
	NewNum = (BigNumber *) malloc(sizeof(BigNumber));
	i = 0;

		// выделяем память
		NewNum->number = (unsigned int *) malloc(sizeof(unsigned int) * (StrLen/8 + 1));
		// сколько букв не влезло в целое слово
		rest = StrLen % 8;
		if(rest == 0)
			NewNum->length = (StrLen/8);
		else
			NewNum->length = (StrLen/8)+1;
		// заполняем 0 все значения.
		for(i = 0; i < NewNum->length; i++)
			NewNum->number[i] = 0;
		i = 0;
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
			for(k = 0; k < rest; k++)
			{
				tstr[k] = Str[k];
			}
			tstr[k] = 0;
			NewNum->number[i] = strtoul(tstr, NULL, 16);

	return NewNum;
}

BigNumber *addBigNums(const BigNumber *BN1, const BigNumber *BN2)
{
	const BigNumber *Bigger, *Smaller;
	unsigned long long i, carry, rest;
	BigNumber *Res;
	i = 0;
	carry = 0;
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
	Res->number = (unsigned int *) malloc(sizeof(unsigned int) * (Bigger->length + 1));
	// складываем сначала оба числа
		for(i=0; i < Smaller->length; i++)
		{
			if(BN1->number[i] > INT_MAX && BN2->number[i] > INT_MAX)
			{
				Res->number[i] = (BN1->number[i] - INT_MAX) + (BN2->number[i] - INT_MAX)  + carry - 2;
				carry = 1;
				// если у нас первое число меньше второго
			} else if(BN1->number[i] > INT_MAX && BN2->number[i] <= INT_MAX)
			{
				if((BN1->number[i] - INT_MAX) + BN2->number[i] + carry >= INT_MAX)
				{
					Res->number[i] = (BN1->number[i] - INT_MAX) + BN2->number[i] + carry - INT_MAX - 2;
					carry = 1;
				}else
				{
					Res->number[i] = BN1->number[i] + BN2->number[i] + carry;
					carry = 0;
				}
			} else if(BN1->number[i] <= INT_MAX && BN2->number[i] > INT_MAX)
			{
				// если у нас второе число меньше половины
				if(BN1->number[i] + (BN2->number[i] - INT_MAX) + carry >= INT_MAX)
				{
					Res->number[i] = BN1->number[i] + (BN2->number[i] - INT_MAX) + carry - INT_MAX - 2;
					carry = 1;
				}else
				{
					Res->number[i] = BN1->number[i] + BN2->number[i] + carry;
					carry = 0;
				}
				// если у нас оба числа меньше половины - то не будет переполнения
			} else
			{
				Res->number[i] = BN1->number[i] + BN2->number[i] + carry;
				carry = 0;
			}
		}
		// теперь копируем одно число + перенос, если есть
		for(; i < Bigger->length; i++)
		{
			if(Bigger->number[i] == 0xFFFFFFFF && carry == 1)
			{
				carry = 1;
			} else
			{
				Res->number[i] = Bigger->number[i] + carry;
				carry = 0;
			}
		}
		// i++ уже увеличено, и если есть перенос, 
		// при том, что мы уже все числа скопировали, то тогда создаем новый разряд
		// и устанавливаем значение Length!
		if(carry == 1)
		{
			Res->number[i] = 1;
			Res->length = i+1;
		}else
			Res->length = i;

	return Res;
}

void printBigNum(BigNumber *BN)
{
	unsigned long long i;
	i = BN->length-1;
	if(i == 0)
	{
		printf("%08X \n", BN->number[i]);
		return;
	}
	while(1){
		printf("%08X ", BN->number[i]);
		if(i == 0)
			break;
		i--;
	}
	printf("\n");
}

