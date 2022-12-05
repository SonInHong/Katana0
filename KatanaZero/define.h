#pragma once

struct WindowData
{
	HWND hwnd;
	HDC hdc;
	int height;
	int width;

	void clear()
	{
		hwnd = 0;
		hdc = 0;
		height = 0;
		width = 0;
	}

};

struct doublepoint
{
	double x;
	double y;

	doublepoint operator + (doublepoint _other)
	{
		return doublepoint{ x + _other.x, y + _other.y };
	}

	doublepoint operator - (doublepoint _other)
	{
		return doublepoint{ x - _other.x, y - _other.y };
	}

	doublepoint operator * (double a)
	{
		return doublepoint{ x * a,y * a };
	}

	doublepoint operator / (double a)
	{
		return doublepoint{ x / a,y / a };
	}

	double Norm() { return sqrt(pow(x, 2) + pow(y, 2)); }
	doublepoint Normalize() { return doublepoint{ x / Norm(),y / Norm() }; }
	double Angle() { return atan2(y, x) * 180 / M_PI; }
	
};

enum class Object_State
{
	ON_FLOOR,
	JUMP,
	FALL,
	RUN,
	END,
};

enum class GROUP_TYPE
{
	DEFAULT,
	MAP,
	BACKGROUND,
	FLOOR,
	Door,
	TILE,
	PARTICLE_EMITOR,
	PARTICLE,
	TRAP,
	MONSTER_PROJECTILE,
	MONSTER,
	PLAYER,
	
	PLAYER_PROJECTILE,
	NEUTRAL_OBJECT,
	CENSOR,
	FINALEFFECT,
	UI = 30,
	CURSOR = 31,
	END = 32,
};

enum class SCENE_TYPE
{
	TOOL,
	START,
	STAGE_01,
	STAGE_02,
	END,

};

enum class COMPONENT_TYPE
{
	DEFAULT,
	ANIMATOR,
	RIGIDBODY,
	COLLIDER,

	END,
};

enum class EVENT_TYPE
{
	CREATEOBJECT,
	DESTROYOBJECT,
	CHANGESCENE,
	END,
};

enum class ANIMATOR_TYPE
{
	MAIN,
	EFFECT,
	END,
};

enum class Main_Order
{
	LeanLeft,
	LeanRight,
	IdleRight,
	IdleLeft,
	RoamAround,
	GetToRoamingPoint,
	PlayerDetected,
	AttackLeft,
	AttackRight,
	ShotRight,
	ShotLeft,
	GetHurt,
	GetBurn,
	PlayerStun,
	MonsterStun,
	Dead,
	End,
};

enum class Move_Order
{
	Start,
	Stay,
	MoveRight,
	MoveLeft,
	ClimbDownRight,
	ClimbDownLeft,
	MoveToTopFloorRight,
	MoveToTopFloorLeft,

	CannotDetectPlayer,

	End,

};

enum class Action_Order
{ 
	LeanLeft,
	LeanRight,
	IdleLeft,
	IdleRight,
	TurnLeft,
	TurnRight,
	AttackLeft,
	AttackRight,
	ShotRight,
	ShotLeft,
	WalkRight,
	WalkLeft,
	RunRight,
	RunLeft,
	HurtGround,
	HurtFly,
	Stun,
	End,
};
enum class CameraRenderMode
{
	NONE,
	FADEIN,
	FADEOUT,
	SubtleFadeIn,
	SubtleFadeOut,
};
enum class CameraMode
{
	DontMove,
	FollowPlayer,
};

enum class CameraEffect
{
	LITTLESHAKE,
	SHAKE,
	End,
};

enum class PenColor
{
	BLACK,
	MAGENTA,
	YELLOW,
	AQUA,
	SKY,
};

enum class WallCamState
{
	FollowPlayer,
	Stay,
	Roam,
};

enum class ThrowingObjState
{
	Neutral,
	Stick,
	OwnedByPlayer,
	Throwing,
};

union CollisionID
{
	struct
	{
		UINT LEFT;
		UINT RIGHT;
	} INPUT;

	ULONGLONG ID;
};

#define ColliderRender 1
#define Right 1
#define Left -1
#define scaleA 1.5
