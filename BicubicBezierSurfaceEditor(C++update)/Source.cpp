#include "curve.h"
#include "viewport.h"
#include "string.h"

/* global */
CubicBezierCurve bezcur;
BicubicBezierSurface bezsur;
GLsizei width = 800, height = 600;
float viewportwidth = 400, viewportheight = 300;

int selectedscene = 0;
int selected = -1;
bool isDrawControlMesh = true;
bool isDottedLine = false;

/*3D viewport*/
Vector3d eye;
Vector3d center;
Vector3d upVector;
bool isDragging = false;
float radius;

int mouseButton = -1;
int lastX = -1;
int lastY = -1;

void init()
{
	//BicubicBezierSurface
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			SET_PT3(bezsur.control_pts[i][j], 50 * i + 50, 20 * i - 75 * (i == 2) + 200 - j * 50, j * 50 + 50);
	bezsur.CalculateSurface3D();

	eye = Vector3d(750, 750, 750);
	center = Vector3d(0, 0, 0);
	upVector = Vector3d(0, 1, 0);
}

int hit_index(int x, int y, int scene)
{
	int xx, yy;
	switch (scene)
	{
	case 1:
		xx = 0, yy = 1;
		break;
	case 3:
		xx = 0, yy = 2;
		break;
	case 4:
		xx = 1, yy = 2;
		break;
	}
	int min = 30;
	int minp = -1;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			float tx = bezsur.control_pts[i][j][xx] - x;
			float ty = bezsur.control_pts[i][j][yy] - y;
			if ((tx * tx + ty * ty) < min)
			{
				min = (tx * tx + ty * ty);
				minp = i * 10 + j;
			}
		}
	}
	return minp;
}

void reshape_callback(GLint nw, GLint nh)
{
	width = nw;
	height = nh;

	viewportwidth = width / 2.0f;
	viewportheight = height / 2.0f;
	radius = std::sqrt(viewportwidth * viewportwidth + viewportheight * viewportheight) / 2;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glutPostRedisplay();
}

void display_callback(void)
{
	//BezierSurface
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex3f(-1, 0, 0);
	glVertex3f(1, 0, 0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0, -1, 0);
	glVertex3f(0, 1, 0);
	glEnd();

	glPointSize(8.0f);
	// XY
	glViewport(0, viewportheight, viewportwidth, viewportheight);
	glLoadIdentity();
	gluOrtho2D(0, (double)viewportwidth, 0, (double)viewportheight);

	//annotation
	glColor3ub(0, 0, 0);
	glRasterPos2f(10.0f, viewportheight - 20);
	drawString("XY coordinate");

	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			glVertex2f(bezsur.control_pts[i][j][0], bezsur.control_pts[i][j][1]);
	glEnd();

	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 3; j++)
		{
			glVertex2f(bezsur.control_pts[i][j][0], bezsur.control_pts[i][j][1]);
			glVertex2f(bezsur.control_pts[i][j + 1][0], bezsur.control_pts[i][j + 1][1]);
			glVertex2f(bezsur.control_pts[j][i][0], bezsur.control_pts[j][i][1]);
			glVertex2f(bezsur.control_pts[j + 1][i][0], bezsur.control_pts[j + 1][i][1]);
		}
	glEnd();

	// XZ
	glViewport(0, 0, viewportwidth, viewportheight);
	glLoadIdentity();
	gluOrtho2D(0, (double)viewportwidth, 0, (double)viewportheight);

	//annotation
	glColor3ub(0, 0, 0);
	glRasterPos2f(10.0f, viewportheight - 20);
	drawString("XZ coordinate");

	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			glVertex2f(bezsur.control_pts[i][j][0], bezsur.control_pts[i][j][2]);
	glEnd();
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 3; j++)
		{
			glVertex2f(bezsur.control_pts[i][j][0], bezsur.control_pts[i][j][2]);
			glVertex2f(bezsur.control_pts[i][j + 1][0], bezsur.control_pts[i][j + 1][2]);
			glVertex2f(bezsur.control_pts[j][i][0], bezsur.control_pts[j][i][2]);
			glVertex2f(bezsur.control_pts[j + 1][i][0], bezsur.control_pts[j + 1][i][2]);
		}
	glEnd();

	// YZ
	glViewport(viewportwidth, 0, viewportwidth, viewportheight);
	glLoadIdentity();
	gluOrtho2D(0, (double)viewportwidth, 0, (double)viewportheight);
	
	//annotation
	glColor3ub(0, 0, 0);
	glRasterPos2f(10.0f, viewportheight - 20);
	drawString("YZ coordinate");

	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			glVertex2f(bezsur.control_pts[i][j][1], bezsur.control_pts[i][j][2]);
	glEnd();
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 3; j++)
		{
			glVertex2f(bezsur.control_pts[i][j][1], bezsur.control_pts[i][j][2]);
			glVertex2f(bezsur.control_pts[i][j + 1][1], bezsur.control_pts[i][j + 1][2]);
			glVertex2f(bezsur.control_pts[j][i][1], bezsur.control_pts[j][i][2]);
			glVertex2f(bezsur.control_pts[j + 1][i][1], bezsur.control_pts[j + 1][i][2]);
		}
	glEnd();

	//3D
	glViewport(viewportwidth, viewportheight, viewportwidth, viewportheight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//annotation
	glColor3ub(0, 0, 0);
	glRasterPos2f(10.0f, viewportheight - 20);
	drawString("Bicubic Bezier Surface");

	gluPerspective(25, width / (double)height, 0.1, 25000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
		
	gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, upVector.x, upVector.y, upVector.z);

	glEnable(GL_DEPTH_TEST);
	glBegin(GL_LINES);
	glColor3f(1.0f, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(500.0f, 0, 0);
	glColor3f(0, 1.0f, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 500.0f, 0);
	glColor3f(0, 0, 1.0f);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 500.0f);
	glEnd();

	bezsur.DrawSurface3D();
	glDisable(GL_DEPTH_TEST);

	glutSwapBuffers();
}

