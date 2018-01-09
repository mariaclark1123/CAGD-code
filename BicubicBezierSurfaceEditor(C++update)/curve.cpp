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

/*Cubic Bezier Curve*/
void BicubicBezierSurface::EvaluateSurface3D(const REAL t1, const REAL t2, Point3 value)
{
	const float t1_inv = 1.0f - t1;
	const float t1_inv_sq = t1_inv * t1_inv;
	const float t1_sq = t1 * t1;
	const float t2_inv = 1.0f - t2;
	const float t2_inv_sq = t2_inv * t2_inv;
	const float t2_sq = t2 * t2;

	float b[2][4];
	b[0][0] = t1_inv_sq * t1_inv;
	b[0][1] = 3 * t1_inv_sq * t1;
	b[0][2] = 3 * t1_inv * t1_sq;
	b[0][3] = t1_sq * t1;
	b[1][0] = t2_inv_sq * t2_inv;
	b[1][1] = 3 * t2_inv_sq * t2;
	b[1][2] = 3 * t2_inv * t2_sq;
	b[1][3] = t2_sq * t2;

	SET_PT3(value, 0, 0, 0);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			VECTOR3_X_SCALA_ADD(value, this->control_pts[i][j], b[0][i] * b[1][j]);
}

void BicubicBezierSurface::CalculateSurface3D()
{
	/*Calculate surface*/
	for (int i = 0; i <= RES; i++)
		for (int j = 0; j <= RES; j++)
		{
			this->EvaluateSurface3D(i / (float)RES, j / (float)RES, this->points[i][j]);
		}
}

void BicubicBezierSurface::DrawSurface3D()
{
	for (int i = 0; i <= RES; i += 4)
	{
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j <= RES; j++)
			glVertex3f(this->points[i][j][0], this->points[i][j][1], this->points[i][j][2]);
		glEnd();
	}
	for (int i = 0; i <= RES; i += 4)
	{
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j <= RES; j++)
			glVertex3f(this->points[j][i][0], this->points[j][i][1], this->points[j][i][2]);
		glEnd();
	}

	glColor3f(1.0f, 0.75f, 0.75f);
	glBegin(GL_QUADS);
	for (int i = 0; i < RES; i++)
	{
		for (int j = 0; j < RES; j++)
		{
			glVertex3f(this->points[i][j][0], this->points[i][j][1], this->points[i][j][2]);
			glVertex3f(this->points[i + 1][j][0], this->points[i + 1][j][1], this->points[i + 1][j][2]);
			glVertex3f(this->points[i + 1][j + 1][0], this->points[i + 1][j + 1][1], this->points[i + 1][j + 1][2]);
			glVertex3f(this->points[i][j + 1][0], this->points[i][j + 1][1], this->points[i][j + 1][2]);
		}
	}
	glEnd();
}
