#pragma once

#include <string>
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

constexpr int cx_nMaxMsgSize = 1024;	// メッセージの最大サイズ

/// <summary>
/// CIPクラス
/// </summary>
class CIP
{
public:

	/// <summary>
	/// レスポンス構造体
	/// </summary>
	struct Response
	{
		sockaddr_in IPaddress{};	// アドレス情報
		std::string strData;		// データ
	};

	CIP();
	virtual ~CIP();
	virtual void Bind() = 0;

	/// <summary>
	/// メッセージの送信
	/// </summary>
	/// <param name="[In_strMsg]">送信するメッセージ</param>
	virtual void Send(const std::string& In_strMsg) = 0;

	/// <summary>
	/// メッセージの受信
	/// </summary>
	/// <returns>受信したメッセージ</returns>
	virtual std::string Recv() = 0;

	/// <summary>
	/// メッセージの受信→応答
	/// </summary>
	/// <returns>受信したメッセージ</returns>
	/// <remarks>サーバー用</remarks>
	virtual Response ReceiveAndRespond() = 0;

	/// <summary>
	/// メッセージの送信→受信
	/// </summary>
	/// <param name="[In_strServerAddress]">サーバーアドレス</param>
	/// <param name="[In_nServerPort]">サーバーポート</param>
	/// <param name="[In_strMessage]">送信するメッセージ</param>
	/// <returns>受信したメッセージ</returns>
	/// <remarks>クライアント用</remarks>
	virtual std::string SendAndReceive(const std::string& In_strServerAddress,
		int In_nServerPort, const std::string& In_strMsg) = 0;

	/// <summary>
	/// メッセージの送信→受信
	/// </summary>
	/// <param name="[In_strMsg]">送信するメッセージ</param>
	/// <returns>受信したメッセージ</returns>
	virtual std::string SendAndReceive(const std::string& In_strMsg) = 0;

protected:
	SOCKET m_upSocket;		// ソケット
	sockaddr_in m_SAddress;	// アドレス
};
