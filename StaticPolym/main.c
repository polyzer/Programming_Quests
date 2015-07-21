#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Polynomial.h"

struct Polynomial *Polynomials;
char *todo;
/*
int findMaxDegree(char *strPoly)
{
	int i = 0, j = 0, readNum = 1, readDegree = 0, degreeLen = 0, num;
	char *degree = NULL, *tempDegree = NULL;
	for (i = (strlen(strPoly)); i >= 0; i--) 
	{
		if (strPoly[i] == '+' || i == 0)
		{
			if (i == 0)
				j = 0;
			else 
				j = i+1;
			for(;strPoly[j] != '\0'; j++)
			{
//				if(strPoly[j] == '+')
//					break;
				if (readNum)
					if (strPoly[j] == 'x' || strPoly[j] == 'X')
					{
						readNum = 0;
						readDegree = 1;
						continue;
					}
				if (readDegree)
				{
					tempDegree = (char *) realloc(tempDegree, degreeLen * sizeof(char) + 2);
					tempDegree[degreeLen] = strPoly[j];
					degreeLen++;
				}
			}
			if(degreeLen != 0){
				tempDegree[degreeLen] = '\0';
				if ((degree == NULL) || (atof(degree) < atof(tempDegree)))
				{
					degree = (char *) realloc(degree, sizeof(char) * (strlen(tempDegree) + 1));
					strcpy(degree, tempDegree);
					degree[strlen(tempDegree)] = '\0';
				}
				degreeLen = 0;
				readNum = 1;
				readDegree = 0;

			}
		}
	}
	free(tempDegree);
	tempDegree = NULL;
	if (degree == NULL)
		return 0;
	else 
	{
		num = atof(degree);
		free(degree);
		degree = NULL;
		return num;
	}

}
*/

void printPolynomial(struct Polynomial* poly)
{
	int i = 0, j;
	while (1)
	{
		if((poly->monomials[i].degree == 0) && (poly->monomials[i].factor == 0))
			break;
		printf("%f %i\n", poly->monomials[i].factor, poly->monomials[i].degree);
		i++;
	}

}

struct Polynomial *parsePolynomial(char *strPoly)
{
	int i = 0, j = 0, readNum = 1, readDegree = 0, degreeLen = 0, num = 0, factorLen = 0, monomialCount = 0;
	char *tempDegree = NULL, *tempFactor = NULL;//
	struct Polynomial *polynomial = (struct Polynomial *) malloc(sizeof(struct Polynomial)); // объ€вл€ем полином
	polynomial->monomials = (struct Monomial *) malloc(sizeof(struct Monomial) * 2);
	polynomial->monomials[monomialCount].degree = 0;
	polynomial->monomials[monomialCount].factor = 0;
	polynomial->monomials[monomialCount + 1].degree = 0;
	polynomial->monomials[monomialCount + 1].factor = 0;

	for (i = (strlen(strPoly)); i >= 0; i--) 
	{
		if (strPoly[i] == '+' || strPoly[i] == '-' || i == 0)
		{
			if (i == 0)
				j = 0;
			else {
				j = i+1;
				tempFactor = (char *) realloc(tempFactor, sizeof(char) * (factorLen + 2));
				tempFactor[factorLen] = strPoly[i];
				factorLen++;
			}
			for(;strPoly[j] != '\0'; j++)
			{
				if(strPoly[j] == '+' || strPoly[j] == '-')
					break;
				if (readNum){
					if (strPoly[j] == 'x' || strPoly[j] == 'X')
					{
						readNum = 0;
						readDegree = 1;
						continue;
					}else
					{
						tempFactor = (char *) realloc(tempFactor, sizeof(char) * (factorLen + 2));
						tempFactor[factorLen] = strPoly[j];
						factorLen++;
					}
				}
				if (readDegree)
				{
					tempDegree = (char *) realloc(tempDegree, sizeof(char) * (degreeLen + 2));
					tempDegree[degreeLen] = strPoly[j];
					degreeLen++;
				}
			}
			if (factorLen != 0)
			{
				tempFactor[factorLen] = '\0';
			}
			if(degreeLen != 0)
			{
				tempDegree[degreeLen] = '\0';
			}
			if ((factorLen != 0) && (degreeLen == 0))//если у нас нет коэффициента, но есть степень
			{
				polynomial->monomials[monomialCount].degree = 0;
				polynomial->monomials[monomialCount].factor = atof(tempFactor);
				monomialCount++;
			}
			if ((factorLen == 0) && (degreeLen != 0)) //если у нас нет степени, но есть коэффициент
			{
				polynomial->monomials[monomialCount].degree = atoi(tempDegree);
				polynomial->monomials[monomialCount].factor = 0;				
				monomialCount++;
			}
			if ((factorLen != 0) && (degreeLen != 0))
			{
				polynomial->monomials[monomialCount].degree = atoi(tempDegree);
				polynomial->monomials[monomialCount].factor = atof(tempFactor);				
				monomialCount++;
			}
			polynomial->monomials = (struct Monomial *) realloc(polynomial->monomials, sizeof(struct Monomial) * (monomialCount + 2));
			polynomial->monomials[monomialCount+1].degree = 0;
			polynomial->monomials[monomialCount+1].factor = 0;
			readNum = 1;
			readDegree = 0;
			factorLen = 0;
			degreeLen = 0;
			free(tempFactor);
			free(tempDegree);
			tempDegree = NULL;
			tempFactor = NULL;
		}
	}
	return polynomial;
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
		}// прочитали им€
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
	
	//parseAction("c += b");
	printPolynomial(parsePolynomial("100x3-200"));
	system("pause");
    return 0;
}
