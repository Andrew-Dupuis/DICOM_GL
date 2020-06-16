// Developed with Reference to Matrices class by Song Ho Ahn (song.ahn@gmail.com)

#include <cmath>
#include <algorithm>
#include "Matrices.h"
#include <string>

const float ToRadians = 3.141593f / 180.0f;
const float ToDegrees = 180.0f / 3.141593f;
const float Epsilon = 0.00001f;

#pragma region Functions for Matrix2
Matrix2& Matrix2::transpose()
{
	std::swap(data[1], data[2]);
	return *this;
}

float Matrix2::getDeterminant() const
{
	return data[0] * data[3] - data[1] * data[2];
}

Matrix2& Matrix2::invert()
{
	if (fabs(getDeterminant()) <= Epsilon) // if absolute value less than epsilon
	{
		return identity();
	}

	float oldDataZero = data[0];
	float inverseDeterminant = 1.0f / getDeterminant();;
	data[0] = inverseDeterminant * data[3];
	data[1] = -inverseDeterminant * data[1];
	data[2] = -inverseDeterminant * data[2];
	data[3] = inverseDeterminant * oldDataZero;
	return *this;
}


float Matrix2::getAngle() const
{
	return ToDegrees * atan2f(data[1], data[0]);
}
#pragma endregion

#pragma region Functions for Matrix3
Matrix3& Matrix3::transpose()
{
	std::swap(data[1], data[3]);
	std::swap(data[2], data[6]);
	std::swap(data[5], data[7]);

	return *this;
}

float Matrix3::getDeterminant() const
{
	return data[0] * (data[4] * data[8] - data[5] * data[7]) -
		data[1] * (data[3] * data[8] - data[5] * data[6]) +
		data[2] * (data[3] * data[7] - data[4] * data[6]);
}

Matrix3& Matrix3::invert()
{
	float determinant, inverseDeterminant;
	float cofaconstructors[9];

	cofaconstructors[0] = data[4] * data[8] - data[5] * data[7];
	cofaconstructors[1] = data[7] * data[2] - data[8] * data[1];
	cofaconstructors[2] = data[1] * data[5] - data[2] * data[4];
	cofaconstructors[3] = data[5] * data[6] - data[3] * data[8];
	cofaconstructors[4] = data[0] * data[8] - data[2] * data[6];
	cofaconstructors[5] = data[2] * data[3] - data[0] * data[5];
	cofaconstructors[6] = data[3] * data[7] - data[4] * data[6];
	cofaconstructors[7] = data[6] * data[1] - data[7] * data[0];
	cofaconstructors[8] = data[0] * data[4] - data[1] * data[3];

	// check determinant if it is 0
	determinant = data[0] * cofaconstructors[0] + data[1] * cofaconstructors[3] + data[2] * cofaconstructors[6];
	if (fabs(determinant) <= Epsilon)
	{
		return identity();
	}

	// divide by the determinant
	inverseDeterminant = 1.0f / determinant;
	data[0] = inverseDeterminant * cofaconstructors[0];
	data[1] = inverseDeterminant * cofaconstructors[1];
	data[2] = inverseDeterminant * cofaconstructors[2];
	data[3] = inverseDeterminant * cofaconstructors[3];
	data[4] = inverseDeterminant * cofaconstructors[4];
	data[5] = inverseDeterminant * cofaconstructors[5];
	data[6] = inverseDeterminant * cofaconstructors[6];
	data[7] = inverseDeterminant * cofaconstructors[7];
	data[8] = inverseDeterminant * cofaconstructors[8];

	return *this;
}


Vector3 Matrix3::getAngle() const
{
	float xRot, yRot, zRot;

	yRot = ToDegrees * asinf(data[6]);
	if (data[8] < 0)
	{
		if (yRot >= 0) yRot = 180.0f - yRot;
		else         yRot = -180.0f - yRot;
	}

	if (data[0] > -Epsilon && data[0] < Epsilon)
	{
		zRot = 0;
		xRot = ToDegrees * atan2f(data[1], data[4]);
	}
	else
	{
		zRot = ToDegrees * atan2f(-data[3], data[0]);
		xRot = ToDegrees * atan2f(-data[7], data[8]);
	}

	return Vector3(xRot, yRot, zRot);
}
#pragma endregion

#pragma region Functions for Matrix4
Matrix4& Matrix4::transpose()
{
	std::swap(data[1], data[4]);
	std::swap(data[2], data[8]);
	std::swap(data[3], data[12]);
	std::swap(data[6], data[9]);
	std::swap(data[7], data[13]);
	std::swap(data[11], data[14]);

	return *this;
}

