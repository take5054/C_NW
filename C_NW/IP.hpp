#pragma once

#include <string>
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

constexpr int cx_nMaxMsgSize = 1024;	// ���b�Z�[�W�̍ő�T�C�Y

/// <summary>
/// CIP�N���X
/// </summary>
class CIP
{
public:

	/// <summary>
	/// ���X�|���X�\����
	/// </summary>
	struct Response
	{
		sockaddr_in IPaddress{};	// �A�h���X���
		std::string strData;		// �f�[�^
	};

	CIP();
	virtual ~CIP();
	virtual void Bind() = 0;

	/// <summary>
	/// ���b�Z�[�W�̑��M
	/// </summary>
	/// <param name="[In_strMsg]">���M���郁�b�Z�[�W</param>
	virtual void Send(const std::string& In_strMsg) = 0;

	/// <summary>
	/// ���b�Z�[�W�̎�M
	/// </summary>
	/// <returns>��M�������b�Z�[�W</returns>
	virtual std::string Recv() = 0;

	/// <summary>
	/// ���b�Z�[�W�̎�M������
	/// </summary>
	/// <returns>��M�������b�Z�[�W</returns>
	/// <remarks>�T�[�o�[�p</remarks>
	virtual Response ReceiveAndRespond() = 0;

	/// <summary>
	/// ���b�Z�[�W�̑��M����M
	/// </summary>
	/// <param name="[In_strServerAddress]">�T�[�o�[�A�h���X</param>
	/// <param name="[In_nServerPort]">�T�[�o�[�|�[�g</param>
	/// <param name="[In_strMessage]">���M���郁�b�Z�[�W</param>
	/// <returns>��M�������b�Z�[�W</returns>
	/// <remarks>�N���C�A���g�p</remarks>
	virtual std::string SendAndReceive(const std::string& In_strServerAddress,
		int In_nServerPort, const std::string& In_strMsg) = 0;

	/// <summary>
	/// ���b�Z�[�W�̑��M����M
	/// </summary>
	/// <param name="[In_strMsg]">���M���郁�b�Z�[�W</param>
	/// <returns>��M�������b�Z�[�W</returns>
	virtual std::string SendAndReceive(const std::string& In_strMsg) = 0;

protected:
	SOCKET m_upSocket;		// �\�P�b�g
	sockaddr_in m_SAddress;	// �A�h���X
};
