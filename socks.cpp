#include <iostream>
#include <string>
#include <vector>
#include <WinSock2.h>
#include <Windows.h>
#include <clocale>

#define MY_PORT 666

#define PRINTNUSERS if (nclients) printf("%d user on-line\n", nclients); else printf("No user On-Line\n");

DWORD WINAPI SexToClient(LPVOID client_socket);

int nclients = 0;


/// подключаться к элементам подчата
/// WANNA_TO_CHAT:[1] - Номер элемента
/// хотя, лучше просто сделать соединение
/// с конкретным юзером, который должен его допустить!
/// подключиться к юзеру - создать подчат
/// WANNA_USERCHAT:nickname

struct Client{
	//сокет данного клиента
	SOCKET *socket;
	// ник данного клиента
	std::string *nickname;
	// указатель на след клиента в списке
	Client *next;
	// конструкторы!
	Client(){}
	Client(SOCKET *socket, const char *nick) 
	{
		this->socket = socket; 
		this->nickname = new std::string(nick);
		this->next = NULL;
	}
};

struct SubchatElement
{
	std::vector <Client*> Members;
	std::string Name;
//	SubchatElement *next;
	SubchatElement(Client *Client1, Client *Client2, std::string name)
	{
		this->Members.push_back(Client1);
		this->Members.push_back(Client2);
		this->Name = name;
	}
};

std::vector <SubchatElement*> Subchats;

Client *getClientByNickname(Client **ListHead, Client **ListEnd, std::string nickname);
void removeMemberFromSubchatElementByName(SubchatElement *SE_p, std::string member_name);
// отправляет предложение пользователям
// членам подчата и
// возвращает - согласны/не согласны ли они
bool sendSentenceToAllSubchatMembersAndGetBoolAnswers(SubchatElement *SE_p, std::string sentence);
// отправляет предложение пользователю и
// возвращает - согласен/не согласен ли он
bool sendSentenceToUserAndGetBoolAnswer(SOCKET *socket, std::string sentence);
// функция возвращает подчат, если в нем находится данный пользователь
// или NULL - если пользователя нет ни в одном подчате!
SubchatElement *getSubchatElementByName(std::string subchat_name, std::vector <SubchatElement*> &Subchats);
SubchatElement *getSubchatElementByMemberName(std::string member_name, std::vector <SubchatElement*> &Subchats);
// фукнкция присылает данному пользователю список всех имеющихся подчатов
// и пользователей
void sendListOfUsersAndSubchats(Client **ListHead, Client **ListEnd, std::vector <SubchatElement*> &Subchats, SOCKET socket);
// вставка в конец листа. Для этого передаются двойной указатель на голову, конец, и вставляемый элемент!
Client *insertElementTailList(Client **ListHead, Client **ListEnd, Client *newelem);
// удаление всего листа
void freeList(Client **ListHead, Client **ListEnd);
// выделение и инициализация элемента, который затем возвратится, для вставки
Client *allocAndInitElement(SOCKET *socket);
// удаление элемента листа от сокета
void removeListElementBySocket(Client **ListHead, Client **ListEnd, SOCKET *socket);
// рассылка сообщения всем клиентам!
void sendToAllClients(char *buff, int bytes_recv, Client **ListHead, Client **ListEnd);
Client *clientInit(SOCKET *socket, const char *buff);

//Задаются два элемента
// первый указывает на голову списка
// второй на конец списка!
Client *ListHead = NULL;
Client *ListEnd = NULL;

