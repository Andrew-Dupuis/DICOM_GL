// Developed with Reference to Matrices class by Song Ho Ahn (song.ahn@gmail.com)
#pragma once

#include <iostream>
#include <iomanip>
#include "Vectors.h"

class Matrix2
{
public:
	// construconstructors
	Matrix2();  // init with identity
	Matrix2(const float src[4]);
	Matrix2(float m0, float m1, float m2, float m3);

	void        set(const float src[4]);
	void        set(float m0, float m1, float m2, float m3);
	void        setRow(int index, const float row[2]);
	void        setRow(int index, const Vector2& v);
	void        setColumn(int index, const float col[2]);
	void        setColumn(int index, const Vector2& v);

	const float* get() const;
	float       getDeterminant() const;
	float       getAngle() const;                       // retrieve angle (degree) from matrix

	Matrix2& identity();
	Matrix2& transpose();                            // transpose itself and return reference
	Matrix2& invert();

	// operators
	Matrix2     operator+(const Matrix2& rhs) const;    // add rhs
	Matrix2     operator-(const Matrix2& rhs) const;    // subtract rhs
	Matrix2& operator+=(const Matrix2& rhs);         // add rhs and update this object
	Matrix2& operator-=(const Matrix2& rhs);         // subtract rhs and update this object
	Vector2     operator*(const Vector2& rhs) const;    // multiplication: v' = M * v
	Matrix2     operator*(const Matrix2& rhs) const;    // multiplication: M3 = M1 * M2
	Matrix2& operator*=(const Matrix2& rhs);         // multiplication: M1' = M1 * M2
	bool        operator==(const Matrix2& rhs) const;   // exact compare, no epsilon
	bool        operator!=(const Matrix2& rhs) const;   // exact compare, no epsilon
	float       operator[](int index) const;            // subscript operator v[0], v[1]
	float& operator[](int index);                  // subscript operator v[0], v[1]

	// friends functions
	friend Matrix2 operator-(const Matrix2& m);                     // unary operator (-)
	friend Matrix2 operator*(float scalar, const Matrix2& m);       // pre-multiplication
	friend Vector2 operator*(const Vector2& vec, const Matrix2& m); // pre-multiplication
	friend std::ostream& operator<<(std::ostream& os, const Matrix2& m);

private:
	float data[4];

};

class Matrix3
{
public:
	// construconstructors
	Matrix3();  // init with identity
	Matrix3(const float src[9]);
	Matrix3(float m0, float m1, float m2,           // 1st column
		float m3, float m4, float m5,           // 2nd column
		float m6, float m7, float m8);          // 3rd column

	void        set(const float src[9]);
	void        set(float m0, float m1, float m2,   // 1st column
		float m3, float m4, float m5,   // 2nd column
		float m6, float m7, float m8);  // 3rd column
	void        setRow(int index, const float row[3]);
	void        setRow(int index, const Vector3& v);
	void        setColumn(int index, const float col[3]);
	void        setColumn(int index, const Vector3& v);

	const float* get() const;
	float       getDeterminant() const;
	Vector3     getAngle() const;                       // return (pitch, yaw, roll)

	Matrix3& identity();
	Matrix3& transpose();                            // transpose itself and return reference
	Matrix3& invert();

	// operators
	Matrix3     operator+(const Matrix3& rhs) const;    // add rhs
	Matrix3     operator-(const Matrix3& rhs) const;    // subtract rhs
	Matrix3& operator+=(const Matrix3& rhs);         // add rhs and update this object
	Matrix3& operator-=(const Matrix3& rhs);         // subtract rhs and update this object
	Vector3     operator*(const Vector3& rhs) const;    // multiplication: v' = M * v
	Matrix3     operator*(const Matrix3& rhs) const;    // multiplication: M3 = M1 * M2
	Matrix3& operator*=(const Matrix3& rhs);         // multiplication: M1' = M1 * M2
	bool        operator==(const Matrix3& rhs) const;   // exact compare, no epsilon
	bool        operator!=(const Matrix3& rhs) const;   // exact compare, no epsilon
	float       operator[](int index) const;            // subscript operator v[0], v[1]
	float& operator[](int index);                  // subscript operator v[0], v[1]

