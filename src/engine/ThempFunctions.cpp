#include "ThempSystem.h"
#include "ThempFunctions.h"
#undef min
#undef max
using namespace DirectX;
namespace Themp
{
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
	XMFLOAT3 Normalize(XMFLOAT3 v)
	{
		XMVECTOR x = XMLoadFloat3(&v);
		x = XMVector3Normalize(x);
		XMFLOAT3 r;
		XMStoreFloat3(&r, x);
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
	XMFLOAT4 ToXMFLOAT4(const XMFLOAT3& a, float b)
	{
		return XMFLOAT4(a.x, a.y, a.z, b);
	}
	XMFLOAT3 ToXMFLOAT3(const XMFLOAT4& a)
	{
		return XMFLOAT3(a.x, a.y, a.z);
	}
}
