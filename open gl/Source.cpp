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
using namespace std;
HDC			hDC = NULL;
HGLRC		hRC = NULL;
HWND		hWnd = NULL;
HINSTANCE	hInstance;
float mouseX = 0, mouseY = 0;
bool isLClicked = false, isRClicked = false;
bool	keys[256];
bool	active = TRUE;
bool	fullscreen = TRUE;
int InitGL(GLvoid);
GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
GLvoid KillGLWindow(GLvoid);
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag);
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
int main(HINSTANCE hlnstance, HINSTANCE hPrevlnstance, LPSTR lpCmdLine, int nCmdShow);

//////////////Drawing Variables////////////////
Camera Ca;
int space, sky, ground;
Model_3DS *rocket = new Model_3DS();
Model_3DS *tree = new Model_3DS();
GLTexture rocketTexture, leaf, wood;

//////////////Physics Variables////////////////
struct Position
{
	float x, y, z;
};
Position CreatePosition(float x, float y, float z)
{
	Position tmp;
	tmp.x = x;
	tmp.y = y;
	tmp.z = z;
	return tmp;
}
Position CopyPosition(Position p)
{
	Position tmp = CreatePosition(p.x, p.y, p.z);
	return tmp;
}
void PrintPosition(Position P) {
	cout << P.x << "  " << P.y << "  " << P.z << endl;
}

struct Vector {
	float a, b, c;
};
Vector CreateVector(float a, float b, float c)
{
	Vector tmp;
	tmp.a = a;
	tmp.b = b;
	tmp.c = c;
	return tmp;
}
Vector CopyVector(Vector V)
{
	Vector tmp = CreateVector(V.a, V.b, V.c);
	return tmp;
}
void PrintVector(Vector V) {
	cout << V.a << "  " << V.b << "  " << V.c << endl;
}
void FR_Calculate(), W_Calculate(), Power_Calculate(), V_Calculate(), SF_Calculate(), A_Calculate(), AX_AY_AZ_Calculate(), New_Distance_Calculate(), V_Calculate();
Position Rocket_Pos;
Vector V_Vector, W_Vector, FR_Vector, Power_Vector,  SF_Vector, A_Vector, PH_Vector;
float V, V0 = 0 , W, FR, Power = 30000, PH = 0, A, Full_M, Rocket_M = 500, Diesel_M, Diesel_Amount, R = 0.5, VX, VY, VZ, AX, AY, AZ, DX, DY, DZ;
float PI = 3.143, G = 9.81, Time = 0.0, a = 0, r = 5, angle , dist = 0;
bool start = false, in_Sky = true;
int option;
void FR_Calculate()
{
	if (in_Sky){
		FR_Vector = CreateVector(0, 0, 0);
		FR_Vector.a = -1 * V_Vector.a;
		FR_Vector.b = -1 * V_Vector.b;
		FR_Vector.c = -1 * V_Vector.c;
		FR = 0.5 * 0.04 * 1.293 * PI * R * R * V * V;
		if (sqrt(FR_Vector.a *FR_Vector.a + FR_Vector.b * FR_Vector.b + FR_Vector.c * FR_Vector.c)){
			float k = FR / sqrt(FR_Vector.a * FR_Vector.a + FR_Vector.b * FR_Vector.b + FR_Vector.c * FR_Vector.c);
			FR_Vector = CreateVector(k * FR_Vector.a, k * FR_Vector.b, k * FR_Vector.c);
		}
		else
			FR_Vector = CreateVector(0, 0, 0);
	}
	else
		FR_Vector = CreateVector(0, 0, 0);
}
void Diesel_M_Calculate()
{
	Diesel_M = (Diesel_Amount * 850) / 1000;
}
void W_Calculate()
{
	W_Vector = CreateVector(0, -1 * Full_M * G, 0);
}
void PH_Calculate()
{
	PH_Vector = CreateVector(PH, 0, 0);
}
void Power_Calculate()
{
	Power_Vector = CreateVector(0, Power, 0);
	if (Full_M <= Rocket_M){
		if (Power > 0){
			Power -= 300 ;
			PH += 0.01;
		}
		else{
			Power = 0;
			PH += 0.01;
		}
	}
}
void SF_Calculate() 
{
	if (in_Sky)
		SF_Vector = CreateVector(V_Vector.a + FR_Vector.a + W_Vector.a + PH_Vector.a + Power_Vector.a, V_Vector.b + FR_Vector.b + W_Vector.b + PH_Vector.b + Power_Vector.b, V_Vector.c + FR_Vector.c + W_Vector.c + PH_Vector.c +  Power_Vector.c);
	else
		SF_Vector = CreateVector(Power_Vector.a, Power_Vector.b, Power_Vector.c);
}
void A_Calculate() 
{
	A_Vector = CreateVector(SF_Vector.a / Full_M, SF_Vector.b / Full_M, SF_Vector.c / Full_M);
	if (Full_M > Rocket_M){
		Full_M -= 2;
		PH += 0.0001;
	}
}
void AX_AY_AZ_Calculate()
{
	AX = A_Vector.a;
	AY = A_Vector.b;
	AZ = A_Vector.c;
}
void New_Distance_Calculate()
{
	DX = 0.5 * AX * Time * Time + VX * Time;
	DY = 0.5 * AY * Time * Time + VY * Time;
	DZ = 0.5 * AZ * Time * Time + VZ * Time;
	Rocket_Pos = CreatePosition(Rocket_Pos.x + DX, Rocket_Pos.y + DY, Rocket_Pos.z + DZ);
}
void V_Calculate()
{
	VX = V0 + AX * Time;
	VY = V0 + AY * Time;
	VZ = V0 + AZ * Time;
	V_Vector = CreateVector(VX, VY, VZ);
	V = sqrt(VX*VX + VY*VY + VZ*VZ);
}
void Angle_Calculate()
{
	if (in_Sky){
		a = atan2(PH , r);
		angle -= a;
	}
}
void Check_IF_In_Sky()
{
	if (Rocket_Pos.y / 200 < 2000)
		in_Sky = true;
	else
		in_Sky = false;
}
void Calculate_Results() {
	if (Diesel_M > 0){
		FR_Calculate();
		W_Calculate();
		PH_Calculate();
		Power_Calculate();
		SF_Calculate();
		A_Calculate();
		AX_AY_AZ_Calculate();
		New_Distance_Calculate();
		V_Calculate();
		Angle_Calculate();
		Check_IF_In_Sky();
		cout << "=================================================";
		cout << endl;
		cout << "Full Mass : " << Full_M << endl;
		cout << "W : "; PrintVector(W_Vector);
		cout << "FR : "; PrintVector(FR_Vector);
		cout << "PH : "; PrintVector(PH_Vector);
		cout << "Power : "; PrintVector(Power_Vector);
		cout << "SF : "; PrintVector(SF_Vector);
		cout << "A  : "; PrintVector(A_Vector);
		cout << "V : "; PrintVector(V_Vector);
		cout << "Angle : " << angle << endl;
		cout << "Rocket Position :" << Rocket_Pos.x << "  " << Rocket_Pos.y / 200 << "  " << Rocket_Pos.z << endl;
		cout << endl;
		cout << "=================================================";
	}
	
}

