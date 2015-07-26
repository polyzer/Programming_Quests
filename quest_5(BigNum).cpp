#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct BigNum{
	char *StrNum;
	char sign; // знак, <0< 
};

struct BigNum *addBigNum(struct BigNum *bigNum1, struct BigNum *bigNum2)
{
	unsigned int digit = 1, i = 0;
	unsigned short int curNum1 = 0, curNum2 = 0;
	struct BigNum *tempBigNum = NULL;
	
	tempBigNum = (struct BigNum *) malloc(sizeof(struct BigNum));
	tempBigNum->StrNum = (char *) realloc(tempBigNum->StrNum, sizeof(char) * strlen(bigNum1->StrNum));
	
	for(i = 0; i < strlen(bigNum1->StrNum); i++)
	{
		tempBigNum->StrNum[i] = bigNum1->StrNum[i];
	}

	for (i = 0; i < strlen(bigNum2->StrNum); i++)
	{
		if (i < strlen(bigNum1->StrNum))
		{
			tempBigNum[
		}
	}
	tempBigNum->StrNum[strlen(bigNum1->StrNum)] = '\0';
}
struct BigNum *mulBigNum(struct BigNum *bigNum1, struct BigNum *bigNum2)
{
}
struct BigNum *divBigNum(struct BigNum *bigNum1, struct BigNum *bigNum2)
{
}
struct BigNum *modBigNum(struct BigNum *bigNum1, struct BigNum *bigNum2)
{
}

int main(int argc, char **argv) {


	return 0;
}