	// friends functions
	friend Matrix3 operator-(const Matrix3& m);                     // unary operator (-)
	friend Matrix3 operator*(float scalar, const Matrix3& m);       // pre-multiplication
	friend Vector3 operator*(const Vector3& vec, const Matrix3& m); // pre-multiplication
	friend std::ostream& operator<<(std::ostream& os, const Matrix3& m);

private:
	float data[9];
};

class Matrix4
{
public:
	// construconstructors
	Matrix4();  // init with identity
	Matrix4(const float src[16]);
	Matrix4(float m00, float m01, float m02, float m03, // 1st column
		float m04, float m05, float m06, float m07, // 2nd column
		float m08, float m09, float m10, float m11, // 3rd column
		float m12, float m13, float m14, float m15);// 4th column

	void        set(const float src[16]);
	void        set(float m00, float m01, float m02, float m03, // 1st column
		float m04, float m05, float m06, float m07, // 2nd column
		float m08, float m09, float m10, float m11, // 3rd column
		float m12, float m13, float m14, float m15);// 4th column
	void        setRow(int index, const float row[4]);
	void        setRow(int index, const Vector4& v);
	void        setRow(int index, const Vector3& v);
	void        setColumn(int index, const float col[4]);
	void        setColumn(int index, const Vector4& v);
	void        setColumn(int index, const Vector3& v);


	const float* get() const;
	const float* getTranspose();                        // return transposed matrix
	float       getDeterminant() const;
	Matrix3     getRotationMatrix() const;              // return 3x3 rotation part
	Vector3     getAngle() const;                       // return (pitch, yaw, roll)

	Vector3 translationVector();
	Vector4 getColumn(int index);
	Vector4 getRow(int index);
	Matrix4& identity();
	Matrix4& transpose();                            // transpose itself and return reference
	Matrix4& getMatrix();
	Matrix4 getInverse();
	Matrix4& invert();                               // check best inverse method before inverse
	Matrix4& invertEuclidean();                      // inverse of Euclidean transform matrix
	Matrix4& invertAffine();                         // inverse of affine transform matrix
	Matrix4& invertGeneral();                        // inverse of generic matrix

	// transform matrix
	Matrix4& translate(float x, float y, float z);   // translation by (x,y,z)
	Matrix4& translate(const Vector3& v);            //
	Matrix4& rotate(float angle, const Vector3& axis); // rotate angle(degree) along the given axix
	Matrix4& rotate(float angle, float x, float y, float z);
	Matrix4& scale(float scale);                     // uniform scale
	Matrix4& scale(float sx, float sy, float sz);    // scale by (sx, sy, sz) on each axis
	Matrix4& lookAt(float tx, float ty, float tz);   // face object to the target direction
	Matrix4& lookAt(float tx, float ty, float tz, float ux, float uy, float uz);
	Matrix4& lookAt(const Vector3& target);
	Matrix4& lookAt(const Vector3& target, const Vector3& up);
	//@@Matrix4&    skew(float angle, const Vector3& axis); //

	// operators
	Matrix4     operator+(const Matrix4& rhs) const;    // add rhs
	Matrix4     operator-(const Matrix4& rhs) const;    // subtract rhs
	Matrix4& operator+=(const Matrix4& rhs);         // add rhs and update this object
	Matrix4& operator-=(const Matrix4& rhs);         // subtract rhs and update this object
	Vector4     operator*(const Vector4& rhs) const;    // multiplication: v' = M * v
	Vector3     operator*(const Vector3& rhs) const;    // multiplication: v' = M * v
	Matrix4     operator*(const Matrix4& rhs) const;    // multiplication: M3 = M1 * M2
	Matrix4& operator*=(const Matrix4& rhs);         // multiplication: M1' = M1 * M2
	bool        operator==(const Matrix4& rhs) const;   // exact compare, no epsilon
	bool        operator!=(const Matrix4& rhs) const;   // exact compare, no epsilon
	float       operator[](int index) const;            // subscript operator v[0], v[1]
	float& operator[](int index);                  // subscript operator v[0], v[1]

