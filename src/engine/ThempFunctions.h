#pragma once
#include <DirectXMath.h>
#include <algorithm>
#undef min
#undef max
using namespace DirectX;
namespace Themp
{
	DirectX::XMFLOAT2 operator-(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b);
	DirectX::XMFLOAT2 operator-=(DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b);
	DirectX::XMFLOAT3 operator+=(DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);
	DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);
	DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);
	DirectX::XMFLOAT3 operator-=(DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);
	DirectX::XMFLOAT3 operator*=(DirectX::XMFLOAT3& a, const float& b);
	DirectX::XMFLOAT3 operator*(const DirectX::XMFLOAT3& a, const float& b);
	DirectX::XMFLOAT3 operator/(const DirectX::XMFLOAT3& a, const float& b);
	DirectX::XMFLOAT4 operator+(const DirectX::XMFLOAT4& a, const DirectX::XMFLOAT4& b);
	DirectX::XMFLOAT4 operator-(const DirectX::XMFLOAT4& a, const DirectX::XMFLOAT4& b);
	DirectX::XMFLOAT4 operator*(const DirectX::XMFLOAT4& a, const float& b);
	DirectX::XMFLOAT3 Normalize(const DirectX::XMFLOAT3& v);
	DirectX::XMFLOAT2 Normalize(const DirectX::XMFLOAT2& v);
	DirectX::XMFLOAT3 Add(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b);
	DirectX::XMFLOAT4 Add4(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b);
	DirectX::XMFLOAT3 Clamp(const DirectX::XMFLOAT3& val, float a_min, float a_max);
	float Dot(const DirectX::XMFLOAT3 x, const DirectX::XMFLOAT3 y);
	DirectX::XMFLOAT3 Cross(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);
	float Lerp(float v0, float v1, float t);
	DirectX::XMFLOAT2 Lerp(const DirectX::XMFLOAT2& v0, const DirectX::XMFLOAT2& v1, float t);
	DirectX::XMFLOAT3 Lerp(const DirectX::XMFLOAT3& v0, const DirectX::XMFLOAT3& v1, float t);
	DirectX::XMFLOAT4 ToXMFLOAT4(const DirectX::XMFLOAT3& a, float b);
	DirectX::XMFLOAT3 ToXMFLOAT3(const DirectX::XMFLOAT4& a);

	bool operator==(const DirectX::XMFLOAT2& lhs, const DirectX::XMFLOAT2& rhs);
	bool operator==(const DirectX::XMINT2& lhs, const DirectX::XMINT2& rhs);
	DirectX::XMFLOAT2 operator+(const DirectX::XMFLOAT2& lhs, const DirectX::XMFLOAT2& rhs);
	DirectX::XMINT2 operator+(const DirectX::XMINT2& lhs, const DirectX::XMINT2& rhs);
}
