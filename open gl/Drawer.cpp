#include "Drawer.h"
#include <iostream>
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <glut.h>
#include <string>
#include <math.h>
#include "Texture.h"
#include "Model_3DS.h"
#include "camera.h"

Drawer::Drawer()
{
}

void Drawer::skybox(int sky){
	glBindTexture(GL_TEXTURE_2D, sky); // front
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex3d(-300, 0, -300);
	glTexCoord2d(1, 0);
	glVertex3d(300, 0, -300);
	glTexCoord2d(1, 1);
	glVertex3d(300, 600, -300);
	glTexCoord2d(0, 1);
	glVertex3d(-300, 600, -300);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, sky); // back
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex3d(-300, 0, 300);
	glTexCoord2d(1, 0);
	glVertex3d(300, 0, 300);
	glTexCoord2d(1, 1);
	glVertex3d(300, 600, 300);
	glTexCoord2d(0, 1);
	glVertex3d(-300, 600, 300);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, sky); // left
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex3d(-300, 0, 300);
	glTexCoord2d(1, 0);
	glVertex3d(-300, 0, -300);
	glTexCoord2d(1, 1);
	glVertex3d(-300, 600, -300);
	glTexCoord2d(0, 1);
	glVertex3d(-300, 600, 300);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, sky); // right
	glBegin(GL_QUADS);
	glTexCoord2d(1, 0);
	glVertex3d(300, 0, 300);
	glTexCoord2d(0, 0);
	glVertex3d(300, 0, -300);
	glTexCoord2d(0, 1);
	glVertex3d(300, 600, -300);
	glTexCoord2d(1, 1);
	glVertex3d(300, 600, 300);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, sky); // top
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex3d(-300, 600, -300);
	glTexCoord2d(1, 0);
	glVertex3d(300, 600, -300);
	glTexCoord2d(1, 1);
	glVertex3d(300, 600, 300);
	glTexCoord2d(0, 1);
	glVertex3d(-300, 600, 300);
	glEnd();
}

void Drawer::spacebox(int space){
	
		glBindTexture(GL_TEXTURE_2D, space); // front
		glBegin(GL_QUADS);
		glTexCoord2d(0, 0);
		glVertex3d(-500, 0, -500);
		glTexCoord2d(1, 0);
		glVertex3d(500, 0, -500);
		glTexCoord2d(1, 1);
		glVertex3d(500, 800, -500);
		glTexCoord2d(0, 1);
		glVertex3d(-500, 800, -500);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, space); // back
		glBegin(GL_QUADS);
		glTexCoord2d(0, 0);
		glVertex3d(-500, 0, 500);
		glTexCoord2d(1, 0);
		glVertex3d(500, 0, 500);
		glTexCoord2d(1, 1);
		glVertex3d(500, 800, 500);
		glTexCoord2d(0, 1);
		glVertex3d(-500, 800, 500);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, space); // left
		glBegin(GL_QUADS);
		glTexCoord2d(0, 0);
		glVertex3d(-500, 0, 500);
		glTexCoord2d(1, 0);
		glVertex3d(-500, 0, -500);
		glTexCoord2d(1, 1);
		glVertex3d(-500, 800, -500);
		glTexCoord2d(0, 1);
		glVertex3d(-500, 800, 500);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, space); // right
		glBegin(GL_QUADS);
		glTexCoord2d(1, 0);
		glVertex3d(500, 0, 500);
		glTexCoord2d(0, 0);
		glVertex3d(500, 0, -500);
		glTexCoord2d(0, 1);
		glVertex3d(500, 800, -500);
		glTexCoord2d(1, 1);
		glVertex3d(500, 800, 500);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, space); // top
		glBegin(GL_QUADS);
		glTexCoord2d(0, 0);
		glVertex3d(-500, 800, -500);
		glTexCoord2d(1, 0);
		glVertex3d(500, 800, -500);
		glTexCoord2d(1, 1);
		glVertex3d(500, 800, 500);
		glTexCoord2d(0, 1);
		glVertex3d(-500, 800, 500);
		glEnd();

	
}

void Drawer::drawTree(Model_3DS *tree , GLTexture leaf , GLTexture wood)
{
	tree->Materials[0].tex = wood;
	tree->Materials[1].tex = wood;
	tree->Materials[2].tex = leaf;
	tree->Materials[3].tex = leaf;
	tree->Materials[4].tex = leaf;
	tree->scale = 1;
	tree->Draw();
	tree->visible;
}

/*void Drawer::drawGround(Model_3DS *tree, GLTexture leaf, GLTexture wood){
	glColor3f(1, 0, 0);
	glBegin(GL_QUADS);
	glVertex3d(-300, 0, -300);
	glVertex3d(300, 0, -300);
	glVertex3d(300, 0, 300);
	glVertex3d(-300, 0, 300);
	glEnd();
	glColor3f(1, 1, 1);
	glTranslated(0, 0, -50);
	for (int i = 0; i < 5; i++){
		drawTree(tree , leaf , wood);
		glTranslated(20, 0, 0);
	}

	glTranslated(-100, 0, -50);
	for (int i = 0; i < 5; i++){
		drawTree(tree, leaf, wood);
		glTranslated(20, 0, 0);
	}

	glTranslated(-100, 0, -50);
	for (int i = 0; i < 5; i++){
		drawTree(tree, leaf, wood);
		glTranslated(20, 0, 0);
	}

}
*/
void Drawer::drawRocket(Model_3DS *rocket, GLTexture r){
	rocket->pos.x = 0;
	rocket->pos.y = -2;
	rocket->pos.z = 0;
	rocket->Materials[0].tex = r;
	rocket->scale = 5;
	rocket->Draw();
	rocket->visible;
}
Drawer::~Drawer()
{
}
