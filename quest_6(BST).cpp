#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

//ключ данного родительского узла больше 
struct Data {
	char *key;
	char *value;
};

struct Node {
	struct Data data;
	struct Node *left; // указатель на левый 
	struct Node *right; // указатель на правый.
	struct Node *parent;
};
void REMOVE(struct Node **node, const char *key);
int INSERT(struct Node *node, const char *key, const char *value);
struct Node *FIND(struct Node *node, const char *key);
struct Node *createNode(const char *key, const char *value);
void setParent(struct Node **node, struct Node **parent);
void copyNode(struct Node **node, struct Node **CopyNode);
void copyNodeData(struct Node **node, struct Node **CopyNode);
void freeNode(struct Node **node);
struct Node *findMinBiggerNode(struct Node *node);
void BSTFunc();
void PrintMenu();

int main(int argc, char **argv) {
	
	setlocale(LC_ALL, "Russian");
	BSTFunc();

	return 0;
}

void BSTFunc()
{
	int menu = 0;
	char *key = (char *) malloc( 100 * sizeof(char));
	char *value = (char *) malloc (100 * sizeof (char));
	struct Node *head = NULL; //точка входа в дерево, главный элемент		
	PrintMenu();
	while(scanf("%d", &menu) != EOF){
		if (menu == 1){
			printf("¬ведите ключ и значение:\n");
			scanf("%s%s", key, value);
			if (head == NULL){
				head = createNode(key, value);
				head->parent = NULL;
			}
			else
				INSERT(head, key, value);
		}
		if (menu == 2){
			printf("¬ведите ключ дл€ поиска:\n");
			scanf("%s", key);
			if (head == NULL)
				printf("ћассив пуст\n");
			else {
				struct Node *findNode =	FIND(head, key);
				if(findNode == NULL)
					printf("Ёлемента с данным индексом не найдено.\n");
				else
				{
					printf("%s\n\n", findNode->data.value);
					findNode = NULL;
				}
			}
		}
		if (menu == 3){
			printf("¬ведите ключ дл€ удалени€:\n");
			scanf("%s", key);
			if (head == NULL)
				printf("ћассив пуст\n");
			else{
				if (!strcmp(head->data.key, key) && head->left == NULL && head->right == NULL){
					freeNode(&head);
					head = NULL;
				}
				else
					REMOVE(&head, key);
			}
		}
		PrintMenu();
	}

}

void PrintMenu()
{
	printf("***********ƒвоичное дерево поиска.**********\n");
	printf("1. ƒобавить элемент\n2. Ќайти значение по ключу\n3. ”далить элемент по заданному ключу\n");
}


struct Node *FIND(struct Node *node, const char *key)
{
	struct Node *cur_node = node;
	while (1){
		if (!strcmp(cur_node->data.key, key)) 
		{ //если у текущего нода совпал ключ с заданным
			return cur_node;
		}
		if (strcmp(key, cur_node->data.key) < 0) 
		{ //если ключ меньше то идем в левый поднод
			if (cur_node->left == NULL) 
			{ //если его еще не существует
				cur_node = NULL;
				return NULL;
			} else {
				cur_node = cur_node->left; // если он уже существует, переходим к нему
				continue;
			}		
		} else 
		{
			if (cur_node->right == NULL) 
			{ //если правого еще не существует
				cur_node = NULL;
				return NULL;
			} else {
				cur_node = cur_node->right; // если он уже существует, переходим к нему
				continue;
			}

		}
	}
}

//рекурсивна€ вставка... углубл€емс€ до вставки.
int INSERT(struct Node *node, const char *key, const char *value) 
{
	struct Node *cur_node = node;
	while (1){
		if (!strcmp(cur_node->data.key, key)) 
		{ //если у текущего нода совпал ключ с заданным
			strcpy(cur_node->data.value, value); //замен€ем 
			return 0; // и выходим
		}
		if (strcmp(key, cur_node->data.key) < 0) 
		{ //если ключ меньше то идем в левый поднод
			if (cur_node->left == NULL) 
			{ //если его еще не существует
				cur_node->left = createNode(key, value); // создаем его
				setParent(&cur_node->left, &cur_node);
				return 0; // и выходим
			} else 
			{
				cur_node = cur_node->left; // если он уже существует, переходим к нему
				continue;
			}		
		} else 
		{
			if (cur_node->right == NULL) 
			{ //если правого еще не существует
				cur_node->right = createNode(key, value); // задаем его
				setParent(&cur_node->right, &cur_node);
				return 0; // и выходим
			} else 
			{
				cur_node = cur_node->right; // если он уже существует, переходим к нему
				continue;
			}	
		}
	}
} //конец рекурсивной вставки