Matrix4& Matrix4::getMatrix() {
	return *this;
}

Matrix4 Matrix4::getInverse() {
	Matrix4 copy = Matrix4(data[0], data[1], data[2], data[3],
		data[4], data[5] , data[6], data[7] ,
		data[8] , data[9], data[10] , data[11],
		data[12] , data[13] , data[14], data[15] );
	return copy.invert();
}

Matrix4& Matrix4::invert()
{
	// If the 4th row is [0,0,0,1] then it is affine matrix and
	// it has no projective transformation.
	if (data[3] == 0 && data[7] == 0 && data[11] == 0 && data[15] == 1)
		this->invertAffine();
	else
	{
		this->invertGeneral();
	}

	return *this;
}

Matrix4& Matrix4::invertEuclidean() // Adapted directly from Song Ho Ahn
{
	float tmp;
	tmp = data[1];  data[1] = data[4];  data[4] = tmp;
	tmp = data[2];  data[2] = data[8];  data[8] = tmp;
	tmp = data[6];  data[6] = data[9];  data[9] = tmp;

	float x = data[12];
	float y = data[13];
	float z = data[14];
	data[12] = -(data[0] * x + data[4] * y + data[8] * z);
	data[13] = -(data[1] * x + data[5] * y + data[9] * z);
	data[14] = -(data[2] * x + data[6] * y + data[10] * z);
	return *this;
}


Matrix4& Matrix4::invertAffine() // Adapted directly from Song Ho Ahn
{
	// R^-1
	Matrix3 r(data[0], data[1], data[2], data[4], data[5], data[6], data[8], data[9], data[10]);
	r.invert();
	data[0] = r[0];  data[1] = r[1];  data[2] = r[2];
	data[4] = r[3];  data[5] = r[4];  data[6] = r[5];
	data[8] = r[6];  data[9] = r[7];  data[10] = r[8];

	// -R^-1 * T
	float x = data[12];
	float y = data[13];
	float z = data[14];
	data[12] = -(r[0] * x + r[3] * y + r[6] * z);
	data[13] = -(r[1] * x + r[4] * y + r[7] * z);
	data[14] = -(r[2] * x + r[5] * y + r[8] * z);

	return *this;
}

Matrix4& Matrix4::invertGeneral()
{
	float cofaconstructors[16];

	cofaconstructors[0] = getCofactor(data[5], data[6], data[7], data[9], data[10], data[11], data[13], data[14], data[15]);
	cofaconstructors[1] = getCofactor(data[4], data[6], data[7], data[8], data[10], data[11], data[12], data[14], data[15]);
	cofaconstructors[2] = getCofactor(data[4], data[5], data[7], data[8], data[9], data[11], data[12], data[13], data[15]);
	cofaconstructors[3] = getCofactor(data[4], data[5], data[6], data[8], data[9], data[10], data[12], data[13], data[14]);
	float determinant = data[0] * cofaconstructors[0] - data[1] * cofaconstructors[1] + data[2] * cofaconstructors[2] - data[3] * cofaconstructors[3];

	if (fabs(determinant) <= Epsilon)
	{
		return identity();
	}

	cofaconstructors[4] = getCofactor(data[1], data[2], data[3], data[9], data[10], data[11], data[13], data[14], data[15]);
	cofaconstructors[5] = getCofactor(data[0], data[2], data[3], data[8], data[10], data[11], data[12], data[14], data[15]);
	cofaconstructors[6] = getCofactor(data[0], data[1], data[3], data[8], data[9], data[11], data[12], data[13], data[15]);
	cofaconstructors[7] = getCofactor(data[0], data[1], data[2], data[8], data[9], data[10], data[12], data[13], data[14]);
	cofaconstructors[8] = getCofactor(data[1], data[2], data[3], data[5], data[6], data[7], data[13], data[14], data[15]);
	cofaconstructors[9] = getCofactor(data[0], data[2], data[3], data[4], data[6], data[7], data[12], data[14], data[15]);
	cofaconstructors[10] = getCofactor(data[0], data[1], data[3], data[4], data[5], data[7], data[12], data[13], data[15]);
	cofaconstructors[11] = getCofactor(data[0], data[1], data[2], data[4], data[5], data[6], data[12], data[13], data[14]);
	cofaconstructors[12] = getCofactor(data[1], data[2], data[3], data[5], data[6], data[7], data[9], data[10], data[11]);
	cofaconstructors[13] = getCofactor(data[0], data[2], data[3], data[4], data[6], data[7], data[8], data[10], data[11]);
	cofaconstructors[14] = getCofactor(data[0], data[1], data[3], data[4], data[5], data[7], data[8], data[9], data[11]);
	cofaconstructors[15] = getCofactor(data[0], data[1], data[2], data[4], data[5], data[6], data[8], data[9], data[10]);

	// build inverse matrix = adj(M) / det(M)
	// adjugate of M is the transpose of the cofactor matrix of M
	float invDeterminant = 1.0f / determinant;
	data[0] = invDeterminant * cofaconstructors[0];
	data[1] = -invDeterminant * cofaconstructors[4];
	data[2] = invDeterminant * cofaconstructors[8];
	data[3] = -invDeterminant * cofaconstructors[12];

	data[4] = -invDeterminant * cofaconstructors[1];
	data[5] = invDeterminant * cofaconstructors[5];
	data[6] = -invDeterminant * cofaconstructors[9];
	data[7] = invDeterminant * cofaconstructors[13];

	data[8] = invDeterminant * cofaconstructors[2];
	data[9] = -invDeterminant * cofaconstructors[6];
	data[10] = invDeterminant * cofaconstructors[10];
	data[11] = -invDeterminant * cofaconstructors[14];

	data[12] = -invDeterminant * cofaconstructors[3];
	data[13] = invDeterminant * cofaconstructors[7];
	data[14] = -invDeterminant * cofaconstructors[11];
	data[15] = invDeterminant * cofaconstructors[15];

	return *this;
}

