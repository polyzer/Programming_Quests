#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INDEX_PRINT_FUNCTION  0
#define INDEX_INIT_FUNCTION 1
#define INDEX_ADD_FUNCTION  2
#define INDEX_MUL_FUNCTION 3
//#define INDEX_ADD_DIFF_FUNCTION 2
//#define INDEX_SUB_DIFF_FUNCTION 4
#define INDEX_SUB_FUNCTION 4
//#define INDEX_MUL_DIFF_FUNCTION 6
#define INDEX_DET_FUNCTION 5
#define INDEX_REV_FUNCTION 6

// обобщённая структура
typedef struct _OperationsType OperationsType;

// тип указателя на обобщённую функцию сложения
typedef OperationsType * (*OperationProto) (OperationsType *a, OperationsType *b);
// тип указателя на обобщённую функцию вывода
typedef void * (*OperationProtoPrint) (OperationsType *a);
// тип указателя на обобщённую функцию инициализации
typedef void * (*OperationProtoInit) (OperationsType *a, const char *str);


struct _OperationsType {
  
    OperationProto *vtable; // указатель на массив функций
};

typedef struct _IMatrix{
	int columns;
	int rows;
	int **arr;
} IMatrix;

typedef struct _DMatrix{
	int columns;
	int rows;
	double **arr;
} DMatrix;


typedef struct _OperationsTypeInt {
    
    // содержит функциональность
    OperationsType head;

    // содержит данные
    int value;

} OperationsTypeInt;


typedef struct _OperationsTypeDouble {

    // содержит функциональность
    OperationsType head;

    // содержит данные
    double value;

} OperationsTypeDouble;

typedef struct _OperationsTypeIMatrix {

    // содержит функциональность
    OperationsType head;

    // содержит данные
    IMatrix *value;

} OperationsTypeIMatrix;

typedef struct _OperationsTypeDMatrix {

    // содержит функциональность
    OperationsType head;

    // содержит данные
    DMatrix *value;

} OperationsTypeDMatrix;

/////////////////////// выделение памяти для двумерного массива
int **malIArr(int cols, int rows)
{
	int **a, i;
	a = (int **) malloc(sizeof(int *) * cols);

	for(i = 0; i < cols; i++)
	{
		a[i] = (int *) malloc(sizeof(int) * rows);
	}
	return a;
}
double **malDArr(int cols, int rows)
{
	double **a;
	int i;
	a = (double **) malloc(sizeof(double *) * cols);

	for(i = 0; i < cols; i++)
	{
		a[i] = (double *) malloc(sizeof(double) * rows);
	}
	return a;
}

////////////// полиморфная функция, умеющая складывать сущности различного типа
OperationsType* Add (OperationsType *a, OperationsType *b) {
    
    return a->vtable[INDEX_ADD_FUNCTION] (a, b);
}


OperationsTypeInt * AddInt (OperationsTypeInt *a, OperationsTypeInt *b) {

OperationsTypeInt *sum = (OperationsTypeInt*) malloc (sizeof(OperationsTypeInt));

    sum->head = a->head;
    sum->value = a->value + b->value;

    return sum;
}


OperationsTypeDouble * AddDouble (OperationsTypeDouble *a, OperationsTypeDouble *b) {

OperationsTypeDouble *sum = (OperationsTypeDouble*) malloc (sizeof(OperationsTypeDouble));

    sum->head = a->head;
    sum->value = a->value + b->value;

    return sum;
}

/////////////// сложение матриц
// полиморфная функция, умеющая складывать сущности различного типа
OperationsType* AddMatrix (OperationsType *a, OperationsType *b) {
    // можно ведь работать с двумя виртуальными таблицами..
	OperationsType* c;
	c = a->vtable[INDEX_ADD_FUNCTION] (a, b);
	return c;
/*	if(c != NULL)
		return c;
	else{
		return c = a->vtable[INDEX_ADD_DIFF_FUNCTION] (a, b);
	}
*/
}

OperationsTypeDMatrix * AddDDMatrix (OperationsTypeDMatrix *a, OperationsTypeDMatrix *b) {
	int i, j;
	OperationsTypeDMatrix *sum;
	i = 0; j = 0;
	sum = (OperationsTypeDMatrix*) malloc (sizeof(OperationsTypeDMatrix));
	// складывать можно только матрицы одинаковой размерности
	if(a->value->columns != b->value->columns || a->value->rows != b->value->rows)
		return NULL;
	if(sizeof(a->value->arr[0][0]) != sizeof(b->value->arr[0][0]))
	{
		return NULL;
	}		
	sum->head = a->head;
	sum->value = (DMatrix *) malloc (sizeof(DMatrix));
	sum->value->columns = a->value->columns;
	sum->value->rows = a->value->rows;

	sum->value->arr = malDArr(sum->value->columns, sum->value->rows);


	for(i = 0; i < sum->value->columns; i++)
	{
		for(j = 0; j < sum->value->rows; j++)
		{
			sum->value->arr[i][j] =  a->value->arr[i][j] + b->value->arr[i][j];
		}
	}

    return sum;
}

