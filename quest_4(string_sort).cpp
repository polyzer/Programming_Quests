#include <locale.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

unsigned int stringsCount, maxStringSize;

void genStrings
(
	unsigned int stringsCount,
	unsigned int maxStringSize
);
void sortStrings (char **list);	
int strComparison(char *str1, char *str2);
char *genString(int maxStringSize);

char alphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIGKLMNOPQRSTUVWXYZ";

int main(int argc, char **argv) {
	setlocale(LC_ALL, "Russian");
	printf("Создание строк.\n");
	printf("Введите количество строк:");
	scanf("%i", &stringsCount);
	printf("Введите длину строк:");
	scanf("%i", &maxStringSize);
	srand(time(NULL));
	genStrings(stringsCount, maxStringSize);
	return 0;
}

void genStrings(
	 unsigned int stringsCount,
	 unsigned int maxStringSize
	 )
{
		char **list = (char **) malloc(stringsCount * sizeof(char *));
		int i;
		for (i = 0; i < stringsCount; i++)
			list[i] = genString(maxStringSize);
		//ВЫЗВАТЬ ФУНКЦИЮ СОРТИРОВКИ
		sortStrings(list);
		i = 0;
		for (i = 0; i < stringsCount; i++) {
			printf("%s\n", list[i]);
		}
		system("pause");
		//ОСВОБОЖДЕНИЕ УКАЗАТЕЛЕЙ!!!  
		for (i = 0; i < stringsCount; i++) {
			free(list[i]);
		}
		free(list);
}

void sortStrings(char **list) 
{
	char *buf_elem = NULL;
	int i, j;
	i = 0;
	j = 0;
	for (i = 0; i < (stringsCount - 1); i++) {
		for (j = 0; j < (stringsCount - 1); j++) { 
			if (strComparison(list[j], list[j+1]) < 0)
			{
				buf_elem = list[j];
				list[j] = list[j + 1];
				list[j+1] = buf_elem;
				buf_elem = NULL;
			}
		}
	}
	buf_elem = NULL;
}

int strComparison(char *str1, char *str2) {
	int i, stringLength;
	if (strlen(str1) <= strlen(str2)){
		stringLength = strlen(str1);
	} else {
		stringLength = strlen(str2);
	}
	for (i = 0; i < stringLength; i++) {
		if (str1[i] < str2[i]) {
			return 1;
		}
		if (str1[i] > str2[i]) {
			return -1;
		}
	}
	return 0;
	
}


char *genString(int str_len) {
	unsigned int i;
	char *String;
	if (str_len == 1)
		str_len++;
	str_len = 1 + rand()%(str_len - 1);
	String = (char *) malloc((str_len + 1) * sizeof(char));
	for(i = 0; i < str_len; i++) {
		String[i] = alphabet[rand()%(strlen(alphabet))];
	}
	String[str_len] = '\0';
	return String;
}
