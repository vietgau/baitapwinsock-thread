// client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>

#pragma comment (lib, "Ws2_32.lib")
using namespace std; 

int main()
{
    WSADATA wsaData;
    int wsaStartUp;

    wsaStartUp = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (wsaStartUp != 0) {
        cout << "WSAStartup khong thanh cong" << endl;
    }
    else
    {
        cout << "WSAStartup thanh cong \n" << endl;
    }

    SOCKET clientSocket;

    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (clientSocket == INVALID_SOCKET) {
        cout << "tao socket khong thanh cong" << endl;
    }
    else
    {
        cout << "tao socket thanh cong " << endl;
    }
   
    struct sockaddr_in ServerAdd;
    string ipAddress = "127.0.0.1";			// IP Address of the server
    ServerAdd.sin_family = AF_INET;
    ServerAdd.sin_port = htons(54000);
    inet_pton(AF_INET, ipAddress.c_str(), &ServerAdd.sin_addr);

    int iConnect;
    iConnect = connect(clientSocket, (SOCKADDR*)&ServerAdd, sizeof(ServerAdd));


    char* recvBuff = NULL;
    DWORD bufferSize = 0;

    while (true)
    {
        int iSend;
        string senBuff;
        cout << "nhap cmd:" << endl;
        getline(cin, senBuff);

        int iSenderBuffer = senBuff.length();
        iSend = send(clientSocket, senBuff.c_str(), iSenderBuffer, 0);
        int iRecv;
        iRecv = recv(clientSocket, (char*)&bufferSize, sizeof(bufferSize), 0);
        cout << bufferSize << endl;
        recvBuff = (char*)calloc(bufferSize, sizeof(char));
        iRecv = recv(clientSocket, recvBuff, bufferSize , 0);
        if (iRecv > 0)
        {
            recvBuff[iRecv] = '\0';
            cout << recvBuff << endl;
        }

        else if (iRecv < 0)
            break;
    }
    
   /* while (true)
    {
       

        int iRecv;
        iRecv = recv(clientSocket, (char*)&bufferSize, sizeof(bufferSize), 0);
        recvBuff = (char*)calloc(bufferSize + 1, sizeof(char));
        iRecv = recv(clientSocket, recvBuff, sizeof(recvBuff), 0);
        if (iRecv > 0)
        {
            recvBuff[iRecv] = '\0';
            cout << recvBuff << endl;
        }

        else if (iRecv < 0)
            break;
    }*/
}
