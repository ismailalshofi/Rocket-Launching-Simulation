#ifdef UNICODE
#undef UNICODE
#endif
#ifndef GLTEXTURE_H
#define GLTEXTURE_H
#include <windows.h>
#include <gl.h>
#include <glu.h>
#include <glaux.h>
#pragma comment(lib,"glaux.lib")
class GLTexture
{
public:
	char *texturename;
	unsigned int texture[1];
	int width;
	int height;
	void Use();
	void BuildColorTexture(unsigned char r, unsigned char g, unsigned char b);
	void LoadTGAResource(char *name);
	void LoadBMPResource(char *name);
	void LoadFromResource(char *name);
	void LoadTGA(char *name);
	void LoadBMP(char *name);
	void Load(char *name);
	GLTexture();
	virtual ~GLTexture();
};
#endif GLTEXTURE_H
