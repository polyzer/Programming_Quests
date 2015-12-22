#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define SON 10
// большое число, будем хранить 10-ричные числа

char *createBigNum(const char *Str);
char *addBigNums(const char *BN1, const char *BN2);
void printBigNum(const char *BN);


int main (int argc, char **argv)
{
	char *BN1;
	char *BN2;
	char *BN3;

	setlocale(LC_ALL, "Russian");
	
	BN1 = createBigNum("23432");
	BN2 = createBigNum("6846");
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
	// выделяем память под число и \0
	int i = 0;
	int StrLen = strlen(Str);
	char *NewNum = (char *) malloc(sizeof(char) * (StrLen + 1));
	for(i = (StrLen - 1); i >= 0; i--)
	{
		NewNum[(StrLen - 1) - i] = Str[i];
	}
	NewNum[strlen(Str)] = '\0';

	return NewNum;
}



char *addBigNums(const char *BN1, const char *BN2)
{
	char *NewNum;
	char Bigger;
	int i = 0, j = 0, nextDigit = 0, tempNum = 0;

	if(strlen(BN1) >= strlen(BN2))
		Bigger = '1';
	else
		Bigger = '2';

	if(Bigger == '1')
	{
		// выделяем память по формуле:
		// общая длина + 1 под новый разряд + 1 под \0
		NewNum = (char *) malloc(sizeof(char) * (strlen(BN1) + 2));
	} else
	{
		// выделяем память по формуле:
		// общая длина + 1 под новый разряд + 1 под \0
		NewNum = (char *) malloc(sizeof(char) * (strlen(BN2) + 2));	
	}

	if (Bigger == '1')
	{
		for (i = 0; i < strlen(BN1); i++)
		{
			//если во втором числе разряды кончились
			if (BN2[i] == '\0')
			{
				//здесь сделать проверку на получившийся новый разряд
				for (j = i; j < strlen(BN1); j++)
				{
					if (j == i)
					{
						NewNum[j] = (((int)(BN1[j] - '0')) + tempNum / SON) + '0';
					} else
						NewNum[j] = BN1[j];
				}
				NewNum[strlen(BN1)] = '\0';
				return NewNum;
			} else
				// если у нас оба числа еще не кончились!
			{
				// получаем число - сумму чисел из разряда!
				// здесь прибавляем в текущий разряд нового числа 1 или 0
				NewNum[i] = '0' + (tempNum / SON);
				// здесь вычисляем сумму текущих разрядов для чисел, которые нужно суммировать
				tempNum = (int)((int)(BN1[i] - '0') + (int)(BN2[i] - '0'));
				// здесь в текущий разряд нового числа добавляем остаток отделения
				// временного числа на систему счисления!
				if (('0' + ((tempNum % SON) + (int)(NewNum[i] - '0'))) == ':')
				{
					NewNum[i] = '0';
					tempNum += SON;
				} else
				{
					NewNum[i] = '0' + ((tempNum % SON) + (int)(NewNum[i] - '0'));
				}
				if (i == (strlen(BN1) - 1))
				{
					if (tempNum / SON == 0)
					{
						NewNum[i+1] = '\0';
						return NewNum;
					}
					else 
					{
						NewNum[i+1] = '0' + (tempNum / SON); 
						NewNum[i+2] = '\0';
						return NewNum;
					}
				}
			}
		}
	}
		else
	{
		for (i = 0; i < strlen(BN2); i++)
		{
			//если во втором числе разряды кончились
			if (BN1[i] == '\0')
			{
				//здесь сделать проверку на получившийся новый разряд
				for (j = i; j < strlen(BN2); j++)
				{
					if (j == i)
					{
						NewNum[j] = (((int)(BN2[j] - '0')) + tempNum / SON) + '0';
					} else
						NewNum[j] = BN2[j];
				}
				NewNum[strlen(BN2)] = '\0';
				return NewNum;
			} else
				// если у нас оба числа еще не кончились!
			{
				// получаем число - сумму чисел из разряда!
				// здесь прибавляем в текущий разряд нового числа 1 или 0
				NewNum[i] = '0' + (tempNum / SON);
				// здесь вычисляем сумму текущих разрядов для чисел, которые нужно суммировать
				tempNum = (int)((int)(BN1[i] - '0') + (int)(BN2[i] - '0'));
				// здесь в текущий разряд нового числа добавляем остаток отделения
				// временного числа на систему счисления!
				if (('0' + ((tempNum % SON) + (int)(NewNum[i] - '0'))) == ':')
				{
					NewNum[i] = '0';
					tempNum += SON;
				} else
				{
					NewNum[i] = '0' + ((tempNum % SON) + (int)(NewNum[i] - '0'));
				}
				if (i == (strlen(BN2) - 1))
				{
					if (tempNum / SON == 0)
					{
						NewNum[i+1] = '\0';
						return NewNum;
					}
					else 
					{
						NewNum[i+1] = '0' + (tempNum / SON); 
						NewNum[i+2] = '\0';
						return NewNum;
					}
				}
			}
		}
	}
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
