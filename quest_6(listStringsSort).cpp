#include <locale.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

struct ListElement{
	char *str;
	struct ListElement *next;
	struct ListElement *prev;
};
void sortListStrings (struct ListElement *list);	
int strComparison(char *str1, char *str2);
char *genString(const int len);
void DoubleLinkListFunc();
void listFree(struct ListElement *list);

void InsertStringTailList(struct ListElement *list, char *str);
struct ListElement *AllocAndInitListElement(char* str);
void InsertElementTailList(struct ListElement *list, struct ListElement *newelem);
void setPrevElementTailList(struct ListElement *list);

int fillList(struct ListElement *list, const int count, const int len);
struct ListElement *createListElement(struct ListElement *parent, int len);
char alphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIGKLMNOPQRSTUVWXYZ0123456789";

int main(int argc, char **argv) {
	setlocale(LC_ALL, "Russian");
	DoubleLinkListFunc();
	return 0;
}

void DoubleLinkListFunc() 
{
	int stringsCount, maxStringSize;
	struct ListElement *list = NULL;
	struct ListElement *cur_elem = NULL;
	srand(clock());	
	printf("Ñîçäàíèå ñïèñêîâ.\n");
	printf("Ââåäèòå êîëè÷åñòâî ñòðîê:");
	scanf("%i", &stringsCount);
	printf("Ââåäèòå äëèíó ñòðîê:");
	scanf("%i", &maxStringSize);	
	list = AllocAndInitListElement(genString(maxStringSize));
	fillList(list, stringsCount, maxStringSize);
	//ÂÛÇÂÀÒÜ ÔÓÍÊÖÈÞ ÑÎÐÒÈÐÎÂÊÈ
	//sortListStrings(list);    
	cur_elem = list;
	while (cur_elem->next != NULL) {
			printf("%s\n", cur_elem->str);
			cur_elem = cur_elem->next;
	}
	cur_elem = NULL;
	system("pause");
	//ÎÑÂÎÁÎÆÄÅÍÈÅ ÓÊÀÇÀÒÅËÅÉ!!!  
	listFree(list);
}
struct ListElement *AllocAndInitListElement(char* str)
{
	struct ListElement *element = (struct ListElement *) malloc(sizeof(struct ListElement));
	int strlength = strlen(str);
	element->next = NULL;
	element->prev = NULL;
	element->str = str;
	return element;
}
void InsertElementTailList(struct ListElement *list, struct ListElement *newelem)
{
	struct ListElement *cur_elem = list;
	while (cur_elem->next != NULL) {
			cur_elem = cur_elem->next;
	}
	cur_elem->next = newelem;
	setPrevElementTailList(cur_elem);
	cur_elem = NULL;
}
void setPrevElementTailList(struct ListElement *list)
{
	struct ListElement *cur_elem = list;
	while (cur_elem->next->next != NULL) {
			cur_elem = cur_elem->next;
	}
	cur_elem->next->prev = cur_elem;
	cur_elem = NULL;
}
void InsertStringTailList(struct ListElement *list, char *str)
{
	struct ListElement *cur_elem = list;
	while (cur_elem->next != NULL) {
			cur_elem = cur_elem->next;
	}
	cur_elem->str = str;
	cur_elem = NULL;
}


void listFree(struct ListElement *list) 
{
	struct ListElement *cur_elem = list;
	list = NULL;
	while (1) {
		free(cur_elem->str);
		free(cur_elem->prev);
		cur_elem->prev = NULL;
		if (cur_elem->next == NULL)
			return;
		cur_elem = cur_elem->next;
	}		
	free(cur_elem);
	cur_elem = NULL;
}
int fillList(struct ListElement *list, const int count, const int len) {
	int p;
	struct ListElement *element;	
	for (p = 0; p < count; p++) {
		InsertElementTailList(list, AllocAndInitListElement(genString(len)));
		printf("%d\n", p);
	}
	return 0;
}
char *genString(const int len) 
{
	int i;
	int str_len = len;
	int alphlen = strlen(alphabet);
	char *str;
	if (str_len == 1)
		str_len++;
	str_len = 1 + rand()%(len - 1);
	str = (char *) malloc((str_len + 1) * sizeof(char));
	for(i = 0; i < str_len; i++) 
		{
			str[i] = alphabet[rand()%(alphlen)];
		}
	str[str_len] = '\0';
	return str;
}

void sortListStrings(struct ListElement *list) 
{
	struct ListElement *cur_elem;
	char *buf_char = NULL;
	int work = 1;
	int i;
	while (work) 
	{
		cur_elem = list;
		while(cur_elem->next != NULL) 
		{
			if (strComparison(cur_elem->str, cur_elem->next->str) < 0)
			{
				buf_char = cur_elem->str;
				cur_elem->str = cur_elem->next->str;
				cur_elem->next->str = buf_char;
				buf_char = NULL;
				break;
			}
			if (cur_elem->next->next == NULL)
				work = 0;
			cur_elem = cur_elem->next;
		}
	}
	cur_elem = NULL;
}

int strComparison(char *str1, char *str2) {
	int i, stringLength;
	if (strlen(str1) <= strlen(str2)){
		stringLength = strlen(str1);
	} else {
		stringLength = strlen(str2);
	}
	for (i = 0; i < stringLength; i++) {
		if ((int) str1[i] < (int) str2[i]) {
			return 1;
		}
		if ((int) str1[i] > (int) str2[i]) {
			return -1;
		}
	}
	return 0;
}

