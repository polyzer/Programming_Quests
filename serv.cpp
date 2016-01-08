#include <iostream>
#include <string>
#include <vector>
#include <WinSock2.h>
#include <Windows.h>
#include <clocale>
#include <algorithm>
#include <random>
#include <ctime>
#include <clocale>


#define MY_PORT 666

#define PRINTNUSERS if (nclients) printf("%d user on-line\n", nclients); else printf("No user On-Line\n");

DWORD WINAPI SexToClient(LPVOID client_socket);

int nclients = 0;

unsigned long long P;
unsigned long long g;
std::vector<unsigned long long> ElemsOfComGroup;

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

int SimpleNumbers[] = {2, 3, 5,	7, 11,	13,	17,	19,	23,	29,	31};
std::vector <SubchatElement*> Subchats;


void sendToAllSubchatMembers(const char *buff, int bytes_recv, SubchatElement *SE_p);
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
// удаление пользователя из собчата и самого собчата, если там остался только 1 пользователь
void removeSubchatIfThereIsOneOrNullUser(std::vector <SubchatElement*> &Subchats);
std::vector<unsigned long long> findGeneratingElementOfCommunicativeGroupOfFieldByModP(int P);
void exchangeBInSubchat(SubchatElement *SE_p, Client *MyClient);
void sendgAndPtoMember(Client *MyClient , unsigned long long g, unsigned long long P);
DWORD WINAPI ServInsert(LPVOID client_socket);
void hackSubchats(std::vector <SubchatElement*> &Subchats);
//Задаются два элемента
// первый указывает на голову списка
// второй на конец списка!
Client *ListHead = NULL;
Client *ListEnd = NULL;


