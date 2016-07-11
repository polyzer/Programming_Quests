/*

Шаблон проекта для библиотек mpir и gmp.


Маткин Илья Александрович 27.02.2013
*/

 #define PCRE_STATIC 1
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#include "../../expr/include/pcre.h"
#include "../../expr/include/pcreposix.h"


//----------------------------------------

//#define _USE_GMP
//#define _USE_DLL


#ifdef _WIN64

#include <mpir.h>
#ifdef _USE_DLL
//#pragma comment( lib, "lib\\mpir\\64\\dll\\gc\\mpir.lib" )
#pragma comment( lib, "lib\\mpir\\64\\dll\\core2\\mpir.lib" )
//#pragma comment( lib, "lib\\mpir\\64\\dll\\sandybridge\\mpir.lib" )
#else // _USE_DLL
//#pragma comment( lib, "lib\\mpir\\64\\lib\\gc\\mpir.lib" )
#pragma comment( lib, "lib\\mpir\\64\\lib\\core2\\mpir.lib" )
//#pragma comment( lib, "lib\\mpir\\64\\lib\\sandybridge\\mpir.lib" )
#endif // _USE_DLL

#else // _WIN64

#ifdef _USE_GMP

#ifdef _USE_DLL
#include <gmp_d.h>
#pragma comment( lib, "lib\\gmp\\32\\libgmp_5.1.1.dll.a" )
#else // _USE_DLL
#include <gmp_s.h>
#pragma comment( lib, "lib\\gmp\\32\\libgmp.a" )
#pragma comment( lib, "libgcc.a" )
#endif // _USE_DLL

#else // _USE_GMP

#include <mpir.h>
#ifdef _USE_DLL
//#pragma comment( lib, "lib\\mpir\\32\\dll\\gc\\mpir.lib" )
//#pragma comment( lib, "lib\\mpir\\32\\dll\\pentium4\\mpir.lib" )
#pragma comment( lib, "lib\\mpir\\32\\dll\\pentium4_sse2\\mpir.lib" )
#else // _USE_DLL
#pragma comment( lib, "lib\\mpir\\32\\lib\\gc\\mpir.lib" )
//#pragma comment( lib, "lib\\mpir\\32\\lib\\pentium4\\mpir.lib" )
//#pragma comment( lib, "lib\\mpir\\32\\lib\\pentium4_sse2\\mpir.lib" )
#endif // _USE_DLL

#endif // _USE_GMP

#endif // _WIN64


//----------------------------------------


clock_t start;
clock_t end;

#define calculate_time(start_time, end_time, action) \
    start_time = clock();\
    (action);\
    end_time = clock();\
    printf ("%f\n", (double)(end_time-start_time)/CLK_TCK);


//----------------------------------------
//----------------------------------------
typedef struct _StringMapEntry {

    struct _StringMapEntry *next;
    char *key;
    char *value;
    
} StringMapEntry;

typedef struct _StringMap {
    
    StringMapEntry *first;

} StringMap;

struct CommandMapEntry // список комманд в функции
{
	struct CommandMapEntry *next; // указатель на следующую
	char *CommandString; // строка, содержащая команду, которая при каждом вызове будет интерпретироваться  

};

typedef struct ArgumentMapEntry // список аргументов
{
	struct ArgumentMapEntry *next; // указатель на следующий аргумент
	char *Name; // имя переменной, которое будет считываться из команды, и будет использоваться данный аргумент
	struct _StringMapEntry *real; // указывает на переменную, которая была передана в качестве аргумента

};

typedef struct _FunctionMapEntry {
    
	struct _FunctionMapEntry *next; // указатель на следующую функцию
	struct CommandMapEntry *commands; // комманды данной функции
	char *name; // имя функции
	struct ArgumentMapEntry *args; // список аргументов

} FunctionMapEntry;


typedef struct _FunctionMap
{
	FunctionMapEntry *first;

} FunctionMap;




typedef StringMap VariableContainer;
typedef FunctionMap FunctionContainer;

//----------------------------------------


VariableContainer glVars;

//----------------------------------------

void InitFuncsMap (FunctionMap *fmap) {
    fmap->first = NULL;
    return;
}

void InitStringMap (StringMap *smap) {
    smap->first = NULL;
    return;
}



StringMapEntry *CreateAndInitStringMapEntry (const char *key, const char *value) {
    
	StringMapEntry *entry;

    entry = (StringMapEntry*) malloc (sizeof(StringMapEntry));
    if (!entry) {
        return NULL;
        }

    entry->key = (char*) malloc (strlen (key) + 1);
    if (!entry->key) {
        free (entry);
        return NULL;
        }
    entry->value = (char*) malloc (strlen (value) + 1);
    if (!entry->value) {
        free (entry->key);
        free (entry);
        return NULL;
        }
    strcpy (entry->key, key);
    strcpy (entry->value, value);
	entry->next = NULL;
    return entry;
}

