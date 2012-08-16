/*
Copyright (C) 2007 The Android Open Source Project

*/

#include "quakedef.h"
#include "touchscreenkeyboard.h"
//add key code map 
// android key => quake key
typedef enum {
        /** @name ASCII mapped keysyms
         *  The keyboard syms have been cleverly chosen to map to ASCII
         */
        /*@{*/
	SDLK_UNKNOWN		= 0,
	SDLK_FIRST		= 0,
	SDLK_BACKSPACE		= 8,
	SDLK_TAB		= 9,
	SDLK_CLEAR		= 12,
	SDLK_RETURN		= 13,
	SDLK_PAUSE		= 19,
	SDLK_ESCAPE		= 27,
	SDLK_SPACE		= 32,
	SDLK_EXCLAIM		= 33,
	SDLK_QUOTEDBL		= 34,
	SDLK_HASH		= 35,
	SDLK_DOLLAR		= 36,
	SDLK_AMPERSAND		= 38,
	SDLK_QUOTE		= 39,
	SDLK_LEFTPAREN		= 40,
	SDLK_RIGHTPAREN		= 41,
	SDLK_ASTERISK		= 42,
	SDLK_PLUS		= 43,
	SDLK_COMMA		= 44,
	SDLK_MINUS		= 45,
	SDLK_PERIOD		= 46,
	SDLK_SLASH		= 47,
	SDLK_0			= 48,
	SDLK_1			= 49,
	SDLK_2			= 50,
	SDLK_3			= 51,
	SDLK_4			= 52,
	SDLK_5			= 53,
	SDLK_6			= 54,
	SDLK_7			= 55,
	SDLK_8			= 56,
	SDLK_9			= 57,
	SDLK_COLON		= 58,
	SDLK_SEMICOLON		= 59,
	SDLK_LESS		= 60,
	SDLK_EQUALS		= 61,
	SDLK_GREATER		= 62,
	SDLK_QUESTION		= 63,
	SDLK_AT			= 64,
	/* 
	   Skip uppercase letters
	 */
	SDLK_LEFTBRACKET	= 91,
	SDLK_BACKSLASH		= 92,
	SDLK_RIGHTBRACKET	= 93,
	SDLK_CARET		= 94,
	SDLK_UNDERSCORE		= 95,
	SDLK_BACKQUOTE		= 96,
	SDLK_a			= 97,
	SDLK_b			= 98,
	SDLK_c			= 99,
	SDLK_d			= 100,
	SDLK_e			= 101,
	SDLK_f			= 102,
	SDLK_g			= 103,
	SDLK_h			= 104,
	SDLK_i			= 105,
	SDLK_j			= 106,
	SDLK_k			= 107,
	SDLK_l			= 108,
	SDLK_m			= 109,
	SDLK_n			= 110,
	SDLK_o			= 111,
	SDLK_p			= 112,
	SDLK_q			= 113,
	SDLK_r			= 114,
	SDLK_s			= 115,
	SDLK_t			= 116,
	SDLK_u			= 117,
	SDLK_v			= 118,
	SDLK_w			= 119,
	SDLK_x			= 120,
	SDLK_y			= 121,
	SDLK_z			= 122,
	SDLK_DELETE		= 127,
	/* End of ASCII mapped keysyms */
        /*@}*/

	/** @name International keyboard syms */
        /*@{*/
	SDLK_WORLD_0		= 160,		/* 0xA0 */
	SDLK_WORLD_1		= 161,
	SDLK_WORLD_2		= 162,
	SDLK_WORLD_3		= 163,
	SDLK_WORLD_4		= 164,
	SDLK_WORLD_5		= 165,
	SDLK_WORLD_6		= 166,
	SDLK_WORLD_7		= 167,
	SDLK_WORLD_8		= 168,
	SDLK_WORLD_9		= 169,
	SDLK_WORLD_10		= 170,
	SDLK_WORLD_11		= 171,
	SDLK_WORLD_12		= 172,
	SDLK_WORLD_13		= 173,
	SDLK_WORLD_14		= 174,
	SDLK_WORLD_15		= 175,
	SDLK_WORLD_16		= 176,
	SDLK_WORLD_17		= 177,
	SDLK_WORLD_18		= 178,
	SDLK_WORLD_19		= 179,
	SDLK_WORLD_20		= 180,
	SDLK_WORLD_21		= 181,
	SDLK_WORLD_22		= 182,
	SDLK_WORLD_23		= 183,
	SDLK_WORLD_24		= 184,
	SDLK_WORLD_25		= 185,
	SDLK_WORLD_26		= 186,
	SDLK_WORLD_27		= 187,
	SDLK_WORLD_28		= 188,
	SDLK_WORLD_29		= 189,
	SDLK_WORLD_30		= 190,
	SDLK_WORLD_31		= 191,
	SDLK_WORLD_32		= 192,
	SDLK_WORLD_33		= 193,
	SDLK_WORLD_34		= 194,
	SDLK_WORLD_35		= 195,
	SDLK_WORLD_36		= 196,
	SDLK_WORLD_37		= 197,
	SDLK_WORLD_38		= 198,
	SDLK_WORLD_39		= 199,
	SDLK_WORLD_40		= 200,
	SDLK_WORLD_41		= 201,
	SDLK_WORLD_42		= 202,
	SDLK_WORLD_43		= 203,
	SDLK_WORLD_44		= 204,
	SDLK_WORLD_45		= 205,
	SDLK_WORLD_46		= 206,
	SDLK_WORLD_47		= 207,
	SDLK_WORLD_48		= 208,
	SDLK_WORLD_49		= 209,
	SDLK_WORLD_50		= 210,
	SDLK_WORLD_51		= 211,
	SDLK_WORLD_52		= 212,
	SDLK_WORLD_53		= 213,
	SDLK_WORLD_54		= 214,
	SDLK_WORLD_55		= 215,
	SDLK_WORLD_56		= 216,
	SDLK_WORLD_57		= 217,
	SDLK_WORLD_58		= 218,
	SDLK_WORLD_59		= 219,
	SDLK_WORLD_60		= 220,
	SDLK_WORLD_61		= 221,
	SDLK_WORLD_62		= 222,
	SDLK_WORLD_63		= 223,
	SDLK_WORLD_64		= 224,
	SDLK_WORLD_65		= 225,
	SDLK_WORLD_66		= 226,
	SDLK_WORLD_67		= 227,
	SDLK_WORLD_68		= 228,
	SDLK_WORLD_69		= 229,
	SDLK_WORLD_70		= 230,
	SDLK_WORLD_71		= 231,
	SDLK_WORLD_72		= 232,
	SDLK_WORLD_73		= 233,
	SDLK_WORLD_74		= 234,
	SDLK_WORLD_75		= 235,
	SDLK_WORLD_76		= 236,
	SDLK_WORLD_77		= 237,
	SDLK_WORLD_78		= 238,
	SDLK_WORLD_79		= 239,
	SDLK_WORLD_80		= 240,
	SDLK_WORLD_81		= 241,
	SDLK_WORLD_82		= 242,
	SDLK_WORLD_83		= 243,
	SDLK_WORLD_84		= 244,
	SDLK_WORLD_85		= 245,
	SDLK_WORLD_86		= 246,
	SDLK_WORLD_87		= 247,
	SDLK_WORLD_88		= 248,
	SDLK_WORLD_89		= 249,
	SDLK_WORLD_90		= 250,
	SDLK_WORLD_91		= 251,
	SDLK_WORLD_92		= 252,
	SDLK_WORLD_93		= 253,
	SDLK_WORLD_94		= 254,
	SDLK_WORLD_95		= 255,		/* 0xFF */
        /*@}*/

	/** @name Numeric keypad */
        /*@{*/
	SDLK_KP0		= 256,
	SDLK_KP1		= 257,
	SDLK_KP2		= 258,
	SDLK_KP3		= 259,
	SDLK_KP4		= 260,
	SDLK_KP5		= 261,
	SDLK_KP6		= 262,
	SDLK_KP7		= 263,
	SDLK_KP8		= 264,
	SDLK_KP9		= 265,
	SDLK_KP_PERIOD		= 266,
	SDLK_KP_DIVIDE		= 267,
	SDLK_KP_MULTIPLY	= 268,
	SDLK_KP_MINUS		= 269,
	SDLK_KP_PLUS		= 270,
	SDLK_KP_ENTER		= 271,
	SDLK_KP_EQUALS		= 272,
        /*@}*/

	/** @name Arrows + Home/End pad */
        /*@{*/
	SDLK_UP			= 273,
	SDLK_DOWN		= 274,
	SDLK_RIGHT		= 275,
	SDLK_LEFT		= 276,
	SDLK_INSERT		= 277,
	SDLK_HOME		= 278,
	SDLK_END		= 279,
	SDLK_PAGEUP		= 280,
	SDLK_PAGEDOWN		= 281,
        /*@}*/

	/** @name Function keys */
        /*@{*/
	SDLK_F1			= 282,
	SDLK_F2			= 283,
	SDLK_F3			= 284,
	SDLK_F4			= 285,
	SDLK_F5			= 286,
	SDLK_F6			= 287,
	SDLK_F7			= 288,
	SDLK_F8			= 289,
	SDLK_F9			= 290,
	SDLK_F10		= 291,
	SDLK_F11		= 292,
	SDLK_F12		= 293,
	SDLK_F13		= 294,
	SDLK_F14		= 295,
	SDLK_F15		= 296,
        /*@}*/

	/** @name Key state modifier keys */
        /*@{*/
	SDLK_NUMLOCK		= 300,
	SDLK_CAPSLOCK		= 301,
	SDLK_SCROLLOCK		= 302,
	SDLK_RSHIFT		= 303,
	SDLK_LSHIFT		= 304,
	SDLK_RCTRL		= 305,
	SDLK_LCTRL		= 306,
	SDLK_RALT		= 307,
	SDLK_LALT		= 308,
	SDLK_RMETA		= 309,
	SDLK_LMETA		= 310,
	SDLK_LSUPER		= 311,		/**< Left "Windows" key */
	SDLK_RSUPER		= 312,		/**< Right "Windows" key */
	SDLK_MODE		= 313,		/**< "Alt Gr" key */
	SDLK_COMPOSE		= 314,		/**< Multi-key compose key */
        /*@}*/

	/** @name Miscellaneous function keys */
        /*@{*/
	SDLK_HELP		= 315,
	SDLK_PRINT		= 316,
	SDLK_SYSREQ		= 317,
	SDLK_BREAK		= 318,
	SDLK_MENU		= 319,
	SDLK_POWER		= 320,		/**< Power Macintosh power key */
	SDLK_EURO		= 321,		/**< Some european keyboards */
	SDLK_UNDO		= 322,		/**< Atari keyboard has Undo */
        /*@}*/

	/* Add any other keys here */

	SDLK_LAST
} SDLKey;


