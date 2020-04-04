#include "PANDUVector4.h"

namespace Pandu
{
	const Vector4 Vector4::UNIT_X = Vector4(1.0f,0.0f,0.0f,0.0f);
	const Vector4 Vector4::UNIT_Y = Vector4(0.0f,1.0f,0.0f,0.0f);
	const Vector4 Vector4::UNIT_Z = Vector4(0.0f,0.0f,1.0f,0.0f);
	const Vector4 Vector4::UNIT_W = Vector4(0.0f,0.0f,0.0f,1.0f);
	const Vector4 Vector4::NEGATIVE_UNIT_X = Vector4(-1.0f,0.0f,0.0f,0.0f);
	const Vector4 Vector4::NEGATIVE_UNIT_Y = Vector4(0.0f,-1.0f,0.0f,0.0f);
	const Vector4 Vector4::NEGATIVE_UNIT_Z = Vector4(0.0f,0.0f,-1.0f,0.0f);
	const Vector4 Vector4::NEGATIVE_UNIT_W = Vector4(0.0f,0.0f,0.0f,-1.0f);
	const Vector4 Vector4::ZERO = Vector4(0.0f,0.0f,0.0f,0.0f);
	const Vector4 Vector4::UNIT = Vector4(1.0f,1.0f,1.0f,1.0f);
}