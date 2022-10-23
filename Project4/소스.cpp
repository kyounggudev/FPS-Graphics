#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include<iostream>
#include<Windows.h>
#include<mmsystem.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#endif
#define ESCAPE 27
#define GL_PI 3.1415f
float WheelRot;
void resizeScene(int Width, int Height);
void renderScene(void);
void processNormalKeys(unsigned char key, int x, int y);
void processSpecialKeys(int key, int x, int y);
void mouseMove(int x, int y);


float angle = 0;
float angle1 = -5000;
float lx = 0;
float ly = 0;
float lz = 1;
float xc = 0;
float yc = 0;
float zc = 10;
float speed = 0.1;
float rotSpeed = 0.0005;
float CloudPosition[10][3] = { 0 };
float CloudETC[10][2] = { 0 };
int WorldTime = 0;
int SkyColor[3] = { 165, 205, 255 };


int window;
void WorldTimer(int value) {	// Å¸ÀÌ¸Ó
	for (int c = 0; c < 10; c++) {
		CloudETC[c][0] = CloudETC[c][0] + 0.2;
		if (CloudETC[c][0] >= 360.0) {
			CloudETC[c][0] = CloudETC[c][0] - 360.0;
		}
	}

	glutTimerFunc(10, WorldTimer, 1);
	glutPostRedisplay();
}

void MakeCloud(float x = 0.0, float y = 0.0, float z = 0.0, float scale = 1.0) {
	glPushMatrix();
	glTranslatef(x, 20.0 + y, z);
	glScalef(scale, scale, scale);
	glutSolidSphere(2.2, 25, 25);
	glTranslatef(3.25, 0.0, 0.0);
	glutSolidSphere(2.6, 25, 25);
	glTranslatef(3.25, 0.0, 0.0);
	glutSolidSphere(2.2, 25, 25);
	glPopMatrix();
}

void Cloud() {
	for (int i = 0; i < 10; i++) {
		glPushMatrix();
		glRotatef(CloudETC[i][0], 0.0, 1.0, 0.0);
		MakeCloud(CloudPosition[i][0], CloudPosition[i][1], CloudPosition[i][2], CloudETC[i][1]);
		glPopMatrix();
	}
}

void Wheel() {		//°ü¶÷Â÷
	int color[3] = { 200,100,100 };
	GLUquadricObj* pQuad;
	pQuad = gluNewQuadric();

	gluQuadricDrawStyle(pQuad, GLU_FILL);	//±âµÕ
	glColor3f((float)127 / 255.0, (float)127 / 255.0, (float)127 / 255.0);
	glPushMatrix();
	glTranslatef(0.0, 13.2, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.6, 0.0);
	gluCylinder(pQuad, 0.4, 0.4, 15.0, 100, 100);
	glPopMatrix();

	gluQuadricDrawStyle(pQuad, GLU_LINE);	//¹ÙÄû
	glColor3f((float)50 / 255.0, (float)35 / 255.0, (float)20 / 255.0);	//¸öÅë»ö±ò
	glPushMatrix();
	glTranslatef(0.0, 13.3, -2.5);
	glRotatef(WheelRot, 0.0, 0.0, 1.0);
	glLineWidth(3.0f);	//¹ÙÄû»ì ±½°Ô
	gluCylinder(pQuad, 6.0, 6.0, 3.0, 10, 1);
	gluDisk(pQuad, 0.0, 12.0, 10, 2);
	glTranslatef(0.0, 0.0, 3.0);
	gluDisk(pQuad, 0.0, 12.0, 10, 2);
	glLineWidth(1.0f);	//¼±±½±â ¿ø»óº¹±¸
	for (float angle = 0.0f; angle < (2.0f * GL_PI); angle = angle + (2.0f * GL_PI / 10.0f)) {	//°ü¶÷Â÷ ¾Ë¸ÍÀÌ
		float WheelX = 12.0f * sin(angle);
		float WheelY = 12.0f * cos(angle);
		glPushMatrix();
		glTranslatef(WheelX, WheelY, -1.5);
		glRotatef(-WheelRot, 0.0, 0.0, 1.0);
		glColor3f((float)color[0] / 255.0, (float)color[1] / 255.0, (float)color[2] / 255.0);	//¾Ë¸ÍÀÌ »ö±ò-»ö±ò¿©·¯°³
		glutSolidCube(3.0);
		glColor3f((float)0 / 255.0, (float)0 / 255.0, (float)0 / 255.0);	//Å×µÎ¸®
		glutWireCube(3.0);
		glPopMatrix();
		color[0] = color[0] - 10;
		color[1] = color[1] + 10;
		color[2] = color[2] + 10;
	}
	glPopMatrix();

	gluQuadricDrawStyle(pQuad, GLU_FILL);	//±âµÕ
	glColor3f((float)127 / 255.0, (float)127 / 255.0, (float)127 / 255.0);
	glPushMatrix();
	glTranslatef(0.0, 13.2, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, -2.4, 0.0);
	gluCylinder(pQuad, 0.4, 0.4, 15.0, 100, 100);
	glPopMatrix();
		glColor3f(1, 1, 1);
}

void RotWheel(int value) { //È¸Àü ½ÃÅ°±â
	WheelRot = WheelRot + 0.5;
	if (WheelRot == 360) {
		WheelRot = 0;
	}
	glutTimerFunc(40, RotWheel, 1);
	glutPostRedisplay();
}

GLfloat xAngle, yAngle, zAngle;