#define KP_0 KP0
#define KP_1 KP1
#define KP_2 KP2
#define KP_3 KP3
#define KP_4 KP4
#define KP_5 KP5
#define KP_6 KP6
#define KP_7 KP7
#define KP_8 KP8
#define KP_9 KP9
#define NUMLOCKCLEAR NUMLOCK
#define GRAVE DOLLAR
#define APOSTROPHE QUOTE
#define LGUI LMETA
// Overkill haha
#define A a
#define B b
#define C c
#define D d
#define E e
#define F f
#define G g
#define H h
#define I i
#define J j
#define K k
#define L l
#define M m
#define N n
#define O o
#define P p
#define Q q
#define R r
#define S s
#define T t
#define U u
#define V v
#define W w
#define X x
#define Y y
#define Z z


int SDL_android_keymap[KEYCODE_LAST+1];


#define SDL_KEY2(X) SDLK_ ## X
#define SDL_KEY(X) SDL_KEY2(X)

void ANDROID_InitOSKeymap()
{
  int i;
  int * keymap = SDL_android_keymap;


  // TODO: keys are mapped rather randomly

  for (i=0; i<KEYCODE_LAST+1; ++i)
    SDL_android_keymap[i] = SDL_KEY(UNKNOWN);

  keymap[KEYCODE_UNKNOWN] = SDL_KEY(UNKNOWN);

  keymap[KEYCODE_BACK] = SDL_KEY(ESCAPE);//SDL_KEY(SDL_KEY_VAL(SDL_ANDROID_KEYCODE_5));

  // TODO: make this configurable
  keymap[KEYCODE_MENU] = SDL_KEY(SPACE);

  keymap[KEYCODE_SEARCH] = SDL_KEY(SPACE);
  keymap[KEYCODE_CALL] = SDL_KEY(SPACE);
  keymap[KEYCODE_DPAD_CENTER] = SDL_KEY(RCTRL);

  //keymap[KEYCODE_CALL] = SDL_KEY(RCTRL);
  //keymap[KEYCODE_DPAD_CENTER] = SDL_KEY(LALT);
  
  keymap[KEYCODE_VOLUME_UP] = SDL_KEY(COMMA);
  keymap[KEYCODE_VOLUME_DOWN] = SDL_KEY(PERIOD);
  
  keymap[KEYCODE_HOME] = SDL_KEY(HOME); // Cannot be used in application

  keymap[KEYCODE_CAMERA] = '~';

  keymap[KEYCODE_0] = SDL_KEY(0);
  keymap[KEYCODE_1] = SDL_KEY(1);
  keymap[KEYCODE_2] = SDL_KEY(2);
  keymap[KEYCODE_3] = SDL_KEY(3);
  keymap[KEYCODE_4] = SDL_KEY(4);
  keymap[KEYCODE_5] = SDL_KEY(5);
  keymap[KEYCODE_6] = SDL_KEY(6);
  keymap[KEYCODE_7] = SDL_KEY(7);
  keymap[KEYCODE_8] = SDL_KEY(8);
  keymap[KEYCODE_9] = SDL_KEY(9);
  keymap[KEYCODE_STAR] = SDL_KEY(KP_DIVIDE);
  keymap[KEYCODE_POUND] = SDL_KEY(KP_MULTIPLY);

  keymap[KEYCODE_DPAD_UP] = SDL_KEY(UP);
  keymap[KEYCODE_DPAD_DOWN] = SDL_KEY(DOWN);
  keymap[KEYCODE_DPAD_LEFT] = SDL_KEY(LEFT);
  keymap[KEYCODE_DPAD_RIGHT] = SDL_KEY(RIGHT);

  keymap[KEYCODE_SOFT_LEFT] = SDL_KEY(COMMA);
  keymap[KEYCODE_SOFT_RIGHT] = SDL_KEY(PERIOD);
  keymap[KEYCODE_ENTER] = SDL_KEY(RETURN); //SDL_KEY(KP_ENTER);


  keymap[KEYCODE_CLEAR] = SDL_KEY(BACKSPACE);
  keymap[KEYCODE_A] = SDL_KEY(A);
  keymap[KEYCODE_B] = SDL_KEY(B);
  keymap[KEYCODE_C] = SDL_KEY(C);
  keymap[KEYCODE_D] = SDL_KEY(D);
  keymap[KEYCODE_E] = SDL_KEY(E);
  keymap[KEYCODE_F] = SDL_KEY(F);
  keymap[KEYCODE_G] = SDL_KEY(G);
  keymap[KEYCODE_H] = SDL_KEY(H);
  keymap[KEYCODE_I] = SDL_KEY(I);
  keymap[KEYCODE_J] = SDL_KEY(J);
  keymap[KEYCODE_K] = SDL_KEY(K);
  keymap[KEYCODE_L] = SDL_KEY(L);
  keymap[KEYCODE_M] = SDL_KEY(M);
  keymap[KEYCODE_N] = SDL_KEY(N);
  keymap[KEYCODE_O] = SDL_KEY(O);
  keymap[KEYCODE_P] = SDL_KEY(P);
  keymap[KEYCODE_Q] = SDL_KEY(Q);
  keymap[KEYCODE_R] = SDL_KEY(R);
  keymap[KEYCODE_S] = SDL_KEY(S);
  keymap[KEYCODE_T] = SDL_KEY(T);
  keymap[KEYCODE_U] = SDL_KEY(U);
  keymap[KEYCODE_V] = SDL_KEY(V);
  keymap[KEYCODE_W] = SDL_KEY(W);
  keymap[KEYCODE_X] = SDL_KEY(X);
  keymap[KEYCODE_Y] = SDL_KEY(Y);
  keymap[KEYCODE_Z] = SDL_KEY(Z);
  keymap[KEYCODE_COMMA] = SDL_KEY(COMMA);
  keymap[KEYCODE_PERIOD] = SDL_KEY(PERIOD);
  keymap[KEYCODE_TAB] = SDL_KEY(TAB);
  keymap[KEYCODE_SPACE] = SDL_KEY(SPACE);
  keymap[KEYCODE_DEL] = SDL_KEY(DELETE);
  keymap[KEYCODE_GRAVE] = SDL_KEY(GREATER);
  keymap[KEYCODE_MINUS] = SDL_KEY(KP_MINUS);
  keymap[KEYCODE_PLUS] = SDL_KEY(KP_PLUS);
  keymap[KEYCODE_EQUALS] = SDL_KEY(EQUALS);
  keymap[KEYCODE_LEFT_BRACKET] = SDL_KEY(LEFTBRACKET);
  keymap[KEYCODE_RIGHT_BRACKET] = SDL_KEY(RIGHTBRACKET);
  keymap[KEYCODE_BACKSLASH] = SDL_KEY(BACKSLASH);
  keymap[KEYCODE_SEMICOLON] = SDL_KEY(SEMICOLON);
  keymap[KEYCODE_APOSTROPHE] = SDL_KEY(AMPERSAND);
  keymap[KEYCODE_SLASH] = SDL_KEY(SLASH);
  keymap[KEYCODE_AT] = SDL_KEY(KP_PERIOD);

  keymap[KEYCODE_MEDIA_PLAY_PAUSE] = SDL_KEY(KP_2);
  keymap[KEYCODE_MEDIA_STOP] = SDL_KEY(HELP);
  keymap[KEYCODE_MEDIA_NEXT] = SDL_KEY(KP_8);
  keymap[KEYCODE_MEDIA_PREVIOUS] = SDL_KEY(KP_5);
  keymap[KEYCODE_MEDIA_REWIND] = SDL_KEY(KP_1);
  keymap[KEYCODE_MEDIA_FAST_FORWARD] = SDL_KEY(KP_3);
  keymap[KEYCODE_MUTE] = SDL_KEY(KP_0);

  keymap[KEYCODE_SYM] = SDL_KEY(LGUI);
  keymap[KEYCODE_NUM] = SDL_KEY(NUMLOCKCLEAR);

  keymap[KEYCODE_ALT_LEFT] = SDL_KEY(KP_7);
  keymap[KEYCODE_ALT_RIGHT] = SDL_KEY(KP_9);

  keymap[KEYCODE_SHIFT_LEFT] = SDL_KEY(F1);
  keymap[KEYCODE_SHIFT_RIGHT] = SDL_KEY(F2);

  keymap[KEYCODE_EXPLORER] = SDL_KEY(F3);
  keymap[KEYCODE_ENVELOPE] = SDL_KEY(F4);

  keymap[KEYCODE_HEADSETHOOK] = SDL_KEY(F5);
  keymap[KEYCODE_FOCUS] = SDL_KEY(F6);
  keymap[KEYCODE_NOTIFICATION] = SDL_KEY(F7);

  // Cannot be received by application, OS internal
  keymap[KEYCODE_ENDCALL] = SDL_KEY(LSHIFT);
  keymap[KEYCODE_POWER] = SDL_KEY(CAPSLOCK);
  keymap[KEYCODE_BUTTON_L1] = 'a'; //L1
  keymap[KEYCODE_BUTTON_R1] = 'd';//R1
  keymap[KEYCODE_BUTTON_X] = 133;// ctrl
  keymap[KEYCODE_BUTTON_Y] = 13;//enter
  keymap[KEYCODE_BUTTON_START] = SDL_KEY(ESCAPE);//KEYCODE_BUTTON_START
  keymap[KEYCODE_BUTTON_SELECT] = SDL_KEY(SPACE); //MAP

}
//translate android key to sdl key
int translate_key(int android_key){
	if(android_key > KEYCODE_LAST)
		return 0;
	return SDL_android_keymap[android_key];
}
////
unsigned short	d_8to16table[256];
unsigned	d_8to24table[256];