	// friends functions
	friend Matrix4 operator-(const Matrix4& m);                     // unary operator (-)
	friend Matrix4 operator*(float scalar, const Matrix4& m);       // pre-multiplication
	friend Vector3 operator*(const Vector3& vec, const Matrix4& m); // pre-multiplication
	friend Vector4 operator*(const Vector4& vec, const Matrix4& m); // pre-multiplication
	friend std::ostream& operator<<(std::ostream& os, const Matrix4& m);

	float       getCofactor(float m0, float m1, float m2,
		float m3, float m4, float m5,
		float m6, float m7, float m8) const;

	float data[16];
	float tm[16];                                       // transpose m

};

#pragma region Inline functions for Matrix2
inline Matrix2::Matrix2()
{
	identity(); // initially identity matrix
}

inline Matrix2::Matrix2(const float src[4])
{
	set(src);
}

inline Matrix2::Matrix2(float m0, float m1, float m2, float m3)
{
	set(m0, m1, m2, m3);
}

inline void Matrix2::set(const float src[4])
{
	data[0] = src[0];  data[1] = src[1];  data[2] = src[2];  data[3] = src[3];
}

inline void Matrix2::set(float m0, float m1, float m2, float m3)
{
	data[0] = m0;  data[1] = m1;  data[2] = m2;  data[3] = m3;
}

inline void Matrix2::setRow(int index, const float row[2])
{
	data[index] = row[0];  data[index + 2] = row[1];
}

inline void Matrix2::setRow(int index, const Vector2& v)
{
	data[index] = v.x;  data[index + 2] = v.y;
}

inline void Matrix2::setColumn(int index, const float col[2])
{
	data[index * 2] = col[0];  data[index * 2 + 1] = col[1];
}

inline void Matrix2::setColumn(int index, const Vector2& v)
{
	data[index * 2] = v.x;  data[index * 2 + 1] = v.y;
}

inline const float* Matrix2::get() const
{
	return data;
}

inline Matrix2& Matrix2::identity()
{
	data[0] = data[3] = 1.0f;
	data[1] = data[2] = 0.0f;
	return *this;
}

inline Matrix2 Matrix2::operator+(const Matrix2& rhs) const
{
	return Matrix2(data[0] + rhs[0], data[1] + rhs[1], data[2] + rhs[2], data[3] + rhs[3]);
}

inline Matrix2 Matrix2::operator-(const Matrix2& rhs) const
{
	return Matrix2(data[0] - rhs[0], data[1] - rhs[1], data[2] - rhs[2], data[3] - rhs[3]);
}

inline Matrix2& Matrix2::operator+=(const Matrix2& rhs)
{
	data[0] += rhs[0];  data[1] += rhs[1];  data[2] += rhs[2];  data[3] += rhs[3];
	return *this;
}

inline Matrix2& Matrix2::operator-=(const Matrix2& rhs)
{
	data[0] -= rhs[0];  data[1] -= rhs[1];  data[2] -= rhs[2];  data[3] -= rhs[3];
	return *this;
}

inline Vector2 Matrix2::operator*(const Vector2& rhs) const
{
	return Vector2(data[0] * rhs.x + data[2] * rhs.y, data[1] * rhs.x + data[3] * rhs.y);
}

inline Matrix2 Matrix2::operator*(const Matrix2& rhs) const
{
	return Matrix2(data[0] * rhs[0] + data[2] * rhs[1], data[1] * rhs[0] + data[3] * rhs[1],
		data[0] * rhs[2] + data[2] * rhs[3], data[1] * rhs[2] + data[3] * rhs[3]);
}

inline Matrix2& Matrix2::operator*=(const Matrix2& rhs)
{
	*this = *this * rhs;
	return *this;
}

inline bool Matrix2::operator==(const Matrix2& rhs) const
{
	return (data[0] == rhs[0]) && (data[1] == rhs[1]) && (data[2] == rhs[2]) && (data[3] == rhs[3]);
}

inline bool Matrix2::operator!=(const Matrix2& rhs) const
{
	return (data[0] != rhs[0]) || (data[1] != rhs[1]) || (data[2] != rhs[2]) || (data[3] != rhs[3]);
}

inline float Matrix2::operator[](int index) const
{
	return data[index];
}

inline float& Matrix2::operator[](int index)
{
	return data[index];
}