OperationsTypeIMatrix * AddIIMatrix (OperationsTypeIMatrix *a, OperationsTypeIMatrix *b) {
	int i, j;
	OperationsTypeIMatrix *sum;
	i = 0; j = 0;
	sum = (OperationsTypeIMatrix*) malloc (sizeof(OperationsTypeIMatrix));
	// складывать можно только матрицы одинаковой размерности
	if(a->value->columns != b->value->columns || a->value->rows != b->value->rows)
		return NULL;
	if(sizeof(a->value->arr[0][0]) != sizeof(b->value->arr[0][0]))
	{
		return NULL;
	}		
	sum->head = a->head;
	sum->value = (IMatrix *) malloc (sizeof(IMatrix));
	sum->value->columns = a->value->columns;
	sum->value->rows = a->value->rows;

	sum->value->arr = malIArr(sum->value->columns, sum->value->rows);


	for(i = 0; i < sum->value->columns; i++)
	{
		for(j = 0; j < sum->value->rows; j++)
		{
			sum->value->arr[i][j] =  a->value->arr[i][j] + b->value->arr[i][j];
		}
	}

    return sum;
}
///////////////////// Перемножение матриц
OperationsType* MulMatrix (OperationsType *a, OperationsType *b) {
    // можно ведь работать с двумя виртуальными таблицами..
	OperationsType* c;
	c = a->vtable[INDEX_MUL_FUNCTION] (a, b);
	return c;
/*	if(c != NULL)
		return c;
	else{
		return c = a->vtable[INDEX_ADD_DIFF_FUNCTION] (a, b);
	}
*/
}

OperationsTypeDMatrix * MulDDMatrix (OperationsTypeDMatrix *a, OperationsTypeDMatrix *b) {
	int i, j, k;
	OperationsTypeDMatrix *mul;
	i = 0; j = 0; k = 0;
	mul = (OperationsTypeDMatrix*) malloc (sizeof(OperationsTypeDMatrix));
	// переможать можно только матрицы с columns = rows
	if(a->value->columns != b->value->rows || a->value->rows != b->value->columns)
		return NULL;
	if(sizeof(a->value->arr[0][0]) != sizeof(b->value->arr[0][0]))
	{
		return NULL;
	}		
	mul->head = a->head;
	mul->value = (DMatrix *) malloc (sizeof(DMatrix));
	mul->value->columns = a->value->columns;
	mul->value->rows = b->value->rows;

	mul->value->arr = malDArr(mul->value->columns, mul->value->rows);

	for(i = 0; i < mul->value->columns; i++)
	{
		for(j = 0; j < mul->value->rows; j++)
		{
			mul->value->arr[i][j] = 0;			
			for(k = 0; k < a->value->rows; k++)
			{
				mul->value->arr[i][j] += a->value->arr[i][k] * b->value->arr[k][j];
			}
		}
	}

    return mul;
}

OperationsTypeIMatrix * MulIIMatrix (OperationsTypeIMatrix *a, OperationsTypeIMatrix *b) {
	int i, j, k;
	OperationsTypeIMatrix *mul;
	i = 0; j = 0; k = 0;
	mul = (OperationsTypeIMatrix*) malloc (sizeof(OperationsTypeIMatrix));
	// переможать можно только матрицы с columns = rows
	if(a->value->columns != b->value->rows || a->value->rows != b->value->columns)
		return NULL;
	if(sizeof(a->value->arr[0][0]) != sizeof(b->value->arr[0][0]))
	{
		return NULL;
	}		
	mul->head = a->head;
	mul->value = (IMatrix *) malloc (sizeof(IMatrix));
	mul->value->columns = a->value->columns;
	mul->value->rows = b->value->rows;

	mul->value->arr = malIArr(mul->value->columns, mul->value->rows);


	for(i = 0; i < mul->value->columns; i++)
	{
		for(j = 0; j < mul->value->rows; j++)
		{
			mul->value->arr[i][j] = 0;			
			for(k = 0; k < a->value->rows; k++)
			{
				mul->value->arr[i][j] += a->value->arr[i][k] * b->value->arr[k][j];
			}
		}
	}

    return mul;
}


