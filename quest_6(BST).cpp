#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//ключ данного родительского узла больше 
struct Data {
	char *key;
	char *value;
};


struct Node {
	struct Data data;
	struct Node *left; // указатель на левый 
	struct Node *right; // указатель на правый.
};

struct Node *FIND(const char *key);
bool INSERT(const char *key, const char *value);
void REMOVE(const char *key);
bool rInsertion(struct Node *node, const char *key, const char *value);
struct Node *rFind(struct Node *node, const char *key);
struct Node *head = (struct Node *) malloc(sizeof(struct Node)); //точка входа в дерево, главный элемент


int main(int argc, char **argv) {

	return 0;
}

void BSTFunc()
{
	int menu = 0;
	char *key, *value;
	PrintMenu();
	while(scanf("%d", menu) != EOF){
		if (menu == 1){
			printf("Введите ключ и значение:");
			scanf("%s%s", key, value);
			INSERT(key, value);
		}
		if (menu == 2){
			printf("Введите ключа для поиска:");
			scanf("%s", key);
			FIND(key);
		}
		if (menu == 3){
			printf("Введите ключ для удаления:");
			scanf("%s", key);
			REMOVE(key);
		}
		if (menu == 4){
			printf("Очистка дерева.");
			//удаление
			//освобождение key, value.
			exit(0);
		}
		PrintMenu();
	}

}

void PrintMenu()
{
	printf("***********Двоичное дерево поиска.**********");
	printf("1. Добавить элемент\n2. Найти значение по ключу\n3. Удалить элемент по заданному ключу\n 4. Выход");
}

struct Node *FIND (const char *key)
{
	return	rFind(head, key);
}

struct Node *rFind(struct Node *node, const char *key)
{
	if (node == NULL){
		printf("Элемента с заданным ключом '%s' не существует", key);
	}
	//struct Node *answer = NULL;
	if (!strcmp(node->data.key, key)) 
	{ //если у текущего нода совпал ключ с заданным
		return node; // тогда возвращаем его
	}
	if (strcmp(node->data.key, key) < 0) 
	{ //если ключ меньше то идем в левый поднод
		if (node->left == NULL) 
		{ //если его еще не существует
			printf("Элемента с заданным ключом '%s' не существует", key);
			return NULL; // и выходим
		} else {
			return 	rFind(node->left, key); // если он cуществует, переходим к нему
		}
	} else { //если заданный ключ >= нашего
		if (node->right == NULL)
		{ //если правого еще не существует
			printf("Элемента с заданным ключом '%s' не существует", key);
			return NULL; // и выходим
		} else {
			return rFind(node->right, key);
		}		
	}
}

//вставка
bool INSERT(const char *key, const char *value)
{
	if (head == NULL) {
		head = createNode(key, value);
		return true;
	}
	rInsertion(head, key, value);
}

//рекурсивная вставка... углубляемся до вставки.
bool rInsertion(struct Node *node, const char *key, const char *value) {
	if (node == NULL) {// для верхушки дерева
		node = createNode(key, value);//
	}
	if (!strcmp(node->data.key, key)) 
	{ //если у текущего нода совпал ключ с заданным
		strcpy(node->data.value, value); //заменяем 
		return true; // и выходим
	}
	if (strcmp(node->data.key, key) < 0) 
	{ //если ключ меньше то идем в левый поднод
		if (node->left == NULL) 
		{ //если его еще не существует
			node->left = createNode(key, value); // создаем его
			return true; // и выходим
		} else {
			rInsertion(node->left, key, value); // если он уже существует, переходим к нему
			return true;
		}
	} else { //если заданный ключ >= нашего
		if (node->right == NULL) 
		{ //если правого еще не существует
			node->right = createNode(key, value); // задаем его
			return true; // и выходим
		} else {
			rInsertion(node->right, key, value); // если он уже существует, переходим к нему
			return true;
		}		
	}
} //конец рекурсивной вставки



//создает ноду
struct Node *createNode(const char *key, const char *value)
{
	struct Node *newNode = (struct Node *) malloc(sizeof(struct Node));
	newNode->data.key = (char *) malloc((strlen(key) + 1) * sizeof(char));
	newNode->data.value = (char *) malloc((strlen(value) + 1) * sizeof(char));
	strcpy(newNode->data.key, key);
	strcpy(newNode->data.value, value);
	newNode->left = NULL;
	newNode->right = NULL;
	return newNode;
}