void drawTree(){
	tree->Materials[0].tex = wood;
	tree->Materials[1].tex = wood;
	tree->Materials[2].tex = leaf;
	tree->Materials[3].tex = leaf;
	tree->Materials[4].tex = leaf;
	tree->Draw();
	tree->visible;
}
void Keyboard_And_Camera_And_Light()
{
	if (keys['W'])
		Ca.MoveForward(1);
	if (keys['S'])
		Ca.MoveForward(-1);
	if (keys['D'])
		Ca.MoveRight(1);
	if (keys['A'])
		Ca.MoveRight(-1);
	if (keys['E'])
		Ca.MoveUpward(1);
	if (keys['Q'])
		Ca.MoveUpward(-1);

	if (keys[VK_DOWN])
		Ca.RotateX(-1);
	if (keys[VK_UP])
		Ca.RotateX(1);
	if (keys[VK_RIGHT])
		Ca.RotateY(-1);
	if (keys[VK_LEFT])
		Ca.RotateY(1);
	if (keys['I'])
		Ca.RotateZ(-1);
	if (keys['Y'])
		Ca.RotateZ(1);
	if (keys['R'])
		gluLookAt(-10, 0, 0, -40, 0, 0, 0, 1, 0);
	if (keys['P'])
		start = true;
	Ca.Render();

}
void spacebox()
{
	glBindTexture(GL_TEXTURE_2D, space); // front
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex3d(-2000, 0, -2000);
	glTexCoord2d(1, 0);
	glVertex3d(2000, 0, -2000);
	glTexCoord2d(1, 1);
	glVertex3d(2000, 4000, -2000);
	glTexCoord2d(0, 1);
	glVertex3d(-2000, 4000, -2000);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, space); // back
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex3d(-2000, 0, 2000);
	glTexCoord2d(1, 0);
	glVertex3d(2000, 0, 2000);
	glTexCoord2d(1, 1);
	glVertex3d(2000, 4000, 2000);
	glTexCoord2d(0, 1);
	glVertex3d(-2000, 4000, 2000);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, space); // left
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex3d(-2000, 0, 2000);
	glTexCoord2d(1, 0);
	glVertex3d(-2000, 0, -2000);
	glTexCoord2d(1, 1);
	glVertex3d(-2000, 4000, -2000);
	glTexCoord2d(0, 1);
	glVertex3d(-2000, 4000, 2000);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, space); // right
	glBegin(GL_QUADS);
	glTexCoord2d(1, 0);
	glVertex3d(2000, 0, 2000);
	glTexCoord2d(0, 0);
	glVertex3d(2000, 0, -2000);
	glTexCoord2d(0, 1);
	glVertex3d(2000, 4000, -2000);
	glTexCoord2d(1, 1);
	glVertex3d(2000, 4000, 2000);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, space); // top
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex3d(-2000, 4000, -2000);
	glTexCoord2d(1, 0);
	glVertex3d(2000, 4000, -2000);
	glTexCoord2d(1, 1);
	glVertex3d(2000, 4000, 2000);
	glTexCoord2d(0, 1);
	glVertex3d(-2000, 4000, 2000);
	glEnd();

}
void skybox()
{
	glBindTexture(GL_TEXTURE_2D, sky); // front
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex3d(-1000, 0, -1000);
	glTexCoord2d(1, 0);
	glVertex3d(1000, 0, -1000);
	glTexCoord2d(1, 1);
	glVertex3d(1000, 2000, -1000);
	glTexCoord2d(0, 1);
	glVertex3d(-1000, 2000, -1000);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, sky); // back
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex3d(-1000, 0, 1000);
	glTexCoord2d(1, 0);
	glVertex3d(1000, 0, 1000);
	glTexCoord2d(1, 1);
	glVertex3d(1000, 2000, 1000);
	glTexCoord2d(0, 1);
	glVertex3d(-1000, 2000, 1000);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, sky); // left
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex3d(-1000, 0, 1000);
	glTexCoord2d(1, 0);
	glVertex3d(-1000, 0, -1000);
	glTexCoord2d(1, 1);
	glVertex3d(-1000, 2000, -1000);
	glTexCoord2d(0, 1);
	glVertex3d(-1000, 2000, 1000);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, sky); // right
	glBegin(GL_QUADS);
	glTexCoord2d(1, 0);
	glVertex3d(1000, 0, 1000);
	glTexCoord2d(0, 0);
	glVertex3d(1000, 0, -1000);
	glTexCoord2d(0, 1);
	glVertex3d(1000, 2000, -1000);
	glTexCoord2d(1, 1);
	glVertex3d(1000, 2000, 1000);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, sky); // top
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex3d(-1000, 2000, -1000);
	glTexCoord2d(1, 0);
	glVertex3d(1000, 2000, -1000);
	glTexCoord2d(1, 1);
	glVertex3d(1000, 2000, 1000);
	glTexCoord2d(0, 1);
	glVertex3d(-1000, 2000, 1000);
	glEnd();

}
void drawGround(){
	
	glBindTexture(GL_TEXTURE_2D, ground);
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex3d(-1000, 0, -1000);
	glTexCoord2d(1, 0);
	glVertex3d(1000, 0, -1000);
	glTexCoord2d(1, 1);
	glVertex3d(1000, 0, 1000);
	glTexCoord2d(0, 1);
	glVertex3d(-1000, 0, 1000);
	glEnd();
	glColor3f(1, 1, 1);
	glTranslated(0, 0, -50);
	for (int i = 0; i < 5; i++){
		drawTree();
		glTranslated(20, 0, 0);
	}

	glTranslated(-100, 0, -50);
	for (int i = 0; i < 5; i++){
		drawTree();
		glTranslated(20, 0, 0);
	}

	glTranslated(-100, 0, -50);
	for (int i = 0; i < 5; i++){
		drawTree();
		glTranslated(20, 0, 0);
	}

}
void drawRocket(double d){
	rocket->pos.x = Rocket_Pos.x;	
	rocket->pos.y = Rocket_Pos.y / 200 + d;
	rocket->pos.z = Rocket_Pos.z;
	rocket->Materials[0].tex = rocketTexture;
	if (angle > -180)
		rocket->rot.z = angle;
	else
		rocket->rot.z -= 0;
	rocket->Draw();
	rocket->visible;
}
void DrawGLScene(GLvoid)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	Keyboard_And_Camera_And_Light();
	gluLookAt(Rocket_Pos.x, Rocket_Pos.y / 200 + 10, 20, Rocket_Pos.x, Rocket_Pos.y / 200, 0, 0, 1, 0);
	spacebox();
	skybox();
	drawGround();
	glTranslated(-100, 0, +150);

	if (start){

		if (Rocket_Pos.y / 200 <= 3000)
		{
			if (Rocket_Pos.y / 200 >= 0)
			{
				Calculate_Results();
				drawRocket(0);
				Time += 0.01;
			}
			else
			{
				drawRocket(2 - (Rocket_Pos.y / 200));
			}
		}
		else
		{
			drawRocket(0);
		}
	}
	else
	{
		drawRocket(0);
	}



	glFlush();
	SwapBuffers(hDC);
}
void publicInitial()
{

}

