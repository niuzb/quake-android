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

extern int screen_width;
extern int screen_height;
 static inline int InsideRect(const SDL_Rect * r, int x, int y)
{
	return ( x >= r->x && x <= r->x + r->w ) && ( y >= r->y && y <= r->y + r->h );
}

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "quake", __VA_ARGS__)

#ifdef __cplusplus
}
#endif
#endif
