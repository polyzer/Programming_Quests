#include <iostream>
#include <string>
#include <cstdlib>
#include <winsock2.h>
#include <windows.h>
#include <random>
#include <ctime>
#include <clocale>
#include <algorithm>


#define PORT 666
#define SERVERADDR "127.0.0.1"

struct Client
{
	// наша степень
	unsigned long long a;
	// P - простое число
	unsigned long long P;
	// g - порождающий элемент поля
	unsigned long long g;
	// это ключ. Который = (B^a)mod P
	unsigned long long Key;
	// это тот элемент который будем генерить и отправлять
	// чтобы другой юзер смог сгенерить свой ключ!
	unsigned long long B;
	
	// флаг показывает, находится ли пользователь в
	// шифрованном чате!
	bool crypto_flag;
	// имя клиента
	char Name[20];
	// функция инициализации
	void Init(SOCKET sock);
	Client() {
		 this->crypto_flag = false;
		 this->B = 0;
		 this->P = 0;
		 this->g = 0;
		 this->Key = 0;
	}

	unsigned long long get_g(){return this->g;}
	void set_g(unsigned long long s_g) {this->g = s_g;}
	unsigned long long get_P(){return this->P;}
	void set_P(unsigned long long s_P){this->P = s_P;}
	unsigned long long get_B()
	{
		if (this->B == 0)
		{
			this->gen_B();
			return this->B;
		}else
			return this->B;
	}

	bool compareRecievedKeyAndMyKey(unsigned long long B)
	{
		unsigned long long s_K = (unsigned long long) pow(B, this->a) % this->P;
		if (s_K == this->get_Key())
			return true;
		else
			return false;
	}
	// генерируем нашу степень
	void gen_a() 
	{
		if (this->P != 0)
		{
			// генерим степень от 2 до P-2
			this->a = (rand() % (P-4)) + 2;
		}
	}
	// генерируем наш B
	void gen_B()
	{
		if (this->g != 0 && this->P != 0 && this->a != 0)
			this->B = (unsigned long long) pow(g, this->a) % (this->P);
	}
	void gen_Key(unsigned long long B)
	{
		this->Key = (unsigned long long) pow(B, this->a) % (this->P);
	}
	// функция отправляет число B которое у нее запрашивают!
	void sendMyB(SOCKET sock, std::string status)
	{
		std::string send_str;
		// сначала скипаем прослушивание сокета нашим собственным
		// серверным потоком ЕСЛИ это НУЖНО.
		if(status == "SKIP")
			send(sock, "[off]", strlen("[off]") * sizeof(char) , 0);
		// а тепень отправляем наш B
		send_str = "[MY_B]:" + std::to_string(this->B);
		send(sock, send_str.c_str(), send_str.length() * sizeof(char), 0);
	}
	void sendBInMyA(SOCKET sock, unsigned long long s_B, std::string status)
	{
		std::string send_str;
		// сначала скипаем прослушивание сокета нашим собственным
		// серверным потоком ЕСЛИ это НУЖНО.
		if(status == "SKIP")
			send(sock, "[off]", strlen("[off]") * sizeof(char) , 0);
		// а тепень отправляем сгенерированный B
		send_str = "[MY_B]:" + std::to_string((unsigned long long) pow(s_B, this->a) % (this->P));
		send(sock, send_str.c_str(), send_str.length() * sizeof(char), 0);
	}

private:
	unsigned long long get_Key() {return this->Key;}
};

int recvOver = 0;
int sendOver = 0;
DWORD WINAPI recvProcess(LPVOID param_sock);
DWORD WINAPI sendProcess(LPVOID param_sock);
std::string SENTENCE_ANSWERING = "[NO]";
bool GET_B_IN_MY_A_REQUEST = false;
Client *MyClient = new Client();


int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "Russian");
	char buff[20*1024];
	int bytes_recv;
	printf("TCP DEMO CLIENT \n");

	if (WSAStartup(0x202, (WSADATA * )&buff[0]))
	{
		printf("WSAStart error %d\n", WSAGetLastError());
		return -1;
	}
	SOCKET my_sock;
	my_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (my_sock < 0)
	{
		printf("Socket() error %d\n", WSAGetLastError());
		return -1;
	}

	sockaddr_in dest_addr;
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(PORT);
	HOSTENT *hst;

	if(inet_addr(SERVERADDR) != INADDR_NONE)
		dest_addr.sin_addr.s_addr = inet_addr(SERVERADDR);
	else
		if(hst = gethostbyname(SERVERADDR))
			((unsigned long *)&dest_addr.sin_addr)[0] =
				((unsigned long **)hst->h_addr_list)[0][0];
		else
		{
			printf("Invalid address %s\n", SERVERADDR);
			closesocket(my_sock);
			WSACleanup();
			return -1;
		}
		if(connect(my_sock, (sockaddr *)&dest_addr, sizeof(dest_addr)))
		{
			printf("Connect error %d\n", WSAGetLastError());
			return -1;
		}

		printf("Соединение с %s успешно установлено\n\
			   Type quit for quit \n\n", SERVERADDR);

		bytes_recv = recv(my_sock, &buff[0], sizeof(buff), 0);
		buff[bytes_recv] = 0;
		printf("%s \n", buff);
		
		MyClient->Init(my_sock);
		
		DWORD thID1;
		DWORD thID2;
		CreateThread(NULL, NULL, sendProcess, &my_sock, NULL, &thID1);
		CreateThread(NULL, NULL, recvProcess, &my_sock, NULL, &thID2);
		while (true)
		{
			if(sendOver && recvOver)
			{
				printf("Program is closed");
				break;
			}
			Sleep(1000);
		}
		closesocket(my_sock);
		
		WSACleanup();
		return 0;
}


