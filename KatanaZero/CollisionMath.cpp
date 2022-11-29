#include "pch.h"
#include "CollisionMath.h"

bool CollisionMath::CheckifLineCollide(doublepoint A1, doublepoint A2, doublepoint B1, doublepoint B2)
{
	double a = (A2.y - A1.y);
	double b = -(A2.x - A1.x);
	double m1 = (A2.y - A1.y) * A1.x - (A2.x - A1.x) * A1.y;

	double c = (B2.y - B1.y);
	double d = -(B2.x - B1.x);
	double m2 = (B2.y - B1.y) * B1.x - (B2.x - B1.x) * B1.y;

	double det = a * d - b * c;

	if (det == 0)
	{
		if (a * B1.x + b * B1.y == m1)
		{
			if (((A1.x - B1.x) * (A2.x - B1.x) <= 0 && (A1.y - B1.y) * (A2.y - B1.y) <= 0)
				|| ((A1.x - B2.x) * (A2.x - B2.x) <= 0 && (A1.y - B2.y) * (A2.y - B2.y) <= 0))
				return true;
		}

		return false;
	}


	double a2 = d / det;
	double b2 = -b / det;
	double c2 = -c / det;
	double d2 = a / det;

	double x = a2 * m1 + b2 * m2;
	double y = c2 * m1 + d2 * m2;

	if ((A1.x - x) * (A2.x - x) <= 1 &&
		(B1.x - x) * (B2.x - x) <= 1 &&
		(A1.y - y) * (A2.y - y) <= 1 &&
		(B1.y - y) * (B2.y - y) <= 1)
		return true;


	return false;
}

bool CollisionMath::CheckLineBoxCollision(doublepoint Pos, doublepoint Scale, doublepoint p, doublepoint q)
{
	doublepoint A, B, C, D;

	A.x = Pos.x - Scale.x / 2;
	A.y = Pos.y - Scale.y / 2;

	B.x = Pos.x + Scale.x / 2;
	B.y = Pos.y - Scale.y / 2;

	C.x = Pos.x + Scale.x / 2;
	C.y = Pos.y + Scale.y / 2;

	D.x = Pos.x - Scale.x / 2;
	D.y = Pos.y + Scale.y / 2;

	if (p.x <= Pos.x + Scale.x / 2 && p.x >= Pos.x - Scale.x / 2 && p.y <= Pos.y + Scale.y / 2 && p.y >= Pos.y - Scale.y / 2)
		return true;

	if (q.x <= Pos.x + Scale.x / 2 && q.x >= Pos.x - Scale.x / 2 && q.y <= Pos.y + Scale.y / 2 && q.y >= Pos.y - Scale.y / 2)
		return true;


	bool b1 = CheckifLineCollide(A, B, p, q);
	bool b2 = CheckifLineCollide(B, C, p, q);
	bool b3 = CheckifLineCollide(C, D, p, q);
	bool b4 = CheckifLineCollide(D, A, p, q);

	if (b1 || b2 || b3 || b4)
		return true;

	return false;
}