#ifdef SUPPORT_8BIT_MIPMAPGENERATION
unsigned char d_15to8table[65536];
#endif

cvar_t  mouse_button_commands[3] =
{
    CVAR2("mouse1","+attack"),
    CVAR2("mouse2","+strafe"),
    CVAR2("mouse3","+forward"),
};

static const int MOUSE_LEFTBUTTON = 1;
static const int MOUSE_MIDDLEBUTTON = 2;
static const int MOUSE_RIGHTBUTTON = 4;

bool     mouse_tap;
float   mouse_x, mouse_y;
float   old_mouse_x, old_mouse_y;
float     mx, my;
bool    mouse_buttonstate;
bool    mouse_oldbuttonstate;

cvar_t  m_filter = CVAR2("m_filter","1");

int scr_width, scr_height;

cvar_t	_windowed_mouse = CVAR3("_windowed_mouse","0", true);

/*-----------------------------------------------------------------------*/

//int		texture_mode = GL_NEAREST;
//int		texture_mode = GL_NEAREST_MIPMAP_NEAREST;
//int		texture_mode = GL_NEAREST_MIPMAP_LINEAR;
int		texture_mode = GL_LINEAR;
// int		texture_mode = GL_LINEAR_MIPMAP_NEAREST;
//int		texture_mode = GL_LINEAR_MIPMAP_LINEAR;