DWORD WINAPI recvProcess(LPVOID param_sock)
{
	SOCKET my_sock;
	my_sock = ((SOCKET *) param_sock)[0];
	char buff[1024];
	std::string buffstr;
	int nsize;
	while((nsize = recv(my_sock, &buff[0], sizeof(buff)-1,0))!= SOCKET_ERROR)
	{
		if (!sendOver)
		{
			buff[nsize] = 0;
			if(!strncmp(buff, "[SKIP][GET_YOUR_B]", strlen("[SKIP][GET_YOUR_B]")))
			{ 
				MyClient->sendMyB(my_sock, "SKIP");
			} else if(!strncmp(buff, "[NO_SKIP][GET_YOUR_B]", strlen("[NO_SKIP][GET_YOUR_B]")))
			{ 
				MyClient->sendMyB(my_sock, "NO_SKIP");
			} else if(!strncmp(buff, "[SKIP][GET_B_IN_MY_A_REQUEST]:", strlen("[SKIP][GET_B_IN_MY_A_REQUEST]:")))
			{ 
				buffstr = buff;
				buffstr = buffstr.substr(strlen("[SKIP][GET_B_IN_MY_A_REQUEST]:"));
				unsigned long long B = std::stoull(buffstr);
				MyClient->sendBInMyA(my_sock, B, "SKIP");
			} else if(!strncmp(buff, "[NO_SKIP][GET_B_IN_MY_A_REQUEST]:", strlen("[NO_SKIP][GET_B_IN_MY_A_REQUEST]:")))
			{ 
				buffstr = buff;
				buffstr = buffstr.substr(strlen("[NO_SKIP][GET_B_IN_MY_A_REQUEST]:"));
				unsigned long long B = std::stoull(buffstr);
				MyClient->sendBInMyA(my_sock, B, "NO_SKIP");
			} else if(!strncmp(buff, "[IN_CRYPTOCHAT]", strlen("[IN_CRYPTOCHAT]")))
			{ 
				MyClient->crypto_flag = true;
			} else if(!strncmp(buff, "[OUT_CRYPTOCHAT]", strlen("[OUT_CRYPTOCHAT]")))
			{ 
				MyClient->crypto_flag = false;
			} else if(!strncmp(buff, "[SENTENCE_FROM_ME]:", strlen("[SENTENCE_FROM_ME]:")))
			{
				SENTENCE_ANSWERING = "[SENTENCE_FROM_ME]";
				buffstr = buff;
				buffstr = buffstr.substr(strlen("[SENTENCE_FROM_ME]:"));
				std::cout << buffstr << std::endl;
			} else if(!strncmp(buff, "[SENTENCE_FROM_ANOTHER]:", strlen("[SENTENCE_FROM_ANOTHER]:")))
			{
				SENTENCE_ANSWERING = "[SENTENCE_FROM_ANOTHER]";
				buffstr = buff;
				buffstr = buffstr.substr(strlen("[SENTENCE_FROM_ANOTHER]:"));
				std::cout << buffstr << std::endl;
			} else if(!strncmp(buff, "[B_TO_KEYGEN]:", strlen("[B_TO_KEYGEN]:")))
			{
				buffstr = buff;
				buffstr = buffstr.substr(strlen("[B_TO_KEYGEN]:"));
				
				unsigned long long B = std::stoull(buffstr);
				MyClient->gen_Key(B);
			}
				// присылается сообщение вида:
				// [B_TO_COMPARE]:число_для_возведения_в_степень[MESSAGE]:шифротекст (на самом деле простой текст :=)
			 else if(!strncmp(buff, "[B_TO_COMPARE]:", strlen("[B_TO_COMPARE]:")))
			{
				buffstr = buff;
				std::size_t find_pos =  buffstr.find("[MESSAGE]:");
				// вырезаем сообщение
				std::string msg = buffstr.substr(find_pos + strlen("[MESSAGE]:"));
				/// вырезаем число
				buffstr = buffstr.substr(strlen("[B_TO_COMPARE]:"), find_pos);
				unsigned long long B = std::stoull(buffstr);
				// здесь у нас хранится сообщение!
				// если у нас все совпало, и сооб
				if(MyClient->compareRecievedKeyAndMyKey(B))
				{
					std::cout << msg << std::endl;
					/////////////////////////////////////////////////
				}else
				// если не совпало ничего!
				{
					std::cout << "Пришло сообщение из недоверенного источника!" << std::endl; 
				}
			// если пришло сообщение вида:
			// [SET_G]:g_number[SET_P]:P_number
			// установка g и P
			} else if(!strncmp(buff, "[SET_G]:", strlen("[SET_G]:")))
			{
				buffstr = buff;
				// нашли часть, где начинается P
				std::size_t find_pos = buffstr.find("[SET_P]:");
				// вырезал P и оставили в буфере для преобразования!
				buffstr = buffstr.substr(find_pos + strlen("[SET_P]:"));				
				unsigned long long P = std::stoull(buffstr);
				MyClient->set_P(P);
				buffstr = buff;
				buffstr = buffstr.substr(strlen("[SET_G]:"), find_pos);				
				unsigned long long g = std::stoull(buffstr);
				MyClient->set_g(g);
				// так как мы установили g и P
				// то можем генерить степень!
				MyClient->gen_a();
			}else
			{
				buffstr = buff;
				std::cout << buffstr << std::endl;
			}
		} else
		{
			recvOver = 1;
			ExitProcess(0);
		}
	}
//	printf ("Recv error %d\n", WSAGetLastError());
//	closesocket(my_sock);
//	WSACleanup();
}

