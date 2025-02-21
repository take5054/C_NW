#include "IP.hpp"
#include <iphlpapi.h>
#include <iostream>
#pragma comment(lib, "iphlpapi.lib")

CIP::CIP()
	: m_upSocket(INVALID_SOCKET), m_SAddress()
{
	WSADATA wsaData;
	// WinSock�̏�����
	WSACleanup();
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		throw std::runtime_error("WSAStartup failed");	// WinSock�̏������Ɏ��s
}

CIP::~CIP()
{
	closesocket(m_upSocket);	// �\�P�b�g�̃N���[�Y
	WSACleanup();				// WinSock�̏I��
}