int		texture_extension_number = 1;

float		gldepthmin, gldepthmax;

cvar_t	gl_ztrick = CVAR2("gl_ztrick","0");

const char *gl_vendor;
const char *gl_renderer;
const char *gl_version;
const char *gl_extensions;

qboolean is8bit = false;
qboolean isPermedia = false;
qboolean gl_mtexable = false;

/*-----------------------------------------------------------------------*/
void D_BeginDirectRect (int x, int y, byte *pbitmap, int width, int height)
{
}

void D_EndDirectRect (int x, int y, int width, int height)
{
}

void VID_Shutdown(void)
{
}

void VID_ShiftPalette(unsigned char *p)
{
//	VID_SetPalette(p);
}

void	VID_SetPalette (unsigned char *palette)
{
  byte	*pal;
  unsigned r,g,b;
  unsigned v;
  int     r1,g1,b1;
  int		k;
  unsigned short i;
  unsigned	*table;
  FILE *f;
  char s[255];
  int dist, bestdist;
  static qboolean palflag = false;

  PMPBEGIN(("VID_SetPalette"));
//
// 8 8 8 encoding
//pal array is store 8bit color like rgbrgbrgb
//bellow pack rgb to argb into a int , and store in array
//d_8to24table table
  Con_Printf("Converting 8to24\n");

  pal = palette;
  table = d_8to24table;
  for (i=0 ; i<256 ; i++)
  {
    r = pal[0];
    g = pal[1];
    b = pal[2];
    pal += 3;

//		v = (255<<24) + (r<<16) + (g<<8) + (b<<0);
//		v = (255<<0) + (r<<8) + (g<<16) + (b<<24);
    v = (255<<24) + (r<<0) + (g<<8) + (b<<16);
    *table++ = v;
  }
  d_8to24table[255] &= 0xffffff;	// 255 is transparent

#ifdef SUPPORT_8BIT_MIPMAPGENERATION

  // JACK: 3D distance calcs - k is last closest, l is the distance.
  // FIXME: Precalculate this and cache to disk.
  if (palflag)
    return;
  palflag = true;

  COM_FOpenFile("glquake/15to8.pal", &f);
  if (f) {
    fread(d_15to8table, 1<<15, 1, f);
    fclose(f);
  } else {
    PMPBEGIN(("Creating 15to8 palette"));
    for (i=0; i < (1<<15); i++) {
      /* Maps
       0000.0000.0000.0000
       0000.0000.0001.1111 = Red   = 0x001F
       0000.0011.1110.0000 = Green = 0x03E0
       0111.1100.0000.0000 = Blue  = 0x7C00
       */
       r = ((i & 0x1F) << 3)+4;
       g = ((i & 0x03E0) >> (5-3)) +4;
       b = ((i & 0x7C00) >> (10-3))+4;
      pal = (unsigned char *)d_8to24table;
      for (v=0,k=0,bestdist=0x7FFFFFFF; v<256; v++,pal+=4) {
         r1 = (int)r - (int)pal[0];
         g1 = (int)g - (int)pal[1];
         b1 = (int)b - (int)pal[2];
        dist = ((r1*r1)+(g1*g1)+(b1*b1));
        if (dist < bestdist) {
          k=v;
          bestdist = dist;
        }
      }
      d_15to8table[i]=k;
    }
    PMPEND(("Creating 15to8 palette"));
    sprintf(s, "%s/glquake", com_gamedir);
     Sys_mkdir (s);
    sprintf(s, "%s/glquake/15to8.pal", com_gamedir);
    if ((f = fopen(s, "wb")) != NULL) {
      fwrite(d_15to8table, 1<<15, 1, f);
      fclose(f);
    }
    else
    {
      Con_Printf("Could not write %s\n", s);
    }
  }

#endif // SUPPORT_8BIT_MIPMAPGENERATION
  PMPEND(("VID_SetPalette"));
}