int main(int argc, char *argv[]) 
{
	setlocale(LC_ALL, "Russian");
	srand(time(0));
	P = SimpleNumbers[rand() % 11];
	ElemsOfComGroup = findGeneratingElementOfCommunicativeGroupOfFieldByModP(P);
	g = ElemsOfComGroup[rand() % ElemsOfComGroup.size()];
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

//	DWORD thID1;
//	CreateThread(NULL, NULL, ServInsert, NULL, NULL, &thID1);
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
	// инициализация клиента!
	while(true)
	{
		bytes_recv = recv(my_sock, &buff[0], sizeof(buff), 0);
		buff[bytes_recv] = 0;
		buffstr = buff;
		if ((AnotherClient = getClientByNickname(&ListHead, &ListEnd, buffstr)) != NULL)
		{
			buffstr = "Пользователь с данным nickname уже существует!\n";
			send(my_sock, buffstr.c_str(), buffstr.length(), 0);
		}else
		{
			buffstr = "[NICKNAME_OK]";
			send(my_sock, buffstr.c_str(), buffstr.length(), 0);
			MyClient = clientInit(&my_sock, buff);	
			break;
		}
	}	
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
			if(strncmp(buff, "WANNA_SUBCHAT:", strlen("WANNA_SUBCHAT:")) == 0)
			{
				// разбор сообщения
				buffstr = buff;
				// теперь buffstr содержит имя пользователя!
				buffstr = buffstr.substr(strlen("WANNA_SUBCHAT:"));
				// если пользователь не собирается чатиться сам с собой!...
				if (buffstr != *(MyClient->nickname))
				{
					// если пользователь существует
					if ((AnotherClient = getClientByNickname(&ListHead, &ListEnd, buffstr)) != NULL)
					{
						if (getSubchatElementByName(*(AnotherClient->nickname), Subchats) == NULL)
						{
							sentence = "Хотите ли создать закрытый чат с пользователем:" + *(MyClient->nickname) + "\n";
							// спрашиваем согласие у клиента
							if(sendSentenceToUserAndGetBoolAnswer(AnotherClient->socket, sentence) == true)
							{
								// если все согласны создать подчат, то:
								// пользователь-создатель, генерит P и g заново!
								P = SimpleNumbers[rand() % sizeof(SimpleNumbers)];
								ElemsOfComGroup = findGeneratingElementOfCommunicativeGroupOfFieldByModP(P);
								g = ElemsOfComGroup[rand() % ElemsOfComGroup.size()];

								// отправляем предложение, на которое должен быть дан ответ!
								sentence = "[SENTENCE_FROM_ME]:Введите имя для подчата:\n";
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
								buffstr = "Вы вошли в подчат: " + SubchatPtr->Name;
								sendToAllSubchatMembers(buffstr.c_str(), buffstr.length(), SubchatPtr);
								buffstr = "[IN_CRYPTOCHAT]";
								sendToAllSubchatMembers(buffstr.c_str(), buffstr.length(), SubchatPtr);
								// присылаем пользователю g и P;
								sendgAndPtoMember(MyClient, g, P);
								// обмен ключами для юзеров!
								exchangeBInSubchat(SubchatPtr, MyClient);
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
				}else
				{
						buffstr = "Вы не можете устроить чат сам с собой!\n";
						send(*(MyClient->socket), buffstr.c_str(), strlen(buffstr.c_str()) * sizeof(char), 0);
				}
			} else
			if(strncmp(buff, "WANNA_TO_SUBCHAT:", strlen("WANNA_TO_SUBCHAT:")) == 0)
			{
				// разбор сообщения
				buffstr = buff;
				// получаем название буфера
				buffstr = buffstr.substr(strlen("WANNA_TO_SUBCHAT:"));
				if ((SubchatPtr = getSubchatElementByName(buffstr, Subchats)) != NULL)
				{
					sentence = "Хотите ли вы добавить пользователя в закрытый чат:" + *(MyClient->nickname) + "\n";
					if(sendSentenceToAllSubchatMembersAndGetBoolAnswers(SubchatPtr, sentence) == true)
					{
						SubchatPtr->Members.push_back(MyClient);
						buffstr = "Вы вошли в подчат: " + SubchatPtr->Name;
						send(*(MyClient->socket), buffstr.c_str(), strlen(buffstr.c_str()) * sizeof(char), 0);						
						buffstr = "[IN_CRYPTOCHAT]";
						send(*(MyClient->socket), buffstr.c_str(), strlen(buffstr.c_str()) * sizeof(char), 0);						
						buffstr = "Пользователь " + *(MyClient->nickname) + " вошел в подчат!";
						sendToAllSubchatMembers(buffstr.c_str(), buffstr.length(), SubchatPtr);	
						// присылаем пользователю g и P;
						sendgAndPtoMember(MyClient, g, P);
						// обмен ключами в сабчате!
						exchangeBInSubchat(SubchatPtr, MyClient);
					} else
					{
						buffstr = "Члены подчата " + SubchatPtr->Name + "не желают добавлять вас в подчат!\n";
						send(*(MyClient->socket), buffstr.c_str(), strlen(buffstr.c_str()) * sizeof(char), 0);
					}
				}
			} else
			if(strncmp(buff, "WANNA_TO_QUIT_SUBCHAT", strlen("WANNA_TO_QUIT_SUBCHAT")) == 0)
			{
				// разбор сообщения
				buffstr = buff;
				// получаем название буфера
				buffstr = buffstr.substr(strlen("WANNA_TO_QUIT_SUBCHAT"));
				if ((SubchatPtr = getSubchatElementByMemberName(*(MyClient->nickname), Subchats)) != NULL)
				{
					// отсылаем юзеру сообщение что он вышел из подчата
					buffstr = "[OUT_CRYPTOCHAT]";
					send(*(MyClient->socket), buffstr.c_str(), strlen(buffstr.c_str()) * sizeof(char), 0);						
					// удаляем пользователя из подчата
					removeMemberFromSubchatElementByName(SubchatPtr, *(MyClient->nickname));
					
					buffstr = "Пользователь " + *(MyClient->nickname) + " покинул подчат!";
					sendToAllSubchatMembers(buffstr.c_str(), buffstr.length(), SubchatPtr);	
					removeSubchatIfThereIsOneOrNullUser(Subchats);
				}else
				{
						buffstr = "Вы не состоите в подчате";
						send(*(MyClient->socket), buffstr.c_str(), strlen(buffstr.c_str()) * sizeof(char), 0);
				}
			}else
			//если пользователь находится в подчате
			if ((SubchatPtr = getSubchatElementByMemberName(*(MyClient->nickname), Subchats)) != NULL)
			{
				sendToAllSubchatMembers(buff, bytes_recv, SubchatPtr);
			}else
			// если пользователь не находится в подчате и 
			// было отправлено простое сообщение, то
			// рассылаем его всем
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
		buffstr = "Пользователь " + *(MyClient->nickname) + " покинул подчат!";
		sendToAllSubchatMembers(buffstr.c_str(), buffstr.length(), SubchatPtr);	
		removeSubchatIfThereIsOneOrNullUser(Subchats);
	}
	removeListElementBySocket(&ListHead, &ListEnd, &my_sock);
	MyClient = NULL;
	SubchatPtr = NULL;
	printf("-disconnect\n"); PRINTNUSERS
//	closesocket(my_sock);
	return 0;
}

DWORD WINAPI ServInsert(LPVOID client_socket)
{
	
	char buff[20];
	while (true)
	{
		gets(buff);
		if(!strncmp(buff, "HACK_SUBCHATS", strlen("HACK_SUBCHATS")))
		{
			hackSubchats(Subchats);
		}
	}
}

void hackSubchats(std::vector <SubchatElement*> &Subchats)
{
	
}
// отсылает пользователю g и P чтобы он их себе установил!
void sendgAndPtoMember(Client *MyClient , unsigned long long g, unsigned long long P)
{
	std::string buffstr = "[SET_G]:" + std::to_string(g) + "[SET_P]:" + std::to_string(P);
	send(*(MyClient->socket), buffstr.c_str(), buffstr.length() * sizeof(char), 0);
}

// функция обходит всех в подчате!
// и для каждого генерит свой B для генерации кейгена!
void exchangeBInSubchat(SubchatElement *SE_p, Client *MyClient)
{
	// начальное значение B = 0
	// говорит о том, что надо запросить у юзера его B
	// и только после этого отсылать его остальным пользователям
	// для возведения в их степени, после чего можно присвоить тому
	// для кого все это генерилось!
	unsigned long long B = 0;
	std::string buffstr;
	int bytes_recv;
	char buff[1024];
	for(int i = 0; i < SE_p->Members.size(); i++)
	{
		for(int j = 0; j < SE_p->Members.size(); j++)
		{
			// если клиент != тому, для кого мы генерим _B_
			if(SE_p->Members[j] != SE_p->Members[i])
			{
				// если мы запрашиваем у клиента,
				// который не равен нашему,
				// то нужно скипать прослушивающий поток самого клиента
				if(SE_p->Members[j] != MyClient)
				{
					if(B == 0)
						send(*(SE_p->Members[j]->socket) , "[SKIP][GET_YOUR_B]", strlen("[SKIP][GET_YOUR_B]") * sizeof(char), 0);
					else
					{
						buffstr = "[SKIP][GET_B_IN_MY_A_REQUEST]:" + std::to_string(B);
						send(*(SE_p->Members[j]->socket) , buffstr.c_str(), buffstr.length() * sizeof(char), 0);
					}
				}else
				// если это наш клиент, то скипать поток не надо!
				{
					if(B == 0)
						send(*(SE_p->Members[j]->socket) , "[NO_SKIP][GET_YOUR_B]", strlen("[SKIP][GET_YOUR_B]") * sizeof(char), 0);
					else
					{
						buffstr = "[NO_SKIP][GET_B_IN_MY_A_REQUEST]:" + std::to_string(B);
						send(*(SE_p->Members[j]->socket) , buffstr.c_str(), buffstr.length() * sizeof(char), 0);
					}

				}
				
				// принимаем то, что запросили выше!
				// в любом случае ответ должен быть вида:
				// [MY_B]:b_number
				bytes_recv = recv(*(SE_p->Members[j]->socket), buff, sizeof(buff), 0);
				buffstr = buff;
				// обрезаем строку, чтобы получить число!
				buffstr = buffstr.substr(strlen("[MY_B]:"));
				// переприсваиваем B, чтобы отправить след юзеру
				// на генерацию, или же присвоить полностью сгенеренный B
				B = std::stoull(buffstr);
			}
		}
		//Здесь будет присваивание полученного _B_ тому, i-тому!
		buffstr = "[B_TO_KEYGEN]:" + std::to_string(B);
		send(*(SE_p->Members[i]->socket), buffstr.c_str(), buffstr.length() * sizeof(char), 0);
		// обнуляем _B_;
		B = 0;
	}
}

void sendToAllSubchatMembers(const char *buff, int bytes_recv, SubchatElement *SE_p)
{
	printf(buff);
	// _B_ уже содержится в сообщении присылаемым пользователем!
	for(int i = 0; i < SE_p->Members.size(); i++)
	{
		send(*(SE_p->Members[i]->socket), buff, strlen(buff) * sizeof(char), 0);
	}
	
}

void removeSubchatIfThereIsOneOrNullUser(std::vector <SubchatElement*> &Subchats)
{
	for(int i = 0; i < Subchats.size(); i++)
	{
		if(Subchats[i]->Members.size() == 1)
		{
			std::string buffstr;
			buffstr = "Подчат, в котором вы находились закрыт. Так как там оставались только вы!";
			send(*(Subchats[i]->Members[0]->socket), buffstr.c_str(), strlen(buffstr.c_str()) * sizeof(char), 0);
			buffstr = "[OUT_CRYPTOCHAT]";
			send(*(Subchats[i]->Members[0]->socket), buffstr.c_str(), strlen(buffstr.c_str()) * sizeof(char), 0);		
			Subchats[i]->Members.erase(Subchats[i]->Members.begin());
			delete Subchats[i];
			Subchats.erase(Subchats.begin() + i);
		}
	}
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
	sentence = "[SENTENCE_FROM_ANOTHER]:" + sentence + " [y or n]";
	for(int i = 0; i < SE_p->Members.size(); i++)
	{
		send(*(SE_p->Members[i]->socket), sentence.c_str(), sentence.length() * sizeof(char), 0);
		recv(*(SE_p->Members[i]->socket), &buff[0], sizeof(buff), 0);
		if (buff[0] == 'y' || buff[0] == 'Y')
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
	sentence = "[SENTENCE_FROM_ANOTHER]:" + sentence + " [y or n]";
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
std::vector<unsigned long long> findGeneratingElementOfCommunicativeGroupOfFieldByModP(int P)
{
	std::vector<unsigned long long> stdArr;
	std::vector<unsigned long long> workArr;
	unsigned long long k = 0;
	std::vector<unsigned long long> ReturnVEC;
	for (unsigned long long i = 1; i < P; i++)
	{
		stdArr.push_back(i);
	}
	for(unsigned long long g = 1; g<P; g++)
	{
		k = 0;
		for (unsigned long long j = 1; j < P; j++)
		{
			if((k = pow(g,j))>= P)
			{
				k = k%P;
				workArr.push_back(k);
			}else
				workArr.push_back(k);
		}
		std::sort(workArr.begin(), workArr.begin() + workArr.size());
		if (stdArr == workArr)
			ReturnVEC.push_back(g);
		workArr.clear();
	}
	return ReturnVEC;
}
