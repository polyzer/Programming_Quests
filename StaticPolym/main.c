#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Polynomial.h"


//переменные начинаюстя с $

int findMaxDegree(struct Polynomial *poly)
{
	int i, num = 0;
	for(i = 0; i < poly->monomialsCount; i++) 
	{
		if (poly->monomials[i].degree > num)
		{
			num = poly->monomials[i].degree;
		}
	}
	return num;
}
struct Polynomial *GCD(struct Polynomial *poly1, struct Poynomial *poly2);
struct Polynomial *divPolynomials(struct Polynomial *poly1, struct Polynomial *poly2)
{
	//полиномы должны быть отсортированы в обратном порядке - в [0] должен лежать больший.
	struct Polynomial *answer = (struct Polynomial *) malloc(sizeof(struct Polynomial)); // то что получилось в рез-те деления
	struct Polynomial *residue = subPolynomials(poly1, 0), *temp; // остаток
	struct Polynomial *subtrahend; // вычитаемое
	int tempMaxDeg1 = findMaxDegree(poly1), tempMaxDeg2 = findMaxDegree(poly2), i = 0; //макс степени и счетчик
	answer->monomialsCount = 1;
	residue->monomialsCount = 1;

	if (tempMaxDeg1 < tempMaxDeg2)
		return NULL;
	while(1)
	{
		answer->monomials = (struct Monomial *) malloc(sizeof(struct Monomial) * answer->monomialsCount);
		answer->monomials[answer->monomialsCount - 1].factor = residue->monomials[answer->monomialsCount - 1].factor / 
															   poly2->monomials[answer->monomialsCount - 1].factor;
		answer->monomials[answer->monomialsCount - 1].degree = residue->monomials[answer->monomialsCount - 1].degree - 
															   poly2->monomials[answer->monomialsCount - 1].degree;
		subtrahend = mulPolynomials(residue, answer);
		temp = subPolynomials(residue, subtrahend);
		free(residue);// освобождаем старый, он больше не нужен
		residue = temp;
		temp = NULL;
		
		if (findMaxDegree(poly1) < findMaxDegree(poly2)){
			free(subtrahend->monomials);
			free(subtrahend);
			free(residue->monomials);
			free(residue);
			return answer;
		}
		answer->monomialsCount++;
	}


}
struct Polynomial *mulPolynomials(struct Polynomial *poly1, struct Polynomial *poly2)
{
	struct Polynomial *temp = (struct Polynomial *) malloc(sizeof(struct Polynomial));
	int i = 0, j = 0, number = 0;
	temp->monomialsCount = poly1->monomialsCount * poly2->monomialsCount;
	temp->monomials = (struct Monomial*) realloc(temp->monomials, sizeof(struct Monomial) * (temp->monomialsCount)); 
	//теперь добавляем данные из 
	for(i = 0; i < poly2->monomialsCount; i++)
	{
		for(j = 0; j < poly1->monomialsCount; j++)
		{
			temp->monomials[number].factor = poly2->monomials[i].factor * poly1->monomials[j].factor;
			if (poly1->monomials[j].degree == 0){// если в первом полиноме у текущего монома степень == 0
				temp->monomials[number].degree = poly2->monomials[i].degree;
			} else if (poly2->monomials[i].degree == 0){ // если нет, но у второго полинома у текущего монома степень == 0
				temp->monomials[number].degree = poly1->monomials[j].degree;
			} else{ //если ни то, ни другое, то умножаем степени и записываем
				temp->monomials[number].degree = poly1->monomials[j].degree * poly2->monomials[i].degree;
			}
		}
	}
	return temp;
}
struct Polynomial *subPolynomials(struct Polynomial *poly1, struct Polynomial *poly2)
{
	struct Polynomial *temp = (struct Polynomial *) malloc(sizeof(struct Polynomial));
	//сначала копируем данные из poly1;
	int i = 0, j = 0, wasSubstructed = 0;
	temp->monomialsCount = poly1->monomialsCount;
	temp->monomials = (struct Monomial*) realloc(temp->monomials, sizeof(struct Monomial) * (temp->monomialsCount)); 
	for(i = 0; i < poly1->monomialsCount; i++)
	{
		temp->monomials[i].degree = poly1->monomials[i].degree;
		temp->monomials[i].factor = poly1->monomials[i].factor;
	}
	//теперь добавляем данные из 
	for(i = 0; i < poly2->monomialsCount; i++)
	{
		for(j = 0; j < temp->monomialsCount; j++)
		{
			if(temp->monomials[j].degree == poly2->monomials[i].degree) //здесь будем перебирать j-тые для временного
			{
				temp->monomials[j].factor -= poly2->monomials[i].factor;
				wasSubstructed++;
				break;
			}
		}
		if (wasSubstructed > 0)
			wasSubstructed = 0;
		else 
		{
			temp->monomialsCount++;
			temp->monomials = (struct Monomial*) realloc(temp->monomials, sizeof(struct Monomial) * (temp->monomialsCount)); 
			temp->monomials[temp->monomialsCount].degree = poly2->monomials[i].degree;
			temp->monomials[temp->monomialsCount].factor = poly2->monomials[i].factor;
		}
	}
	return temp;

}
struct Polynomial *addPolynomials(struct Polynomial *poly1, struct Polynomial *poly2)
{
	struct Polynomial *temp = (struct Polynomial *) malloc(sizeof(struct Polynomial));
	//сначала копируем данные из poly1;
	int i = 0, j = 0, wasAdded = 0;
	temp->monomialsCount = poly1->monomialsCount;
	temp->monomials = (struct Monomial*) realloc(temp->monomials, sizeof(struct Monomial) * (temp->monomialsCount)); 
	for(i = 0; i < poly1->monomialsCount; i++)
	{
		temp->monomials[i].degree = poly1->monomials[i].degree;
		temp->monomials[i].factor = poly1->monomials[i].factor;
	}
	//теперь добавляем данные из 
	for(i = 0; i < poly2->monomialsCount; i++)
	{
		for(j = 0; j < temp->monomialsCount; j++)
		{
			if(temp->monomials[j].degree == poly2->monomials[i].degree) //здесь будем перебирать j-тые для временного
			{
				temp->monomials[j].factor += poly2->monomials[i].factor;
				wasAdded++;
				break;
			}
		}
		if (wasAdded > 0)
			wasAdded = 0;
		else 
		{
			temp->monomialsCount++;
			temp->monomials = (struct Monomial*) realloc(temp->monomials, sizeof(struct Monomial) * (temp->monomialsCount)); 
			temp->monomials[temp->monomialsCount].degree = poly2->monomials[i].degree;
			temp->monomials[temp->monomialsCount].factor = poly2->monomials[i].factor;
		}
	}
	return temp;
}
struct Polynomial **findPolynomialByName(struct PolynomialsArray &PolyArr, const char *name)
{
	int i;
	for (i = 0; i <= PolyArr.count; i++) 
	{
		if (!strcmp(PolyArr.Polynomials[i]->name, name)){
			return &PolyArr.Polynomials[i];
		}
	}
	return NULL;
}
void deletePolyArray (struct PolynomialsArray &PolyArray)
{
	int i = 0;
	while (PolyArray.Polynomials[i] != NULL) 
	{
		free(PolyArray.Polynomials[i]);
		i++;
	}
	free(PolyArray.Polynomials);
}
void appendPolynomialToArray(struct PolynomialsArray &PolyArray, struct Polynomial **polynomial) 
{
	PolyArray.Polynomials = (struct Polynomial **) realloc(PolyArray.Polynomials, sizeof(struct Polynomial *) * (PolyArray.count + 1));
	PolyArray.Polynomials[PolyArray.count] = *polynomial;
	PolyArray.count++;
}
int identificationVariable(char *expression)
{
	if (expression[0] == '$')
		return 1;
	else
		return 0;
}
void printPolynomial(struct Polynomial* poly)
{
	int i = 0, j;
	for(i = 0; i < poly->monomialsCount; i++)
	{
		printf("%f %i\n", poly->monomials[i].factor, poly->monomials[i].degree);
	}

}
struct Polynomial *getPolynomialFromString(char *strPoly)
{
	int i = 0, j = 0, readNum = 1, readDegree = 0, degreeLen = 0, num = 0, factorLen = 0;
	char *tempDegree = NULL, *tempFactor = NULL;//
	struct Polynomial *polynomial = (struct Polynomial *) malloc(sizeof(struct Polynomial)); // объявляем полином
	polynomial->monomialsCount = 1;
	polynomial->monomials = NULL;
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
			if ((factorLen != 0) && (degreeLen == 0))//если у нас нет коэффициента, но есть множитель, то пишем в него 0
			{
				polynomial->monomials = (struct Monomial *) realloc(polynomial->monomials, sizeof(struct Monomial) * (polynomial->monomialsCount));
				polynomial->monomials[polynomial->monomialsCount - 1].degree = 0;
				polynomial->monomials[polynomial->monomialsCount - 1].factor = atof(tempFactor);
				polynomial->monomialsCount++;
			}
			if ((factorLen == 0) && (degreeLen != 0)) //если степень != 0, а множителя не обнаружили, то вводим 1 пример "x2"
			{
				polynomial->monomials = (struct Monomial *) realloc(polynomial->monomials, sizeof(struct Monomial) * (polynomial->monomialsCount));
				polynomial->monomials[polynomial->monomialsCount - 1].degree = atoi(tempDegree);
				polynomial->monomials[polynomial->monomialsCount - 1].factor = 1;				
				polynomial->monomialsCount++;
			}
			if ((factorLen != 0) && (degreeLen != 0))
			{
				polynomial->monomials = (struct Monomial *) realloc(polynomial->monomials, sizeof(struct Monomial) * (polynomial->monomialsCount));
				polynomial->monomials[polynomial->monomialsCount - 1].degree = atoi(tempDegree);
				polynomial->monomials[polynomial->monomialsCount - 1].factor = atof(tempFactor);				
				polynomial->monomialsCount++;
			}
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
char **parseAction(const char *action)
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
	return actArray;


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
	struct PolynomialsArray PolyArray;
	char ** expression;
	char *todo;
	PolyArray.count = 0;
	PolyArray.Polynomials = NULL;

	//parseAction("c += b");
	printPolynomial(getPolynomialFromString("100x3-200"));
	system("pause");
    return 0;
}
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