/*
===============
GL_Init
===============
*/
void GL_Init (void)
{
  gl_vendor = (char*) glGetString (GL_VENDOR);
  Con_Printf ("GL_VENDOR: %s\n", gl_vendor);
  GLCHECK("glGetString");
  gl_renderer = (char*) glGetString (GL_RENDERER);
  Con_Printf ("GL_RENDERER: %s\n", gl_renderer);
  GLCHECK("glGetString");

  gl_version = (char*) glGetString (GL_VERSION);
  Con_Printf ("GL_VERSION: %s\n", gl_version);
  GLCHECK("glGetString");
  gl_extensions = (char*) glGetString (GL_EXTENSIONS);
  Con_Printf ("GL_EXTENSIONS: %s\n", gl_extensions);
  GLCHECK("glGetString");

//	Con_Printf ("%s %s\n", gl_renderer, gl_version);

  // Enable/disable multitexture:

  gl_mtexable = true;

  glClearColor (1,0,0,0);
  glCullFace(GL_FRONT);
  glEnable(GL_TEXTURE_2D);

  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER, 0.333);

#ifdef USE_OPENGLES
#else
  glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
#endif
  glShadeModel(GL_FLAT);
    glDisable(GL_DITHER);

    // perspective correction don't work well currently
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

#ifdef USE_OPENGLES
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
#endif
}

