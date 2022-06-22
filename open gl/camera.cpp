#include "camera.h"
#define NULL_VECTOR Vector3dCreate(0.0f,0.0f,0.0f)
Vector3dStruct Vector3dCreate(GLfloat x, GLfloat y, GLfloat z)
{
	Vector3dStruct tmp;
	tmp.x = x;
	tmp.y = y;
	tmp.z = z;
	return tmp;
}
GLfloat LengthVector3d(Vector3dStruct * v)
{
	return (GLfloat)(sqrt(v->x * v->x + v->y * v->y + v->z * v->z));
}
Vector3dStruct NormalizeVector3d(Vector3dStruct v)
{
	Vector3dStruct res;
	float L = LengthVector3d(&v);
	if (L == 0.0f)
		return NULL_VECTOR;
	res.x = v.x / L;
	res.y = v.y / L;
	res.z = v.z / L;
	return res;
}
Vector3dStruct operator+ (Vector3dStruct v, Vector3dStruct u)
{
	Vector3dStruct res;
	res.x = v.x + u.x;
	res.y = v.y + u.y;
	res.z = v.z + u.z;
	return res;
}
Vector3dStruct operator- (Vector3dStruct v, Vector3dStruct u)
{
	Vector3dStruct res;
	res.x = v.x - u.x;
	res.y = v.y - u.y;
	res.z = v.z - u.z;
	return res;
}
Vector3dStruct operator* (Vector3dStruct v, float r)
{
	Vector3dStruct res;
	res.x = v.x * r;
	res.y = v.y * r;
	res.z = v.z * r;
	return res;
}
Vector3dStruct CrossProduct(Vector3dStruct * u, Vector3dStruct * v)
{
	Vector3dStruct resVector;
	resVector.x = u->y * v->z - u->z * v->y;
	resVector.y = u->z * v->x - u->x * v->z;
	resVector.z = u->x * v->y - u->y * v->x;
	return resVector;
}
float operator* (Vector3dStruct v, Vector3dStruct u)	//dot product
{
	return v.x * u.x + v.y * u.y + v.z * u.z;
}

Camera::Camera()
{
	Position = Vector3dCreate(0.0, 0.0, 0.0);
	View = Vector3dCreate(0.0, 0.0, -1.0);
	RightVector = Vector3dCreate(1.0, 0.0, 0.0);
	Up = Vector3dCreate(0.0, 1.0, 0.0);
	RotatedX = RotatedY = RotatedZ = 0.0;
}
void Camera::Move(Vector3dStruct Direction)
{
	Position = Position + Direction;
}
void Camera::RotateX(GLfloat Angle)
{
	RotatedX += Angle;
	//Rotate viewdir around the right vector:
	View = NormalizeVector3d(View * cosf(Angle * 22 / 7 / 180) + Up * sinf(Angle * 22 / 7 / 180));
	//now compute the new UpVector (by cross product)
	Up = CrossProduct(&View, &RightVector) * -1;
}
void Camera::RotateY(GLfloat Angle)
{
	RotatedY += Angle;
	//Rotate viewdir around the up vector:
	View = NormalizeVector3d(View * cosf(Angle * 22 / 7 / 180) - RightVector * sinf(Angle * 22 / 7 / 180));
	//now compute the new RightVector (by cross product)
	RightVector = CrossProduct(&View, &Up);
}
void Camera::RotateZ(GLfloat Angle)
{
	RotatedZ += Angle;
	//Rotate viewdir around the right vector:
	RightVector = NormalizeVector3d(RightVector * cosf(Angle * 22 / 7 / 180) + Up * sinf(Angle * 22 / 7 / 180));
	//now compute the new UpVector (by cross product)
	Up = CrossProduct(&View, &RightVector) * -1;
}
void Camera::Render()
{
	//calculate view point
	Vector3dStruct ViewPoint;
	ViewPoint = Position + View;
	//ViewPoint = Position;
	gluLookAt(Position.x, Position.y, Position.z, ViewPoint.x, ViewPoint.y, ViewPoint.z, Up.x, Up.y, Up.z);
}
void Camera::MoveForward(GLfloat Distance)
{
	Position = Position + (View * Distance);
	if (Position.z >= 30)
		Position.z -= 0.3;
}
void Camera::MoveRight(GLfloat Distance)
{
	Position = Position + (RightVector * Distance);
}
void Camera::MoveUpward(GLfloat Distance)
{
	Position = Position + (Up * Distance);
}
void Camera::SetRotateX(GLfloat Angle)
{
	View = Vector3dCreate(0.0, 0.0, -1.0);
	RightVector = Vector3dCreate(1.0, 0.0, 0.0);
	Up = Vector3dCreate(0.0, 1.0, 0.0);
	RotatedX = Angle;
}
