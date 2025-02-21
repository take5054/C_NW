#pragma once

#include "IP.hpp"

/// <summary>
/// UDPクラス
/// </summary>
class CUDP : public CIP
{
public:
	CUDP();
	CUDP(const std::string& In_strAddress, int In_nPort);
	virtual ~CUDP() override;

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
	/// 接続先設定
	/// </summary>
	/// <param name="[In_strAddress]">アドレス</param>
	/// <param name="[In_nPort]">ポート</param>
	/// <returns>接続先情報</returns>
	sockaddr_in SetConnect(const std::string& In_strAddress, int In_nPort);

	/// <summary>
	/// タイムアウトチェック
	/// </summary>
	/// <returns>true:タイムアウト, false:接続できた</returns>
	bool TimeoutCheck();
};
