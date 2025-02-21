#include "UDP.hpp"
#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)	// inet_addr�֐��̌x��(�G���[)�𖳎�

CUDP::CUDP()
	: CIP()
{
	// UDP�\�P�b�g�̍쐬
	m_upSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	// �\�P�b�g�̍쐬�Ɏ��s�����ꍇ
	if (m_upSocket == INVALID_SOCKET)
	{
		WSACleanup();
		throw std::runtime_error("Socket creation failed");
	}
}

CUDP::CUDP(const std::string& In_strAddress, int In_nPort)
	: CUDP()
{
	// �A�h���X:�|�[�g�̐ݒ�
	m_SAddress = SetConnect(In_strAddress, In_nPort);
}

CUDP::~CUDP()
{
	CIP::~CIP();
}

void CUDP::Bind()
{
	// ----- �o�C���h -----
	if (bind(m_upSocket, (sockaddr*)&m_SAddress, sizeof(m_SAddress)) == SOCKET_ERROR)
	{
		// �o�C���h�Ɏ��s�����ꍇ
		std::cerr << "bind failed: " << WSAGetLastError() << std::endl;
		CUDP::~CUDP();
		throw std::runtime_error("Bind failed");
	}
}

void CUDP::Send(const std::string& word)
{
	// ----- ���M -----
	int nResult = sendto(m_upSocket, word.c_str(), static_cast<int>(word.size()), 0,
		(sockaddr*)&m_SAddress, sizeof(m_SAddress));

	if (nResult == SOCKET_ERROR)	// ���M�Ɏ��s�����ꍇ
		return;
}

std::string CUDP::Recv()
{
	char szRecvBuffer[cx_nMaxMsgSize];

	// ----- ��M -----
	int nResult = recv(m_upSocket, szRecvBuffer, sizeof(szRecvBuffer), 0);
	if (nResult == SOCKET_ERROR)	// ��M�Ɏ��s�����ꍇ
		return "";

	return std::string(szRecvBuffer, nResult);
}

CUDP::Response CUDP::ReceiveAndRespond()
{
	// ----- �ϐ��錾 -----
	char cRecvBuffer[cx_nMaxMsgSize];
	sockaddr_in sClientAddress;
	int nClientAddrSize = sizeof(sClientAddress);

	// ----- ��M -----
	int bytesReceived = recvfrom(m_upSocket,
		cRecvBuffer, sizeof(cRecvBuffer), 0,
		(sockaddr*)&sClientAddress, &nClientAddrSize);

	// ----- �G���[�`�F�b�N -----
	if (bytesReceived == SOCKET_ERROR)
		return Response();	// ��M�Ɏ��s�����ꍇ

	// ----- �N���C�A���g�����\���̂Ɋi�[ -----
	Response sClientInfo;
	sClientInfo.IPaddress = sClientAddress;
	sClientInfo.strData = std::string(cRecvBuffer, bytesReceived);

	return sClientInfo;
}

std::string CUDP::SendAndReceive(const std::string& In_strServerAddress, int In_nServerPort,
	const std::string& In_strMsg)
{
	// ----- ���M��̐ݒ� -----
	sockaddr_in sServerAddress = SetConnect(In_strServerAddress, In_nServerPort);

	// ----- ���M -----
	int nSendResult = sendto(m_upSocket,
		In_strMsg.c_str(), static_cast<int>(In_strMsg.size()), 0,
		(sockaddr*)&sServerAddress, sizeof(sServerAddress));

	// ----- �G���[�`�F�b�N -----
	if (nSendResult == SOCKET_ERROR)
		return "";	// ���M�Ɏ��s�����ꍇ

	if (TimeoutCheck())
		return "";	// �^�C���A�E�g�����ꍇ

	return Recv();	// ��M
}

std::string CUDP::SendAndReceive(const std::string& In_strMsg)
{
	// ----- ���M -----
	int nResult = sendto(m_upSocket,
		In_strMsg.c_str(), static_cast<int>(In_strMsg.size()), 0,
		(sockaddr*)&m_SAddress, sizeof(m_SAddress));

	// ----- �G���[�`�F�b�N -----
	if (nResult == SOCKET_ERROR)
		return "";	// ���M�Ɏ��s�����ꍇ

	if (TimeoutCheck())
		return "";	// �^�C���A�E�g�����ꍇ

	return Recv();	// ��M
}

sockaddr_in CUDP::SetConnect(const std::string& In_strAddress, int In_nPort)
{
	sockaddr_in sConnectAddress;
	sConnectAddress.sin_family = AF_INET;		// �A�h���X�t�@�~���[��ݒ�
	sConnectAddress.sin_port = htons(In_nPort);	// �|�[�g��ݒ�
	// �A�h���X��ݒ�
	sConnectAddress.sin_addr.S_un.S_addr =
		In_strAddress.empty() ? INADDR_ANY : inet_addr(In_strAddress.c_str());
	return sConnectAddress;
}

bool CUDP::TimeoutCheck()
{
	fd_set sReadfds;
	FD_ZERO(&sReadfds);				// ������
	FD_SET(m_upSocket, &sReadfds);	// �\�P�b�g��ݒ�

	timeval sTimeoutTime{ 0, 1000 };	// �^�C���A�E�g�ɂȂ鎞��(1ms)
	// �^�C���A�E�g�`�F�b�N
	int nResult = select(0, &sReadfds, NULL, NULL, &sTimeoutTime);

	// ----- �G���[�`�F�b�N -----
	if (nResult == SOCKET_ERROR)
		return true;	// �\�P�b�g�G���[
	if (nResult == 0)
		return true;  // �^�C���A�E�g

	return false;
}
