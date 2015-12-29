#include <iostream>
#include <string>
#include <winsock2.h>
#include <windows.h>
#include <locale.h>

#define PORT 666
#define SERVERADDR "127.0.0.1"

int recvOver = 0;
int sendOver = 0;
char *UserName;
DWORD WINAPI recvProcess(LPVOID param_sock);
DWORD WINAPI sendProcess(LPVOID param_sock);
char *userInit(SOCKET sock);
std::string SENTENCE_ANSWERING = "NO";

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

		UserName = userInit(my_sock);
		
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
			if(!strncmp(buff, "SENTENCE_FROM_ME:", strlen("SENTENCE_FROM_ME:")))
			{
				SENTENCE_ANSWERING = "SENTENCE_FROM_ME";
				buff[nsize] = 0;
				buffstr = buff;
				buffstr = buffstr.substr(strlen("SENTENCE_FROM_ME:"));
				std::cout << buffstr << std::endl;
			} else
			if(!strncmp(buff, "SENTENCE_FROM_ANOTHER:", strlen("SENTENCE_FROM_ANOTHER:")))
			{
				SENTENCE_ANSWERING = "SENTENCE_FROM_ANOTHER";
				buff[nsize] = 0;
				buffstr = buff;
				buffstr = buffstr.substr(strlen("SENTENCE_FROM_ANOTHER:"));
				std::cout << buffstr << std::endl;
			} else
			{
				buff[nsize] = 0;
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
		if (!strncmp(buff, "WANNA_USERCHAT:", strlen("WANNA_USERCHAT:"))||
			!strncmp(buff, "WANNA_TO_CHAT:", strlen("WANNA_TO_CHAT:"))||
			!strncmp(buff, "WANNA_LIST", strlen("WANNA_LIST"))||
			!strncmp(buff, "WANNA_TO_QUIT_SUBCHAT", strlen("WANNA_TO_QUIT_SUBCHAT")))
		{
			strcpy(buff1, buff);
		} else if(!strcmp(&buff[0], "quit\n")) // вот тут была ;!!!
		{
			printf("Exit...");
			sendOver = 1;// закрываем!
			ExitProcess(0);
		} else if (SENTENCE_ANSWERING == "SENTENCE_FROM_ANOTHER")// если отвечаем на предложение!
		{
			SENTENCE_ANSWERING = "NO";
			// скипаем прослушивание канала от нашего потока,
			// чтобы отправить данные в спрашивающий!
			send(my_sock, "[off]", strlen("[off]") * sizeof(char) , 0);
			strcpy(buff1, buff);
		} else if (SENTENCE_ANSWERING == "SENTENCE_FROM_ME")// если отвечаем на предложение!
		{
			SENTENCE_ANSWERING = "NO";
			// ничего не скипаем!
			strcpy(buff1, buff);
		} else
		{
			strcpy(buff1, "[");
			strcat(buff1, UserName);
			strcat(buff1, "] ");
			strcat(buff1, buff);
		}
		send(my_sock, &buff1[0], strlen(buff1)*sizeof(char), 0);
	}
}

char *userInit(SOCKET sock)
{
	int bytes_recv;
	char localbuff[1024];
	char name[1024];
		// ввод данных в строку!
	while (true)
	{
		printf("Введите nickname: \n");
		gets(name);
		send(sock, &name[0], strlen(name)*sizeof(char), 0);	
		bytes_recv = recv(sock, &localbuff[0], sizeof(localbuff), 0);
		localbuff[bytes_recv] = 0;
		if(strncmp(localbuff, "[NICKNAME_OK]", strlen("[NICKNAME_OK]")))
		{
			printf("%s", localbuff);
		}
		else
		{
			printf("Можете общаться! \n");
			return name;
		}
	}

}