/*
=================
GL_BeginRendering

=================
*/
void GL_BeginRendering (int *x, int *y, int *width, int *height)
{
  extern cvar_t gl_clear;

  *x = *y = 0;
  *width = scr_width;
  *height = scr_height;

//    if (!wglMakeCurrent( maindc, baseRC ))
//		Sys_Error ("wglMakeCurrent failed");

//	glViewport (*x, *y, *width, *height);
}


void GL_EndRendering (void)
{
  //glFlush();
  // !!! Swap buffers.
}

void Init_KBD(void)
{
}

// This function controls whether or not 8-bit paletted textures are used:

qboolean VID_Is8bit(void)
{
  return 0;
}

static void Check_Gamma (unsigned char *pal)
{
    float vid_gamma;
  float	f, inf;
  unsigned char	palette[768];
  int		i;

  if ((i = COM_CheckParm("-gamma")) == 0) {
    vid_gamma = 0.5;	// brighten up game.
  } else
    vid_gamma = Q_atof(com_argv[i+1]);

  if(vid_gamma != 1)
  {
    for (i=0 ; i<768 ; i++)
    {
      f = pow ( (pal[i]+1)/256.0 , vid_gamma );
      inf = f*255 + 0.5;
      if (inf < 0)
        inf = 0;
      if (inf > 255)
        inf = 255;
      palette[i] = (unsigned char) inf;
    }
  }

  memcpy (pal, palette, sizeof(palette));
}

void VID_Init(unsigned char *palette)
{
  int i;
  GLint attribs[32];
  char	gldir[MAX_OSPATH];
  int width = scr_width, height = scr_height;

  S_Init();//sound init

  Init_KBD();

  Cvar_RegisterVariable (&gl_ztrick);

  vid.maxwarpwidth = scr_width;
  vid.maxwarpheight = height;
  vid.colormap = host_colormap;
  vid.fullbright = 0xffff;
  //edit by niuzb
  vid.aspect = (float) scr_width / (float) scr_height;
  vid.numpages = 2;
  vid.rowbytes = 2 * scr_width;
  vid.width = scr_width;
  vid.height = scr_height;
 //added by niuzb 
  vid.height = 200;
  vid.width = (unsigned)(vid.height*vid.aspect);
//end added by  niuzb
  //vid.conwidth = scr_width;
  //vid.conheight = scr_height;
  vid.conwidth =  vid.height;
  vid.conheight = vid.width;

// interpret command-line params

// set vid parameters

  GL_Init();

  sprintf (gldir, "%s/glquake", com_gamedir);
  Sys_mkdir (gldir);

  Check_Gamma(palette);
  VID_SetPalette(palette);

  Con_SafePrintf ("Video mode %dx%d initialized.\n", width, height);

  vid.recalc_refdef = 1;				// force a surface cache flush
}

// Android Key event codes. Some of these are
// only generated from the simulator.
// Not all Android devices can generate all codes.

byte scantokey[] =
{
    '$', K_ESCAPE, '$', '$',  K_ESCAPE, '$', '$', '0', //  0.. 7
    '1', '2', '3', '4',  '5', '6', '7', '8', //  8..15
    '9', '$', '$', K_UPARROW,  K_DOWNARROW, K_LEFTARROW, K_RIGHTARROW, K_ENTER, // 16..23
    '$', '$', '$', '$',  '$', 'a', 'b', 'c', // 24..31

    'd', 'e', 'f', 'g',  'h', 'i', 'j', 'k', // 32..39
    'l', 'm', 'n', 'o',  'p', 'q', 'r', 's', // 40..47
    't', 'u', 'v', 'w',  'x', 'y', 'z', ',', // 48..55
    '.', K_CTRL, K_SHIFT, K_TAB,  ' ', '$', '$', '$', // 56..63
  '$', '$', K_ENTER, K_BACKSPACE, '`', '-',  '=', '[', // 64..71
  ']', '\\', ';', '\'', '/', '@',  '#', '$', // 72..79
  '$', '$', K_ESCAPE, '$'                       // 80..83
};

byte scantokeyAlt[] =
{
    0, 0, 0, 0,  0, 0, 0, 0, //  0.. 7
    0, 0, 0, 0,  0, 0, 0, 0, //  8..15
    0, 0, 0, 0,  0, 0, 0, 0, // 16..23
    0, 0, 0, 0,  0, '%', '=', '8', // 24..31

    '5', '2', '6', '-',  '[', '$', ']', '"', // 32..39
    '\'', '>', '<', '(',  ')', '*', '3', '4', // 40..47
    '+', '&', '9', '1',  '7', '!', '#', ';', // 48..55
    ':', 0, 0, 0,  K_TAB, 0, 0, 0, // 56..63
  0, 0, 0, 0,  0, 0, 0, 0, // 64..71
  0, 0, '?', '0',  0, 0, 0, 0, // 72..79
  0, 0, K_ESCAPE, 0                       // 80..83
};

#if 0

byte scantokeyCap[] =
{
    0, 0, 0, 0,  0, 0, 0, 0, //  0.. 7
    0, 0, 0, 0,  0, 0, 0, 0, //  8..15
    0, 0, 0, 0,  0, 0, 0, 0, // 16..23
    0, 0, 0, 0,  0, 'A', 'B', 'C', // 24..31

    'D', 'E', 'F', 'G',  'H', 'I', 'J', 'K', // 32..39
    'L', 'M', 'N', 'O',  'P', 'Q', 'R', 'S', // 40..47
    'T', 'U', 'V', 'W',  'X', 'Y', 'Z', 0, // 48..55
    0, 0, 0, 0,  0, 0, 0, 0, // 56..63
  0, 0, 0, 0,  0, 0, 0, 0, // 64..71
  0, 0, 0, 0,  0, 0, 0, 0, // 72..79
  0, 0, K_ESCAPE, 0                       // 80..83
};

