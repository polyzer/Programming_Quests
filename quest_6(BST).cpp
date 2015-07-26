#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

//���� ������� ������������� ���� ������ 
struct Data {
	char *key;
	char *value;
};

struct Node {
	struct Data data;
	struct Node *left; // ��������� �� ����� 
	struct Node *right; // ��������� �� ������.
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
	struct Node *head = NULL; //����� ����� � ������, ������� �������		
	PrintMenu();
	while(scanf("%d", &menu) != EOF){
		if (menu == 1){
			printf("������� ���� � ��������:\n");
			scanf("%s%s", key, value);
			if (head == NULL){
				head = createNode(key, value);
				head->parent = NULL;
			}
			else
				INSERT(head, key, value);
		}
		if (menu == 2){
			printf("������� ���� ��� ������:\n");
			scanf("%s", key);
			if (head == NULL)
				printf("������ ����\n");
			else {
				struct Node *findNode =	FIND(head, key);
				if(findNode == NULL)
					printf("�������� � ������ �������� �� �������.\n");
				else
				{
					printf("%s\n\n", findNode->data.value);
					findNode = NULL;
				}
			}
		}
		if (menu == 3){
			printf("������� ���� ��� ��������:\n");
			scanf("%s", key);
			if (head == NULL)
				printf("������ ����\n");
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
	printf("***********�������� ������ ������.**********\n");
	printf("1. �������� �������\n2. ����� �������� �� �����\n3. ������� ������� �� ��������� �����\n");
}


struct Node *FIND(struct Node *node, const char *key)
{
	struct Node *cur_node = node;
	while (1){
		if (!strcmp(cur_node->data.key, key)) 
		{ //���� � �������� ���� ������ ���� � ��������
			return cur_node;
		}
		if (strcmp(key, cur_node->data.key) < 0) 
		{ //���� ���� ������ �� ���� � ����� ������
			if (cur_node->left == NULL) 
			{ //���� ��� ��� �� ����������
				cur_node = NULL;
				return NULL;
			} else {
				cur_node = cur_node->left; // ���� �� ��� ����������, ��������� � ����
				continue;
			}		
		} else 
		{
			if (cur_node->right == NULL) 
			{ //���� ������� ��� �� ����������
				cur_node = NULL;
				return NULL;
			} else {
				cur_node = cur_node->right; // ���� �� ��� ����������, ��������� � ����
				continue;
			}

		}
	}
}

//����������� �������... ����������� �� �������.
int INSERT(struct Node *node, const char *key, const char *value) 
{
	struct Node *cur_node = node;
	while (1){
		if (!strcmp(cur_node->data.key, key)) 
		{ //���� � �������� ���� ������ ���� � ��������
			strcpy(cur_node->data.value, value); //�������� 
			return 0; // � �������
		}
		if (strcmp(key, cur_node->data.key) < 0) 
		{ //���� ���� ������ �� ���� � ����� ������
			if (cur_node->left == NULL) 
			{ //���� ��� ��� �� ����������
				cur_node->left = createNode(key, value); // ������� ���
				setParent(&cur_node->left, &cur_node);
				return 0; // � �������
			} else 
			{
				cur_node = cur_node->left; // ���� �� ��� ����������, ��������� � ����
				continue;
			}		
		} else 
		{
			if (cur_node->right == NULL) 
			{ //���� ������� ��� �� ����������
				cur_node->right = createNode(key, value); // ������ ���
				setParent(&cur_node->right, &cur_node);
				return 0; // � �������
			} else 
			{
				cur_node = cur_node->right; // ���� �� ��� ����������, ��������� � ����
				continue;
			}	
		}
	}
} //����� ����������� �������

void REMOVE(struct Node **head, const char *key)
{
	struct Node *cur_node = *head, *right, *left;
	struct Node *minMoreNode = NULL;
	while (1){
		if (!strcmp(cur_node->data.key, key)) 
		{ //���� � �������� ���� ������ ���� � ��������
			left = cur_node->left;
			right = cur_node->right;
			if(left == NULL && right == NULL)
			{//���� ��� ���������� == 0
				if (cur_node->parent->left == cur_node) {
					cur_node->parent->left = NULL;//�������� � ������ ���� ������� - �����
				}
				if (cur_node->parent->right == cur_node) {
					cur_node->parent->right = NULL;//�������� � ������ ���� ������� - ������
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
			//���� � ��� ���������� ��� 
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
		{ //���� ���� ������ �� ���� � ����� ������
			if (left != NULL) 
			{ //���� ����� ����������
				cur_node = cur_node->left; // ���� �� ��� ����������, ��������� � ����
				continue;
			} else 
			{
				printf("�������� � �������� ������ �� ����������.\n");
				left = NULL;
				right = NULL;
				return;
			}		
		} else 
		{
			if(right != NULL) 
			{ //���� ������ ����������
				cur_node = cur_node->right; // ���� �� ��� ����������, ��������� � ����
				continue;
			} else 
			{
				printf("�������� � �������� ������ �� ����������\n");
				left = NULL;
				right = NULL;
				return;
			}	
		}
	}


}

//������� ����
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
	(*node)->data.key = (char *) malloc((strlen((*CopyNode)->data.key) + 1) * sizeof(char)); //����� ��� key (+1) - ��� \0
	strcpy((*node)->data.key, (*CopyNode)->data.key);//�������� key
	(*node)->data.value = (char *) malloc((strlen((*CopyNode)->data.value) + 1) * sizeof(char)); //����� ��� value (+ 1) - ��� \0
	strcpy((*node)->data.value, (*CopyNode)->data.value); // �������� value
	(*node)->left = (*CopyNode)->left;//��������� �� ������ �������
	(*node)->right = (*CopyNode)->right; // ��������� �� ������� �������
}
void copyNodeData(struct Node **node, struct Node **CopyNode)
{
	(*node)->data.key = (char *) malloc((strlen((*CopyNode)->data.key + 1) * sizeof(char))); //����� ��� key (+1) - ��� \0
	strcpy((*node)->data.key, (*CopyNode)->data.key);//�������� key
	(*node)->data.value = (char *) malloc((strlen((*CopyNode)->data.value + 1) * sizeof(char))); //����� ��� value (+ 1) - ��� \0
	strcpy((*node)->data.value, (*CopyNode)->data.value); // �������� value
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