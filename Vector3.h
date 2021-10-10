#pragma once
class Vector3
{

public:
	float x;
	float y;
	float z;
public:
	//�R���X�g���N�^
	Vector3();							//��x�N�g���Ƃ���
	Vector3(float x, float y, float z);	//x�����Ay�����Az�������w�肵�Ă̐���

	//�����o�֐�
	static  float length(const Vector3 &v) ;				//�m�����i�����j�����߂�
	static Vector3 normalize(const Vector3 &v);				//���K������

	static float dot(const Vector3 &v1,const Vector3 &v2);	//���ς����߂�
	static Vector3 cross(const Vector3 &v1, const Vector3 &v2);//�O�ς����߂�

	//�P�����Z�q�I�[�o�[���[�h
	Vector3 operator+() const;
	Vector3 operator-() const;

	//������Z�q�I�[�o�[���[�h
	Vector3 &operator+=(const Vector3 &v);
	Vector3 &operator-=(const Vector3 &v);
	Vector3 &operator*=(float s);
	Vector3 &operator/=(float s);
};

//�Q�����Z�q�I�[�o�[���[�h
const Vector3 operator+(const Vector3& v1,const Vector3& v2);
const Vector3 operator-(const Vector3& v1,const Vector3& v2);
const Vector3 operator*(const Vector3& v,float s);
const Vector3 operator*(float s,const Vector3& v);
const Vector3 operator/(const Vector3& v,float s);