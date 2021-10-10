#include "Vector3.h"
#include<cmath>

using namespace std;

Vector3::Vector3() :
	Vector3(0, 0, 0)
{

}

Vector3::Vector3(float x, float y, float z) :
	x(x), y(y), z(z)
{
}

float Vector3::length(const Vector3 &v)
{
	return (float)sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3 Vector3::normalize(const Vector3 &v)
{
	float len = length(v);
	if (len != 0)
	{
		return Vector3(v.x / len, v.y / len, v.z / len);
	}
	return Vector3(v.x, v.y, v.z);
}

float Vector3::dot(const Vector3 &v1, const Vector3 &v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector3 Vector3::cross(const Vector3 &v1, const Vector3 &v2)
{
	Vector3 temp;
	temp.x = v1.y * v2.z - v1.z * v2.y;
	temp.y = v1.z * v2.x - v1.x * v2.z;
	temp.z = v1.x * v2.y - v1.y * v2.x;
	return temp;
}

Vector3 Vector3::operator+() const
{
	return *this;
}

Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

Vector3 &Vector3::operator+=(const Vector3 &v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3 &Vector3::operator-=(const Vector3 &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector3 &Vector3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector3 &Vector3::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

const Vector3 operator+(const Vector3 &v1, const Vector3 &v2)
{
	Vector3 temp(v1);
	return  temp += v2;
}

const Vector3 operator-(const Vector3 &v1, const Vector3 &v2)
{
	Vector3 temp(v1);
	return  temp -= v2;
}

const Vector3 operator*(const Vector3 &v, float s)
{
	Vector3 temp(v);
	return  temp *= s;
}

const Vector3 operator*(float s, const Vector3 &v)
{
	return v * s;
}

const Vector3 operator/(const Vector3 &v, float s)
{
	Vector3 temp(v);
	return  temp /= s;
}
