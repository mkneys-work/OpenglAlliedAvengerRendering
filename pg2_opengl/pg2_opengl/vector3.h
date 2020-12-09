#ifndef VECTOR3_H_
#define VECTOR3_H_

struct Vector3
{
public:
	union
	{
		struct
		{
			float x;
			float y;
			float z;
		};

		float data[3];
	};

	Vector3() : x( 0 ), y( 0 ), z( 0 ) { }	
	Vector3( const float x, const float y, const float z ) : x( x ), y( y ), z( z ) { }
	Vector3( const float * v );

	float L2Norm() const;
	float SqrL2Norm() const;
	float Normalize();
	Vector3 CrossProduct( const Vector3 & v ) const;	
	Vector3 Abs() const;
	Vector3 Max( const float a = 0 ) const;
	float DotProduct( const Vector3 & v ) const;
	char LargestComponent( const bool absolute_value = false );	

	void Print();

	friend Vector3 operator-( const Vector3 & v );

	friend Vector3 operator+( const Vector3 & u, const Vector3 & v );
	friend Vector3 operator-( const Vector3 & u, const Vector3 & v );

	friend Vector3 operator*( const Vector3 & v, const float a );	
	friend Vector3 operator*( const float a, const Vector3 & v );
	friend Vector3 operator*( const Vector3 & u, const Vector3 & v );

	friend Vector3 operator/( const Vector3 & v, const float a );

	friend void operator+=( Vector3 & u, const Vector3 & v );
	friend void operator-=( Vector3 & u, const Vector3 & v );
	friend void operator*=( Vector3 & v, const float a );
	friend void operator/=( Vector3 & v, const float a );		
};

#endif
