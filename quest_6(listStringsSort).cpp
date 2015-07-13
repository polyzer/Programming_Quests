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
void DoubleLinkListFunc();
void listFree(struct ListElement *list);
bool fillList(struct ListElement *element, const int count, int deep, const int len);
struct ListElement *createListElement(struct ListElement *parent, int len);
char alphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIGKLMNOPQRSTUVWXYZ0123456789";

int main(int argc, char **argv) {
	setlocale(LC_ALL, "Russian");
	DoubleLinkListFunc();
	return 0;
}

void DoubleLinkListFunc() 
{
	srand(clock());	
	int stringsCount, maxStringSize;
	struct ListElement *list = NULL;
	struct ListElement *cur_elem = NULL;
	printf("Ñîçäàíèå ñïèñêîâ.\n");
	printf("Ââåäèòå êîëè÷åñòâî ñòðîê:");
	scanf("%i", &stringsCount);
	printf("Ââåäèòå äëèíó ñòðîê:");
	scanf("%i", &maxStringSize);	
	list = createListElement(NULL, maxStringSize);
	fillList(list, stringsCount - 1, 1, maxStringSize);
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


void listFree(struct ListElement *list) 
{
	struct ListElement *cur_elem = list;
	list = NULL;
	while (true) {
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
bool fillList(struct ListElement *element, const int count, int deep, const int len) {
	int p;
	unsigned int i;
	struct ListElement *cur_elem = element;
	int str_len = len;
	int alphlen = strlen(alphabet);
	if (str_len == 1)
		str_len++;
	for (p = 0; p < count; p++) {
		cur_elem->next = (struct ListElement *) malloc(sizeof(struct ListElement));
		cur_elem->next->next = NULL;
		cur_elem->next->prev = cur_elem;
		str_len = 1 + rand()%(len - 1);
		cur_elem->next->str = (char *) malloc((str_len + 1) * sizeof(char));
		for(i = 0; i < str_len; i++) 
		{
			cur_elem->next->str[i] = alphabet[rand()%(alphlen)];
		}
		cur_elem->next->str[str_len] = '\0';
		cur_elem = cur_elem->next;
	}
	cur_elem = NULL;
	return true;
}

struct ListElement *createListElement(struct ListElement *parent, const int len){
		unsigned int i;
		int str_len = len;
		struct ListElement *element = (struct ListElement *) malloc(sizeof(struct ListElement));
		if (str_len == 1)
			str_len++;
		element->next = NULL;
		element->prev = parent;
		str_len = 1 + rand()%(str_len - 1);
		element->str = (char *) malloc((str_len + 1) * sizeof(char));
		for(i = 0; i < str_len; i++) 
		{
			element->str[i] = alphabet[rand()%(strlen(alphabet))];
		}
		element->str[str_len] = '\0';
		return element;
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

