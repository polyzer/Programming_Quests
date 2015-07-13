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

struct Node *head = NULL;


int main(int argc, char **argv) {

	return 0;
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
	strcpy(newNode->data.key, key);
	strcpy(newNode->data.value, value);
	return newNode;
}