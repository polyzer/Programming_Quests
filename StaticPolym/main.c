#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Polynomial.h"

struct Polynomial *Polynomials;
char *todo;

int findMaxDegree(char *strPoly)
{
	int i = 0, j = 0, readNum = 1, readDegree = 0, degreeLen = 0, num;
	char *degree = NULL, *tempDegree = NULL;
	for (i = strlen(strPoly); i >= 0; i--) 
	{
		if (strPoly[i] == '+' || i == 0)
		{
			for (j = i+1; strPoly[j] != '+' || strPoly[j] != '\0'; j++)
			{
				if (readNum)
					if (strPoly[j] == 'x' || strPoly[j] == 'X')
					{
						readNum = 0;
						readDegree = 1;
						continue;
					}
				if (readDegree)
				{
					tempDegree = (char *) realloc(degree, degreeLen * sizeof(char) + 2);
					tempDegree[degreeLen] = strPoly[j];
					degreeLen++;
				}
			}
			if(degreeLen != 0){
				tempDegree[degreeLen + 1] = '\0';
				if (strcmp(degree, tempDegree) < 0)
				{
					degree = (char *) realloc(degree, sizeof(char) * strlen(tempDegree) + 1);
					strcpy(degree, tempDegree);
				}
				degreeLen = 0;
			}
		}
	}
	free(tempDegree);
	if (degreeLen == NULL)
		return 0;
	else 
	{
		num = atof(degree);
	}

}


struct Polynomial *parsePolynomial(char *strPoly)
{
	struct Polynomial *polynomial = (struct Polynomial *) malloc(sizeof(struct Polynomial));
	polynomial->poly = (double*) malloc(sizeof(double));
	polynomial->poly[0] = NULL;
	int i,j, plus, len = strlen(strPoly), count = 0, tempDegree = 0, tempNum = 0, toX = 1, toNum = 0;
	j = len;
	for (i = (len - 1); i >= 0; i--)// разбиваем строку на данные. мб c = a + b , c += b; 
	{
		if (strPoly[i] == '+' || i == 0)
		{
			polynomial->poly = (double*) realloc(polynomial, (count + 2) * sizeof(double));
			polynomial->poly[count+1] = NULL;
//			for(tempLen = j; tempLen > i; tempLen--, j--) 
			{
				if (toX)
				{
					if (strPoly[j] == 'x')
						polynomial->[count][tempLen] = action[j];
				}
				
			}
//			actArray[count][tempLen] = '\0';//присваиваем конец строки
			count++;
			j++; // увеличиваем на 1, чтобы попасть на элемент за ' '
		}// прочитали имя
	}

}


void parseAction(const char *action)
{
	char actions[] = {'=', '+', '-', '*', '/', '%'};
	int len = strlen(action), i = 0, j = 0, count = 0, tempLen;
	char **actArray = (char **) malloc(sizeof(char*));
	char **cur_op;
	//actArray[0] = (char*) malloc(sizeof(char));
	actArray[0] = NULL;
	for (i = 0; i <= len; i++)// разбиваем строку на данные. мб c = a + b , c += b; 
	{
		if (action[i] == ' ' || action[i] == '\0')
		{
			actArray = (char**) realloc(actArray, (count + 2) * sizeof(char*));
			free(actArray[count]);
			actArray[count] = (char*) malloc(sizeof(char) * (i - j + 1));
			//actArray[count+1] = (char*) malloc(sizeof(char*));
			actArray[count+1] = NULL;
			for(tempLen = 0; j < i; tempLen++, j++) 
			{
				actArray[count][tempLen] = action[j];
			}
			actArray[count][tempLen] = '\0';//присваиваем конец строки
			count++;
			j++; // увеличиваем на 1, чтобы попасть на элемент за ' '
		}// прочитали имя
	}
	i = 0;
	cur_op = actArray;
	while(cur_op[i] != NULL) {
		printf("%s\n", cur_op[i]);
		free(cur_op[i]);
		i++;
	}
	system("pause");
	free(cur_op);
	cur_op = NULL;
	//free(actArray);
	actArray = NULL;
}


int main () {
	
	parseAction("c += b");

    return 0;
}