FunctionMapEntry *CreateAndInitFunctionMapEntry (const char *name, struct CommandMapEntry *comap, struct ArgumentMapEntry *argmap) {
    
	FunctionMapEntry *entry;

    entry = (FunctionMapEntry*) malloc (sizeof(FunctionMapEntry));
    if (!entry) {
        return NULL;
        }

	entry->name = (char*) malloc (strlen(name) + 1);
    if (!entry->name) {
        free (entry);
        return NULL;
        }
	entry->args = (struct ArgumentMapEntry*) malloc (sizeof(struct ArgumentMapEntry));
    if (!entry->args) {
        free (entry->name);
        free (entry);
        return NULL;
        }
    
	entry->args = argmap;
	entry->commands = comap;
	strcpy(entry->name, name);
	entry->next = NULL;
    return entry;
}

// вставка строки в список
void InsertStringMap (StringMap *map, const char *key, const char *value) {

	StringMapEntry *newEntry;
	StringMapEntry *tt;
    newEntry = CreateAndInitStringMapEntry (key, value);
    if (!newEntry) {
        return;
        }
	if(map->first == NULL)
	{
		map->first = newEntry;
		return;
	}
	tt = map->first;
	while(tt->next!= NULL)
	{
		tt = tt->next;
	}
	tt->next = newEntry;
    return;
}



const char * GetStringByKey (StringMap *map, const char *key) {

	StringMapEntry *entry = map->first;

    while (entry) {
        if (!strcmp (entry->key, key)) {
            return entry->value;
            }
        entry = entry->next;
        }

    return NULL;
}


#define MAX_MATCH 10
const char *ComputeExpressionAndGetResult_s(const char *Num1, char operation,const char *Num2){
	mpz_t number1;
	mpz_t number2;
	const char *ret;

	mpz_init_set_str(number1,Num1,0);
	mpz_init_set_str(number2,Num2,0);

	if(operation == '+'){
		mpz_add(number1, number1, number2);
	}
	if(operation == '-'){
		mpz_sub(number1, number1, number2);
	}
	if(operation == '*'){
		mpz_mul(number1, number1, number2);
	}
	if(operation == '/'){
		mpz_div(number1, number1, number2);
	}
	ret = mpz_get_str(NULL, 10, number1);
	mpz_clear(number1);
	mpz_clear(number2);
	return ret;

}