int main(int argc, char *argv[]) 
{

	setlocale(LC_ALL, "Russian");
	char buff[1024];

	std::cout << "TCP SERVER DEMO" << std::endl;

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
	Client *MyClient;
	Client *AnotherClient;
	// сокет клиента
	SOCKET my_sock;
	my_sock = ((SOCKET *) client_socket)[0];
	// буфер
	char buff[20*1024];
	int bytes_recv;
	// std::string буфер для упрощения
	std::string buffstr;
	// предложения, которые будут отправляться пользователям
	std::string sentence;
	// указатель на подчат, 
	SubchatElement *SubchatPtr = NULL;
	#define sHELLO "Вы подключены\r\n"	
	send(my_sock, sHELLO, sizeof(sHELLO), 0);
	// инициализируем клиента!
	// в buff приходит только никнейм клиента
	bytes_recv = recv(my_sock, &buff[0], sizeof(buff), 0);
	buff[bytes_recv] = 0;
	MyClient = clientInit(&my_sock, buff);
	while(true)
	{
		bytes_recv = recv(my_sock, &buff[0], sizeof(buff), 0);
		// ЗДЕСЬ ОТСЕКАЕТСЯ ПРИСЫЛАЕМЫЙ \n в конце, и заменяется на \0
		if (bytes_recv && bytes_recv != SOCKET_ERROR){			
			// если юзер захотел 
			if (buff[bytes_recv - 1] == '\n')
				buff[bytes_recv - 1] = 0;
			else
				buff[bytes_recv] = 0;
			// управляющая конструкция, которая пропускает чтение сообщения:
			if (strncmp(buff, "[off]", strlen("[off]")) == 0)
			{
				continue;
			}
			// нормальный разбор сообщения!
			if(strncmp(buff, "WANNA_LIST", strlen("WANNA_LIST")) == 0)
			{
				sendListOfUsersAndSubchats(&ListHead, &ListEnd, Subchats, my_sock);
			} else
			if(strncmp(buff, "WANNA_USERCHAT:", strlen("WANNA_USERCHAT:")) == 0)
			{
				// разбор сообщения
				buffstr = buff;
				// теперь buffstr содержит имя пользователя!
				buffstr = buffstr.substr(strlen("WANNA_USERCHAT:"));
				// если пользователь не состоит ни в одном 
				if ((AnotherClient = getClientByNickname(&ListHead, &ListEnd, buffstr)) != NULL)
				{
					if (getSubchatElementByName(*(AnotherClient->nickname), Subchats) == NULL)
					{
						sentence = "Хотите ли создать закрытый чат с пользователем:" + *(MyClient->nickname) + "\n";
						// спрашиваем согласие у клиента
						if(sendSentenceToUserAndGetBoolAnswer(AnotherClient->socket, sentence) == true)
						{
							// отправляем предложение, на которое должен быть дан ответ!
							sentence = "SENTENCE_FROM_ME:Введите имя для подчата:\n";
							send(*(MyClient->socket), sentence.c_str(), sentence.length() * sizeof(char), 0);
							bytes_recv = recv(*(MyClient->socket), buff, sizeof(buff), 0);
							if (buff[bytes_recv - 1] == '\n')
								buff[bytes_recv - 1] = 0;
							else
								buff[bytes_recv] = 0;
							buffstr = buff;
							// создаем подчат
							// и добавляем в него элемент
							SubchatPtr = new SubchatElement(MyClient, AnotherClient, buffstr);
							Subchats.push_back(SubchatPtr);
						}else
						{
							buffstr = "Пользователь " + *(AnotherClient->nickname) + "не желает создавать подчат!\n";
							send(*(MyClient->socket), buffstr.c_str(), strlen(buffstr.c_str()) * sizeof(char), 0);
						}
					} else
					{
						buffstr = "Пользователь " + *(AnotherClient->nickname) + "уже состоит в подчате!\n";
						send(*(MyClient->socket), buffstr.c_str(), strlen(buffstr.c_str()) * sizeof(char), 0);
					}
				}else
				{
					buffstr = "Пользователя с данным никнеймом нет в чате\n";
					send(*(MyClient->socket), buffstr.c_str(), strlen(buffstr.c_str()) * sizeof(char), 0);
				}

			} else
			if(strncmp(buff, "WANNA_TO_CHAT:", strlen("WANNA_TO_CHAT:")) == 0)
			{
				// разбор сообщения
				buffstr = buff;
				// получаем название буфера
				buffstr = buffstr.substr(strlen("WANNA_TO_CHAT:"));
				if ((SubchatPtr = getSubchatElementByName(buffstr, Subchats)) != NULL)
				{
					sentence = "Хотите ли вы добавить пользователя в закрытый чат:" + *(MyClient->nickname) + "\n";
					if(sendSentenceToAllSubchatMembersAndGetBoolAnswers(SubchatPtr, sentence) == true)
					{
						SubchatPtr->Members.push_back(MyClient);					
					} else
					{
						buffstr = "Члены подчата " + *(AnotherClient->nickname) + "не желает создавать подчат!\n";
						send(*(AnotherClient->socket), buffstr.c_str(), strlen(buffstr.c_str()) * sizeof(char), 0);
					}
				}
			}
			else
			{
				sendToAllClients(buff, bytes_recv, &ListHead, &ListEnd);
			}
		} else 
		{
			break;
		}
	}
	
	nclients--;

	//удаление клиента!
	if ((SubchatPtr = getSubchatElementByMemberName(*(MyClient->nickname), Subchats)) != NULL)
	{
		removeMemberFromSubchatElementByName(SubchatPtr, *(MyClient->nickname));
		// удаление подчата, если в нем больше никого не осталось
		if (SubchatPtr->Members.size() == 0)
		{
			for(int i = 0; i<Subchats.size(); i++)
			{
				if (Subchats[i] == SubchatPtr)
				{
					Subchats.erase(Subchats.begin() + i);
					delete SubchatPtr;
				}
			}
		}
	}
	removeListElementBySocket(&ListHead, &ListEnd, &my_sock);
	MyClient = NULL;
	SubchatPtr = NULL;
	printf("-disconnect\n"); PRINTNUSERS
//	closesocket(my_sock);
	return 0;
}

