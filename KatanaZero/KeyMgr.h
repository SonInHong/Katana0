#pragma once
enum Key
{
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	LCTRL,
	LSHIFT,
	LALT,
	SPACEBAR,
	LEFTMOUSE,
	RIGHTMOUSE,

	END,
};

enum KeyState
{
	TAP,
	PRESS,
	RELEASE,
	NONE,

};




struct KeyInfo
{
	KeyState key_state;
	bool pressed;
};

class KeyMgr
{
private:
	KeyMgr();
	~KeyMgr();

	std::vector<KeyInfo> keyinfo;
	POINT mousePos;
	bool wasInModal;


	int ASCII[Key::END] = {
	'A',
	'B',
	'C',
	'D',
	'E',
	'F',
	'G',
	'H',
	'I',
	'J',
	'K',
	'L',
	'M',
	'N',
	'O',
	'P',
	'Q',
	'R',
	'S',
	'T',
	'U',
	'V',
	'W',
	'X',
	'Y',
	'Z',
	VK_LEFT,
	VK_RIGHT,
	VK_UP,
	VK_DOWN,
	VK_LCONTROL,
	VK_LSHIFT,
	VK_LMENU,	
	' ',
	VK_LBUTTON,
	VK_RBUTTON};

public:
	static KeyMgr* Create()
	{
		static KeyMgr p;
		return &p;
	}

	KeyInfo& key(Key k)
	{
		return keyinfo[k];
	}

	void DontGetInput()
	{
		
		keyinfo[A].key_state = KeyState::NONE;
		keyinfo[A].pressed = false;

		keyinfo[D].key_state = KeyState::NONE;
		keyinfo[D].pressed = false;

		keyinfo[S].key_state = KeyState::NONE;
		keyinfo[S].pressed = false;

		keyinfo[W].key_state = KeyState::NONE;
		keyinfo[W].pressed = false;

		keyinfo[LEFTMOUSE].key_state = KeyState::NONE;
		keyinfo[LEFTMOUSE].pressed = false;

		keyinfo[RIGHTMOUSE].key_state = KeyState::NONE;
		keyinfo[RIGHTMOUSE].pressed = false;
	}

	POINT GetMousePos() { return mousePos; }

	void Initialize();
	void Update();
	void Reset();
	void SetModal(bool b) { wasInModal = b; }
};

