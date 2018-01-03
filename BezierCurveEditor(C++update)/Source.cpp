#include "curve.h"
#include <GL/glut.h>

/* global */
CubicBezierCurve bezcur;
int width = 800, height = 600;
int edit_ctrlpts_idx = -1;


void init()
{
	//Bezier curve
	SET_PT2(bezcur.control_pts[0], 150, 200);
	SET_PT2(bezcur.control_pts[1], 300, 400);
	SET_PT2(bezcur.control_pts[2], 500, 400);
	SET_PT2(bezcur.control_pts[3], 650, 200);

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, width, 0, height);
}

void reshape_callback(GLint nw, GLint nh)
{
	width = nw;
	height = nh;
	//Bezier curve
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);

	glutPostRedisplay();
}

void display_callback(void)
{
	/* curve */
	glClear(GL_COLOR_BUFFER_BIT);

	//BezierSurface
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	bezcur.DrawCurve2D();

	glutSwapBuffers();
}

void mouse_callback(GLint button, GLint action, GLint x, GLint y)
{
	if (GLUT_LEFT_BUTTON == button) {
		switch (action) {
		case GLUT_DOWN:
			edit_ctrlpts_idx = bezcur.HitIndex2D(x, height - y);
			break;

		case GLUT_UP:
			edit_ctrlpts_idx = -1;
			break;
		}
	}
	glutSwapBuffers();
}

void mouse_move_callback(GLint x, GLint y)
{
	if (edit_ctrlpts_idx != -1) {
		bezcur.control_pts[edit_ctrlpts_idx][0] = (REAL)x;
		bezcur.control_pts[edit_ctrlpts_idx][1] = (REAL)(height - y);
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
	glutMainLoop();
	return 0;
}
