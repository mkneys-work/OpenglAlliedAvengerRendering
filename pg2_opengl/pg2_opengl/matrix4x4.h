#ifndef MATRIX_4X4_H_
#define MATRIX_4X4_H_

#include "matrix3x3.h"

class Matrix4x4
{
public:
	Matrix4x4();
	Matrix4x4( const float m00, const float m01, const float m02, const float m03,
		const float m10, const float m11, const float m12, const float m13,
		const float m20, const float m21, const float m22, const float m23,
		const float m30, const float m31, const float m32, const float m33 );

	Matrix4x4( const Vector3 & axis_x, const Vector3 & axis_y, const Vector3 & axis_z,
		const Vector3 & view_from );

	void transpose();
	void euclideanInverse();
	static Matrix4x4 euclideanInverse( Matrix4x4 m );
	void set( const int row, const int column, const float value );
	float get( const int row, const int column ) const;
	float * getData();

	Matrix3x3 so3() const;
	Vector3 tr3() const;
	void so3(const Matrix3x3 & m);
	void tr3( const Vector3 & v );	

	std::string toString() const;
	bool operator==( const Matrix4x4 & m ) const;

	friend Matrix4x4 operator*( const Matrix4x4 & a, const Matrix4x4 & b );

private:
#pragma warning( push )
#pragma warning ( disable : 4201 )
	union
	{
		struct
		{
			float _m00;
			float _m01;
			float _m02;
			float _m03;

			float _m10;
			float _m11;
			float _m12;
			float _m13;

			float _m20;
			float _m21;
			float _m22;
			float _m23;

			float _m30;
			float _m31;
			float _m32;
			float _m33;
		};

		float data_[4 * 4];
	};
#pragma warning( pop ) 
};

typedef Matrix4x4 Matrix4;

template<class T>
class Matrix
{
public:
	Matrix( const int rows, const int cols )
	{
		assert( ( rows > 0 ) && ( cols > 0 ) );
		
		data_ = new T[rows*cols];
		rows_ = rows;
		cols_ = cols;
	}

	~Matrix()
	{
		SAFE_DELETE_ARRAY( data_ );
		rows_ = 0;
		cols_ = 0;
	}

	T * operator()( const int row )
	{
		assert( ( row >= 0 ) && ( row < rows_ ) );

		return &data_[row * cols_];
	}

	T & operator()( const int row, const int col )
	{
		assert( ( row >= 0 ) && ( row < rows_ ) );
		assert( ( col >= 0 ) && ( col < cols_ ) );

		return data_[row * cols_ + col];
	}

private:
	T * data_;
	int rows_;
	int cols_;
};

#endif
