#pragma once
class CollisionMath
{
public:
	
	static bool CheckifLineCollide(doublepoint A1, doublepoint A2, doublepoint B1, doublepoint B2);
	static bool CheckLineBoxCollision(doublepoint Pos, doublepoint Scale, doublepoint p, doublepoint q);
	
};