void mouse_callback(GLint button, GLint action, GLint x, GLint y)
{
	int scene = 0;
	if (x < viewportwidth)
	{
		if (y < viewportheight)
			scene = 1;
		else
		{
			scene = 3;
			y -= (int)viewportheight;
		}
	}
	else
	{
		x -= (int)viewportwidth;
		if (y < viewportheight)
			scene = 2;
		else
		{
			scene = 4;
			y -= (int)viewportheight;
		}
	}

	if (action == GLUT_UP)
	{
		isDragging = false;
		mouseButton = -1;
	}

	if (scene == 2)
	{
		if (action == GLUT_DOWN)
		{
			mouseButton = button;
			isDragging = true;
			lastX = x;
			lastY = y;
		}
	}
	else
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			switch (action)
			{
			case GLUT_DOWN:
				selectedscene = scene;
				selected = hit_index(x, (int)viewportheight - y, scene);
				break;
			case GLUT_UP:
				selected = -1;
				break;
			default: break;
			}
		}
	}
	glutPostRedisplay();
}

void mouse_move_callback(GLint x, GLint y)
{
	Vector3d lastP = getMousePoint(lastX, lastY, viewportwidth, viewportheight, radius);
	Vector3d currentP = getMousePoint(x - viewportwidth, y, viewportwidth, viewportheight, radius);

	if (mouseButton == GLUT_LEFT_BUTTON)
	{
		Vector3d rotateVector;
		rotateVector.cross(currentP, lastP);
		double angle = -currentP.angle(lastP) * 2;
		rotateVector = unProjectToEye(rotateVector, eye, center, upVector);

		Vector3d dEye;
		dEye.sub(center, eye);
		dEye = rotate(dEye, rotateVector, -angle);
		upVector = rotate(upVector, rotateVector, -angle);
		eye.sub(center, dEye);
	}
	else if (mouseButton == GLUT_RIGHT_BUTTON) {
		Vector3d dEye;
		dEye.sub(center, eye);
		double offset = 0.025;
		if ((y - lastY) < 0) {
			dEye.scale(1 - offset);
		}
		else {
			dEye.scale(1 + offset);
		}
		eye.sub(center, dEye);
	}
	else if (mouseButton == GLUT_MIDDLE_BUTTON) {
		double dx = x - viewportwidth - lastX;
		double dy = y - lastY;
		if (dx != 0 || dy != 0)
		{
			Vector3d moveVector(dx, dy, 0);
			moveVector = unProjectToEye(moveVector, eye, center, upVector);
			moveVector.normalize();
			double eyeDistance = Vector3d(eye).distance(Vector3d(center));
			moveVector.scale(std::sqrt(dx*dx + dy*dy) / 1000 * eyeDistance);
			center.add(moveVector);
			eye.add(moveVector);
		}
	}

	lastX = x - viewportwidth;
	lastY = y;

	if (selected != -1)
	{
		int xx = 0;
		int yy = 0;
		switch (selectedscene)
		{
		case 1:
			xx = 0, yy = 1;
			break;
		case 3:
			xx = 0, yy = 2;
			y -= (int)viewportheight;
			break;
		case 4:
			xx = 1, yy = 2;
			x -= (int)viewportwidth;
			y -= (int)viewportheight;
			break;
		}
		x = MAX(x, 0);
		x = MIN(x, (int)viewportwidth);
		y = MAX((int)viewportheight - y, 0);
		y = MIN(y, (int)viewportheight);
		bezsur.control_pts[selected / 10][selected % 10][xx] = static_cast<float>(x);
		bezsur.control_pts[selected / 10][selected % 10][yy] = static_cast<float>(y);
		bezsur.CalculateSurface3D();
	}
	glutPostRedisplay();
}

// void glutKeyboardFunc(void (*func)(unsigned char key, int x, int y));
void keyboard_callback(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'i': case 'I':
		init();
		break;
	case 'l': case 'L':
		isDottedLine ^= true;
		break;
	case 'c': case 'C':
		isDrawControlMesh ^= true;
		break;
	case (27): exit(0); break;
	default: break;
	}
	glutPostRedisplay();
}


int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(width, height);
	glutCreateWindow("Bezier Editor");

	init();
	glutReshapeFunc(reshape_callback);
	glutMouseFunc(mouse_callback);
	glutMotionFunc(mouse_move_callback);
	glutDisplayFunc(display_callback);
	glutKeyboardFunc(keyboard_callback);
	glutMainLoop();
	return 0;
}