Client *getClientByNickname(Client **ListHead, Client **ListEnd, std::string nickname)
{
	Client *cur_elem = *ListHead;
	while (cur_elem != NULL) {
		if(*(cur_elem->nickname) == nickname)
			return cur_elem;
		else 
			cur_elem = cur_elem->next;
	}		
	return NULL;
}
void removeMemberFromSubchatElementByName(SubchatElement *SE_p, std::string member_name)
{
	for (int i = 0; i < SE_p->Members.size(); i++)
	{
		if(*(SE_p->Members[i]->nickname) == member_name)
		{
			// указатель переводится на NULL;
			SE_p->Members[i] = NULL;
			// удаляется из списка членов подчата
			SE_p->Members.erase(SE_p->Members.begin() + i);
			return;
		}
	}
}
// отправляет предложение пользователям
// членам подчата и
// возвращает - согласны/не согласны ли они
bool sendSentenceToAllSubchatMembersAndGetBoolAnswers(SubchatElement *SE_p, std::string sentence)
{
	char buff[10];
	// счетчик согласий
	// вначале мы забиваем счетчик, затем уменьшаем.
	int counter_yes = SE_p->Members.size();
	sentence = "SENTENCE_FROM_ANOTHER:" + sentence + " [y or n]";
	for(int i = 0; i < SE_p->Members.size(); i++)
	{
		send(*(SE_p->Members[i]->socket), sentence.c_str(), sentence.length() * sizeof(char), 0);
		recv(*(SE_p->Members[i]->socket), &buff[0], sizeof(buff), 0);
		if (buff == "y" || buff == "Y")
		{
			counter_yes--;
		}
	}
	// если значение счетчика == 0
	// значит - все согласились
	if (counter_yes == 0)
		return true;
	else
		return false;
		
}
// отправляет предложение пользователю и
// возвращает - согласен/не согласен ли он
bool sendSentenceToUserAndGetBoolAnswer(SOCKET *socket, std::string sentence)
{
	char buff[1024];
	int bytes_recv;
	sentence = "SENTENCE_FROM_ANOTHER:" + sentence + " [y or n]";
	send(*socket, sentence.c_str(), strlen(sentence.c_str()) * sizeof(char), 0);
	bytes_recv = recv(*socket, &buff[0], sizeof(buff), 0);
	if (buff[0] == 'y' || buff[0] =='Y')
		return true;
	else
		return false;
}
// функция возвращает подчат, если в нем находится данный пользователь
// или NULL - если пользователя нет ни в одном подчате!
SubchatElement *getSubchatElementByName(std::string subchat_name, std::vector <SubchatElement*> &Subchats)
{
	for (int i = 0; i < Subchats.size(); i++)
	{
		if(subchat_name == (Subchats[i]->Name))
		{
			return Subchats[i];
		}
	}
	return NULL;
}

