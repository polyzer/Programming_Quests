#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//���� ������� ������������� ���� ������ 
struct Data {
	char *key;
	char *value;
};


struct Node {
	struct Data data;
	struct Node *left; // ��������� �� ����� 
	struct Node *right; // ��������� �� ������.
};

struct Node *FIND(const char *key);
bool INSERT(const char *key, const char *value);
void REMOVE(const char *key);
bool rInsertion(struct Node *node, const char *key, const char *value);
struct Node *rFind(struct Node *node, const char *key);
struct Node *head = (struct Node *) malloc(sizeof(struct Node)); //����� ����� � ������, ������� �������


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
			printf("������� ���� � ��������:");
			scanf("%s%s", key, value);
			INSERT(key, value);
		}
		if (menu == 2){
			printf("������� ����� ��� ������:");
			scanf("%s", key);
			FIND(key);
		}
		if (menu == 3){
			printf("������� ���� ��� ��������:");
			scanf("%s", key);
			REMOVE(key);
		}
		if (menu == 4){
			printf("������� ������.");
			//��������
			//������������ key, value.
			exit(0);
		}
		PrintMenu();
	}

}

void PrintMenu()
{
	printf("***********�������� ������ ������.**********");
	printf("1. �������� �������\n2. ����� �������� �� �����\n3. ������� ������� �� ��������� �����\n 4. �����");
}

struct Node *FIND (const char *key)
{
	return	rFind(head, key);
}

struct Node *rFind(struct Node *node, const char *key)
{
	if (node == NULL){
		printf("�������� � �������� ������ '%s' �� ����������", key);
	}
	//struct Node *answer = NULL;
	if (!strcmp(node->data.key, key)) 
	{ //���� � �������� ���� ������ ���� � ��������
		return node; // ����� ���������� ���
	}
	if (strcmp(node->data.key, key) < 0) 
	{ //���� ���� ������ �� ���� � ����� ������
		if (node->left == NULL) 
		{ //���� ��� ��� �� ����������
			printf("�������� � �������� ������ '%s' �� ����������", key);
			return NULL; // � �������
		} else {
			return 	rFind(node->left, key); // ���� �� c���������, ��������� � ����
		}
	} else { //���� �������� ���� >= ������
		if (node->right == NULL)
		{ //���� ������� ��� �� ����������
			printf("�������� � �������� ������ '%s' �� ����������", key);
			return NULL; // � �������
		} else {
			return rFind(node->right, key);
		}		
	}
}

//�������
bool INSERT(const char *key, const char *value)
{
	if (head == NULL) {
		head = createNode(key, value);
		return true;
	}
	rInsertion(head, key, value);
}

//����������� �������... ����������� �� �������.
bool rInsertion(struct Node *node, const char *key, const char *value) {
	if (node == NULL) {// ��� �������� ������
		node = createNode(key, value);//
	}
	if (!strcmp(node->data.key, key)) 
	{ //���� � �������� ���� ������ ���� � ��������
		strcpy(node->data.value, value); //�������� 
		return true; // � �������
	}
	if (strcmp(node->data.key, key) < 0) 
	{ //���� ���� ������ �� ���� � ����� ������
		if (node->left == NULL) 
		{ //���� ��� ��� �� ����������
			node->left = createNode(key, value); // ������� ���
			return true; // � �������
		} else {
			rInsertion(node->left, key, value); // ���� �� ��� ����������, ��������� � ����
			return true;
		}
	} else { //���� �������� ���� >= ������
		if (node->right == NULL) 
		{ //���� ������� ��� �� ����������
			node->right = createNode(key, value); // ������ ���
			return true; // � �������
		} else {
			rInsertion(node->right, key, value); // ���� �� ��� ����������, ��������� � ����
			return true;
		}		
	}
} //����� ����������� �������



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