void REMOVE(struct Node **head, const char *key)
{
	struct Node *cur_node = *head, *right, *left;
	struct Node *minMoreNode = NULL;
	while (1){
		if (!strcmp(cur_node->data.key, key)) 
		{ //если у текущего нода совпал ключ с заданным
			left = cur_node->left;
			right = cur_node->right;
			if(left == NULL && right == NULL)
			{//если оба наследника == 0
				if (cur_node->parent->left == cur_node) {
					cur_node->parent->left = NULL;//обнул€ем у предка если текущий - левый
				}
				if (cur_node->parent->right == cur_node) {
					cur_node->parent->right = NULL;//обнул€ем у предка если текущий - правый
				}
				free(cur_node->data.key);
				free(cur_node->data.value);
				cur_node->parent = NULL;
				free(cur_node);
				cur_node = NULL;
				return;
			}
			if (cur_node->left == NULL){
				copyNode(&cur_node, &right);
				freeNode(&right);
				return;
			}
			if (cur_node->right == NULL){
				copyNode(&cur_node, &left);
				freeNode(&left);
				return;
			}
			//если у нас существуют оба 
			minMoreNode = findMinBiggerNode(cur_node);
			if (minMoreNode->right != NULL)
			{
				minMoreNode->parent->left = minMoreNode->right;
			}
			copyNodeData(&cur_node, &minMoreNode);
			freeNode(&minMoreNode);
			return;
		}
		left = cur_node->left;
		right = cur_node->right;
		if (strcmp(cur_node->data.key, key) > 0) 
		{ //если ключ меньше то идем в левый поднод
			if (left != NULL) 
			{ //если левый существует
				cur_node = cur_node->left; // если он уже существует, переходим к нему
				continue;
			} else 
			{
				printf("Ёлемента с заданным ключом не существует.\n");
				left = NULL;
				right = NULL;
				return;
			}		
		} else 
		{
			if(right != NULL) 
			{ //если правый существует
				cur_node = cur_node->right; // если он уже существует, переходим к нему
				continue;
			} else 
			{
				printf("Ёлемента с заданным ключом не существует\n");
				left = NULL;
				right = NULL;
				return;
			}	
		}
	}


}

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

void setParent(struct Node **node, struct Node **parent){
	(*node)->parent = *parent;
}

void copyNode(struct Node **node, struct Node **CopyNode)
{
	(*node)->data.key = (char *) malloc((strlen((*CopyNode)->data.key) + 1) * sizeof(char)); //место под key (+1) - дл€ \0
	strcpy((*node)->data.key, (*CopyNode)->data.key);//копируем key
	(*node)->data.value = (char *) malloc((strlen((*CopyNode)->data.value) + 1) * sizeof(char)); //место под value (+ 1) - дл€ \0
	strcpy((*node)->data.value, (*CopyNode)->data.value); // копируем value
	(*node)->left = (*CopyNode)->left;//указатель на левого потомка
	(*node)->right = (*CopyNode)->right; // указатель на правого потомка
}
void copyNodeData(struct Node **node, struct Node **CopyNode)
{
	(*node)->data.key = (char *) malloc((strlen((*CopyNode)->data.key + 1) * sizeof(char))); //место под key (+1) - дл€ \0
	strcpy((*node)->data.key, (*CopyNode)->data.key);//копируем key
	(*node)->data.value = (char *) malloc((strlen((*CopyNode)->data.value + 1) * sizeof(char))); //место под value (+ 1) - дл€ \0
	strcpy((*node)->data.value, (*CopyNode)->data.value); // копируем value
}
void freeNode(struct Node **node)
{

	free((*node)->data.key);
	(*node)->data.key = NULL;
	free((*node)->data.value);
	(*node)->data.value = NULL;
	(*node)->parent = NULL;
	(*node)->left = NULL;
	(*node)->right = NULL;
	free(*node);
	*node = NULL;
	return;
}



struct Node *findMinBiggerNode(struct Node *node)
{
	struct Node *cur_node = node;
	cur_node = cur_node->right;
	while (1)
	{
		if (cur_node->left != NULL)
			cur_node = cur_node->left;
		else
			return cur_node;
	}

}