#include "pch.h"
#include "matrix4x4.h"
#include "utils.h"

Matrix4x4::Matrix4x4()
{
	for ( int r = 0; r < 4; ++r )
	{
		for ( int c = 0; c < 4; ++c )
		{
			data_[c + r * 4] = ( ( r == c ) ? 1.0f : 0.0f );
		}
	}
}

Matrix4x4::Matrix4x4( const float m00, const float m01, const float m02, const float m03, 
		const float m10, const float m11, const float m12, const float m13, 
		const float m20, const float m21, const float m22, const float m23,
		const float m30, const float m31, const float m32, const float m33 )
{
	_m00 = m00;
	_m01 = m01;
	_m02 = m02;
	_m03 = m03;

	_m10 = m10;
	_m11 = m11;
	_m12 = m12;
	_m13 = m13;

	_m20 = m20;
	_m21 = m21;
	_m22 = m22;
	_m23 = m23;

	_m30 = m30;
	_m31 = m31;
	_m32 = m32;
	_m33 = m33;
}

Matrix4x4::Matrix4x4( const Vector3 & axis_x, const Vector3 & axis_y, const Vector3 & axis_z,
	const Vector3 & view_from )
{
	_m00 = axis_x.x;
	_m01 = axis_y.x;
	_m02 = axis_z.x;
	_m03 = view_from.x;

	_m10 = axis_x.y;
	_m11 = axis_y.y;
	_m12 = axis_z.y;
	_m13 = view_from.y;

	_m20 = axis_x.z;
	_m21 = axis_y.z;
	_m22 = axis_z.z;
	_m23 = view_from.z;

	_m30 = 0.0f;
	_m31 = 0.0f;
	_m32 = 0.0f;
	_m33 = 1.0f;
}

void Matrix4x4::transpose()
{	
	float tmp = _m01;
	_m01 = _m10;
	_m10 = tmp;

	tmp = _m02;
	_m02 = _m20;
	_m20 = tmp;

	tmp = _m03;
	_m03 = _m30;
	_m30 = tmp;

	tmp = _m12;
	_m12 = _m21;
	_m21 = tmp;	

	tmp = _m13;
	_m13 = _m31;
	_m31 = tmp;

	tmp = _m23;
	_m23 = _m32;
	_m32 = tmp;
}

void Matrix4x4::euclideanInverse()
{
	const float m03 = -_m00 * _m03 - _m10 * _m13 - _m20 * _m23;
	const float m13 = -_m01 * _m03 - _m11 * _m13 - _m21 * _m23;
	const float m23 = -_m02 * _m03 - _m12 * _m13 - _m22 * _m23;

	_m03 = m03;
	_m13 = m13;
	_m23 = m23;

	float tmp = _m01;
	_m01 = _m10;
	_m10 = tmp;

	tmp = _m02;
	_m02 = _m20;
	_m20 = tmp;

	tmp = _m12;
	_m12 = _m21;
	_m21 = tmp;	
}

Matrix4x4 Matrix4x4::euclideanInverse( Matrix4x4 m )
{
	Matrix4x4 mInv;

	memcpy( mInv.getData(), m.getData(), sizeof( mInv.data_ ) );
	
	mInv._m03 = -m._m00 * m._m03 - m._m10 * m._m13 - m._m20 * m._m23;
	mInv._m13 = -m._m01 * m._m03 - m._m11 * m._m13 - m._m21 * m._m23;
	mInv._m23 = -m._m02 * m._m03 - m._m12 * m._m13 - m._m22 * m._m23;	

	swap<float>( mInv._m01, mInv._m10 );
	swap<float>( mInv._m02, mInv._m20 );
	swap<float>( mInv._m12, mInv._m21 );	

	return mInv;
}

void Matrix4x4::set( const int row, const int column, const float value )
{
	assert( row >= 0 && row < 4 && column >= 0 && column < 4 );
	data_[column + row * 4] = value;
}