const char *ParseLineNotEqualAndGetResult(const char *line)
{
regex_t re;
regmatch_t match[MAX_MATCH];
char *op1;
const char *op1Number;
mpz_t number1;
char *op2;
const char *op2Number;
mpz_t number2;
char *op1Substr;
const char *sub1Number;
char *op2Substr;
const char *sub2Number;
char operation;
const char *ret;
	ret = op1Number = op2Number = sub1Number = sub2Number = NULL;
	op1Substr = op2Substr = NULL;
	memset (match, 0, sizeof(match));
	// если в выражении всего один операнд, без операций
    if (!regcomp (&re, "^\\s*([^\\/\\*\\-\\+\\s\\(\\)][a-zA-Z0-9]+)\\s*;*\\s*$", 0)) {
        if (!regexec (&re, line, MAX_MATCH, match, 0)) {

            op1 = (char*) malloc (match[1].rm_eo - match[1].rm_so + 1);

            memcpy(op1, line + match[1].rm_so, match[1].rm_eo - match[1].rm_so);
            op1[match[1].rm_eo - match[1].rm_so] = 0;

			if (!isdigit(op1[0])){
				op1Number = GetStringByKey(&glVars, op1);
	            free (op1);
			}else{
				mpz_init_set_str(number1,op1,0);
				op1Number = mpz_get_str(NULL, 10, number1);
				free(op1);
				mpz_clear(number1);
			}
		ret = op1Number;
	}
        regfree (&re);
    } 
	// если в строке есть операции
	if (!regcomp (&re, "^\\s*(\\([a-zA-Z0-9\\s\\/\\*\\-\\+]+\\)|[a-zA-Z][a-zA-Z0-9]+|[0-9]+)\\s*(\\+|\\*|\\/|\\-)\\s*(\\([^\\)]+\\)|[a-zA-Z][a-zA-Z0-9]+|[0-9]+)\\s*;*\\s*$", 0)) {
        if (!regexec (&re, line, MAX_MATCH, match, 0)) {
			op1 = (char*) malloc (match[1].rm_eo - match[1].rm_so + 1);
			op2 = (char*) malloc (match[3].rm_eo - match[3].rm_so + 1);
			
            memcpy (op1, line + match[1].rm_so, match[1].rm_eo - match[1].rm_so);
            op1[match[1].rm_eo - match[1].rm_so] = 0;

			memcpy (&operation, line + match[2].rm_so, 1);
            
			memcpy (op2, line + match[3].rm_so, match[3].rm_eo - match[3].rm_so);
            op2[match[3].rm_eo - match[3].rm_so] = 0;

			// если операнды - скобки ->
			if(op1[0] =='(')
			{
				op1Substr = (char*) malloc (strlen(op1)-1);
				memcpy (op1Substr, op1 + 1, strlen(op1)-2);
				op1Substr[strlen(op1)-2] = 0;
				op1Number = ParseLineNotEqualAndGetResult(op1Substr);

			} else if(!isdigit(op1[0]))
			{
	            op1Number = GetStringByKey(&glVars, op1);
			} else
			{
				mpz_init_set_str(number1,op1,0);
				op1Number = mpz_get_str(NULL, 10, number1);
				mpz_clear(number1);
			}

			if(op2[0] =='(')
			{
				op2Substr = (char*) malloc (strlen(op2)-1);
				memcpy (op2Substr, op2 + 1, strlen(op2)-2);
				op2Substr[strlen(op2)-2] = 0;
				op2Number = ParseLineNotEqualAndGetResult(op2Substr);
			} else	if(!isdigit(op2[0]))
			{
	            op2Number = GetStringByKey(&glVars, op2);
			} else
			{
				mpz_init_set_str(number2,op2,0);
				op2Number = mpz_get_str(NULL, 10, number2);
				mpz_clear(number2);
			}

			ret = ComputeExpressionAndGetResult_s(op1Number, operation, op2Number);		
			free (op1);
            free (op2);
			free (op1Substr);
			free (op2Substr);
			}
        regfree (&re);
		return ret;
        }
	return NULL;
}

void ParseLine (const char *line) {

regex_t re;
regmatch_t match[MAX_MATCH];
mpz_t number1;
mpz_t number2;
char *op1;
const char *op1Number;
char *op2;
const char *op2Number;
char operation;

    memset (match, 0, sizeof(match));

	if (!regcomp (&re, "^\\s*([a-zA-Z][a-zA-Z0-9]+)\\s*=\\s*(\\([a-zA-Z0-9\\s\\/\\*\\-\\+]+\\)|[a-zA-Z][a-zA-Z0-9]+|[0-9]+)\\s*;*\\s*$", 0)) {
        if (!regexec (&re, line, MAX_MATCH, match, 0)) {

            op1 = (char*) malloc (match[1].rm_eo - match[1].rm_so + 1);
            op2 = (char*) malloc (match[2].rm_eo - match[2].rm_so + 1);

            memcpy(op1, line + match[1].rm_so, match[1].rm_eo - match[1].rm_so);
            op1[match[1].rm_eo - match[1].rm_so] = 0;

            memcpy(op2, line + match[2].rm_so, match[2].rm_eo - match[2].rm_so);
            op2[match[2].rm_eo - match[2].rm_so] = 0;

			op2Number = ParseLineNotEqualAndGetResult(op2);

			InsertStringMap(&glVars, op1, op2Number);
			
            printf ("%s = %s\n", op1, op2Number);

            free (op1);
            free (op2);
            }
        regfree (&re);
    }
	if (!regcomp (&re, "^\\s*([^\\=][a-zA-Z0-9\\s\\/\\*\\-\\+\\(\\)]+)\\s*;*\\s*$", 0)) {
        if (!regexec (&re, line, MAX_MATCH, match, 0)) {
			op1Number = ParseLineNotEqualAndGetResult(line);
            printf ("%s = %s\n", line, op1Number);
            }
        regfree (&re);
    }

    return;
}



void ParseFile (const char *fileName) {
char line[1024];
FILE *fd;
    
    fd = fopen (fileName, "r");
    while (fgets (line, 1024, fd)) {
        ParseLine (line);
        }
 
    fclose (fd);

    return;
}


int main (int argc, char *argv[], char *envp[]) {

    InitStringMap (&glVars);
    ParseLine ("var1 = 250;");
    ParseLine ("var1 / (10 + 15);");
/*
    if (argc < 2) {
        printf ("usage: %s filename\n", argv[0]);
        return 1;
        }

    ParseFile (argv[1]);
*/
	system("pause");

	return 0;
}
