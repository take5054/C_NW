#include "IP.hpp"
#include <iphlpapi.h>
#include <iostream>
#pragma comment(lib, "iphlpapi.lib")

CIP::CIP()
	: m_upSocket(INVALID_SOCKET), m_SAddress()
{
	WSADATA wsaData;
	// WinSockの初期化
	WSACleanup();
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		throw std::runtime_error("WSAStartup failed");	// WinSockの初期化に失敗
}

CIP::~CIP()
{
	closesocket(m_upSocket);	// ソケットのクローズ
	WSACleanup();				// WinSockの終了
}