// чтобы начать анонимный чат:
// ввести WANNA_USERCHAT
// сервер пришлет
// список доступных пользователей
// когда человек выберет юзера
// начинается анончат.


DWORD WINAPI sendProcess(LPVOID param_sock)
{
	SOCKET my_sock;
	my_sock = ((SOCKET *) param_sock)[0];

	std::string buffstr;
	char buff[2*1024];
	char buff1[2*1024];

	while (true)
	{
		fgets(&buff[0], sizeof(buff)-1, stdin);
		
		// если пользователь захотел установить шифрованный
		// чат
		if (!strncmp(buff, "WANNA_SUBCHAT:", strlen("WANNA_SUBCHAT:"))||
			!strncmp(buff, "WANNA_TO_SUBCHAT:", strlen("WANNA_TO_SUBCHAT:"))||
			!strncmp(buff, "WANNA_LIST", strlen("WANNA_LIST"))||
			!strncmp(buff, "WANNA_TO_QUIT_SUBCHAT", strlen("WANNA_TO_QUIT_SUBCHAT")))
		{
			strcpy(buff1, buff);
		} else if(!strcmp(&buff[0], "quit\n"))
		{
			printf("Exit...");
			sendOver = 1;// закрываем!
			ExitProcess(0);
		} else if (SENTENCE_ANSWERING == "[SENTENCE_FROM_ANOTHER]")// если отвечаем на запрос!
		{
			SENTENCE_ANSWERING = "[NO]";
			// скипаем прослушивание канала от нашего потока,
			// чтобы отправить данные в спрашивающий!
			send(my_sock, "[off]", strlen("[off]") * sizeof(char) , 0);
			strcpy(buff1, buff);
		} else if (SENTENCE_ANSWERING == "[SENTENCE_FROM_ME]")// если отвечаем на запрос!
		{
			SENTENCE_ANSWERING = "[NO]";
			// ничего не скипаем!
			strcpy(buff1, buff);
		}else if (MyClient->crypto_flag)// если мы в чате!
		{
			////////////////////////////
			//////////////////////////
			///////////////////////////
			//////////////////////////
			// СОЗДАЕМ СООБЩЕНИЕ И ПРИПИСЫВАЕМ _B_
		} else
		{
			strcpy(buff1, "[");
			strcat(buff1, MyClient->Name);
			strcat(buff1, "] ");
			strcat(buff1, buff);
		}
		// отправка сообщения
		// в if-ах что выше
		// мы просто создаем это сообщение, которое будет отправлено
		send(my_sock, &buff1[0], strlen(buff1)*sizeof(char), 0);
	}
}

void Client::Init(SOCKET sock)
{
	int bytes_recv;
	char localbuff[1024];
		// ввод данных в строку!
	while (true)
	{
		printf("Введите nickname: \n");
		gets(this->Name);
		send(sock, &(this->Name)[0], strlen((this->Name))*sizeof(char), 0);	
		bytes_recv = recv(sock, &localbuff[0], sizeof(localbuff), 0);
		localbuff[bytes_recv] = 0;
		if(strncmp(localbuff, "[NICKNAME_OK]", strlen("[NICKNAME_OK]")))
		{
			printf("%s", localbuff);
		}
		else
		{
			printf("Можете общаться! \n");
		}
	}

}
