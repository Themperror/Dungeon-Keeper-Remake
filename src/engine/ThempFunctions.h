#pragma once
#include <DirectXMath.h>
#include <algorithm>
#undef min
#undef max
using namespace DirectX;
namespace Themp
{
	DirectX::XMFLOAT3 operator+=(DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);
	DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);
	DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);
	DirectX::XMFLOAT3 operator-=(DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);
	DirectX::XMFLOAT3 operator*=(DirectX::XMFLOAT3& a, const float& b);
	DirectX::XMFLOAT3 operator*(const DirectX::XMFLOAT3& a, const float& b);
	DirectX::XMFLOAT3 operator/(const DirectX::XMFLOAT3& a, const float& b);
	DirectX::XMFLOAT4 operator+(const XMFLOAT4& a, const XMFLOAT4& b);
	DirectX::XMFLOAT4 operator-(const XMFLOAT4& a, const XMFLOAT4& b);
	DirectX::XMFLOAT4 operator*(const XMFLOAT4& a, const float& b);
	DirectX::XMFLOAT3 Normalize(DirectX::XMFLOAT3 v);
	DirectX::XMFLOAT3 Add(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b);
	DirectX::XMFLOAT4 Add4(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b);
	DirectX::XMFLOAT3 Clamp(const DirectX::XMFLOAT3& val, float a_min, float a_max);
	DirectX::XMFLOAT4 ToXMFLOAT4(const DirectX::XMFLOAT3& a, float b);
	DirectX::XMFLOAT3 ToXMFLOAT3(const DirectX::XMFLOAT4& a);

	bool operator==(const DirectX::XMFLOAT2& lhs, const DirectX::XMFLOAT2& rhs);
	bool operator==(const DirectX::XMINT2& lhs, const DirectX::XMINT2& rhs);
	DirectX::XMFLOAT2 operator+(const DirectX::XMFLOAT2& lhs, const DirectX::XMFLOAT2& rhs);
	DirectX::XMINT2 operator+(const DirectX::XMINT2& lhs, const DirectX::XMINT2& rhs);
}
