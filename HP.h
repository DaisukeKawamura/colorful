#pragma once
class HP
{
public://�����o�֐�

	// �R���X�g���N�^
	HP() ;
	// �f�X�g���N�^
	~HP() {};

	//������  //�ŏ���HP�A�ŏ�HP,�ő�HP
	void Init(float CurrenHP, float minHP, float maxHP);

	//�ő�HP�擾
	float GetMaxHP() const;
	//�ŏ�HP�擾
	float GetMinHP() const;
	//���݂�HP�擾
	float GetCurrentHP() const;

	//HP����
	void AddDamage(float value);

	//HP��
	void RecoverDamage(float value);

	//�ő�HP�̐ݒ�
	void SetMaxHP(float value);

	//HP����
	bool isEmpty();

private:
	//HP���ő�l�A�ŏ��l�𒴂��Ȃ��悤��
	void NormalizeHP();

private://�����o�ϐ�
	float P_MaxHP;     // �ő�HP
	float P_MinHP;     // �ŏ�HP
	float P_CurrentHP;// ���݂�HP

};