inline Matrix2 operator-(const Matrix2& rhs)
{
	return Matrix2(-rhs[0], -rhs[1], -rhs[2], -rhs[3]);
}

inline Matrix2 operator*(float s, const Matrix2& rhs)
{
	return Matrix2(s * rhs[0], s * rhs[1], s * rhs[2], s * rhs[3]);
}

inline Vector2 operator*(const Vector2& v, const Matrix2& rhs)
{
	return Vector2(v.x * rhs[0] + v.y * rhs[1], v.x * rhs[2] + v.y * rhs[3]);
}

inline std::ostream& operator<<(std::ostream& os, const Matrix2& m)
{
	os << std::fixed << std::setprecision(5);
	os << "[" << std::setw(10) << m[0] << " " << std::setw(10) << m[2] << "]\n"
		<< "[" << std::setw(10) << m[1] << " " << std::setw(10) << m[3] << "]\n";
	os << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
	return os;
}
#pragma endregion

#pragma region Inline functions for Matrix3
inline Matrix3::Matrix3()
{
	identity();     // initially identity matrix
}

inline Matrix3::Matrix3(const float src[9])
{
	set(src);
}

inline Matrix3::Matrix3(float m0, float m1, float m2,
	float m3, float m4, float m5,
	float m6, float m7, float m8)
{
	set(m0, m1, m2, m3, m4, m5, m6, m7, m8);
}

inline void Matrix3::set(const float src[9])
{
	data[0] = src[0];  data[1] = src[1];  data[2] = src[2];
	data[3] = src[3];  data[4] = src[4];  data[5] = src[5];
	data[6] = src[6];  data[7] = src[7];  data[8] = src[8];
}

inline void Matrix3::set(float m0, float m1, float m2,
	float m3, float m4, float m5,
	float m6, float m7, float m8)
{
	data[0] = m0;  data[1] = m1;  data[2] = m2;
	data[3] = m3;  data[4] = m4;  data[5] = m5;
	data[6] = m6;  data[7] = m7;  data[8] = m8;
}

inline void Matrix3::setRow(int index, const float row[3])
{
	data[index] = row[0];  data[index + 3] = row[1];  data[index + 6] = row[2];
}

inline void Matrix3::setRow(int index, const Vector3& v)
{
	data[index] = v.x;  data[index + 3] = v.y;  data[index + 6] = v.z;
}

inline void Matrix3::setColumn(int index, const float col[3])
{
	data[index * 3] = col[0];  data[index * 3 + 1] = col[1];  data[index * 3 + 2] = col[2];
}

inline void Matrix3::setColumn(int index, const Vector3& v)
{
	data[index * 3] = v.x;  data[index * 3 + 1] = v.y;  data[index * 3 + 2] = v.z;
}

inline const float* Matrix3::get() const
{
	return data;
}

inline Matrix3& Matrix3::identity()
{
	data[0] = data[4] = data[8] = 1.0f;
	data[1] = data[2] = data[3] = data[5] = data[6] = data[7] = 0.0f;
	return *this;
}

inline Matrix3 Matrix3::operator+(const Matrix3& rhs) const
{
	return Matrix3(data[0] + rhs[0], data[1] + rhs[1], data[2] + rhs[2],
		data[3] + rhs[3], data[4] + rhs[4], data[5] + rhs[5],
		data[6] + rhs[6], data[7] + rhs[7], data[8] + rhs[8]);
}

inline Matrix3 Matrix3::operator-(const Matrix3& rhs) const
{
	return Matrix3(data[0] - rhs[0], data[1] - rhs[1], data[2] - rhs[2],
		data[3] - rhs[3], data[4] - rhs[4], data[5] - rhs[5],
		data[6] - rhs[6], data[7] - rhs[7], data[8] - rhs[8]);
}

inline Matrix3& Matrix3::operator+=(const Matrix3& rhs)
{
	data[0] += rhs[0];  data[1] += rhs[1];  data[2] += rhs[2];
	data[3] += rhs[3];  data[4] += rhs[4];  data[5] += rhs[5];
	data[6] += rhs[6];  data[7] += rhs[7];  data[8] += rhs[8];
	return *this;
}

