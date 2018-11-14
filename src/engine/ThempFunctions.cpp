#include "ThempSystem.h"
#include "ThempFunctions.h"
#undef min
#undef max
using namespace DirectX;
namespace Themp
{

	XMFLOAT2 operator-(const XMFLOAT2& a, const XMFLOAT2& b)
	{
		return XMFLOAT2(a.x - b.x, a.y - b.y);
	}
	XMFLOAT2 operator-=(XMFLOAT2& a, const XMFLOAT2& b)
	{
		a = XMFLOAT2(a.x - b.x, a.y - b.y);
		return a;
	}
	XMFLOAT3 operator+=(XMFLOAT3& a, const XMFLOAT3& b)
	{
		a = XMFLOAT3(a.x + b.x, a.y + b.y, a.z + b.z);
		return a;
	}
	XMFLOAT3 operator+(const XMFLOAT3& a, const XMFLOAT3& b)
	{
		return XMFLOAT3(a.x + b.x, a.y + b.y, a.z + b.z);
	}
	XMFLOAT3 operator-(const XMFLOAT3& a, const XMFLOAT3& b)
	{
		return XMFLOAT3(a.x - b.x, a.y - b.y, a.z - b.z);
	}
	XMFLOAT3 operator-=(XMFLOAT3& a, const XMFLOAT3& b)
	{
		a = XMFLOAT3(a.x - b.x, a.y - b.y, a.z - b.z);
		return a;
	}
	XMFLOAT3 operator*=(XMFLOAT3& a, const float& b)
	{
		a = XMFLOAT3(a.x * b, a.y * b, a.z * b);
		return a;
	}
	XMFLOAT3 operator*(const XMFLOAT3& a, const float& b)
	{
		return XMFLOAT3(a.x * b, a.y * b, a.z * b);
	}
	XMFLOAT3 operator/(const XMFLOAT3& a, const float& b)
	{
		return XMFLOAT3(a.x / b, a.y / b, a.z / b);
	}
	XMFLOAT4 operator+(const XMFLOAT4& a, const XMFLOAT4& b)
	{
		return XMFLOAT4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
	}
	XMFLOAT4 operator-(const XMFLOAT4& a, const XMFLOAT4& b)
	{
		return XMFLOAT4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
	}
	XMFLOAT4 operator*(const XMFLOAT4& a, const float& b)
	{
		return XMFLOAT4(a.x * b, a.y * b, a.z * b, a.w * b);
	}
	XMFLOAT3 Normalize(const XMFLOAT3& v)
	{
		XMVECTOR x = XMLoadFloat3(&v);
		x = XMVector3Normalize(x);
		XMFLOAT3 r;
		XMStoreFloat3(&r, x);
		return r;
	}
	XMFLOAT2 Normalize(const XMFLOAT2& v)
	{
		XMVECTOR x = XMLoadFloat2(&v);
		x = XMVector2Normalize(x);
		XMFLOAT2 r;
		XMStoreFloat2(&r, x);
		return r;
	}
	XMFLOAT3 Add(XMFLOAT3 a, XMFLOAT3 b)
	{
		XMFLOAT3 r = XMFLOAT3(a.x + b.x, a.y + b.y, a.z + b.z);
		return r;
	}
	XMFLOAT4 Add4(XMFLOAT3 a, XMFLOAT3 b)
	{
		XMFLOAT4 r = XMFLOAT4(a.x + b.x, a.y + b.y, a.z + b.z, 1.0);
		return r;
	}
	XMFLOAT3 Clamp(const XMFLOAT3& val, float a_min, float a_max)
	{
		return XMFLOAT3(std::min(std::max(val.x, a_min), a_max), std::min(std::max(val.y, a_min), a_max), std::min(std::max(val.z, a_min), a_max));
	}
	float Dot(const DirectX::XMFLOAT3 a, const DirectX::XMFLOAT3 b)
	{
		return (a.x*b.x + a.y * b.y + a.z * b.z);
	}
	XMFLOAT3 Cross(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
	{
		return XMFLOAT3(
			a.y*b.z - a.z*b.y,
			a.z*b.x - a.x*b.z,
			a.x*b.y - a.y*b.x
		);
	}
	float Lerp(float v0, float v1, float t)
	{
		return (1 - t) * v0 + t * v1;
	}
	XMFLOAT2 Lerp(const XMFLOAT2 & v0, const XMFLOAT2 & v1, float t)
	{
		return XMFLOAT2(Lerp(v0.x, v1.x, t), Lerp(v0.y, v1.y, t));
	}
	XMFLOAT3 Lerp(const XMFLOAT3 & v0, const XMFLOAT3 & v1, float t)
	{
		return XMFLOAT3(Lerp(v0.x, v1.x, t), Lerp(v0.y, v1.y, t), Lerp(v0.z, v1.z, t));
	}
	XMFLOAT4 ToXMFLOAT4(const XMFLOAT3& a, float b)
	{
		return XMFLOAT4(a.x, a.y, a.z, b);
	}
	XMFLOAT3 ToXMFLOAT3(const XMFLOAT4& a)
	{
		return XMFLOAT3(a.x, a.y, a.z);
	}
	bool operator==(const DirectX::XMFLOAT2& lhs, const DirectX::XMFLOAT2& rhs)
	{
		return (lhs.x == rhs.x && lhs.y == rhs.y);
	}
	bool operator==(const DirectX::XMINT2& lhs, const DirectX::XMINT2& rhs)
	{
		return (lhs.x == rhs.x && lhs.y == rhs.y);
	}
	XMFLOAT2 operator+(const DirectX::XMFLOAT2& lhs, const DirectX::XMFLOAT2& rhs)
	{
		return XMFLOAT2(lhs.x + rhs.x, lhs.y + rhs.y);
	}
	XMINT2 operator+(const DirectX::XMINT2& lhs, const DirectX::XMINT2& rhs)
	{
		return XMINT2(lhs.x + rhs.x, lhs.y + rhs.y);
	}
}