#endif

byte scantokeySym[] =
{
    0, 0, 0, 0,  0, 0, 0, 0, //  0.. 7
    0, 0, 0, 0,  0, 0, 0, 0, //  8..15
    0, 0, 0, 0,  0, 0, 0, 0, // 16..23
    0, 0, 0, 0,  0, 0, 0, K_F8, // 24..31

    K_F5, K_F2, K_F6, '_',  0, 0, 0, 0, // 32..39
    0, 0, 0, 0,  0, 0, K_F3, K_F4, // 40..47
    K_F11, 0, K_F9, K_F1,  K_F7, K_F12, K_PAUSE, 0, // 48..55
    0, 0, 0, 0,  0, 0, 0, 0, // 56..63
  0, 0, 0, 0,  0, 0, 0, 0, // 64..71
  0, 0, '`', K_F10,  0, 0, 0, 0, // 72..79
  0, 0, K_ESCAPE, 0                       // 80..83
};

#define ALT_KEY_VALUE 57
// #define CAPS_KEY_VALUE 58
#define SYM_KEY_VALUE 61

byte modifierKeyInEffect;

qboolean symKeyDown;
byte symKeyCode;

// Called from stand-alone main() function to process an event.
// Return non-zero if the event is handled.

int AndroidEvent(int type, int value)
{
  if(value >= 0 && value < (int) sizeof(scantokey))
  {
    byte key;
    qboolean isPress = type != 0;

    qboolean isModifier = value == ALT_KEY_VALUE || value == SYM_KEY_VALUE;

    if(isModifier)
    {
      if(isPress)
      {
        if(modifierKeyInEffect == value)
        {
          // Press modifier twice to cancel modifier
          modifierKeyInEffect = 0;
        }
        else
        {
          // Most recent modifier key wins
          modifierKeyInEffect = value;
        }
      }
      return 1;
    }
    else
    {
      switch(modifierKeyInEffect)
      {
        default:	        key = scantokey[value]; break;
        case ALT_KEY_VALUE: key = scantokeyAlt[value]; break;
        // case CAP_KEY_VALUE: key = scantokeyCap[value]; break;
        case SYM_KEY_VALUE: key = scantokeySym[value]; break;
      }
      if(!key)
      {
        key = scantokey[value];
      }

      // Hack: Remap @ and / to K_CTRL in game mode
      if(key_dest == key_game && ! modifierKeyInEffect && (key == '@' || key == '/'))
      {
        key = K_CTRL;
      }

      if(!isPress)
      {
        modifierKeyInEffect = 0;
      }
    }

    Key_Event(key, type);
    // PMPLOG(("type: %d, value: %d -> %d '%c'\n", type, value, key, key));

    return 1;
  }
  else
  {
    PMPWARNING(("unexpected event type: %d, value: %d\n", type, value));
  }
  return 0;
}

// Called from Java to process an event.
// Return non-zero if the event is handled.

int AndroidEvent2(int type, int value)
{
  value = translate_key(value);
  Key_Event(value, type);
  return 0;
}

static const int MOTION_DOWN = 0;
static const int MOTION_UP = 1;
static const int MOTION_MOVE = 2;
static const int MOTION_CANCEL = 3;

class GestureDetector {
private:
    bool mIsScroll;
    bool mIsTap;
    bool mIsDoubleTap;

    float mScrollX;
    float mScrollY;

    static const unsigned long long TAP_TIME_MS = 200;
    static const unsigned long long DOUBLE_TAP_TIME_MS = 400;
    static const int TAP_REGION_MANHATTAN_DISTANCE = 10;

    bool mAlwaysInTapRegion;
    float mDownX;
    float mDownY;
    unsigned long long mDownTime;
    unsigned long long mPreviousDownTime;

    /**
     * Position of the last motion event.
     */
    float mLastMotionY;
    float mLastMotionX;
	//int * points[5] = {NULL,};

public:
    /**
     * Analyze a motion event. Call this once for each motion event
     * that is received by a view.
     * @param ev the motion event to analyze.
     */
    void analyze(unsigned long long eventTime, int action,
            float x, float y, float pressure, float size, int deviceId) {
        mIsScroll = false;
        mIsTap = false;
        mIsDoubleTap = false;

        switch (action) {
          case MOTION_DOWN:
            //printf("Down");
            // Remember where the motion event started
            mDownX = x;
            mDownY = y;
            mPreviousDownTime = mDownTime;
            mDownTime = eventTime;
            mAlwaysInTapRegion = true;
            break;

          case MOTION_MOVE:
          {
            mIsScroll = true;
            mScrollX =  x - mLastMotionX ;
            mScrollY = y - mLastMotionY;
            #if 0
            int manhattanTapDistance = (int) (absf(x - mDownX) +
                    absf(y - mDownY));
            if (manhattanTapDistance > TAP_REGION_MANHATTAN_DISTANCE) {
                mAlwaysInTapRegion = false;
            }
			#endif
          }
          break;

          case MOTION_UP:
          {
              unsigned long long doubleTapDelta =
                  eventTime - mPreviousDownTime;
              unsigned long long singleTapDelta =
                  eventTime - mDownTime;

              if (mAlwaysInTapRegion) {
                  if (doubleTapDelta < DOUBLE_TAP_TIME_MS) {
                      mIsDoubleTap = true;
                  }
                  else if (singleTapDelta < TAP_TIME_MS) {
                      mIsTap = true;
                  }
              }
          }
          break;
        }
		
		mLastMotionX = x;
		mLastMotionY = y;
    }