GLubyte* LoadBmp(const char* Path, int* Width, int* Height) { //BMPÆÄÀÏ ºÒ·¯¿À±â
	HANDLE hFile;
	DWORD FileSize, dwRead;
	BITMAPFILEHEADER* fh = NULL;
	BITMAPINFOHEADER* ih;
	BYTE* pRaster;
	hFile = CreateFileA(
		Path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
	);
	if (hFile == INVALID_HANDLE_VALUE) {
		return NULL;
	}
	FileSize = GetFileSize(hFile, NULL);
	fh = (BITMAPFILEHEADER*)malloc(FileSize);
	ReadFile(hFile, fh, FileSize, &dwRead, NULL);
	CloseHandle(hFile);
	int len = FileSize - fh->bfOffBits;
	pRaster = (GLubyte*)malloc(len);
	memcpy(pRaster, (BYTE*)fh + fh->bfOffBits, len);
	// RGB·Î ¼ø¼­¸¦ ¹Ù²Û´Ù. 
	for (BYTE* p = pRaster; p < pRaster + len - 3; p += 3) {
		BYTE b = *p;
		*p = *(p + 2);
		*(p + 2) = b;
	}
	ih = (BITMAPINFOHEADER*)((PBYTE)fh + sizeof(BITMAPFILEHEADER));
	*Width = ih->biWidth;
	*Height = ih->biHeight;
	free(fh); return pRaster;
}


void resizeScene(int Width, int Height)
{
	if (Height == 0)
		Height = 1;
	glViewport(0, 0, Width, Height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}
void renderScene(void)
{
	GLubyte* data;
	int Width, Height;
	glEnable(GL_TEXTURE_2D);
	data = LoadBmp("grass2.bmp", &Width, &Height);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glShadeModel(GL_FLAT); 
	glEnable(GL_DEPTH_TEST);
	glPushMatrix();
	glRotatef(xAngle, 1.0f, 0.0f, 0.0f);
	glRotatef(yAngle, 0.0f, 1.0f, 0.0f);
	glRotatef(zAngle, 0.0f, 0.0f, 1.0f);

	if (data != NULL) {
		glBegin(GL_POLYGON);
		glNormal3f(0.0, 0.0, 1.0);
		glTexCoord2f(0,0);
		glVertex3f(-70.0f, -1.5f, 70.0f);
		glNormal3f(0.0, 0.0, 1.0);
		glTexCoord2f(0, 1);
		glVertex3f(25.0f, -1.5f, 70.0f);
		glNormal3f(0.0, 0.0, 1.0);
		glTexCoord2f(1,1);
		glVertex3f(25.0f, -1.5f, -25.0f);
		glNormal3f(0.0, 0.0, 1.0);
		glTexCoord2f(0, 1);
		glVertex3f(-70.0f, -1.5f, -25.0f);

		glEnd();
	}
	glPopMatrix();
	glFlush();
	glDisable(GL_TEXTURE_2D);
	glClearColor(0.5, 0.75, 1, 1);
	glLoadIdentity();
	gluLookAt(xc, yc, zc,
		xc + lx, yc + ly, zc + lz,
		0, 1, 0);
	

	Wheel();
	Cloud();

	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y)
{
	if (key == ESCAPE)
	{
		glutDestroyWindow(window);
		exit(0);
	}
}

bool mouseCaptured = true;

void processSpecialKeys(int key, int x, int y)
{
    printf("rot: %f, %f\n",lx,lz);
	switch(key) {
		case GLUT_KEY_UP :
			xc += lx * speed;
			zc += lz * speed;
			break;
		case GLUT_KEY_DOWN :
			xc -= lx * speed;
			zc -= lz * speed;
			break;
        case GLUT_KEY_LEFT :
			xc += lz * speed;
			zc -= lx * speed;
			break;
		case GLUT_KEY_RIGHT :
			xc -= lz * speed;
			zc += lx * speed;
			break;
        case GLUT_KEY_F1:
            mouseCaptured = !mouseCaptured;
            if(mouseCaptured)
            {
                glutSetCursor(GLUT_CURSOR_NONE);
            }
            else
            {
                glutSetCursor(GLUT_CURSOR_INHERIT);
            }
            break;
        default:
            break;
	}
	printf("pos: %f, %f\n",xc,zc);
}

bool warping = false;

void mouseMove(int x, int y)
{
    if(warping)
    {
        warping = false;
        return;
    }
    int dx = x - 100;
    int dy = y - 100;
    angle = angle+dx*rotSpeed;
    angle1 = angle1-dy*rotSpeed;
    lx=sin(angle1)*sin(angle);
    ly=-cos(angle1);
    lz=-sin(angle1)*cos(angle);
    printf("rot: %f, %f,%f angle: %f, angle1: %f\n",lx,ly,lz, angle, angle1);
    if(mouseCaptured)
    {
        warping = true;
        glutWarpPointer(100,100);
    }
}

void SetDisplay() {
	glClearColor(1,1,1,1);	//»ö
	glColor3f(1, 1, 1);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	for (int i = 0; i < 10; i++) {	//±¸¸§ À§Ä¡ Å©±â
		CloudPosition[i][0] = (float)((rand() % 80) - 40);
		CloudPosition[i][1] = (float)(rand() % 15);
		CloudPosition[i][2] = (float)((rand() % 80) - 40);
		CloudETC[i][0] = (float)(rand() % 360);
		CloudETC[i][1] = (float)(((rand() % 3) + 7.0) / 10.0);
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
	glutInitWindowPosition(200, 30);
	glutInitWindowSize(800, 600);
	window = glutCreateWindow("201611563 ±è°æ¹Î ÅÒÇÁ·ÎÁ§Æ®");
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(resizeScene);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	glutPassiveMotionFunc(mouseMove);
	printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
	glutTimerFunc(40, RotWheel, 1);
	glutTimerFunc(10, WorldTimer, 1);
	glutSetCursor(GLUT_CURSOR_NONE);
	glutWarpPointer(100, 100);

	SetDisplay();
	glutMainLoop();
}