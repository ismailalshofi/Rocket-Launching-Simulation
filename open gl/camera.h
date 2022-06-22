#include <iostream>
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <glut.h>
#include <string>
#include <math.h>
#include "Texture.h"
#include "Model_3DS.h"

struct Vector3dStruct
{
	GLfloat x, y, z;
};
Vector3dStruct Vector3dCreate(GLfloat x, GLfloat y, GLfloat z);
Vector3dStruct NormalizeVector3d(Vector3dStruct v);
class Camera
{
public:
	Vector3dStruct View;
	Vector3dStruct RightVector;
	Vector3dStruct Up;
	Vector3dStruct Position;
	GLfloat RotatedX, RotatedY, RotatedZ;
	Camera();				//inits the values (Position: (0|0|0) Target: (0|0|-1) )
	void Render();	//executes some glRotates and a glTranslate command
	//Note: You should call glLoadIdentity before using Render
	void Move(Vector3dStruct Direction);
	void RotateX(GLfloat Angle);
	void RotateY(GLfloat Angle);
	void RotateZ(GLfloat Angle);
	void MoveForward(GLfloat Distance);
	void MoveUpward(GLfloat Distance);
	void MoveRight(GLfloat Distance);
	void SetRotateX(GLfloat Angle);
};