inline Matrix3& Matrix3::operator-=(const Matrix3& rhs)
{
	data[0] -= rhs[0];  data[1] -= rhs[1];  data[2] -= rhs[2];
	data[3] -= rhs[3];  data[4] -= rhs[4];  data[5] -= rhs[5];
	data[6] -= rhs[6];  data[7] -= rhs[7];  data[8] -= rhs[8];
	return *this;
}

inline Vector3 Matrix3::operator*(const Vector3& rhs) const
{
	return Vector3(data[0] * rhs.x + data[3] * rhs.y + data[6] * rhs.z,
		data[1] * rhs.x + data[4] * rhs.y + data[7] * rhs.z,
		data[2] * rhs.x + data[5] * rhs.y + data[8] * rhs.z);
}

inline Matrix3 Matrix3::operator*(const Matrix3& rhs) const
{
	return Matrix3(data[0] * rhs[0] + data[3] * rhs[1] + data[6] * rhs[2], data[1] * rhs[0] + data[4] * rhs[1] + data[7] * rhs[2], data[2] * rhs[0] + data[5] * rhs[1] + data[8] * rhs[2],
		data[0] * rhs[3] + data[3] * rhs[4] + data[6] * rhs[5], data[1] * rhs[3] + data[4] * rhs[4] + data[7] * rhs[5], data[2] * rhs[3] + data[5] * rhs[4] + data[8] * rhs[5],
		data[0] * rhs[6] + data[3] * rhs[7] + data[6] * rhs[8], data[1] * rhs[6] + data[4] * rhs[7] + data[7] * rhs[8], data[2] * rhs[6] + data[5] * rhs[7] + data[8] * rhs[8]);
}

inline Matrix3& Matrix3::operator*=(const Matrix3& rhs)
{
	*this = *this * rhs;
	return *this;
}

inline bool Matrix3::operator==(const Matrix3& rhs) const
{
	return (data[0] == rhs[0]) && (data[1] == rhs[1]) && (data[2] == rhs[2]) &&
		(data[3] == rhs[3]) && (data[4] == rhs[4]) && (data[5] == rhs[5]) &&
		(data[6] == rhs[6]) && (data[7] == rhs[7]) && (data[8] == rhs[8]);
}

inline bool Matrix3::operator!=(const Matrix3& rhs) const
{
	return (data[0] != rhs[0]) || (data[1] != rhs[1]) || (data[2] != rhs[2]) ||
		(data[3] != rhs[3]) || (data[4] != rhs[4]) || (data[5] != rhs[5]) ||
		(data[6] != rhs[6]) || (data[7] != rhs[7]) || (data[8] != rhs[8]);
}

inline float Matrix3::operator[](int index) const
{
	return data[index];
}

inline float& Matrix3::operator[](int index)
{
	return data[index];
}

inline Matrix3 operator-(const Matrix3& rhs)
{
	return Matrix3(-rhs[0], -rhs[1], -rhs[2], -rhs[3], -rhs[4], -rhs[5], -rhs[6], -rhs[7], -rhs[8]);
}

inline Matrix3 operator*(float s, const Matrix3& rhs)
{
	return Matrix3(s * rhs[0], s * rhs[1], s * rhs[2], s * rhs[3], s * rhs[4], s * rhs[5], s * rhs[6], s * rhs[7], s * rhs[8]);
}

inline Vector3 operator*(const Vector3& v, const Matrix3& m)
{
	return Vector3(v.x * m[0] + v.y * m[1] + v.z * m[2], v.x * m[3] + v.y * m[4] + v.z * m[5], v.x * m[6] + v.y * m[7] + v.z * m[8]);
}

inline std::ostream& operator<<(std::ostream& os, const Matrix3& m)
{
	os << std::fixed << std::setprecision(5);
	os << "[" << std::setw(10) << m[0] << " " << std::setw(10) << m[3] << " " << std::setw(10) << m[6] << "]\n"
		<< "[" << std::setw(10) << m[1] << " " << std::setw(10) << m[4] << " " << std::setw(10) << m[7] << "]\n"
		<< "[" << std::setw(10) << m[2] << " " << std::setw(10) << m[5] << " " << std::setw(10) << m[8] << "]\n";
	os << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
	return os;
}
#pragma endregion

#pragma region Inline functions for Matrix4
inline Matrix4::Matrix4()
{
	identity();     // initially identity matrix
}

