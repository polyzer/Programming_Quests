/*

Шаблон проекта для библиотек mpir и gmp.


Маткин Илья Александрович 27.02.2013
*/


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

typedef struct _ArgumentMapEntry // список аргументов
{
	struct ArgumentMapEntry *next; // указатель на следующий аргумент
	char *Name; // имя переменной, которое будет считываться из команды, и будет использоваться данный аргумент
	struct _StringMapEntry *real; // указывает на переменную, которая была передана в качестве аргумента

} ArgumentMapEntry;

typedef struct _FunctionMapEntry {
    
	struct _FunctionMapEntry *next; // указатель на следующую функцию
	struct CommandMapEntry *commands; // комманды данной функции
	char *name; // имя функции
	ArgumentMapEntry *args; // список аргументов

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

FunctionMapEntry *CreateAndInitFunctionMapEntry (const char *name, struct CommandMapEntry *comap, ArgumentMapEntry *argmap) {
    
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
	entry->args = (ArgumentMapEntry*) malloc (sizeof(ArgumentMapEntry));
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

}


const char *ParseLineNotEqualAndGetResult(const char *line)
{
regex_t re;
regmatch_t match[MAX_MATCH];
char *op1;
const char *op1Number;
char *op2;
const char *op2Number;
char *op1Substr;
const char *sub1Number;
char *op2Substr;
const char *sub2Number;
char operation;
const char *ret;
	ret = op1Number = op2Number = sub1Number = sub2Number = NULL;

	memset (match, 0, sizeof(match));
 if (!regcomp (&re, "^\\s*(\\([^\\)]+\\)|[a-zA-Z][a-zA-Z0-9]+|[0-9]+)\\s*(\\+|\\*|\\/|\\-)\\s*(\\([^\\)]+\\)|[a-zA-Z][a-zA-Z0-9]+|[0-9]+)\\s*;*\\s*$", 0)) {
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
				op1Substr = (char*) malloc (sizeof(op1) - 2);
				memcpy (op1Substr, op1 + 1, sizeof(op1) - 2);
				op1Substr[sizeof(op1Substr)] = 0;
				op1Number = ParseLineNotEqualAndGetResult(op1Substr);
			} else if(!isdigit(op1[0]))
			{
	            op1Number = GetStringByKey(&glVars, op1);
			} else
			{
				op1Number = op1;
			}

			if(op2[0] =='(')
			{
				op2Substr = (char*) malloc (sizeof(op2) - 2);
				memcpy (op2Substr, op2 + 1, sizeof(op2) - 2);
				op2Substr[sizeof(op2Substr)] = 0;
				op2Number = ParseLineNotEqualAndGetResult(op2Substr);
			} else	if(!isdigit(op2[0]))
			{
	            op2Number = GetStringByKey(&glVars, op2);
			} else
			{
				op2Number = op2;				
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
char *op3;
const char *op3Number;
char *op4;
const char *op4Number;
char *op5;
const char *op5Number;
char operation;
char operation1;
char operation2;



    memset (match, 0, sizeof(match));

	// var = const; | var = var;
    if (!regcomp (&re, "^\\s*([a-zA-Z][a-zA-Z0-9]*)\\s*=\\s*([0-9]+)\\s*;\\s*$", 0)) {
        if (!regexec (&re, line, MAX_MATCH, match, 0)) {

            op1 = (char*) malloc (match[1].rm_eo - match[1].rm_so + 1);
            op2 = (char*) malloc (match[2].rm_eo - match[2].rm_so + 1);

            memcpy(op1, line + match[1].rm_so, match[1].rm_eo - match[1].rm_so);
            op1[match[1].rm_eo - match[1].rm_so] = 0;

            memcpy(op2, line + match[2].rm_so, match[2].rm_eo - match[2].rm_so);
            op2[match[2].rm_eo - match[2].rm_so] = 0;

			if (isdigit(op2[0])){
				InsertStringMap(&glVars, op2, op2);
	            printf ("%s = %s\n", op1, op2);
			}else{
				op2Number = GetStringByKey(&glVars, op2);
	            printf ("%s = %s\n", op1, op2Number);
			}

            free (op1);
            free (op2);
            }
        regfree (&re);
        }
// ^\\s*(\\([^\\)]+\\)|[a-zA-Z][a-zA-Z0-9]+|[0-9]+)\\s*(\\+|\\*|\\/|\\-)\\s*(\\([^\\)]+\\)|[a-zA-Z][a-zA-Z0-9]+|[0-9]+)
	// var + const | const + const | const + var | var + var;
    if (!regcomp (&re, "^\\s*([a-zA-Z][a-zA-Z0-9]+|[0-9]+)\\s*(\\+|\\*|\\/|\\-)\\s*([a-zA-Z][a-zA-Z0-9]+|[0-9]+)\\s*;\\s*$", 0)) {
        if (!regexec (&re, line, MAX_MATCH, match, 0)) {
			op1 = (char*) malloc (match[1].rm_eo - match[1].rm_so + 1);
			op2 = (char*) malloc (match[3].rm_eo - match[3].rm_so + 1);
			
            memcpy (op1, line + match[1].rm_so, match[1].rm_eo - match[1].rm_so);
            op1[match[1].rm_eo - match[1].rm_so] = 0;

			memcpy (&operation, line + match[2].rm_so, 1);
            
			memcpy (op2, line + match[3].rm_so, match[3].rm_eo - match[3].rm_so);
            op2[match[3].rm_eo - match[3].rm_so] = 0;

			// если оба переданных операнда - переменные!
			if(!isdigit(op1[0]) && !isdigit(op2[0]))
			{
	            op1Number = GetStringByKey(&glVars, op1);
				op2Number = GetStringByKey(&glVars, op2);
				mpz_init_set_str(number1,op1Number,0);
				mpz_init_set_str(number2,op2Number,0);

			} else if(!isdigit(op1[0]))
			{
	            op1Number = GetStringByKey(&glVars, op1);
				mpz_init_set_str(number1,op1Number,0);
				mpz_init_set_str(number2,op2,0);
			} else if(!isdigit(op2[0]))
			{
	            op2Number = GetStringByKey(&glVars, op2);
				mpz_init_set_str(number1,op1,0);
				mpz_init_set_str(number2,op2Number,0);

			} else
			{
				mpz_init_set_str(number1,op1,0);
				mpz_init_set_str(number2,op2,0);
			}

            printf ("%s %c %s = ", op1, operation, op2);
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
			printf("%s\n", mpz_get_str(NULL, 10, number1));
			free (op1);
            free (op2);
            }
        regfree (&re);
        }
		// var = var1 + const; | var = const + const; | var = const + var; | var = var1 + var;
	    if (!regcomp (&re, "^\\s*(\\([^\\)]+\\)|[a-zA-Z][a-zA-Z0-9]+|[0-9]+)\\s*(\\+|\\*|\\/|\\-)\\s*(\\([^\\)]+\\)|[a-zA-Z][a-zA-Z0-9]+|[0-9]+)\\s*;\\s*$", 0)) {
			if (!regexec (&re, line, MAX_MATCH, match, 0)) {
			op1 = (char*) malloc (match[1].rm_eo - match[1].rm_so + 1);
			op2 = (char*) malloc (match[2].rm_eo - match[2].rm_so + 1);
			op3 = (char*) malloc (match[3].rm_eo - match[3].rm_so + 1);
			
            memcpy (op1, line + match[1].rm_so, match[1].rm_eo - match[1].rm_so);
            op1[match[1].rm_eo - match[1].rm_so] = 0;

			memcpy (&operation, line + match[2].rm_so, 1);
            
			memcpy (op2, line + match[3].rm_so, match[3].rm_eo - match[3].rm_so);
            op2[match[3].rm_eo - match[3].rm_so] = 0;

			// если оба переданных операнда - переменные!
			if(!isdigit(op1[0]) && !isdigit(op2[0]))
			{
	            op1Number = GetStringByKey(&glVars, op1);
				op2Number = GetStringByKey(&glVars, op2);
				mpz_init_set_str(number1,op1Number,0);
				mpz_init_set_str(number2,op2Number,0);

			} else if(!isdigit(op1[0]))
			{
	            op1Number = GetStringByKey(&glVars, op1);
				mpz_init_set_str(number1,op1Number,0);
				mpz_init_set_str(number2,op2,0);
			} else if(!isdigit(op2[0]))
			{
	            op2Number = GetStringByKey(&glVars, op2);
				mpz_init_set_str(number1,op1,0);
				mpz_init_set_str(number2,op2Number,0);

			} else
			{
				mpz_init_set_str(number1,op1,0);
				mpz_init_set_str(number2,op2,0);
			}

            printf ("%s %c %s = ", op1, operation, op2);
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
			printf("%s\n", mpz_get_str(NULL, 10, number1));
			free (op1);
            free (op2);
            }
        regfree (&re);
        }
	// присваивание бесконечного количества
	/*
	if (!regcomp (&re, "^\\s*([a-zA-Z][a-zA-Z0-9]*)\\s*=\\s*([a-zA-Z][a-zA-Z0-9]*|[0-9]*\+|\-|\*|\/[a-zA-Z][a-zA-Z0-9]*\\s*)*;\\s*$", 0)) {
        if (!regexec (&re, line, MAX_MATCH, match, 0)) {
            char *varName;
            char *initNumber;
            varName = (char*) malloc (match[1].rm_eo - match[1].rm_so + 1);
            initNumber = (char*) malloc (match[2].rm_eo - match[2].rm_so + 1);

            memcpy (varName, line + match[1].rm_so, match[1].rm_eo - match[1].rm_so);
            varName[match[1].rm_eo - match[1].rm_so] = 0;

            memcpy (initNumber, line + match[2].rm_so, match[2].rm_eo - match[2].rm_so);
            initNumber[match[2].rm_eo - match[2].rm_so] = 0;

            InsertStringMap (&glVars, varName, initNumber);

            printf ("%s = %d\n", varName, atoi (initNumber));

            free (varName);
            free (initNumber);
            }
        regfree (&re);
        }
	*/

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
    ParseLine ("var1 = 15;");
    ParseLine ("var1 / 10;");

    if (argc < 2) {
        printf ("usage: %s filename\n", argv[0]);
        return 1;
        }

    ParseFile (argv[1]);
	system("pause");
    return 0;
}


/*
int main(unsigned int argc,char *argv[]){

mpz_t a;
mpz_t b;
mpz_t c;
mpz_t d;
mpz_t e;
mpz_t f;
mpz_t res;

    mpz_init_set_str(a,"13134147134718327417348173294817394701639162309813204140518230471340176320513129846103247816034612034610234612390846102839641023419862130412034861023846102386401829364102834601892132412341234123412341234123412341234123413241234123412341234123412341234123412341234123412341234123412341234123412341234123412341234123412341234134132412341234123412341234123847189234912374987123984712398471298347981237498123741293874981234781810923481093284091238409123840912834091823409182309481230948123094812309481239041823490128349123849012384123413284183247891234781237418239041092384",0);
    mpz_init_set_str(b,"8910326918657362489173049817048710364891326401982364019823409816234098123409861230984612389461203984610293846120934610293846102934612930846102398641289306419823748912374812348912734891720346121387419082374908172340981723049817230948712039847109834709832134134132412341234123412341234123412341234810293849123481092384091238409123849102384019238401923840912384901238401923849012384901238491283491324132419238409128340918234913413241234192384910238401923840912384091238409123840912384901238401923840912384019234890818324908123094812309481923048091234812034981920341341341234",0);


    //mpz_mul(res,a,b);
    mpz_init (res);
    mpz_init (c);
    mpz_powm_ui(c, a, 2, b);    // c = a^2 (mod b)
    mpz_mul (c, a, b);
    
    // res = a^b (mod c)
    calculate_time (start, end, mpz_powm (res, a, b, c));
    //mpz_powm (res, a, b, c);

    gmp_printf("%Zd\n*\n%Zd\n=\n%Zd",a,b,c);

    mpz_clear(res);
    mpz_clear(b);
    mpz_clear(a);
    mpz_clear(c);

    return 0;
}
*/
