enum SOUNDS
{
	SOUND_BALLHIT,
	SOUND_BLOCKHIT,
	SOUND_WALLHIT,
	SOUND_DEAD,
	SOUND_WON,
	SOUND_MAX
};

enum HITS
{
	HIT_TOP = 1,
	HIT_BOTTOM,
	HIT_LEFT,
	HIT_RIGHT,
	HIT_X,
	HIT_Y,
	NO_HIT,
};

enum
{
	PS_EDITOR,
	PS_SPLASH,
	PS_ACTIVE,
	PS_DEAD,
	PS_WON
};

struct _OBJECT;
typedef struct _OBJECT OBJECT, *OBJp;

typedef struct _OBJECT
{
	int Type;
	int Width, Height;
	int PosX, PosY;
	int VelX, VelY;
	int Frame;
	int Delay;
	OBJp Next;
	OBJp Prev;
} OBJECT;