float Matrix4::getDeterminant() const
{
	return data[0] * getCofactor(data[5], data[6], data[7], data[9], data[10], data[11], data[13], data[14], data[15]) -
		data[1] * getCofactor(data[4], data[6], data[7], data[8], data[10], data[11], data[12], data[14], data[15]) +
		data[2] * getCofactor(data[4], data[5], data[7], data[8], data[9], data[11], data[12], data[13], data[15]) -
		data[3] * getCofactor(data[4], data[5], data[6], data[8], data[9], data[10], data[12], data[13], data[14]);
}

float Matrix4::getCofactor(float m0, float m1, float m2,
	float m3, float m4, float m5,
	float m6, float m7, float m8) const
{
	return m0 * (m4 * m8 - m5 * m7) -
		m1 * (m3 * m8 - m5 * m6) +
		m2 * (m3 * m7 - m4 * m6);
}

Matrix4& Matrix4::translate(const Vector3& v)
{
	return translate(v.x, v.y, v.z);
}

Matrix4& Matrix4::translate(float x, float y, float z)
{
	data[0] += data[3] * x;   data[4] += data[7] * x;   data[8] += data[11] * x;   data[12] += data[15] * x;
	data[1] += data[3] * y;   data[5] += data[7] * y;   data[9] += data[11] * y;   data[13] += data[15] * y;
	data[2] += data[3] * z;   data[6] += data[7] * z;   data[10] += data[11] * z;   data[14] += data[15] * z;

	return *this;
}

Matrix4& Matrix4::scale(float s)
{
	return scale(s, s, s);
}

Matrix4& Matrix4::scale(float x, float y, float z)
{
	data[0] *= x;   data[4] *= x;   data[8] *= x;   data[12] *= x;
	data[1] *= y;   data[5] *= y;   data[9] *= y;   data[13] *= y;
	data[2] *= z;   data[6] *= z;   data[10] *= z;   data[14] *= z;
	return *this;
}

Matrix4& Matrix4::rotate(float angle, const Vector3& axis)
{
	return rotate(angle, axis.x, axis.y, axis.z);
}

