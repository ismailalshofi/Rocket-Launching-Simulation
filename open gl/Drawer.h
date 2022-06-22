#pragma once
class Drawer
{
public:
	Drawer();
	void skybox(int);
	void spacebox(int);
	void drawTree(Model_3DS *tree , GLTexture , GLTexture);
	//void drawGround(Model_3DS*, GLTexture, GLTexture);
	void drawRocket(Model_3DS *rocket , GLTexture r);

	virtual ~Drawer();
};

