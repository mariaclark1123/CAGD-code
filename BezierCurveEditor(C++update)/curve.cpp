#include "curve.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <GL/glut.h>


#define COPY_PT(DST, SRC)               do { (DST)[X] = SRC[X]; (DST)[Y] = SRC[Y]; } while (0)
#define SET_VECTOR2(V, V1, V2)          do { (V)[X] = (V1); (V)[Y] = (V2); } while (0)
#define SET_VECTOR3(V, V1, V2, V3)          do { (V)[X] = (V1); (V)[Y] = (V2); (V)[Z] = (V3);} while (0)

#define VECTOR2_X_SCALA_ADD(O, V, S)    do { O[X] += (S) * (V)[X]; O[Y] += (S) * (V)[Y]; } while (0)
#define VECTOR3_X_SCALA_ADD(O, V, S)    do { O[X] += (S) * (V)[X]; O[Y] += (S) * (V)[Y]; O[Z] += (S) * (V)[Z]; } while (0)

/*Cubic Bezier Curve*/
void CubicBezierCurve::EvaluateCurve2D(const REAL t, Point2 value)
{
	const REAL t_inv = 1.0f - t;
	const REAL t_inv_sq = t_inv * t_inv;
	const REAL t_sq = t * t;
	const REAL b0 = t_inv_sq * t_inv;
	const REAL b1 = 3 * t_inv_sq * t;
	const REAL b2 = 3 * t_inv * t_sq;
	const REAL b3 = t_sq * t;

	SET_VECTOR2(value, 0, 0);
	VECTOR2_X_SCALA_ADD(value, this->control_pts[0], b0);
	VECTOR2_X_SCALA_ADD(value, this->control_pts[1], b1);
	VECTOR2_X_SCALA_ADD(value, this->control_pts[2], b2);
	VECTOR2_X_SCALA_ADD(value, this->control_pts[3], b3);
}

void CubicBezierCurve::EvaluateCurve3D(const REAL t, Point3 value)
{
	const REAL t_inv = 1.0f - t;
	const REAL t_inv_sq = t_inv * t_inv;
	const REAL t_sq = t * t;
	const REAL b0 = t_inv_sq * t_inv;
	const REAL b1 = 3 * t_inv_sq * t;
	const REAL b2 = 3 * t_inv * t_sq;
	const REAL b3 = t_sq * t;

	SET_VECTOR3(value, 0, 0, 0);
	VECTOR3_X_SCALA_ADD(value, this->control_pts[0], b0);
	VECTOR3_X_SCALA_ADD(value, this->control_pts[1], b1);
	VECTOR3_X_SCALA_ADD(value, this->control_pts[2], b2);
	VECTOR3_X_SCALA_ADD(value, this->control_pts[3], b3);
}

void CubicBezierCurve::DrawCurve2D()
{
	int i;
	/* Curve property */
	glColor3ub(120, 120, 120);
	/* curve */
	Point2 pt;
	glBegin(GL_LINE_STRIP);
	for (i = 0; i <= RES; ++i) {
		const REAL t = (REAL)i / (REAL)RES;
		EvaluateCurve2D(t, pt);
		glVertex2f(pt[X], pt[Y]);
	}
	glEnd();

	/* control mesh */
	glColor3ub(255, 0, 0);
	glBegin(GL_LINE_STRIP);
	for (i = 0; i<4; ++i) {
		REAL *pt = this->control_pts[i];
		glVertex2f(pt[X], pt[Y]);
	}
	glEnd();

	/* control pts */
	glColor3ub(75, 119, 190);
	glPointSize(10.0);
	glBegin(GL_POINTS);
	for (i = 0; i < 4; ++i) {
		REAL *pt = this->control_pts[i];
		glVertex2f(pt[X], pt[Y]);
	}
	glEnd();
}

void CubicBezierCurve::DrawCurve3D()
{
	int i;
	/* Curve property */
	glColor3ub(120, 120, 120);
	/* curve */
	Point3 pt;
	glBegin(GL_LINE_STRIP);
	for (i = 0; i <= RES; ++i) {
		const REAL t = (REAL)i / (REAL)RES;
		EvaluateCurve3D(t, pt);
		glVertex3f(pt[X], pt[Y], pt[Z]);
	}
	glEnd();

	/* control mesh */
	glColor3ub(255, 0, 0);
	glBegin(GL_LINE_STRIP);
	for (i = 0; i < 4; ++i) {
		REAL *pt = this->control_pts[i];
		glVertex3f(pt[X], pt[Y], pt[Z]);
	}
	glEnd();

	/* control pts */
	glColor3ub(75, 119, 190);
	glPointSize(10.0);
	glBegin(GL_POINTS);
	for (i = 0; i < 4; ++i) {
		REAL *pt = this->control_pts[i];
		glVertex3f(pt[X], pt[Y], pt[Z]);
	}
	glEnd();
}

int CubicBezierCurve::HitIndex2D(int x, int y)
{
	for (int i = 0; i < 4; ++i) {
		REAL tx = this->control_pts[i][0] - x;
		REAL ty = this->control_pts[i][1] - y;
		if ((tx * tx + ty * ty) < 30)
			return i;
	}
	return -1;
}

void CubicBezierCurve::PrintCtrlPt()
{
	int i;
	printf("CubicBezierCurve\n");
	for (i = 0; i<4; ++i)
		printf("[%f, %f]\n", this->control_pts[i][X], this->control_pts[i][Y]);
	printf("\n");
}