#include "TCP.hpp"
#include <string>
#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)	// inet_addr�֐��̌x��(�G���[)�𖳎�

constexpr int cx_nRetries = 100;	// ���g���C��

CTCP::CTCP(const std::string& address, int port)
	: CIP()
{
	m_upSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_upSocket == INVALID_SOCKET)
	{
		WSACleanup();
		throw std::runtime_error("Socket creation failed");
	}

	m_SAddress.sin_family = AF_INET;
	m_SAddress.sin_port = htons(port);
	m_SAddress.sin_addr.S_un.S_addr =
		address.empty() ? INADDR_ANY : inet_addr(address.c_str());
}

CTCP::~CTCP()
{

}

void CTCP::Bind()
{
	if (bind(m_upSocket, (sockaddr*)&m_SAddress, sizeof(m_SAddress)) == SOCKET_ERROR)
	{
		std::cerr << "bind failed: " << WSAGetLastError() << std::endl;
		CTCP::~CTCP();
		return;
	}

	if (listen(m_upSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cerr << "listen failed: " << WSAGetLastError() << std::endl;
		CTCP::~CTCP();
		return;
	}
}

void CTCP::Send(const std::string& word)
{
	// ���g���C�񐔕����M�����݂�
	for (int i = 0; i < cx_nRetries; ++i)
	{
		int nResult = send(m_upSocket, word.c_str(), static_cast<int>(word.size()), 0);
		if (nResult != SOCKET_ERROR) return;	// ���M����������return
		std::cerr << "send failed: " << WSAGetLastError() << std::endl;
	}
	std::cerr << "Send failed" << std::endl;
}

void CTCP::Send(const std::string& In_strMsg, sockaddr_in In_sAddress)
{
	// ���g���C�񐔕����M�����݂�
	for (int i = 0; i < cx_nRetries; ++i)
	{
		int nResult = sendto(m_upSocket, In_strMsg.c_str(), static_cast<int>(In_strMsg.size()), 0,
			(sockaddr*)&In_sAddress, sizeof(In_sAddress));
		if (nResult != SOCKET_ERROR) return;	// ���M����������return
		std::cerr << "send failed: " << WSAGetLastError() << std::endl;
	}
	std::cerr << "Send failed" << std::endl;
}

std::string CTCP::Recv()
{
	// �w�肳�ꂽ�A�h���X�ɐڑ�
	if (connect(m_upSocket, NULL, NULL) == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		std::cerr << "connect failed: " << error << " (" << gai_strerror(error) << ")" << std::endl;
		//closesocket(tempSocket);
		return "";
	}
	if (listen(m_upSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cerr << "listen failed: " << WSAGetLastError() << std::endl;
		return "";
	}

	// ���g���C�񐔕���M�����݂�
	for (int i = 0; i < cx_nRetries; ++i)
	{
		SOCKET clientSock = accept(m_upSocket, NULL, NULL);
		if (clientSock == INVALID_SOCKET)
		{
			// �R�l�N�V�����m�����s�����玟�̃��g���C��
			std::cerr << "accept failed: " << WSAGetLastError() << std::endl;
			continue;
		}

		char buf[cx_nMaxMsgSize];
		int result = recv(clientSock, buf, sizeof(buf), 0);
		if (result == SOCKET_ERROR)
		{
			// ��M���s�����玟�̃��g���C��
			std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
			continue;
		}

		closesocket(clientSock);
		return std::string(buf, result);
	}
	std::cerr << "Recive failed" << std::endl;
	return "";
}

CIP::Response CTCP::ReceiveAndRespond()
{
	int nCnt = 0;
	if (listen(m_upSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cerr << "listen failed: " << WSAGetLastError() << std::endl;
		return CIP::Response();
	}
	std::cout << "Waiting for connection..." << std::endl;

	// ���g���C�񐔕���M�����݂�
	sockaddr_in sClientAddress;
	SOCKET clientSock;
	int nAddrLen = sizeof(sClientAddress);
	while (true)
	{
		clientSock = accept(m_upSocket, (sockaddr*)&sClientAddress, &nAddrLen);
		if (clientSock == INVALID_SOCKET)
		{
			int error = WSAGetLastError();
			if (error == WSAEWOULDBLOCK || error == WSAECONNRESET)
			{
				std::cerr << "accept failed: " << error << std::endl;
				Sleep(1); // 1ms�ҋ@���Ă���Ď��s
				continue;
			}
			else
			{
				std::cerr << "accept failed with unexpected error: " << error << std::endl;
				return CIP::Response();
			}
		}
		else
		{
			std::cout << "accept done" << std::endl;
			break;
		}
	}

	char buf[cx_nMaxMsgSize];
	int result;
	for (nCnt = 0; nCnt < cx_nRetries; ++nCnt)
	{
		result = recv(clientSock, buf, sizeof(buf), 0);
		if (result == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			if (error == WSAEWOULDBLOCK || error == WSAECONNRESET)
			{
				std::cerr << "recv failed: " << error << std::endl;
				Sleep(1); // 1ms�ҋ@���Ă���Ď��s
				continue;
			}
			else
			{
				std::cerr << "recv failed with unexpected error: " << error << std::endl;
				closesocket(clientSock);
				return CIP::Response();
			}
		}
		else if (result == 0)
		{
			// �ڑ�������ꂽ�ꍇ
			std::cerr << "Connection closed by peer" << std::endl;
			closesocket(clientSock);
			std::cout << "Waiting for connection..." << std::endl;
			return ReceiveAndRespond(); // ���̐ڑ���҂�
		}
		else
		{
			// ��M����
			break;
		}
	}

	if (nCnt >= cx_nRetries)
	{
		std::cerr << "Receive failed" << std::endl;
		closesocket(clientSock);
		return CIP::Response();
	}

	closesocket(clientSock);
	Response sData;
	sData.IPaddress = sClientAddress;
	sData.strData = std::string(buf, result);
	std::cout << "Received data: " << sData.strData << std::endl;
	return sData;
}

std::string CTCP::SendAndReceive(const std::string& In_strServerAddress, int In_nServerPort, const std::string& In_strMsg)
{
	int nCnt = 0;
	// �\�P�b�g�쐬
	m_upSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_upSocket == INVALID_SOCKET)
	{
		std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
		return "";
	}

	// �R�l�N�V�����̊m��
	sockaddr_in SAddress;
	SAddress.sin_family = AF_INET;
	SAddress.sin_port = htons(In_nServerPort);
	SAddress.sin_addr.S_un.S_addr = inet_addr(In_strServerAddress.c_str());
	if (connect(m_upSocket, (sockaddr*)&SAddress, sizeof(SAddress)) == SOCKET_ERROR)
	{
		std::string str = "connect failed: " + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, str.c_str(), "Error", MB_OK);
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			std::cerr << "connect failed: " << WSAGetLastError() << std::endl;
			closesocket(m_upSocket);
			return "";
		}
	}

	// �\�P�b�g�̏�Ԃ��Ď�
	if (TimeoutCheck())
	{
		std::cerr << "connect timeout" << std::endl;
		closesocket(m_upSocket);
		return "";
	}

	// ���g���C�񐔕����M�����݂�
	for (; nCnt < cx_nRetries; ++nCnt)
	{
		OutputDebugStringA("send\n");
		int nSendResult = send(m_upSocket, In_strMsg.c_str(), static_cast<int>(In_strMsg.size()), 0);
		if (nSendResult != SOCKET_ERROR) break;    // ���M���������烋�[�v�𔲂���

		// ���M���s�����玟�̃��g���C��
		std::cerr << "send failed: " << WSAGetLastError() << std::endl;
	}

	// ���g���C�񐔕���M�����݂�
	char szRecvBuffer[cx_nMaxMsgSize];
	int nRecvResult = 0;
	for (; nCnt < cx_nRetries * 10000; ++nCnt)
	{
		OutputDebugStringA("recv\n");
		nRecvResult = recv(m_upSocket, szRecvBuffer, sizeof(szRecvBuffer), 0);
		if (nRecvResult != SOCKET_ERROR && nRecvResult > 0)
			break;    // ��M���������烋�[�v�𔲂���

		// ��M���s�����玟�̃��g���C��
		std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
	}

	if (nRecvResult <= 0)
	{
		std::cerr << "SendAndReceive failed" << std::endl;
		return "";
	}

	OutputDebugStringA("recv done\n");
	std::string str = std::string(szRecvBuffer, (nRecvResult > cx_nMaxMsgSize) ? cx_nMaxMsgSize : nRecvResult);
	OutputDebugStringA(str.c_str());
	return std::string(szRecvBuffer, (nRecvResult > cx_nMaxMsgSize) ? cx_nMaxMsgSize : nRecvResult);
}

std::string CTCP::SendAndReceive(const std::string& In_strMsg)
{
	return std::string();
}


bool CTCP::TimeoutCheck()
{
	// �\�P�b�g�̏�Ԃ��Ď�
	fd_set sReadfds;
	FD_ZERO(&sReadfds);
	FD_SET(m_upSocket, &sReadfds);
	// �������݉\���`�F�b�N
	fd_set writeSet;
	FD_ZERO(&writeSet);
	FD_SET(m_upSocket, &writeSet);

	timeval sTimeoutTime{ 5, 0 };	// �^�C���A�E�g�ɂȂ鎞��(5s)
	// �^�C���A�E�g�`�F�b�N
	int nResult = select(0, &sReadfds, &writeSet, NULL, &sTimeoutTime);

	// ----- �G���[�`�F�b�N -----
	if (nResult == SOCKET_ERROR)
		return true;	// �\�P�b�g�G���[
	if (nResult == 0)
		return true;  // �^�C���A�E�g

	return false;
}
