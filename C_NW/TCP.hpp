#pragma once

#include "IP.hpp"

/// <summary>
/// CTCPクラス
/// </summary>
class CTCP : public CIP
{
public:
	CTCP(const std::string& address, int port);
	virtual ~CTCP() override;

	/// <summary>
	/// バインド
	/// </summary>
	virtual void Bind() override;

	/// <summary>
	/// メッセージの送信
	/// </summary>
	/// <param name="[In_strMsg]">送信するメッセージ</param>
	virtual void Send(const std::string& In_strMsg) override;

	/// <summary>
	/// メッセージの送信
	/// </summary>
	/// <param name="[In_strMsg]">送信するメッセージ</param>
	/// <param name="[In_sAddress]">アドレス</param>
	virtual void Send(const std::string& In_strMsg, sockaddr_in In_sAddress);

	/// <summary>
	/// メッセージの受信
	/// </summary>
	/// <returns>受信したメッセージ</returns>
	virtual std::string Recv() override;

	/// <summary>
	/// メッセージの受信→応答
	/// </summary>
	/// <returns>受信したメッセージ</returns>
	/// <remarks>サーバー用</remarks>
	virtual Response ReceiveAndRespond() override;

	/// <summary>
	/// メッセージの送信→受信
	/// </summary>
	/// <param name="[In_strServerAddress]">サーバーアドレス</param>
	/// <param name="[In_nServerPort]">サーバーポート</param>
	/// <param name="[In_strMessage]">送信するメッセージ</param>
	/// <returns>受信したメッセージ</returns>
	/// <remarks>クライアント用</remarks>
	virtual std::string SendAndReceive(const std::string& In_strServerAddress,
		int In_nServerPort, const std::string& In_strMsg) override;

	/// <summary>
	/// メッセージの送信→受信
	/// </summary>
	/// <param name="[In_strMsg]">送信するメッセージ</param>
	/// <returns>受信したメッセージ</returns>
	virtual std::string SendAndReceive(const std::string& In_strMsg) override;

	/// <summary>
	/// タイムアウトチェック
	/// </summary>
	/// <returns>タイムアウトしたかどうか</returns>
	bool TimeoutCheck();
};