inline Matrix4::Matrix4(const float src[16])
{
	set(src);
}

inline Matrix4::Matrix4(float m00, float m01, float m02, float m03,
	float m04, float m05, float m06, float m07,
	float m08, float m09, float m10, float m11,
	float m12, float m13, float m14, float m15)
{
	set(m00, m01, m02, m03, m04, m05, m06, m07, m08, m09, m10, m11, m12, m13, m14, m15);
}

inline void Matrix4::set(const float src[16])
{
	data[0] = src[0];  data[1] = src[1];  data[2] = src[2];  data[3] = src[3];
	data[4] = src[4];  data[5] = src[5];  data[6] = src[6];  data[7] = src[7];
	data[8] = src[8];  data[9] = src[9];  data[10] = src[10]; data[11] = src[11];
	data[12] = src[12]; data[13] = src[13]; data[14] = src[14]; data[15] = src[15];
}

inline void Matrix4::set(float m00, float m01, float m02, float m03,
	float m04, float m05, float m06, float m07,
	float m08, float m09, float m10, float m11,
	float m12, float m13, float m14, float m15)
{
	data[0] = m00;  data[1] = m01;  data[2] = m02;  data[3] = m03;
	data[4] = m04;  data[5] = m05;  data[6] = m06;  data[7] = m07;
	data[8] = m08;  data[9] = m09;  data[10] = m10;  data[11] = m11;
	data[12] = m12;  data[13] = m13;  data[14] = m14;  data[15] = m15;
}

inline void Matrix4::setRow(int index, const float row[4])
{
	data[index] = row[0];  data[index + 4] = row[1];  data[index + 8] = row[2];  data[index + 12] = row[3];
}

inline void Matrix4::setRow(int index, const Vector4& v)
{
	data[index] = v.x;  data[index + 4] = v.y;  data[index + 8] = v.z;  data[index + 12] = v.w;
}

inline void Matrix4::setRow(int index, const Vector3& v)
{
	data[index] = v.x;  data[index + 4] = v.y;  data[index + 8] = v.z;
}

inline void Matrix4::setColumn(int index, const float col[4])
{
	data[index * 4] = col[0];  data[index * 4 + 1] = col[1];  data[index * 4 + 2] = col[2];  data[index * 4 + 3] = col[3];
}

inline void Matrix4::setColumn(int index, const Vector4& v)
{
	data[index * 4] = v.x;  data[index * 4 + 1] = v.y;  data[index * 4 + 2] = v.z;  data[index * 4 + 3] = v.w;
}

inline void Matrix4::setColumn(int index, const Vector3& v)
{
	data[index * 4] = v.x;  data[index * 4 + 1] = v.y;  data[index * 4 + 2] = v.z;
}

inline const float* Matrix4::get() const
{
	return data;
}


inline const float* Matrix4::getTranspose()
{
	tm[0] = data[0];   tm[1] = data[4];   tm[2] = data[8];   tm[3] = data[12];
	tm[4] = data[1];   tm[5] = data[5];   tm[6] = data[9];   tm[7] = data[13];
	tm[8] = data[2];   tm[9] = data[6];   tm[10] = data[10];  tm[11] = data[14];
	tm[12] = data[3];   tm[13] = data[7];   tm[14] = data[11];  tm[15] = data[15];
	return tm;
}

inline Matrix4& Matrix4::identity()
{
	data[0] = data[5] = data[10] = data[15] = 1.0f;
	data[1] = data[2] = data[3] = data[4] = data[6] = data[7] = data[8] = data[9] = data[11] = data[12] = data[13] = data[14] = 0.0f;
	return *this;
}

inline Matrix4 Matrix4::operator+(const Matrix4& rhs) const
{
	return Matrix4(data[0] + rhs[0], data[1] + rhs[1], data[2] + rhs[2], data[3] + rhs[3],
		data[4] + rhs[4], data[5] + rhs[5], data[6] + rhs[6], data[7] + rhs[7],
		data[8] + rhs[8], data[9] + rhs[9], data[10] + rhs[10], data[11] + rhs[11],
		data[12] + rhs[12], data[13] + rhs[13], data[14] + rhs[14], data[15] + rhs[15]);
}

