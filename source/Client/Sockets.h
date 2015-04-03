#ifndef _SOCKETS_H_
#define _SOCKETS_H_

#include <wS2tcpip.h>
#include <stdio.h>
#include "../Buffer/MessageQueue.h"

#pragma warning(disable:4996)
#pragma comment(lib,"ws2_32.lib")

#define GETLENGTH	4
#define DATA_BUFSIZE 8192

typedef struct _SOCKET_INFORMATION {
	OVERLAPPED Overlapped;
	SOCKET Socket;
	CHAR Buffer[DATA_BUFSIZE];
	WSABUF DataBuf;
	MessageQueue* mqueue;
} SOCKET_INFORMATION, *LPSOCKET_INFORMATION;

class UDPSocket
{
private:
	SOCKET sd;
	MessageQueue* msgqueue;
	HANDLE mutex;
	ip_mreq_source mreq;

public:
	UDPSocket(int port, MessageQueue* mqueue);
	~UDPSocket();
	int Send(void* data, int length, char* dest_ip, int dest_port);
	static DWORD WINAPI UDPThread(LPVOID lpParameter);
	DWORD ThreadStart(void);
	static void CALLBACK UDPRoutine(DWORD Error, DWORD BytesTransferred,
		LPWSAOVERLAPPED Overlapped, DWORD InFlags);

};

class TCPSocket
{
private:
	SOCKET sd;
	MessageQueue* msgqueue;
	HANDLE mutex;

public:
	TCPSocket(char* host, int port, MessageQueue* mqueue);
	~TCPSocket();
	int Send(void* data, int lenght);
	static DWORD WINAPI TCPThread(LPVOID lpParameter);
	DWORD ThreadStart(void);
	static void CALLBACK TCPRoutine(DWORD Error, DWORD BytesTransferred,
		LPWSAOVERLAPPED Overlapped, DWORD InFlags);

};

#endif
