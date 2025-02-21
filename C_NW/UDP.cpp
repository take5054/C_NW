#include "UDP.hpp"
#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)	// inet_addr関数の警告(エラー)を無視

CUDP::CUDP()
	: CIP()
{
	// UDPソケットの作成
	m_upSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	// ソケットの作成に失敗した場合
	if (m_upSocket == INVALID_SOCKET)
	{
		WSACleanup();
		throw std::runtime_error("Socket creation failed");
	}
}

CUDP::CUDP(const std::string& In_strAddress, int In_nPort)
	: CUDP()
{
	// アドレス:ポートの設定
	m_SAddress = SetConnect(In_strAddress, In_nPort);
}

CUDP::~CUDP()
{
	CIP::~CIP();
}

void CUDP::Bind()
{
	// ----- バインド -----
	if (bind(m_upSocket, (sockaddr*)&m_SAddress, sizeof(m_SAddress)) == SOCKET_ERROR)
	{
		// バインドに失敗した場合
		std::cerr << "bind failed: " << WSAGetLastError() << std::endl;
		CUDP::~CUDP();
		throw std::runtime_error("Bind failed");
	}
}

void CUDP::Send(const std::string& word)
{
	// ----- 送信 -----
	int nResult = sendto(m_upSocket, word.c_str(), static_cast<int>(word.size()), 0,
		(sockaddr*)&m_SAddress, sizeof(m_SAddress));

	if (nResult == SOCKET_ERROR)	// 送信に失敗した場合
		return;
}

std::string CUDP::Recv()
{
	char szRecvBuffer[cx_nMaxMsgSize];

	// ----- 受信 -----
	int nResult = recv(m_upSocket, szRecvBuffer, sizeof(szRecvBuffer), 0);
	if (nResult == SOCKET_ERROR)	// 受信に失敗した場合
		return "";

	return std::string(szRecvBuffer, nResult);
}

CUDP::Response CUDP::ReceiveAndRespond()
{
	// ----- 変数宣言 -----
	char cRecvBuffer[cx_nMaxMsgSize];
	sockaddr_in sClientAddress;
	int nClientAddrSize = sizeof(sClientAddress);

	// ----- 受信 -----
	int bytesReceived = recvfrom(m_upSocket,
		cRecvBuffer, sizeof(cRecvBuffer), 0,
		(sockaddr*)&sClientAddress, &nClientAddrSize);

	// ----- エラーチェック -----
	if (bytesReceived == SOCKET_ERROR)
		return Response();	// 受信に失敗した場合

	// ----- クライアント情報を構造体に格納 -----
	Response sClientInfo;
	sClientInfo.IPaddress = sClientAddress;
	sClientInfo.strData = std::string(cRecvBuffer, bytesReceived);

	return sClientInfo;
}

std::string CUDP::SendAndReceive(const std::string& In_strServerAddress, int In_nServerPort,
	const std::string& In_strMsg)
{
	// ----- 送信先の設定 -----
	sockaddr_in sServerAddress = SetConnect(In_strServerAddress, In_nServerPort);

	// ----- 送信 -----
	int nSendResult = sendto(m_upSocket,
		In_strMsg.c_str(), static_cast<int>(In_strMsg.size()), 0,
		(sockaddr*)&sServerAddress, sizeof(sServerAddress));

	// ----- エラーチェック -----
	if (nSendResult == SOCKET_ERROR)
		return "";	// 送信に失敗した場合

	if (TimeoutCheck())
		return "";	// タイムアウトした場合

	return Recv();	// 受信
}

std::string CUDP::SendAndReceive(const std::string& In_strMsg)
{
	// ----- 送信 -----
	int nResult = sendto(m_upSocket,
		In_strMsg.c_str(), static_cast<int>(In_strMsg.size()), 0,
		(sockaddr*)&m_SAddress, sizeof(m_SAddress));

	// ----- エラーチェック -----
	if (nResult == SOCKET_ERROR)
		return "";	// 送信に失敗した場合

	if (TimeoutCheck())
		return "";	// タイムアウトした場合

	return Recv();	// 受信
}

sockaddr_in CUDP::SetConnect(const std::string& In_strAddress, int In_nPort)
{
	sockaddr_in sConnectAddress;
	sConnectAddress.sin_family = AF_INET;		// アドレスファミリーを設定
	sConnectAddress.sin_port = htons(In_nPort);	// ポートを設定
	// アドレスを設定
	sConnectAddress.sin_addr.S_un.S_addr =
		In_strAddress.empty() ? INADDR_ANY : inet_addr(In_strAddress.c_str());
	return sConnectAddress;
}

bool CUDP::TimeoutCheck()
{
	fd_set sReadfds;
	FD_ZERO(&sReadfds);				// 初期化
	FD_SET(m_upSocket, &sReadfds);	// ソケットを設定

	timeval sTimeoutTime{ 0, 1000 };	// タイムアウトになる時間(1ms)
	// タイムアウトチェック
	int nResult = select(0, &sReadfds, NULL, NULL, &sTimeoutTime);

	// ----- エラーチェック -----
	if (nResult == SOCKET_ERROR)
		return true;	// ソケットエラー
	if (nResult == 0)
		return true;  // タイムアウト

	return false;
}