inline Matrix4 Matrix4::operator-(const Matrix4& rhs) const
{
	return Matrix4(data[0] - rhs[0], data[1] - rhs[1], data[2] - rhs[2], data[3] - rhs[3],
		data[4] - rhs[4], data[5] - rhs[5], data[6] - rhs[6], data[7] - rhs[7],
		data[8] - rhs[8], data[9] - rhs[9], data[10] - rhs[10], data[11] - rhs[11],
		data[12] - rhs[12], data[13] - rhs[13], data[14] - rhs[14], data[15] - rhs[15]);
}

inline Matrix4& Matrix4::operator+=(const Matrix4& rhs)
{
	data[0] += rhs[0];   data[1] += rhs[1];   data[2] += rhs[2];   data[3] += rhs[3];
	data[4] += rhs[4];   data[5] += rhs[5];   data[6] += rhs[6];   data[7] += rhs[7];
	data[8] += rhs[8];   data[9] += rhs[9];   data[10] += rhs[10];  data[11] += rhs[11];
	data[12] += rhs[12];  data[13] += rhs[13];  data[14] += rhs[14];  data[15] += rhs[15];
	return *this;
}

inline Matrix4& Matrix4::operator-=(const Matrix4& rhs)
{
	data[0] -= rhs[0];   data[1] -= rhs[1];   data[2] -= rhs[2];   data[3] -= rhs[3];
	data[4] -= rhs[4];   data[5] -= rhs[5];   data[6] -= rhs[6];   data[7] -= rhs[7];
	data[8] -= rhs[8];   data[9] -= rhs[9];   data[10] -= rhs[10];  data[11] -= rhs[11];
	data[12] -= rhs[12];  data[13] -= rhs[13];  data[14] -= rhs[14];  data[15] -= rhs[15];
	return *this;
}

inline Vector4 Matrix4::operator*(const Vector4& rhs) const
{
	return Vector4(data[0] * rhs.x + data[4] * rhs.y + data[8] * rhs.z + data[12] * rhs.w,
		data[1] * rhs.x + data[5] * rhs.y + data[9] * rhs.z + data[13] * rhs.w,
		data[2] * rhs.x + data[6] * rhs.y + data[10] * rhs.z + data[14] * rhs.w,
		data[3] * rhs.x + data[7] * rhs.y + data[11] * rhs.z + data[15] * rhs.w);
}

inline Vector3 Matrix4::operator*(const Vector3& rhs) const
{
	return Vector3(data[0] * rhs.x + data[4] * rhs.y + data[8] * rhs.z + data[12],
		data[1] * rhs.x + data[5] * rhs.y + data[9] * rhs.z + data[13],
		data[2] * rhs.x + data[6] * rhs.y + data[10] * rhs.z + data[14]);
}

inline Matrix4 Matrix4::operator*(const Matrix4& n) const
{
	return Matrix4(data[0] * n[0] + data[4] * n[1] + data[8] * n[2] + data[12] * n[3], data[1] * n[0] + data[5] * n[1] + data[9] * n[2] + data[13] * n[3], data[2] * n[0] + data[6] * n[1] + data[10] * n[2] + data[14] * n[3], data[3] * n[0] + data[7] * n[1] + data[11] * n[2] + data[15] * n[3],
		data[0] * n[4] + data[4] * n[5] + data[8] * n[6] + data[12] * n[7], data[1] * n[4] + data[5] * n[5] + data[9] * n[6] + data[13] * n[7], data[2] * n[4] + data[6] * n[5] + data[10] * n[6] + data[14] * n[7], data[3] * n[4] + data[7] * n[5] + data[11] * n[6] + data[15] * n[7],
		data[0] * n[8] + data[4] * n[9] + data[8] * n[10] + data[12] * n[11], data[1] * n[8] + data[5] * n[9] + data[9] * n[10] + data[13] * n[11], data[2] * n[8] + data[6] * n[9] + data[10] * n[10] + data[14] * n[11], data[3] * n[8] + data[7] * n[9] + data[11] * n[10] + data[15] * n[11],
		data[0] * n[12] + data[4] * n[13] + data[8] * n[14] + data[12] * n[15], data[1] * n[12] + data[5] * n[13] + data[9] * n[14] + data[13] * n[15], data[2] * n[12] + data[6] * n[13] + data[10] * n[14] + data[14] * n[15], data[3] * n[12] + data[7] * n[13] + data[11] * n[14] + data[15] * n[15]);
}

