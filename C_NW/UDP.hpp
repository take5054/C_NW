#pragma once

#include "IP.hpp"

/// <summary>
/// UDP�N���X
/// </summary>
class CUDP : public CIP
{
public:
	CUDP();
	CUDP(const std::string& In_strAddress, int In_nPort);
	virtual ~CUDP() override;

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
	/// �ڑ���ݒ�
	/// </summary>
	/// <param name="[In_strAddress]">�A�h���X</param>
	/// <param name="[In_nPort]">�|�[�g</param>
	/// <returns>�ڑ�����</returns>
	sockaddr_in SetConnect(const std::string& In_strAddress, int In_nPort);

	/// <summary>
	/// �^�C���A�E�g�`�F�b�N
	/// </summary>
	/// <returns>true:�^�C���A�E�g, false:�ڑ��ł���</returns>
	bool TimeoutCheck();
};
