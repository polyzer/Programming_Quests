#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>

#define INDEX_PRINT_FUNCTION  0
#define INDEX_ADD_FUNCTION  1
//#define INDEX_ADD_DIFF_FUNCTION 2
#define INDEX_SUB_FUNCTION 2
//#define INDEX_SUB_DIFF_FUNCTION 4
#define INDEX_MUL_FUNCTION 3
//#define INDEX_MUL_DIFF_FUNCTION 6
#define INDEX_DET_FUNCTION 4
#define INDEX_REV_FUNCTION 5
#define INDEX_INIT_FUNCTION 6

// обобщённая структура
typedef struct _OperationsType OperationsType;

// тип указателя на обобщённую функцию сложения
typedef OperationsType * (*OperationProto1) (OperationsType *a, OperationsType *b);

// тип указателя на обобщённую функцию вывода
typedef void * (*OperationProto2) (OperationsType *a);

struct _OperationsType {
    OperationProto1 *vtable; // указатель на массив функций
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

/// объявления и определения функций
OperationsType* Add (OperationsType *a, OperationsType *b);
OperationsTypeInt * AddInt (OperationsTypeInt *a, OperationsTypeInt *b);
OperationsTypeDouble * AddDouble (OperationsTypeDouble *a, OperationsTypeDouble *b);

OperationsTypeDMatrix * AddDDMatrix (OperationsTypeDMatrix *a, OperationsTypeDMatrix *b);
OperationsTypeDMatrix * AddDIMatrix (OperationsTypeDMatrix *a, OperationsTypeIMatrix *b);
OperationsTypeDMatrix * AddIDMatrix (OperationsTypeIMatrix *a, OperationsTypeDMatrix *b);
OperationsTypeIMatrix * AddIIMatrix (OperationsTypeIMatrix *a, OperationsTypeIMatrix *b);


// полиморфная функция, умеющая складывать сущности различного типа
OperationsType* AddMatrix (OperationsType *a, OperationsType *b) {
    // можно ведь работать с двумя виртуальными таблицами..
	OperationsType* c;
	c = a->vtable[INDEX_ADD_FUNCTION] (a, b);
	if(c != NULL)
		return c;
	else{
		return c = a->vtable[INDEX_ADD_DIFF_FUNCTION] (a, b);
	}
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
	for(i = 0; i < sum->value->columns; i++)
	{
		for(j = 0; j < sum->value->rows; j++)
		{
			sum->value->arr[i][j] =  a->value->arr[i][j] + b->value->arr[i][j];
		}
	}

    return sum;
}

OperationsTypeDMatrix * AddDIMatrix (OperationsTypeDMatrix *a, OperationsTypeIMatrix *b) {
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
	for(i = 0; i < sum->value->columns; i++)
	{
		for(j = 0; j < sum->value->rows; j++)
		{
			sum->value->arr[i][j] =  a->value->arr[i][j] + b->value->arr[i][j];
		}
	}

    return sum;
}

OperationsTypeDMatrix * AddIDMatrix (OperationsTypeIMatrix *a, OperationsTypeDMatrix *b) {
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


void PrintInt (OperationsTypeInt *a) {
    
    printf ("%d\n", a->value);
}

void PrintDouble (OperationsTypeDouble *a) {
    printf ("%f\n", a->value);
}

void PrintIMatrix (OperationsTypeIMatrix *a) {
    int i, j;
	for(i = 0; i < a->value->columns; i++)
	{
		for(j = 0; j < a->value->rows; j++)
		{
			printf("%d", a->value->arr[i][j]);
		}
	}
}

void PrintDMatrix (OperationsTypeDMatrix *a) {
    int i, j;
	for(i = 0; i < a->value->columns; i++)
	{
		for(j = 0; j < a->value->rows; j++)
		{
			printf("%d", a->value->arr[i][j]);
		}
	}
}


void Print (OperationsType *a) {
    ((OperationProto2)a->vtable[INDEX_PRINT_FUNCTION]) (a);
}


int main () {

// массив указателей на функции, работающих с OperationsTypeInt
OperationProto1 vtableInt[] = {(OperationProto1) PrintInt, (OperationProto1)AddInt};
// массив указателей на функции, работающих с OperationsTypeDouble
OperationProto1 vtableDouble[] = {(OperationProto1) PrintDouble, (OperationProto1)AddDouble};
// массив указателей на функции, работающих с OperationsTypeIMatrix
OperationProto1 vtableIMatrix[] = {(OperationProto1) PrintIMatrix, (OperationProto1)AddIIMatrix, (OperationProto1)AddIDMatrix};
// массив указателей на функции, работающих с OperationsTypeDMatrix
OperationProto1 vtableDMatrix[] = {(OperationProto1) PrintDMatrix, (OperationProto1)AddDDMatrix,  (OperationProto1)AddDIMatrix};


OperationsTypeInt intA = {vtableInt, 10};
OperationsTypeInt intB = {vtableInt, 20};
OperationsTypeInt *intSum;
OperationsTypeDouble doubleA = {vtableDouble, 1.0};
OperationsTypeDouble doubleB = {vtableDouble, 2.0};
OperationsTypeDouble *doubleSum;


    intSum = Add (&intA, &intB);
    doubleSum = Add (&doubleA, &doubleB);

    Print (intSum);
    Print (doubleSum);
	system("pause");
    return 0;
}
