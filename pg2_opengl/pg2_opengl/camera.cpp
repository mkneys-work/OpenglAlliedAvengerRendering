#include "pch.h"
#include "camera.h"

Camera::Camera( const int width, const int height, const float fovY,
	const Vector3 viewFrom, const Vector3 viewAt )
{
	this->width = width;
	this->height = height;
	_fovY = fovY;

	_viewFrom = viewFrom;
	_viewAt = viewAt;

	update();
}

Vector3 Camera::getViewFrom() const
{
	return _viewFrom;
}

Matrix3x3 Camera::getMCW() const
{
	return _MCW;
}

float Camera::getFocalLenght() const
{
	return _fy;
}

void Camera::setFovY( const float fov_y )
{
	assert( fov_y > 0.0 );

	_fovY = fov_y;
}

void Camera::update()
{
	_fy = height / ( 2.0f * tanf(_fovY * 0.5f) );

	Vector3 z_c = _viewFrom - _viewAt;
	z_c.Normalize();
	Vector3 x_c = _up.CrossProduct(z_c);
	x_c.Normalize();
	Vector3 y_c = z_c.CrossProduct(x_c);
	y_c.Normalize();
	
	float aspectRatio = width / float(height);
	viewMatrix = Matrix4x4(x_c, y_c, z_c, _viewFrom);
	viewMatrix.euclideanInverse();
	float f = 1.0f;

	projectionMatrix.set(0, 0, 1 / (tanf(_fovY * 0.5) * aspectRatio));
	projectionMatrix.set(1, 1, f * 1 / tanf(_fovY * 0.5));

	projectionMatrix.set(2, 2, (farProjection + nearProjection) / (nearProjection - farProjection));
	projectionMatrix.set(2, 3, (2 * farProjection * nearProjection) / (nearProjection - farProjection));
	projectionMatrix.set(3, 2, -1);

	_MCW = Matrix3x3(x_c, y_c, z_c);
}
