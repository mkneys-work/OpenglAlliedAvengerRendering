#ifndef CAMERA_H_
#define CAMERA_H_

#include "vector3.h"
#include "matrix3x3.h"
#include "matrix4x4.h"

class Camera
{
public:
	Camera() { }
	Camera( const int width, const int height, const float fov_y,
		const Vector3 view_from, const Vector3 view_at );

	Vector3 getViewFrom() const;
	Matrix3x3 getMCW() const;
	float getFocalLenght() const;

	void setFovY( const float fov_y );

	void update();

	int width{ 640 };
	int height{ 480 };

	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;

	float nearProjection = 1.0;
	float farProjection = 1000.0;
private:
	float _fovY{ 0.785f };
	
	Vector3 _viewFrom;
	Vector3 _viewAt;
	Vector3 _up{ Vector3(0.0f, 0.0f, 1.0f) };

	float _fy{ 1.0f };

	Matrix3x3 _MCW;
};

#endif