Matrix4& Matrix4::rotate(float angle, float x, float y, float z)
{
	float c = cosf(angle * ToRadians);    // cosine
	float s = sinf(angle * ToRadians);    // sine
	float c1 = 1.0f - c;                // 1 - c
	float m0 = data[0], m4 = data[4], m8 = data[8], m12 = data[12],
		m1 = data[1], m5 = data[5], m9 = data[9], m13 = data[13],
		m2 = data[2], m6 = data[6], m10 = data[10], m14 = data[14];

	// build rotation matrix
	float r0 = x * x * c1 + c;
	float r1 = x * y * c1 + z * s;
	float r2 = x * z * c1 - y * s;
	float r4 = x * y * c1 - z * s;
	float r5 = y * y * c1 + c;
	float r6 = y * z * c1 + x * s;
	float r8 = x * z * c1 + y * s;
	float r9 = y * z * c1 - x * s;
	float r10 = z * z * c1 + c;

	// multiply rotation matrix
	data[0] = r0 * m0 + r4 * m1 + r8 * m2;
	data[1] = r1 * m0 + r5 * m1 + r9 * m2;
	data[2] = r2 * m0 + r6 * m1 + r10 * m2;
	data[4] = r0 * m4 + r4 * m5 + r8 * m6;
	data[5] = r1 * m4 + r5 * m5 + r9 * m6;
	data[6] = r2 * m4 + r6 * m5 + r10 * m6;
	data[8] = r0 * m8 + r4 * m9 + r8 * m10;
	data[9] = r1 * m8 + r5 * m9 + r9 * m10;
	data[10] = r2 * m8 + r6 * m9 + r10 * m10;
	data[12] = r0 * m12 + r4 * m13 + r8 * m14;
	data[13] = r1 * m12 + r5 * m13 + r9 * m14;
	data[14] = r2 * m12 + r6 * m13 + r10 * m14;

	return *this;
}

Matrix4& Matrix4::lookAt(const Vector3& target)
{
	return lookAt(target, Vector3(0, 1, 0));
}

Matrix4& Matrix4::lookAt(const Vector3& target, const Vector3& upVec)
{
	Vector3 eye = translationVector();
	Vector3 zaxis = (eye - target).normalize();    // The "forward" vector.
	Vector3 xaxis = (upVec.cross(zaxis)).normalize(); // The "right" vector.
	Vector3 yaxis = zaxis.cross(xaxis);     // The "up" vector.

	// Create a 4x4 view matrix from the right, up, forward and eye position veconstructors
	this->identity();
	this->setColumn(0, Vector4(xaxis.x, yaxis.x, zaxis.x, 0));
	this->setColumn(1, Vector4(xaxis.y, yaxis.y, zaxis.y, 0));
	this->setColumn(2, Vector4(xaxis.z, yaxis.z, zaxis.z, 0));
	this->setColumn(3, Vector4(-xaxis.dot(eye), -yaxis.dot(eye), -zaxis.dot(eye), 1));
	return *this;
}

Matrix4& Matrix4::lookAt(float targetX, float targetY, float targetZ)
{
	return lookAt(Vector3(targetX, targetY, targetZ));
}

Matrix4& Matrix4::lookAt(float targetX, float targetY, float targetZ, float upX, float upY, float upZ)
{
	return lookAt(Vector3(targetX, targetY, targetZ), Vector3(upX, upY, upZ));
}

Matrix3 Matrix4::getRotationMatrix() const
{
	Matrix3 mat(data[0], data[1], data[2],
		data[4], data[5], data[6],
		data[8], data[9], data[10]);
	return mat;
}

Vector3 Matrix4::getAngle() const
{
	float xRot, yRot, zRot;         // 3 angles

	// find yaw (around y-axis) first
	// NOTE: asin() returns -90~+90, so correct the angle range -180~+180
	// using z value of forward vector
	yRot = ToDegrees * asinf(data[8]);
	if (data[10] < 0)
	{
		if (yRot >= 0) yRot = 180.0f - yRot;
		else         yRot = -180.0f - yRot;
	}

	// find roll (around z-axis) and pitch (around x-axis)
	// if forward vector is (1,0,0) or (-1,0,0), then m[0]=m[4]=m[9]=m[10]=0
	if (data[0] > -Epsilon && data[0] < Epsilon)
	{
		zRot = 0;  //@@ assume roll=0
		xRot = ToDegrees * atan2f(data[1], data[5]);
	}
	else
	{
		zRot = ToDegrees * atan2f(-data[4], data[0]);
		xRot = ToDegrees * atan2f(-data[9], data[10]);
	}

	return Vector3(xRot, yRot, zRot);
}

Vector3 Matrix4::translationVector() {
	return Vector3(data[12], data[13], data[14]);
}

Vector4 Matrix4::getColumn(int index)
{
	Vector4 column = Vector4(data[index * 4], data[index * 4 + 1], data[index * 4 + 2], data[index * 4 + 3]);
	return column;
}

Vector4 Matrix4::getRow(int index) {
	Vector4 row = Vector4(data[index], data[index + 4], data[index + 8], data[index + 12]);
	return row;
}

#pragma endregion