float Matrix4x4::get( const int row, const int column ) const
{
	assert( row >= 0 && row < 4 && column >= 0 && column < 4 );
	return data_[column + row * 4];
}

float * Matrix4x4::getData()
{
	return &data_[0];
}

Matrix3x3 Matrix4x4::so3() const
{
	return Matrix3x3( _m00, _m01, _m02,
		_m10, _m11, _m12,
		_m20, _m21, _m22 );
}

Vector3 Matrix4x4::tr3() const
{
	return Vector3( _m03, _m13, _m23 );
}

void Matrix4x4::so3( const Matrix3x3 & m )
{
	for ( int i = 0; i < 3; ++i )
	{
		for ( int j = 0; j < 3; ++j )
		{
			set( i, j, m.get( i, j ) );
		}
	}	
}

void Matrix4x4::tr3(const Vector3 & v)
{
	_m03 = v.x;
	_m13 = v.y;
	_m23 = v.z;
}

std::string Matrix4x4::toString() const
{		
	return std::to_string( _m00 ) + " " + std::to_string( _m01 ) + " " + std::to_string( _m02 ) + " " + std::to_string( _m03 ) + "\n" +
		std::to_string( _m10 ) + " " + std::to_string( _m11 ) + " " + std::to_string( _m12 ) + " " + std::to_string( _m13 ) + "\n" +
		std::to_string( _m20 ) + " " + std::to_string( _m21 ) + " " + std::to_string( _m22 ) + " " + std::to_string( _m23 ) + "\n" +
		std::to_string( _m30 ) + " " + std::to_string( _m31 ) + " " + std::to_string( _m32 ) + " " + std::to_string( _m33 ) + "\n";
}

bool Matrix4x4::operator==( const Matrix4x4 & m ) const
{
	for ( int j = 0; j < 4; ++j )
	{
		for ( int i = 0; i < 4; ++i )
		{
			if ( get( i, j ) != m.get( i, j ) ) return false;
		}
	}

	return true;
}

Matrix4x4 operator*( const Matrix4x4 & a, const Matrix4x4 & b )
{
	return Matrix4x4( a._m00 * b._m00 + a._m01 * b._m10 + a._m02 * b._m20 + a._m03 * b._m30,
		a._m00 * b._m01 + a._m01 * b._m11 + a._m02 * b._m21 + a._m03 * b._m31,
		a._m00 * b._m02 + a._m01 * b._m12 + a._m02 * b._m22 + a._m03 * b._m32,
		a._m00 * b._m03 + a._m01 * b._m13 + a._m02 * b._m23 + a._m03 * b._m33,

		a._m10 * b._m00 + a._m11 * b._m10 + a._m12 * b._m20 + a._m13 * b._m30,
		a._m10 * b._m01 + a._m11 * b._m11 + a._m12 * b._m21 + a._m13 * b._m31,
		a._m10 * b._m02 + a._m11 * b._m12 + a._m12 * b._m22 + a._m13 * b._m32,
		a._m10 * b._m03 + a._m11 * b._m13 + a._m12 * b._m23 + a._m13 * b._m33,

		a._m20 * b._m00 + a._m21 * b._m10 + a._m22 * b._m20 + a._m23 * b._m30,
		a._m20 * b._m01 + a._m21 * b._m11 + a._m22 * b._m21 + a._m23 * b._m31,
		a._m20 * b._m02 + a._m21 * b._m12 + a._m22 * b._m22 + a._m23 * b._m32,
		a._m20 * b._m03 + a._m21 * b._m13 + a._m22 * b._m23 + a._m23 * b._m33,

		a._m30 * b._m00 + a._m31 * b._m10 + a._m32 * b._m20 + a._m33 * b._m30,
		a._m30 * b._m01 + a._m31 * b._m11 + a._m32 * b._m21 + a._m33 * b._m31,
		a._m30 * b._m02 + a._m31 * b._m12 + a._m32 * b._m22 + a._m33 * b._m32,
		a._m30 * b._m03 + a._m31 * b._m13 + a._m32 * b._m23 + a._m33 * b._m33 );
}
