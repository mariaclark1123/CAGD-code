#define PRECISION   1e-5
#define EPS         1e-6        /* data type is float */
#define RES			64
#define SNUM		21

typedef float REAL;
typedef int INT;

typedef REAL Point3[3];
typedef REAL Point2[2];
typedef int Face3[3];

class CubicBezierCurve
{
public:
	Point2 control_pts[4];

	void EvaluateCurve2D(const REAL t, Point2 value);
	void EvaluateCurve3D(const REAL t, Point3 value);
	void DrawCurve2D();
	void DrawCurve3D();
	void PrintCtrlPt();
	int HitIndex2D(int x, int y);
};

enum XYZ { X = 0, Y, Z };

#define SET_PT2(V, V1, V2) do { (V)[0] = (V1); (V)[1] = (V2); } while (0)
#define SET_PT3(V, V1, V2, V3) do { (V)[0] = (V1); (V)[1] = (V2); (V)[2] = (V3); } while (0)
#define COPY_PT(V1,V2) do { (V1)[0] = (V2)[0]; (V1)[1] = (V2)[1]; (V1)[2] = (V2)[2]; } while (0)
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