    /**
     * @return true if the current motion event is a scroll
     * event.
     */
    bool isScroll() {
        return mIsScroll;
    }

    /**
     * This value is only defined if {@link #isScroll} is true.
     * @return the X position of the current scroll event.
     * event.
     */
    float scrollX() {
        return mScrollX;
    }

    /**
     * This value is only defined if {@link #isScroll} is true.
     * @return the Y position of the current scroll event.
     * event.
     */
    float scrollY() {
        return mScrollY;
    }

    /**
     * @return true if the current motion event is a single-tap
     * event.
     */
    bool isTap() {
        return mIsTap;
    }

    /**
     * This value is only defined if either {@link #isTap} or
     * {@link #isDoubleTap} is true.
     * @return the X position of the current tap event.
     * event.
     */
    float tapX() {
        return mDownX;
    }

    /**
     * This value is only defined if either {@link #isTap} or
     * {@link #isDoubleTap} is true.
     * @return the Y position of the current tap event.
     * event.
     */
    float tapY() {
        return mDownY;
    }

    /**
     * @return true if the current motion event is a double-tap
     * event.
     */
    bool isDoubleTap() {
        return mIsDoubleTap;
    }

private:
    inline float absf(float a) {
        return a >= 0.0f ? a : -a;
    }
};

GestureDetector gGestureDetector;

int AndroidMotionEvent(unsigned long long eventTime, int action,
        float x, float y, float pressure, float size, int deviceId)
{
    gGestureDetector.analyze(eventTime, action, x, y, pressure, size, deviceId);

    if (gGestureDetector.isTap()) {
        mouse_tap = true;
    }
    else if (gGestureDetector.isScroll()) {
        mx +=  gGestureDetector.scrollX();
        my +=  gGestureDetector.scrollY();
    }

    return true;
}

int AndroidTrackballEvent(unsigned long long eventTime, int action,
        float x, float y)
{
    switch (action ) {
    case MOTION_DOWN:
      mouse_buttonstate = true;
      break;
    case MOTION_UP:
      mouse_buttonstate = false;
      break;
    case MOTION_MOVE:
      mx +=  (36.0f * x);
      my +=  (36.0f * y);
      break;
    }

    return true;
}

void Sys_SendKeyEvents(void)
{
  // Used to poll keyboards on systems that need to poll keyboards.
}

void Force_CenterView_f (void)
{
  cl.viewangles[PITCH] = 0;
}

void IN_Init(void)
{
	ANDROID_InitOSKeymap();//add by niuzb

    Cvar_RegisterVariable (&mouse_button_commands[0]);
    Cvar_RegisterVariable (&mouse_button_commands[1]);
    Cvar_RegisterVariable (&mouse_button_commands[2]);
    Cmd_AddCommand ("force_centerview", Force_CenterView_f);

}

void IN_Shutdown(void)
{
}

/*
===========
IN_Commands
===========
*/
void IN_Commands (void)
{
#if 0
	//not use tap event niuzb
    // perform button actions
    if (mouse_tap) {
        Key_Event (K_MOUSE1, true);
        Key_Event (K_MOUSE1, false);
        mouse_tap = false;
    }
#endif
    if (mouse_buttonstate != mouse_oldbuttonstate) {
      Key_Event (K_MOUSE1, mouse_buttonstate ? true : false);
      mouse_oldbuttonstate = mouse_buttonstate;
    }
}

/*
===========
IN_Move
===========
*/
void IN_MouseMove (usercmd_t *cmd)
{
#if 0
    if (m_filter.value)
    {
        mouse_x = (mx + old_mouse_x) * 0.5;
        mouse_y = (my + old_mouse_y) * 0.5;
    }
    else
#endif
    {
        mouse_x = mx;
        mouse_y = my;
    }
    old_mouse_x = mx;
    old_mouse_y = my;
    mx = my = 0; // clear for next update

    mouse_x *= 5.0f * sensitivity.value;
    mouse_y *= 5.0f * sensitivity.value;

// add mouse X/Y movement to cmd
    if ( (in_strafe.state & 1) || (lookstrafe.value && (in_mlook.state & 1) ))
        cmd->sidemove += m_side.value * mouse_x;
    else
        cl.viewangles[YAW] -= m_yaw.value * mouse_x;

    if (in_mlook.state & 1)
        V_StopPitchDrift ();
	//yeah, we use y to pitch niuzb
    if (1 /*(in_mlook.state & 1) && !(in_strafe.state & 1)*/)
    {
        cl.viewangles[PITCH] += m_pitch.value * mouse_y;
       // cl.viewangles[PITCH] += 150*host_frametime*(mouse_y>0?1:-1);
        if (cl.viewangles[PITCH] > 80)
            cl.viewangles[PITCH] = 80;
        if (cl.viewangles[PITCH] < -70)
            cl.viewangles[PITCH] = -70;
    }
    else
    {
        if ((in_strafe.state & 1) && noclip_anglehack)
            cmd->upmove -= m_forward.value * mouse_y;
        else
            cmd->forwardmove -= m_forward.value * mouse_y;
    }
}

void IN_Move (usercmd_t *cmd)
{
  IN_MouseMove(cmd);
}


