#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <Windows.h>
#include <locale.h>

#define MY_PORT 666

#define PRINTNUSERS if (nclients) printf("%d user on-line\n", nclients); else printf("No user On-Line\n");

DWORD WINAPI SexToClient(LPVOID client_socket);

int nclients = 0;

struct Client{
	SOCKET *socket;
};
struct ListElement{
	struct Client client;
	struct ListElement *next;
};
// вставка в конец листа. Для этого передаются двойной указатель на голову, конец, и вставляемый элемент!
void insertElementTailList(struct ListElement **ListHead, struct ListElement **ListEnd, struct ListElement *newelem);
// удаление всего листа
void freeList(struct ListElement **ListHead, struct ListElement **ListEnd);
// выделение и инициализация элемента, который затем возвратится, для вставки
struct ListElement *allocAndInitElement(SOCKET *socket);
// удаление элемента листа от сокета
void removeListElementBySocket(struct ListElement **ListHead, struct ListElement **ListEnd, SOCKET *socket);
// рассылка сообщения всем клиентам!
void sendToAllClients(char *buff, int bytes_recv, struct ListElement **ListHead, struct ListElement **ListEnd); // отправляет всем участникам списка!

void clientInit(SOCKET *socket);

//Задаются два элемента
// первый указывает на голову списка
// второй на конец списка!
struct ListElement *ListHead = NULL;
struct ListElement *ListEnd = NULL;

