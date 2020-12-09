#ifndef MATRIX_3X3_H_
#define MATRIX_3X3_H_

#include "vector3.h"

class Matrix3x3
{
public:
	Matrix3x3();
	Matrix3x3( const float m00, const float m01, const float m02,
		const float m10, const float m11, const float m12,
		const float m20, const float m21, const float m22 );
	Matrix3x3( const Vector3 basis_x, const Vector3 basis_y, const Vector3 basis_z );		

	Matrix3x3 Transpose() const;
	
	void set( const int row, const int column, const float value );
	float get( const int row, const int column ) const;

	Vector3 column( const int column ) const;
	Vector3 row( const int row ) const;

	float * data();
	
	friend Vector3 operator*( const Matrix3x3 & a, const Vector3 & b );
	friend Matrix3x3 operator*( const Matrix3x3 & a, const Matrix3x3 & b );	

private:
	union
	{
		struct
		{
			float _m00;
			float _m01;
			float _m02;

			float _m10;
			float _m11;
			float _m12;

			float _m20;
			float _m21;
			float _m22;
		};

		float _data[3 * 3];
	};
};

#endif
