#include "pch.h"
#include "matrix3x3.h"

Matrix3x3::Matrix3x3()
{
	for ( int r = 0; r < 3; ++r )
	{
		for ( int c = 0; c < 3; ++c )
		{
			_data[c + r * 3] = ( ( r == c ) ? 1.0f : 0.0f );
		}
	}
}

Matrix3x3::Matrix3x3( const float m00, const float m01, const float m02,
		const float m10, const float m11, const float m12,
		const float m20, const float m21, const float m22 )
{
	_m00 = m00;
	_m01 = m01;
	_m02 = m02;

	_m10 = m10;
	_m11 = m11;
	_m12 = m12;	

	_m20 = m20;
	_m21 = m21;
	_m22 = m22;
}

Matrix3x3::Matrix3x3( const Vector3 basis_x, const Vector3 basis_y, const Vector3 basis_z )
{
	_m00 = basis_x.x;
	_m01 = basis_y.x;
	_m02 = basis_z.x;

	_m10 = basis_x.y;
	_m11 = basis_y.y;
	_m12 = basis_z.y;

	_m20 = basis_x.z;
	_m21 = basis_y.z;
	_m22 = basis_z.z;
}

Matrix3x3 Matrix3x3::Transpose() const
{	
	return Matrix3x3( _m00, _m10, _m20,
		_m01, _m11, _m21,
		_m02, _m12, _m22 );
}

void Matrix3x3::set( const int row, const int column, const float value )
{
	assert( row >= 0 && row < 3 && column >= 0 && column < 3 );
	_data[column + row * 3] = value;
}

float Matrix3x3::get( const int row, const int column ) const
{
	assert( row >= 0 && row < 3 && column >= 0 && column < 3 );
	return _data[column + row * 3];
}

Vector3 Matrix3x3::column( const int column ) const
{
	return Vector3( get( 0, column ), get( 1, column ), get( 2, column ) );
}

Vector3 Matrix3x3::row( const int row ) const
{
	return Vector3( get( row, 0 ), get( row, 1 ), get( row, 2 ) );
}

float * Matrix3x3::data()
{
	return &(_data[0]);
}

Vector3 operator*( const Matrix3x3 & a, const Vector3 & b )
{
	return Vector3( a._m00 * b.x + a._m01 * b.y + a._m02 * b.z,
		a._m10 * b.x + a._m11 * b.y + a._m12 * b.z,
		a._m20 * b.x + a._m21 * b.y + a._m22 * b.z );
}

Matrix3x3 operator*( const Matrix3x3 & a, const Matrix3x3 & b )
{
	return Matrix3x3( a._m00 * b._m00 + a._m01 * b._m10 + a._m02 * b._m20,
		a._m00 * b._m01 + a._m01 * b._m11 + a._m02 * b._m21,
		a._m00 * b._m02 + a._m01 * b._m12 + a._m02 * b._m22,		

		a._m10 * b._m00 + a._m11 * b._m10 + a._m12 * b._m20,
		a._m10 * b._m01 + a._m11 * b._m11 + a._m12 * b._m21,
		a._m10 * b._m02 + a._m11 * b._m12 + a._m12 * b._m22,		

		a._m20 * b._m00 + a._m21 * b._m10 + a._m22 * b._m20,
		a._m20 * b._m01 + a._m21 * b._m11 + a._m22 * b._m21,
		a._m20 * b._m02 + a._m21 * b._m12 + a._m22 * b._m22 );
}
