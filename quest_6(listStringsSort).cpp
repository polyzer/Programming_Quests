#include <locale.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

unsigned int stringsCount, maxStringSize;
char alphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIGKLMNOPQRSTUVWXYZ0123456789";

struct DoubleLinkList{
	char *str;
	struct DoubleLinkList *next;
	struct DoubleLinkList *prev;
};
void GenerateDoubleLinkStrings
(
	unsigned int stringsCount,
	unsigned int maxStringSize
);
void sortListStrings (struct DoubleLinkList *list);	
int strComparison(char *str1, char *str2);
struct DoubleLinkList genListElement(int stringLength);

int main(int argc, char **argv) {
	setlocale(LC_ALL, "Russian");
	srand(clock());
	printf("Создание списков.\n");
	printf("Введите количество строк:");
	scanf("%i", &stringsCount);
	printf("Введите длину строк:");
	scanf("%i", &maxStringSize);
	GenerateDoubleLinkStrings(stringsCount, maxStringSize);
	return 0;
}

void GenerateDoubleLinkStrings(
	 unsigned int stringsCount,
	 unsigned int maxStringSize
	 )
{
		struct DoubleLinkList *list = (struct DoubleLinkList *) malloc(stringsCount * sizeof(struct DoubleLinkList));
		struct DoubleLinkList *cur_elem = list;

		int i;
		for (i = 0; i < stringsCount; i++)
			list[i] = genListElement(maxStringSize);
		
		for (i = 0; i < stringsCount; i++) 
		{
			if(i == 0) {
				list[i].next = &(list[i + 1]);
				list[i].prev = NULL;
			}
			if (i != 0 && i != (stringsCount - 1)) {
				list[i].prev = &(list[i - 1]);
				list[i].next = &(list[i + 1]);
			}
			if (i == (stringsCount - 1)) {
				list[i].prev = &(list[i - 1]);
				list[i].next = NULL;
			}
		}
		//ВЫЗВАТЬ ФУНКЦИЮ СОРТИРОВКИ
		sortListStrings(list);    

		while (cur_elem->next != NULL) {
				printf("%s\n", cur_elem->str);
				cur_elem = cur_elem->next;
		}
		system("pause");
		
		//ОСВОБОЖДЕНИЕ УКАЗАТЕЛЕЙ!!!  
		cur_elem = list;
		while (cur_elem->next != NULL) {
			free(cur_elem->str);
			if (cur_elem->next->next == NULL)
				free(cur_elem->next->str);
			cur_elem = cur_elem->next;
		}
		free(list);
		cur_elem = NULL;  
		free(cur_elem);
}

void sortListStrings(struct DoubleLinkList *list) 
{
	struct DoubleLinkList *cur_elem;
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


struct DoubleLinkList genListElement(int str_len) {
	unsigned int i;
	struct DoubleLinkList listElement;
	if (str_len == 1)
		str_len++;
	str_len = 1 + rand()%(str_len - 1);
	listElement.str = (char *) malloc((str_len + 1) * sizeof(char));
	for(i = 0; i < str_len; i++) {
		listElement.str[i] = alphabet[rand()%(strlen(alphabet))];
	}
	listElement.str[str_len] = '\0';
	return listElement;
}



/*
void sortListStrings(struct DoubleLinkList *list) 
{
	struct DoubleLinkList *cur_elem, *next_elem;
	int work = 1;
	int i;
	while (work) {
		cur_elem = list;
		next_elem = cur_elem.next;
		
		while(next_elem.next != NULL) {
			if (strComparison(elem.str, elem.next.str) < 0){
				next_elem.prev = cur_elem.prev; //Меняем предыд у следующего элемента на текущий предыдущий
				cur_elem.next = next_elem.next; //Меняем у текущего элемента следующий на следующий у следующего
				cur_elem.prev = next_elem; //Меняем предыдущий у текущего элемента на следующий
				next_elem.next.prev = cur_elem; // Меняем предыдущий у следующего у следующего на текущий
				next_elem.next = cur_elem; // меняем следующий у бывшего следующего на текущий
				next_elem = cur_elem.next;
				break;
			}
		}
		
	}
	
	cur_elem = NULL;
	next_elem = NULL;
}
*/