SubchatElement *getSubchatElementByMemberName(std::string member_name, std::vector <SubchatElement*> &Subchats)
{
	for (int i = 0; i < Subchats.size(); i++)
	{
		for (int j = 0; j < Subchats[i]->Members.size(); j++)
		{
			if(member_name == *(Subchats[i]->Members[j]->nickname))
			{
				return Subchats[i];
			}
		}		
	}
	return NULL;
}
// фукнкция присылает данному пользователю список всех имеющихся подчатов
// и пользователей
void sendListOfUsersAndSubchats(Client **ListHead, Client **ListEnd, std::vector <SubchatElement*> &Subchats, SOCKET socket)
{
	std::string localbuff;
	Client *cur_elem = *ListHead;
	// собираем всю инфу
	// для отправки юзеру!
	localbuff += "Users:\n";
	// пишем просто пользователей!
	while (cur_elem != NULL) {
		localbuff += *(cur_elem->nickname) + ", ";
		cur_elem = cur_elem->next;
	}		
	localbuff += "\nSubchats:\n";
	// пишем названия и участников подчатов
	for (int i = 0; i < Subchats.size(); i++)
	{
		localbuff += "[" + Subchats[i]->Name  + "]: ";
		for (int j = 0; j < Subchats[i]->Members.size(); j++)
		{
			localbuff += *(Subchats[i]->Members[j]->nickname) + ", ";
		}
		localbuff += "\n";
	}
	// отправляем полученную информуцию пользователю, который запросил всю информацию!
	send(socket, localbuff.c_str(), strlen(localbuff.c_str()) * sizeof(char), 0);
		
}
//  удаляет клиента по его сокету
void removeListElementBySocket(Client **ListHead, Client **ListEnd, SOCKET *socket)
{
	//Временный объект для хранения указателя,
	//чтобы потом очистить
	Client *cur_elem = *ListHead; // начинаем проход с головы
	Client *timeel = NULL; // 
	
	if (*ListHead == *ListEnd) // если текущий элемент последний в списке
	{
		if (cur_elem->socket == socket)
		{
			closesocket(*(cur_elem->socket)); // у данного закрываем сокет
			cur_elem->socket = NULL; // устанавливаем указатель сокета у следующего элемента на NULL
			delete cur_elem->nickname;
			delete cur_elem;
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

			if(cur_elem->next == *ListEnd && cur_elem->next->socket == socket) 
				// если у текущего элемента и заданный сокеты не сошлись
				// если след элемент - последний в списке и сокет у него совпадает с заданным
			{
				closesocket(*(cur_elem->next->socket)); // у данного закрываем сокет
				cur_elem->next->socket = NULL; // устанавливаем указатель сокета у следующего элемента на NULL
				delete cur_elem->next->nickname;
				cur_elem->next->nickname = NULL;
				delete cur_elem->next;
				cur_elem->next = NULL;
				*ListEnd = cur_elem; // переустанавливаем последний элемент!
				return;
			} else 	if(cur_elem->socket == socket) // если у текущего элемента сокет совпадает с заданным
			{
				closesocket(*(cur_elem->socket)); // у данного закрываем сокет
				cur_elem->socket = cur_elem->next->socket; //
				cur_elem->next->socket = NULL; // устанавливаем указатель сокета у следующего элемента на NULL
				delete cur_elem->nickname;
				// переставляем никнейм
				cur_elem->nickname = cur_elem->next->nickname;
				timeel = cur_elem->next; // копируем указатель на след элемент в timeel для последующего удаления
				cur_elem->next = cur_elem->next->next; // меняем указатель на след на след->след
				timeel->next = NULL; // устанавливем указатель у бывшего next у следующего элемента на NULL
				delete timeel;
				return;
			} 
		cur_elem = cur_elem->next; // переход к следующему элементу в бесконечном цикле
	}
}
// создает пользователя и возвращает его
Client *allocAndInitElement(SOCKET *socket, const char *recv_nick)
{
	Client *el = new Client(socket, recv_nick);
	return el;
}
// вставляет клиента в конце списка
Client *insertElementTailList(Client **list_head, Client **list_end, Client *ins_elem)
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
	return ins_elem;
}
// рассылает сообщение вообще всем клиентам из списка
void sendToAllClients(char *buff, int bytes_recv, Client **ListHead, Client **ListEnd)
{
	Client *cur_el;
	cur_el = *ListHead;
	while (cur_el != NULL)
	{
		//пишем!
		printf("\n1 send");
		send(*(cur_el->socket), &buff[0], bytes_recv, 0);
		cur_el = cur_el->next;
	}
}
// инициализация клиента - когда он 
// он присоединился к серверу!
Client *clientInit(SOCKET *socket, const char *nick)
{
	return insertElementTailList(&ListHead, &ListEnd, allocAndInitElement(socket, nick));
}
