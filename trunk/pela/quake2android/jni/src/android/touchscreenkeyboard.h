#ifndef _H_TOUCHSCREENKEYBOARD
#define _H_TOUCHSCREENKEYBOARD
#ifdef __cplusplus
extern "C" {
#endif
#include <jni.h>

#include <GLES/gl.h>
#include <GLES/glext.h>
#include <android/log.h>

#define MAX_BUTTONS 20
#define PKG_NAME tk_niuzb_quake3
#define JAVA_EXPORT_NAME2(name,package) Java_##package##_##name
#define JAVA_EXPORT_NAME1(name,package) JAVA_EXPORT_NAME2(name,package)
#define JAVA_EXPORT_NAME(name) JAVA_EXPORT_NAME1(name,PKG_NAME)
enum MOUSE_ACTION { MOUSE_DOWN = 0, MOUSE_UP=1, MOUSE_MOVE=2 };
enum { ARROW_LEFT = 1, ARROW_RIGHT = 2, ARROW_UP = 4, ARROW_DOWN = 8 };
enum {SDL_RELEASED , SDL_PRESSED};
typedef struct
{
    GLuint id;
    GLfloat w;
    GLfloat h;
} GLTexture_t;
typedef struct SDL_Rect
{
    int x, y;
    int w, h;
} SDL_Rect;

extern int width;
extern int height;
 static inline int InsideRect(const SDL_Rect * r, int x, int y)
{
	return ( x >= r->x && x <= r->x + r->w ) && ( y >= r->y && y <= r->y + r->h );
}
extern int processTouchscreenKeyboard(int x, int y, int action, int pointerId);
extern int drawTouchscreenKeyboard();

extern 	void init_screen_key_size();
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "quake", __VA_ARGS__)
enum QKEYS {
	K_TAB			= 9,
	K_ENTER			= 13,
	K_ESCAPE		= 27,
	K_SPACE			= 32,

// normal keys should be passed as lowercased ascii

	K_BACKSPACE		= 127,
	K_UPARROW		= 128,
	K_DOWNARROW		= 129,
	K_LEFTARROW		= 130,
	K_RIGHTARROW		= 131,

	K_ALT			= 132,
	K_CTRL			= 133,
	K_SHIFT			= 134,
	K_F1			= 135,
	K_F2			= 136,
	K_F3			= 137,
	K_F4			= 138,
	K_F5			= 139,
	K_F6			= 140,
	K_F7			= 141,
	K_F8			= 142,
	K_F9			= 143,
	K_F10			= 144,
	K_F11			= 145,
	K_F12			= 146,
	K_INS			= 147,
	K_DEL			= 148,
	K_PGDN			= 149,
	K_PGUP			= 150,
	K_HOME			= 151,
	K_END			= 152,

	K_KP_HOME		= 160,
	K_KP_UPARROW		= 161,
	K_KP_PGUP		= 162,
	K_KP_LEFTARROW		= 163,
	K_KP_5			= 164,
	K_KP_RIGHTARROW		= 165,
	K_KP_END		= 166,
	K_KP_DOWNARROW		= 167,
	K_KP_PGDN		= 168,
	K_KP_ENTER		= 169,
	K_KP_INS		= 170,
	K_KP_DEL		= 171,
	K_KP_SLASH		= 172,
	K_KP_MINUS		= 173,
	K_KP_PLUS		= 174,

//
// mouse buttons generate virtual keys
//
	K_MOUSE1		= 200,
	K_MOUSE2		= 201,
	K_MOUSE3		= 202,
	K_MOUSE4		= 241,
	K_MOUSE5		= 242,
	
//
// joystick buttons
//
	K_JOY1			= 203,
	K_JOY2			= 204,
	K_JOY3			= 205,
	K_JOY4			= 206,

//
// aux keys are for multi-buttoned joysticks to generate so they can use
// the normal binding process
//
	K_AUX1			= 207,
	K_AUX2			= 208,
	K_AUX3			= 209,
	K_AUX4			= 210,
	K_AUX5			= 211,
	K_AUX6			= 212,
	K_AUX7			= 213,
	K_AUX8			= 214,
	K_AUX9			= 215,
	K_AUX10			= 216,
	K_AUX11			= 217,
	K_AUX12			= 218,
	K_AUX13			= 219,
	K_AUX14			= 220,
	K_AUX15			= 221,
	K_AUX16			= 222,
	K_AUX17			= 223,
	K_AUX18			= 224,
	K_AUX19			= 225,
	K_AUX20			= 226,
	K_AUX21			= 227,
	K_AUX22			= 228,
	K_AUX23			= 229,
	K_AUX24			= 230,
	K_AUX25			= 231,
	K_AUX26			= 232,
	K_AUX27			= 233,
	K_AUX28			= 234,
	K_AUX29			= 235,
	K_AUX30			= 236,
	K_AUX31			= 237,
	K_AUX32			= 238,

	K_MWHEELDOWN		= 239,
	K_MWHEELUP		= 240,

	K_PAUSE			= 255,

	K_LAST
};

#ifdef __cplusplus
}
#endif
#endif
