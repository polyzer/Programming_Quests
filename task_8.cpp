// Пример разбора арифметических выражений с помощью регулярных выражений.
//
// Маткин Илья Александрович  15.07.2015

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pcre.h"
#include "pcreposix.h"

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
void ParseLine (const char *line) {

regex_t re;
char *data = "asdfadsfa101.102.103.104adsfadf";
regmatch_t match[MAX_MATCH];

    memset (match, 0, sizeof(match));

	// var = const;
    if (!regcomp (&re, "^\\s*([a-zA-Z][a-zA-Z0-9]*)\\s*=\\s*([0-9]+)\\s*;\\s*$", 0)) {
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

            printf ("%s = %d\n", varName, atoi(initNumber));

            free (varName);
            free (initNumber);
            }
        regfree (&re);
        }
	// var + const;
    if (!regcomp (&re, "^\\s*([a-zA-Z][a-zA-Z0-9]+)\\s*(\\+|\\*|\\/|\\-)\\s*([0-9]+)\\s*;\\s*$", 0)) {
        if (!regexec (&re, line, MAX_MATCH, match, 0)) {
            char *varName;
            char *initNumber;
            char operation;
			const char *varValue;
            varName = (char*) malloc (match[1].rm_eo - match[1].rm_so + 1);
			initNumber = (char*) malloc (match[3].rm_eo - match[3].rm_so + 1);
			
            memcpy (varName, line + match[1].rm_so, match[1].rm_eo - match[1].rm_so);
            varName[match[1].rm_eo - match[1].rm_so] = 0;

			memcpy (&operation, line + match[2].rm_so, 1);
            
			memcpy (initNumber, line + match[3].rm_so, match[3].rm_eo - match[3].rm_so);
            initNumber[match[3].rm_eo - match[3].rm_so] = 0;

            varValue = GetStringByKey (&glVars, varName);

            if(operation == '+'){
				printf ("%s + %d = ", varName, atoi (initNumber));
			    printf ("%d\n", atoi (varValue) + atoi (initNumber));
			}
			if(operation == '-')
			{
	        	printf ("%s - %d = ", varName, atoi (initNumber));
			    printf ("%d\n", atoi (varValue) - atoi (initNumber));
			}
			if(operation == '*')
			{
		    	printf ("%s * %d = ", varName, atoi (initNumber));
			    printf ("%d\n", atoi (varValue) * atoi (initNumber));
            }
			if(operation == '/')
			{
				printf ("%s / %d = ", varName, atoi (initNumber));
			    printf ("%d\n", atoi (varValue) / atoi (initNumber));
			}
			free (varName);
            free (initNumber);
            }
        regfree (&re);
        }
		// var = var1 + const;
	    if (!regcomp (&re, "^\\s*([a-zA-Z][a-zA-Z0-9]+)\\s*(\\=)\\s*([a-zA-Z][a-zA-Z0-9]+)\\s*(\\+|\\*|\\/|\\-)\\s*([0-9]+)\\s*;\\s*$", 0)) {
			if (!regexec (&re, line, MAX_MATCH, match, 0)) {
            char *varName;
            char *initNumber;
            char operation; // в данном случае присваивание
			const char *varValue;
            varName = (char*) malloc (match[1].rm_eo - match[1].rm_so + 1);
			initNumber = (char*) malloc (match[3].rm_eo - match[3].rm_so + 1);
			
            memcpy (varName, line + match[1].rm_so, match[1].rm_eo - match[1].rm_so);
            varName[match[1].rm_eo - match[1].rm_so] = 0;

			memcpy (&operation, line + match[2].rm_so, 1);
            
			memcpy (initNumber, line + match[3].rm_so, match[3].rm_eo - match[3].rm_so);
            initNumber[match[3].rm_eo - match[3].rm_so] = 0;

            varValue = GetStringByKey (&glVars, varName);

            if(operation == '+'){
				printf ("%s + %d = ", varName, atoi (initNumber));
			    printf ("%d\n", atoi (varValue) + atoi (initNumber));
			}
			if(operation == '-')
			{
	        	printf ("%s - %d = ", varName, atoi (initNumber));
			    printf ("%d\n", atoi (varValue) - atoi (initNumber));
			}
			if(operation == '*')
			{
		    	printf ("%s * %d = ", varName, atoi (initNumber));
			    printf ("%d\n", atoi (varValue) * atoi (initNumber));
            }
			if(operation == '/')
			{
				printf ("%s / %d = ", varName, atoi (initNumber));
			    printf ("%d\n", atoi (varValue) / atoi (initNumber));
			}
			free (varName);
            free (initNumber);
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
