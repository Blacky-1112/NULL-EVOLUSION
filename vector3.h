#pragma once

#include <math.h>
#include <assert.h>

class Vector3
{
public:
	float x, y, z;
//コンストラクタ
	//デフォルトコンストラクタ
	Vector3() {}

	//コピーコンストラクタ
	Vector3(const Vector3 &a) : x(a.x), y(a.y), z(a.z) {}

	//3つの値で作成
	Vector3(float nx, float ny, float nz) : x(nx), y(ny), z(nz) {}

//標準的なオブジェクトの保守
	
	//代入(Cの慣習に従い値への参照を返す)
	Vector3 &operator = (const Vector3 &a) {
		x = a.x;
		y = a.y;
		z = a.z;
		return *this;
	}

	//等しさのチェック
	bool operator ==(const Vector3 &a)const {
		return x == a.x && y == a.y && z == a.z;
	}
	bool operator !=(const Vector3& a)const {
		return x != a.x || y != a.y || z != a.z;
	}

//ベクトル操作

	//ベクトル設定を0にする
	void zero() {
		x = y = z = 0.0f;
	}

	//単項式のマイナスは反転したベクトルを返す
	Vector3 operator -() const {
		return Vector3(-x, -y, -z);
	}

	//二項式の+と-はベクトルを加算し、減算する
	Vector3 operator +(const Vector3& a) const {
		return Vector3(x + a.x, y + a.y, z + a.z);
	}

	Vector3 operator -(const Vector3& a) const {
		return Vector3(x - a.x, y - a.y, z - a.z);
	}

	//スカラーによる乗算と除算
	Vector3 operator *(float a)const {
		return Vector3(x * a, y * a, z * a);
	}

	Vector3 operator /(float a)const {
		float oneOverA = 1.0f / a;
		return Vector3(x * oneOverA, y * oneOverA, z * oneOverA);
	}

	//Cの表示法に準拠するための
	//組み合わせ代入演算
	Vector3& operator +=(const Vector3& a) {
		x += a.x, y += a.y, z += a.z;
		return *this;
	}

	Vector3& operator -=(const Vector3& a) {
		x -= a.x, y -= a.y, z -= a.z;
		return *this;
	}

	Vector3& operator *=(float a) {
		x *= a, y *= a, z *= a;
		return *this;
	}

	Vector3& operator /=(float a) {
		float oneOverA = 1.0f / a;
		x *= oneOverA, y *= oneOverA, z *= oneOverA;
		return *this;
	}

	float operator[](int i) const
	{
		assert(i >= 0 && i < 3);
		if (i == 0) return x;
		if (i == 1) return y;
		return z;
	}

	float& operator[](int i)
	{
		assert(i >= 0 && i < 3);
		if (i == 0) return x;
		if (i == 1) return y;
		return z;
	}


	//ベクトルを正規化
	void normalize() {
		float magSq = x * x + y * y + z * z;
		if (magSq > 0.0f) {
			float oneOverMag = 1.0f / sqrt(magSq);
			x *= oneOverMag;
			y *= oneOverMag;
			z *= oneOverMag;
		}
	}

	Vector3 normalized() const
	{
		float magSq = x * x + y * y + z * z;

		if (magSq <= 0.0f)
			return Vector3(0, 0, 0);

		float inv = 1.0f / sqrtf(magSq);

		return Vector3(
			x * inv,
			y * inv,
			z * inv
		);
	}

	//二つのオブジェクト間の距離
	float length() const
	{
		return sqrtf(x * x + y * y + z * z);
	}

	float lengthSq() const
	{
		return x * x + y * y + z * z;
	}

	//二つのオブジェクト間の距離(高さ無視)
	float len() const
	{
		return sqrtf(x * x + z * z);
	}

	//内積
	static float dot(const Vector3& a, const Vector3& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	//外積
	static Vector3 cross(const Vector3& a, const Vector3& b)
	{
		Vector3 ret;
		ret.x = a.y * b.z - a.z * b.y;
		ret.y = a.z * b.x - a.x * b.z;
		ret.z = a.x * b.y - a.y * b.x;
		return ret;
	}

	//
	static Vector3 Lerp(const Vector3& a, const Vector3& b, float t)
	{
		if (t < 0.0f) t = 0.0f;
		if (t > 1.0f) t = 1.0f;
		return Vector3(
			a.x + (b.x - a.x) * t,
			a.y + (b.y - a.y) * t,
			a.z + (b.z - a.z) * t
		);
	}
};