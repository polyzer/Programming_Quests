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
	strcpy(newNode->data.key, key);
	strcpy(newNode->data.value, value);
	return newNode;
}