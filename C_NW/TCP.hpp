#pragma once

#include "IP.hpp"

/// <summary>
/// CTCP�N���X
/// </summary>
class CTCP : public CIP
{
public:
	CTCP(const std::string& address, int port);
	virtual ~CTCP() override;

	/// <summary>
	/// �o�C���h
	/// </summary>
	virtual void Bind() override;

	/// <summary>
	/// ���b�Z�[�W�̑��M
	/// </summary>
	/// <param name="[In_strMsg]">���M���郁�b�Z�[�W</param>
	virtual void Send(const std::string& In_strMsg) override;

	/// <summary>
	/// ���b�Z�[�W�̑��M
	/// </summary>
	/// <param name="[In_strMsg]">���M���郁�b�Z�[�W</param>
	/// <param name="[In_sAddress]">�A�h���X</param>
	virtual void Send(const std::string& In_strMsg, sockaddr_in In_sAddress);

	/// <summary>
	/// ���b�Z�[�W�̎�M
	/// </summary>
	/// <returns>��M�������b�Z�[�W</returns>
	virtual std::string Recv() override;

	/// <summary>
	/// ���b�Z�[�W�̎�M������
	/// </summary>
	/// <returns>��M�������b�Z�[�W</returns>
	/// <remarks>�T�[�o�[�p</remarks>
	virtual Response ReceiveAndRespond() override;

	/// <summary>
	/// ���b�Z�[�W�̑��M����M
	/// </summary>
	/// <param name="[In_strServerAddress]">�T�[�o�[�A�h���X</param>
	/// <param name="[In_nServerPort]">�T�[�o�[�|�[�g</param>
	/// <param name="[In_strMessage]">���M���郁�b�Z�[�W</param>
	/// <returns>��M�������b�Z�[�W</returns>
	/// <remarks>�N���C�A���g�p</remarks>
	virtual std::string SendAndReceive(const std::string& In_strServerAddress,
		int In_nServerPort, const std::string& In_strMsg) override;

	/// <summary>
	/// ���b�Z�[�W�̑��M����M
	/// </summary>
	/// <param name="[In_strMsg]">���M���郁�b�Z�[�W</param>
	/// <returns>��M�������b�Z�[�W</returns>
	virtual std::string SendAndReceive(const std::string& In_strMsg) override;

	/// <summary>
	/// �^�C���A�E�g�`�F�b�N
	/// </summary>
	/// <returns>�^�C���A�E�g�������ǂ���</returns>
	bool TimeoutCheck();
};