int main(HINSTANCE hlnstance, HINSTANCE hPrevlnstance, LPSTR lpCmdLine, int nCmdShow)
{
	cout << endl;
	cout << endl;
	cout << " ----------------------- Wellcome To Our Rocket Simulation --------------------------------" << endl;
	cout << endl;
	cout << endl;
	cout << "We Provide Two Options :" << endl;
	cout << "  1)- You Can Enter The Rocket Mass & Diesel Mass & Upward Turust ." << endl;
	cout << "  2)- You Can Only Enter The Diesel Mass." << endl;
	cout << endl;
	cout << "Choose An Option !" << endl;
	cout << "----------------------"<<endl;
	cin >> option;
	if (option == 1){
		cout << endl;
		cout << endl;
		cout << "Enter Rocket Mass = ";
		cin >> Rocket_M;
		cout << endl;
		cout << "Enter How Many Liters Do You Want To Add : ";
		cin >> Diesel_Amount;
		Diesel_M_Calculate();
		Full_M = Diesel_M + Rocket_M;
		float weight = Full_M * G;
		cout << "Total Rocket Wieght = " << weight << endl;
		cout << "And it requirs an upward thrust equal or more then " << weight<< "(Newton) to move up " << endl;
		cout << endl;
		cout << endl;
		cout << "Enter Upward Thrust = ";
		cin >> Power;
	}
	if (option == 2){
		cout << "Enter How Many Liters Do You Want To Add : (1500 Liter Maximum) = ";
		cin >> Diesel_Amount;
		cout << endl;
		cout << endl;
		Diesel_M_Calculate();
		Full_M = Diesel_M + Rocket_M;
		float weight = Full_M * G;
		cout << "Total Rocket Wieght = " << weight << endl;
		cout << " And it requirs an upward thrust equal or more then " << weight << "(Newton) to move up " << endl;
		cout << endl;
		cout << endl;
	}





	WinMain(hlnstance, hPrevlnstance, lpCmdLine, nCmdShow);

	return 0;
}
///////////////END-MAIN/////////////////
int InitGL(GLvoid)
{
	glShadeModel(GL_SMOOTH);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_TEXTURE_2D);
	publicInitial();

	//skybox & spacebox texture
	space = LoadTexture("space1.bmp", 255);
	sky = LoadTexture("space3.bmp", 255);
	ground = LoadTexture("ground.bmp", 255);
	//reocket textures & model

	rocketTexture.Load("texture.bmp");
	rocket->Load("rocket2.3ds");

	// tree textures & models
	tree->Load("Tree1.3ds");
	leaf.Load("leaf.bmp");
	wood.Load("wood.bmp");

	//Sound
	//initialie.InitOpenAL();
	//sound1 = Sound("sound1.wav");

	return TRUE;
}
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)
{
	if (height == 0)
		height = 1;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 2500.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
GLvoid KillGLWindow(GLvoid)
{
	if (fullscreen)
	{
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(TRUE);
	}
	if (hRC)
	{
		if (!wglMakeCurrent(NULL, NULL))
			MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		if (!wglDeleteContext(hRC))
			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hRC = NULL;
	}
	if (hDC && !ReleaseDC(hWnd, hDC))
	{
		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;
	}
	if (hWnd && !DestroyWindow(hWnd))
	{
		MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;
	}
	if (!UnregisterClass("OpenGL", hInstance))
	{
		MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;
	}
}
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;
	WNDCLASS	wc;
	DWORD		dwExStyle;
	DWORD		dwStyle;
	RECT		WindowRect;
	WindowRect.left = (long)0;
	WindowRect.right = (long)width;
	WindowRect.top = (long)0;
	WindowRect.bottom = (long)height;
	fullscreen = fullscreenflag;
	hInstance = GetModuleHandle(NULL);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "OpenGL";
	if (!RegisterClass(&wc))
	{
		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}
	if (fullscreen)
	{
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof (dmScreenSettings));
		dmScreenSettings.dmSize = sizeof (dmScreenSettings);
		dmScreenSettings.dmPelsWidth = width;
		dmScreenSettings.dmPelsHeight = height;
		dmScreenSettings.dmBitsPerPel = bits;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			if (MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
				fullscreen = FALSE;
			else
			{
				MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);
				return FALSE;
			}
		}
	}
	if (fullscreen)
	{
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP;
		ShowCursor(FALSE);
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPEDWINDOW;
	}
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);
	if (!(hWnd = CreateWindowEx(dwExStyle, "OpenGL", title, dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top, NULL, NULL, hInstance, NULL)))
	{
		KillGLWindow();
		MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}
	static	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof (PIXELFORMATDESCRIPTOR), 1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA, bits, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 0, 0, PFD_MAIN_PLANE, 0, 0, 0, 0
	};
	if (!(hDC = GetDC(hWnd)))
	{
		KillGLWindow();
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}
	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))
	{
		KillGLWindow();
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}
	if (!SetPixelFormat(hDC, PixelFormat, &pfd))
	{
		KillGLWindow();
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}
	if (!(hRC = wglCreateContext(hDC)))
	{
		KillGLWindow();
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}
	if (!wglMakeCurrent(hDC, hRC))
	{
		KillGLWindow();
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}
	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);
	ReSizeGLScene(width, height);
	if (!InitGL())
	{
		KillGLWindow();
		MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}
	return TRUE;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static PAINTSTRUCT ps;
	switch (uMsg)
	{
	case WM_PAINT:
		DrawGLScene();
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_TIMER:
		DrawGLScene();
		return 0;
	case WM_ACTIVATE:
	{
						if (!HIWORD(wParam))
							active = TRUE;
						else
							active = FALSE;
						return 0;
	}
	case WM_SYSCOMMAND:
	{
						  switch (wParam)
						  {
						  case SC_SCREENSAVE:
						  case SC_MONITORPOWER:
							  return 0;
						  }
						  break;
	}
	case WM_CLOSE:
	{
					 PostQuitMessage(0);
					 return 0;
	}
	case WM_KEYDOWN:
	{
					   keys[wParam] = TRUE;
					   return 0;
	}
	case WM_KEYUP:
	{
					 keys[wParam] = FALSE;
					 return 0;
	}
	case WM_SIZE:
	{
					ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));
					return 0;
	}
	case WM_MOUSEMOVE:
	{
						 mouseX = (int)LOWORD(lParam); mouseY = (int)HIWORD(lParam);
						 isLClicked = (LOWORD(wParam) & MK_LBUTTON) ? true : false;
						 isRClicked = (LOWORD(wParam) & MK_RBUTTON) ? true : false;
						 break;
	}
	case WM_LBUTTONUP:
		isLClicked = false;
		break;
	case WM_RBUTTONUP:
		isRClicked = false;
		break;
	case WM_LBUTTONDOWN:
		isLClicked = true;
		break;
	case WM_RBUTTONDOWN:
		isRClicked = true;
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE	hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG		msg;
	BOOL	done = FALSE;
	{
		fullscreen = FALSE;
	}
	if (!CreateGLWindow((char*) "RAGHDAN", 640, 480, 16, fullscreen))
		return 0;
	UINT timer = SetTimer(hWnd, 0, 50, (TIMERPROC)NULL);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
