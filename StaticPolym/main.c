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

struct Polynomial *parsePolynomial(char *strPoly)
{
	int i = 0, j = 0, readNum = 1, readDegree = 0, degreeLen = 0, num = 0, factorLen = 0;
	char *degree = NULL, *tempDegree = NULL, *factor = NULL, *tempFactor = NULL;//
	struct Polynomial *polynomial = (struct Polynomial *) malloc(sizeof(struct Polynomial)); // ��������� �������
	int maxDegree = findMaxDegree(strPoly);//������ ������������ ������� ��� ��������� ������
	polynomial->poly = (double *) malloc(sizeof(double) * (maxDegree + 1)); //allocate elements + 1 for set NULL
	for(i=0;i<maxDegree;i++)
	{
		polynomial->poly[i] = NULL;	
	}
	polynomial->poly[maxDegree] == NULL;// set maxDegree element = NULL
	
	for (i = (strlen(strPoly)); i >= 0; i--) 
	{
		if (strPoly[i] == '+' || strPoly[i] == '-' || i == 0)
		{
			if (i == 0)
				j = 0;
			else 
				j = i;
			for(;strPoly[j] != '\0'; j++)
			{
//				if(strPoly[j] == '+' || strPoly[j] == '-')
//					break;
				if (readNum){
					if (strPoly[j] == 'x' || strPoly[j] == 'X')
					{
						readNum = 0;
						readDegree = 1;
						continue;
					}else
					{
						tempFactor = (char *) realloc(tempFactor, factorLen * sizeof(char) + 2);
						tempFactor[factorLen] = strPoly[j];
						factorLen++;
					}
				}
				if (readDegree)
				{
					tempDegree = (char *) realloc(tempDegree, degreeLen * sizeof(char) + 2);
					tempDegree[degreeLen] = strPoly[j];
					degreeLen++;
				}
			}
			if (factorLen != 0)
			{
				tempFactor[factorLen] = '\0';
				
			}
			if(degreeLen != 0){
				tempDegree[degreeLen] = '\0';
				if (factorLen != 0)
				{
					polynomial->poly[atoi(tempDegree)] = 1;
				}else
				{
					polynomial->poly[atoi(tempDegree)] = atof(tempFactor);
				}

				degreeLen = 0;
				readNum = 1;
				readDegree = 0;

			}
		}
	}
	free(tempFactor);
	free(tempDegree);
	tempDegree = NULL;
	tempFactor = NULL;
}

void parseAction(const char *action)
{
	char actions[] = {'=', '+', '-', '*', '/', '%'};
	int len = strlen(action), i = 0, j = 0, count = 0, tempLen;
	char **actArray = (char **) malloc(sizeof(char*));
	char **cur_op;
	//actArray[0] = (char*) malloc(sizeof(char));
	actArray[0] = NULL;
	for (i = 0; i <= len; i++)// ��������� ������ �� ������. �� c = a + b , c += b; 
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
			actArray[count][tempLen] = '\0';//����������� ����� ������
			count++;
			j++; // ����������� �� 1, ����� ������� �� ������� �� ' '
		}// ��������� ���
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
	int num = findMaxDegree("100");
	printf("%i", num);
	system("pause");
    return 0;
}