inline Matrix4& Matrix4::operator*=(const Matrix4& rhs)
{
	*this = *this * rhs;
	return *this;
}

inline bool Matrix4::operator==(const Matrix4& n) const
{
	return (data[0] == n[0]) && (data[1] == n[1]) && (data[2] == n[2]) && (data[3] == n[3]) &&
		(data[4] == n[4]) && (data[5] == n[5]) && (data[6] == n[6]) && (data[7] == n[7]) &&
		(data[8] == n[8]) && (data[9] == n[9]) && (data[10] == n[10]) && (data[11] == n[11]) &&
		(data[12] == n[12]) && (data[13] == n[13]) && (data[14] == n[14]) && (data[15] == n[15]);
}

inline bool Matrix4::operator!=(const Matrix4& n) const
{
	return (data[0] != n[0]) || (data[1] != n[1]) || (data[2] != n[2]) || (data[3] != n[3]) ||
		(data[4] != n[4]) || (data[5] != n[5]) || (data[6] != n[6]) || (data[7] != n[7]) ||
		(data[8] != n[8]) || (data[9] != n[9]) || (data[10] != n[10]) || (data[11] != n[11]) ||
		(data[12] != n[12]) || (data[13] != n[13]) || (data[14] != n[14]) || (data[15] != n[15]);
}

inline float Matrix4::operator[](int index) const
{
	return data[index];
}

inline float& Matrix4::operator[](int index)
{
	return data[index];
}

inline Matrix4 operator-(const Matrix4& rhs)
{
	return Matrix4(-rhs[0], -rhs[1], -rhs[2], -rhs[3], -rhs[4], -rhs[5], -rhs[6], -rhs[7], -rhs[8], -rhs[9], -rhs[10], -rhs[11], -rhs[12], -rhs[13], -rhs[14], -rhs[15]);
}

inline Matrix4 operator*(float s, const Matrix4& rhs)
{
	return Matrix4(s * rhs[0], s * rhs[1], s * rhs[2], s * rhs[3], s * rhs[4], s * rhs[5], s * rhs[6], s * rhs[7], s * rhs[8], s * rhs[9], s * rhs[10], s * rhs[11], s * rhs[12], s * rhs[13], s * rhs[14], s * rhs[15]);
}

inline Vector4 operator*(const Vector4& v, const Matrix4& m)
{
	return Vector4(v.x * m[0] + v.y * m[1] + v.z * m[2] + v.w * m[3], v.x * m[4] + v.y * m[5] + v.z * m[6] + v.w * m[7], v.x * m[8] + v.y * m[9] + v.z * m[10] + v.w * m[11], v.x * m[12] + v.y * m[13] + v.z * m[14] + v.w * m[15]);
}

inline Vector3 operator*(const Vector3& v, const Matrix4& m)
{
	return Vector3(v.x * m[0] + v.y * m[1] + v.z * m[2], v.x * m[4] + v.y * m[5] + v.z * m[6], v.x * m[8] + v.y * m[9] + v.z * m[10]);
}

inline std::ostream& operator<<(std::ostream& os, const Matrix4& data)
{
	os << std::fixed << std::setprecision(5);
	os << "[" << std::setw(10) << data[0] << " " << std::setw(10) << data[4] << " " << std::setw(10) << data[8] << " " << std::setw(10) << data[12] << "]\n"
		<< "[" << std::setw(10) << data[1] << " " << std::setw(10) << data[5] << " " << std::setw(10) << data[9] << " " << std::setw(10) << data[13] << "]\n"
		<< "[" << std::setw(10) << data[2] << " " << std::setw(10) << data[6] << " " << std::setw(10) << data[10] << " " << std::setw(10) << data[14] << "]\n"
		<< "[" << std::setw(10) << data[3] << " " << std::setw(10) << data[7] << " " << std::setw(10) << data[11] << " " << std::setw(10) << data[15] << "]\n\n";
	os << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
	return os;
}
#pragma endregion