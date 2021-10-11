#pragma once
class Vector3
{

public:
	float x;
	float y;
	float z;
public:
	//コンストラクタ
	Vector3();							//零ベクトルとする
	Vector3(float x, float y, float z);	//x成分、y成分、z成分を指定しての生成

	//メンバ関数
	static  float length(const Vector3 &v) ;				//ノルム（長さ）を求める
	static Vector3 normalize(const Vector3 &v);				//正規化する

	static float dot(const Vector3 &v1,const Vector3 &v2);	//内積を求める
	static Vector3 cross(const Vector3 &v1, const Vector3 &v2);//外積を求める

	//単項演算子オーバーロード
	Vector3 operator+() const;
	Vector3 operator-() const;

	//代入演算子オーバーロード
	Vector3 &operator+=(const Vector3 &v);
	Vector3 &operator-=(const Vector3 &v);
	Vector3 &operator*=(float s);
	Vector3 &operator/=(float s);
};

//２項演算子オーバーロード
const Vector3 operator+(const Vector3& v1,const Vector3& v2);
const Vector3 operator-(const Vector3& v1,const Vector3& v2);
const Vector3 operator*(const Vector3& v,float s);
const Vector3 operator*(float s,const Vector3& v);
const Vector3 operator/(const Vector3& v,float s);