int main(int argc, char *argv[]) 
{

	setlocale(LC_ALL, "Russian");
	char buff[1024];

	printf("TCP SERVER DEMO \n");

	if (WSAStartup(0x0202, (WSADATA *) &buff[0]))
	{
		printf("ERROR WSAStartup %d\n", WSAGetLastError());
		return -1;
	}

	SOCKET mysocket;

	if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("ERROR socket %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	sockaddr_in local_addr;
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(MY_PORT);
	local_addr.sin_addr.s_addr = 0;

	if(bind(mysocket, (sockaddr *) &local_addr, sizeof(local_addr)))
	{
		printf("ERROR bind %d\n", WSAGetLastError());
		closesocket(mysocket);
		WSACleanup();
		return -1;
	}

	if(listen(mysocket, 0x109))
	{
		printf("ERROR listen %d \n", WSAGetLastError());
		closesocket(mysocket);
		WSACleanup();
		return -1;
	}

	printf("Ожидание подключений...\n");

	SOCKET client_socket;
	sockaddr_in client_addr;
	int client_addr_size = sizeof(client_addr);

	while((client_socket = accept(mysocket, (sockaddr *) &client_addr, &client_addr_size)))
	{
		nclients++;

		HOSTENT *hst;
		hst = gethostbyaddr((char *) &client_addr.sin_addr.s_addr, 4, AF_INET);

		printf("+%s [%s] new connect! \n",
		(hst)?hst->h_name:"",inet_ntoa(client_addr.sin_addr));
		PRINTNUSERS

		DWORD thID;
		CreateThread(NULL, NULL, SexToClient, &client_socket, NULL, &thID);

	}


	return 0;
}


DWORD WINAPI SexToClient(LPVOID client_socket)
{
	SOCKET my_sock;
	my_sock = ((SOCKET *) client_socket)[0];
	char buff[20*1024];
	#define sHELLO "Вы подключены\r\n"	
	send(my_sock, sHELLO, sizeof(sHELLO), 0);
	// инициализируем клиента!
	clientInit(&my_sock);
	while(true)
	{
		int bytes_recv = recv(my_sock, &buff[0], sizeof(buff), 0);
		if (bytes_recv && bytes_recv != SOCKET_ERROR){			
			sendToAllClients(buff, bytes_recv, &ListHead, &ListEnd);
		} else 
		{
			break;
		}
	}
	
	nclients--;

	printf("-disconnect\n"); PRINTNUSERS
	//удаление клиента!
	removeListElementBySocket(&ListHead, &ListEnd, &my_sock);
//	closesocket(my_sock);
	return 0;
}

void listFree(struct ListElement **ListHead, struct ListElement **ListEnd) 
{
	// позаботиться о ListHead и ListEnd!!!!!!!!!!!!! 
	struct ListElement *cur_elem = *ListHead;
	struct ListElement *timeel = NULL;
	while (1) {
		///ЗДЕСЬ ПОСМОТРЕТЬ!
		closesocket(*(cur_elem->client.socket));
		if (cur_elem->next == NULL)
			break;
		timeel = cur_elem;
		cur_elem = cur_elem->next;
		free(timeel);
	}		
	free(cur_elem);
	cur_elem = NULL;
	timeel = NULL;
	*ListHead = NULL;
	*ListEnd = NULL;
}

void removeListElementBySocket(struct ListElement **ListHead, struct ListElement **ListEnd, SOCKET *socket)
{
	//Временный объект для хранения указателя,
	//чтобы потом очистить
	struct ListElement *cur_elem = *ListHead; // начинаем проход с головы
	struct ListElement *timeel = NULL; // 
	
	if (*ListHead == *ListEnd) // если текущий элемент последний в списке
	{
		if (cur_elem->client.socket == socket)
		{
			closesocket(*(cur_elem->client.socket)); // у данного закрываем сокет
			cur_elem->client.socket = NULL; // устанавливаем указатель сокета у следующего элемента на NULL
			free(cur_elem);
			cur_elem = NULL;
			*ListEnd = NULL; // переустанавливаем последний элемент!
			*ListHead = NULL;
			return;
		} else 
		{
			printf("Сокет не подходит ни к одному из элементов!");
		}
	}
	// Если у нас не один элемент в списке, то идем в цикл
	while (1)
	{
			if(cur_elem->client.socket == socket) // если у текущего элемента сокет совпадает с заданным
			{
				closesocket(*(cur_elem->client.socket)); // у данного закрываем сокет
				cur_elem->client.socket = cur_elem->next->client.socket; //
				cur_elem->next->client.socket = NULL; // устанавливаем указатель сокета у следующего элемента на NULL
				timeel = cur_elem->next; // копируем указатель на след элемент в timeel для последующего удаления
				cur_elem->next = cur_elem->next->next; // меняем указатель на след на след->след
				timeel->next = NULL; // устанавливем указатель у бывшего next у следующего элемента на NULL
				free(timeel);
				return;
			} else 	if(cur_elem->next == *ListEnd && cur_elem->next->client.socket == socket) 
				// если у текущего элемента и заданный сокеты не сошлись
				// если след элемент - последний в списке и сокет у него совпадает с заданным
			{
				closesocket(*(cur_elem->next->client.socket)); // у данного закрываем сокет
				cur_elem->next->client.socket = NULL; // устанавливаем указатель сокета у следующего элемента на NULL
				free(cur_elem->next);
				cur_elem->next = NULL;
				*ListEnd = cur_elem; // переустанавливаем последний элемент!
				return;
			}
		cur_elem = cur_elem->next; // переход к следующему элементу в бесконечном цикле
	}
}
struct ListElement *allocAndInitElement(SOCKET *socket)
{
	struct ListElement *el = (struct ListElement *) malloc(sizeof(struct ListElement));
	el->next = NULL;
	el->client.socket = socket; // указываем на сокет!
	return el;
}

void insertElementTailList(struct ListElement **list_head, struct ListElement **list_end, struct ListElement *ins_elem)
{
	if((*list_head) == NULL && (*list_end) == NULL)
	{
		(*list_head) = ins_elem;
		(*list_end) = ins_elem;
	} else if((*list_end) == (*list_head))
	{
		(*list_head)->next = ins_elem;
		(*list_end) = ins_elem;
	} else
	{
		(*list_end)->next = ins_elem;
		(*list_end) = ins_elem;
	}
}

void sendToAllClients(char *buff, int bytes_recv, struct ListElement **ListHead, struct ListElement **ListEnd)
{
	struct ListElement *cur_el;
	cur_el = *ListHead;
	while (cur_el != NULL)
	{
		//пишем!
		printf("\n1 send");
		send(*(cur_el->client.socket), buff, bytes_recv, 0);
		cur_el = cur_el->next;
	}
}

void clientInit(SOCKET *socket)
{
	insertElementTailList(&ListHead, &ListEnd, allocAndInitElement(socket));
	if (ListEnd->next == NULL)
		printf("ListEnd->next == NULL");
}