//////////////////////// Вывод Матриц
void PrintMatrix (OperationsType *a) {
    
    ((OperationProtoPrint)a->vtable[INDEX_PRINT_FUNCTION]) (a);
}

void PrintIMatrix (OperationsTypeIMatrix *a) {
    int i, j;
	for(i = 0; i < a->value->columns; i++)
	{
		for(j = 0; j < a->value->rows; j++)
		{
			printf("%d ", a->value->arr[i][j]);
		}
		printf("\n");
	}
}

void PrintDMatrix (OperationsTypeDMatrix *a) {
    int i, j;
	for(i = 0; i < a->value->columns; i++)
	{
		for(j = 0; j < a->value->rows; j++)
		{
			printf("%f ", a->value->arr[i][j]);
		}
		printf("\n");
	}
}

//////////////// Инициализация матриц
void *InitMatrix (OperationsType* a, const char *str) 
{    
	return ((OperationProtoInit) a->vtable[INDEX_INIT_FUNCTION]) (a, str);
}

void *InitIMatrix(OperationsTypeIMatrix *a, const char *str1)
{
	char *pch, *str;
	int i, j;
	str = (char *) malloc(strlen(str1) + 1);
	strcpy(str, str1);
	str[strlen(str1)] = 0;
	a->value = (IMatrix *) malloc (sizeof(IMatrix));
	
	pch = strtok(str, " \n;");
	a->value->columns = atoi(pch);
	pch = strtok(NULL, " \n;");
	a->value->rows = atoi(pch);

	a->value->arr = malIArr(a->value->columns, a->value->rows);
	for(i = 0; i < a->value->columns; i++)
	{
		for(j = 0; j < a->value->rows; j++)
		{
			pch = strtok(NULL, " \n;");
			a->value->arr[i][j] = atoi(pch);
		}
	}
	free(str);
}

void *InitDMatrix(OperationsTypeDMatrix *a, const char *str1)
{
	char *pch, *str;
	int i, j;
	str = (char *) malloc(strlen(str1) + 1);
	strcpy(str, str1);
	str[strlen(str1)] = 0;
	a->value = (DMatrix *) malloc (sizeof(DMatrix));
	
	pch = strtok(str, " \n;");
	a->value->columns = atoi(pch);
	pch = strtok(NULL, " \n;");
	a->value->rows = atoi(pch);

	a->value->arr = malDArr(a->value->columns, a->value->rows);

	for(i = 0; i < a->value->columns; i++)
	{
		for(j = 0; j < a->value->rows; j++)
		{
			pch = strtok(NULL, " \n;");
			a->value->arr[i][j] = atof(pch);
		}
	}
	free(str);
}


int main () {

// массив указателей на функции, работающих с OperationsTypeIMatrix
OperationProto vtableIMatrix[] = {(OperationProto) PrintIMatrix, (OperationProto) InitIMatrix,
(OperationProto)AddIIMatrix, (OperationProto)MulIIMatrix};
// массив указателей на функции, работающих с OperationsTypeDMatrix
OperationProto vtableDMatrix[] = {(OperationProto) PrintDMatrix, (OperationProto) InitDMatrix,
(OperationProto)AddDDMatrix, (OperationProto)MulDDMatrix};

OperationsTypeIMatrix IM1 = {vtableIMatrix, NULL};
OperationsTypeIMatrix IM2 = {vtableIMatrix, NULL};
OperationsTypeIMatrix *IM3;
OperationsTypeDMatrix DM1 = {vtableDMatrix, NULL};
OperationsTypeDMatrix DM2 = {vtableDMatrix, NULL};
OperationsTypeDMatrix *DM3;

	InitMatrix(&IM1, "2 2; 5 6 9 3");
	InitMatrix(&IM2, "2 2; 5 6 9 3");
	InitMatrix(&DM1, "2 2; 5.0 6.0 9.0 3.0");
	InitMatrix(&DM2, "2 2; 5.0 6.0 9.0 3.0");

	IM3 = AddMatrix(&IM1, &IM2);
	DM3 = AddMatrix(&DM1, &DM2);

//    intSum = Add (&intA, &intB);
//    doubleSum = Add (&doubleA, &doubleB);

    PrintMatrix(IM3);
    PrintMatrix(DM3);

	IM3 = MulMatrix(&IM1, &IM2);
	DM3 = MulMatrix(&DM1, &DM2);

    PrintMatrix(IM3);
    PrintMatrix(DM3);


	system("pause");
    return 0;
}
