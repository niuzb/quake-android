
#include <jni.h>
#include <android/log.h>
#include <sys/time.h>
#include <time.h>
#include <stdint.h>
#include <math.h>
#include <string.h> // for memset()
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <netinet/in.h>
#include "android_glimp.h"
#include "../client/client.h"
#include "../renderer/tr_local.h"

#include "touchscreenkeyboard.h"
#include "../client/keycodes.h"


#ifndef Uint8
#define Uint8 unsigned char
#endif
static SDL_Rect arrow, buttons[MAX_BUTTONS];
enum {FIRE, SWITCH_GUN, JUMP, CONSOLE, TURN};
static int buttonKeysyms[MAX_BUTTONS] = { 
K_CTRL,//fire
0xff,//chang weapon
K_SPACE,//jump
's',//toggle console
0xff,
};
int oldTurn = 0; //record the last turn code
static int turnKeysyms[2][2] = {

	{  //DpadAsturn = false
		K_LEFTARROW,
		K_RIGHTARROW,
	},
	
	{
			'a',
			'd',
	},
	
};
int arrowkeys[4]={K_LEFTARROW, K_RIGHTARROW, K_UPARROW,K_DOWNARROW };
enum {LEFT, RIGHT, UP, DOWN};

static int show_gun = 0;
//record witch gun is choosen now
static int pressed_num = -1;

//the base/up/down/left/right dpad icon
static GLTexture_t arrowImages = {0,0,0};
//now we use 4 icon, maybe more latter. fire, change gun, jump,,map
static int nbuttons = 5;
//record if the button is pressed. 0: not pressed 1:pressed
static int button_pressed[5] = {0,};
static int DpadAsturn = 0;

//max to MAX_BUTTONS button
static GLTexture_t buttonImages[MAX_BUTTONS*2] = { {0, 0, 0}, };
//static int buttonsize = 1;
static int transparency = 200;

static SDL_Rect * OldCoords[5] = { NULL };
int jump_press = 0,jump_release = 0;
int g_use_touchscreen_icon = 1;//default use touch screen

static __inline__ int
power_of_2(int input)
{
    int value = 1;

    while (value < input) {
        value <<= 1;
    }
    return value;
}
extern 	void queueKeyEvent(int key, int state);
#define Key_Event(a,b) queueKeyEvent(a, b)
extern int abs( int n );
/*check witch arrow key is pressed*/
static inline int ArrowKeysPressed(int x, int y)
{
	int ret = 0, dx, dy;
	dx = x - arrow.x - arrow.w / 2;
	dy = y - arrow.y - arrow.h / 2;
	// Single arrow key pressed
	if( abs(dy / 2) >= abs(dx) )
	{
		if( dy < 0 )
			ret |= ARROW_UP;
		else
			ret |= ARROW_DOWN;
	}
	else if( abs(dx / 2) >= abs(dy) )
	{
		if( dx > 0 )
			ret |= ARROW_RIGHT;
		else
			ret |= ARROW_LEFT;
	}
	else // Two arrow keys pressed
	{
		if( dx > 0 )
			ret |= ARROW_RIGHT;
		else
			ret |= ARROW_LEFT;

		if( dy < 0 )
			ret |= ARROW_UP;
		else
			ret |= ARROW_DOWN;
	}
	return ret;
}

/*set all arrow key up*/
void reset_arrow_key() {
	int i;
	
	for(i = 0; i<4; i++) {
		Key_Event(arrowkeys[i], SDL_RELEASED);
	}
	return;
}

//weather usr touch the gun number, retern the number 
int SDL_ANDROID_get_number(int x, int y) {
  int num = -1;
  int tap = (screen_width-32*9)/9;

  if(y < 32) {
    num = 1+ x/(tap+32);
  }
  return num;
}

/*
//=========================
//process touch screen icon touch event
*/
void __inline__ Key_Event_Alt(int key, int action) 
{
	 if(DpadAsturn == 1) 
	 	Key_Event(key, action); 
	 else {
		key = key==K_LEFTARROW ? 0 : 1;
		Key_Event( turnKeysyms[1][key], action);
	 }
} 
int oldArrows;
//check if press left of the turn
int TurnKeyPressed(x) {
	if( x < (buttons[TURN].x+buttons[TURN].w/2))
		return 1;
	else 
		return 2;

}
int processTouchscreenKeyboard(int x, int y, int action, int pointerId)
{
		int i;
		int keysym;
		
		extern	clientStatic_t		cls;
		if(!g_use_touchscreen_icon){
			return 0;
		}
		if(cls.state != CA_ACTIVE) {
			return 0;
		
		}
		//LOGI("touch x:%d, y:%d, a:%d, id:%d", x, y, action, pointerId);
		if( action == MOUSE_DOWN )
		{
		
			if( OldCoords[pointerId] )
			{
				processTouchscreenKeyboard(x, y, MOUSE_UP, pointerId);
			}
			//do the move stuff
			if( InsideRect( &arrow, x, y ) )
			{
				 OldCoords[pointerId] = &arrow;
				 //i = ArrowKeysPressed(x, y);
				 i = ArrowKeysPressed(x, y);
				 //reset_arrow_key();
				 if( i & ARROW_UP ) {
				   Key_Event(arrowkeys[UP],SDL_PRESSED);
				 }
				 if( i & ARROW_DOWN ) {
					 Key_Event(arrowkeys[DOWN],SDL_PRESSED);
				 }
				 if( i & ARROW_LEFT ) {
					 Key_Event_Alt(arrowkeys[LEFT],SDL_PRESSED);
				 }
				 if( i & ARROW_RIGHT ) {
					 Key_Event_Alt(arrowkeys[RIGHT],SDL_PRESSED);
				 }
				 oldArrows = i;
				//__android_log_print(ANDROID_LOG_INFO, "doom", "down arrow %d",pointerId);
				return 1;
			} 
			if(show_gun) {
			  int num = SDL_ANDROID_get_number(x, y);
			  
			  if (num > 0) {
				//change the gun
				pressed_num = num;
				OldCoords[pointerId] = (SDL_Rect * )&pressed_num;
				//__android_log_print(ANDROID_LOG_INFO, "libSDL", "pressed num:%d", pressed_num);
				
				keysym = '0';
				keysym += num;
				Key_Event(keysym ,SDL_PRESSED);
				return 1;
			  }
			}

			//process other touch icon:fire ,jump ,switch gun
			for( i = 0; i < nbuttons; i++)
			{
				if( InsideRect( &buttons[i], x, y) )
				{
					OldCoords[pointerId] = &buttons[i];
					button_pressed[i] = 1;
					//LOGI("button %d is settttttttttt", i);
					if (i == SWITCH_GUN) {
					  //switch gun
					  show_gun = show_gun==0 ? 1 : 0;
					  return 1;
					}
					
					if( i == TURN) {
						//jump_press= 1;
						//Key_Event(SDL_PRESSED, buttonKeysyms[][]);
						if(x < buttons[i].x+buttons[i].w/2) {
							//SDL_SendKeyboardKey( SDL_PRESSED, GetKeysym( SDL_KEY(A), &keysym) );
							Key_Event( turnKeysyms[DpadAsturn][LEFT], SDL_PRESSED);
							oldTurn = 1;
						}
						else { 
							//SDL_SendKeyboardKey( SDL_PRESSED, GetKeysym( SDL_KEY(D), &keysym) );
							Key_Event( turnKeysyms[DpadAsturn][RIGHT],SDL_PRESSED);
							oldTurn = 2;
						}
						return 1;
					} 
					
					Key_Event(buttonKeysyms[i], SDL_PRESSED);
					
					return 1;
				}
			}
		}
		else if( action == MOUSE_UP )
		{
			if( OldCoords[pointerId] == &arrow)
			{
				OldCoords[pointerId] = NULL;
				//we draw use arrow_move, when touch up.return to the center
				//arrow_move = arrow_cricle;
				
				i = oldArrows;
				//reset_arrow_key();
				if( i & ARROW_UP ) {
				  Key_Event(arrowkeys[UP],SDL_RELEASED);
				}
				if( i & ARROW_DOWN ) {
					Key_Event(arrowkeys[DOWN],SDL_RELEASED);
				}
				if( i & ARROW_LEFT ) {
					Key_Event_Alt(arrowkeys[LEFT],SDL_RELEASED);
				}
				if( i & ARROW_RIGHT ) {
					Key_Event_Alt(arrowkeys[RIGHT],SDL_RELEASED);
				}
				oldArrows = 0;
				//__android_log_print(ANDROID_LOG_INFO, "doom", "up arrow %d",pointerId);
				return 1;
			} 

			for( i = 0; i < nbuttons; i++ )
			{
				if( OldCoords[pointerId] == &buttons[i] )
				{
					button_pressed[i] = 0;
					//LOGI("button %d is clear", i);
					OldCoords[pointerId] = NULL;
					if(i == SWITCH_GUN) {
					 // OldCoords[pointerId] = NULL;
					  //show_gun = 0;
					  return 1;
					}
					if( i == TURN) {
						
						 //jump_release= 1;
						oldTurn = 0;
						Key_Event( turnKeysyms[DpadAsturn][LEFT], SDL_RELEASED);
						Key_Event( turnKeysyms[DpadAsturn][RIGHT], SDL_RELEASED);
						return 1;
					} 
					Key_Event(buttonKeysyms[i], SDL_RELEASED);
					
					
				
					return 1;
				}
			}
			//process the gun number
			if(show_gun && OldCoords[pointerId] == (SDL_Rect * )&pressed_num) {
			  if (pressed_num>0) {
				
				keysym = '0'+pressed_num;
				Key_Event( keysym, SDL_RELEASED);
				//SDL_SendKeyboardKey(SDL_RELEASED, GetKeysym( SDL_KEY(0), &keysym)+pressed_num );
				pressed_num = -1;
				OldCoords[pointerId] = NULL;
				return 1;
			  }
			}
		}
		else if( action == MOUSE_MOVE )
		{
			//finger slip to ohter place
	 		if( OldCoords[pointerId] && !InsideRect(OldCoords[pointerId], x, y) )
	 		{
	 			processTouchscreenKeyboard(x, y, MOUSE_UP, pointerId);
	 			return processTouchscreenKeyboard(x, y, MOUSE_DOWN, pointerId);
	 		}
	
			 if( OldCoords[pointerId] == &arrow )
			 {
				i = ArrowKeysPressed(x, y);
				if( i == oldArrows )
					return 1;
				
		
				if( oldArrows & ARROW_UP && ! (i & ARROW_UP) )
					Key_Event(arrowkeys[UP],SDL_RELEASED);
				if( oldArrows & ARROW_DOWN && ! (i & ARROW_DOWN) )
					Key_Event(arrowkeys[DOWN],SDL_RELEASED);
				if( oldArrows & ARROW_LEFT && ! (i & ARROW_LEFT) )
					Key_Event_Alt(arrowkeys[LEFT],SDL_RELEASED);
				if( oldArrows & ARROW_RIGHT && ! (i & ARROW_RIGHT) )
					Key_Event_Alt(arrowkeys[RIGHT],SDL_RELEASED);
				if( i & ARROW_UP )
					Key_Event(arrowkeys[UP],SDL_PRESSED);
				if( i & ARROW_DOWN )
					Key_Event(arrowkeys[DOWN],SDL_PRESSED);
				if( i & ARROW_LEFT )
					Key_Event_Alt(arrowkeys[LEFT],SDL_PRESSED);
				if( i & ARROW_RIGHT )
					Key_Event_Alt(arrowkeys[RIGHT],SDL_PRESSED);
				oldArrows = i;
			} else if(OldCoords[pointerId] == &buttons[TURN]){
				 i = TurnKeyPressed(x, y);
				 
				 if( i == oldTurn)
					 return 1;
				 if(oldTurn == 1 && i == 2) {
					 Key_Event( turnKeysyms[DpadAsturn][LEFT], SDL_RELEASED);
					 Key_Event( turnKeysyms[DpadAsturn][RIGHT], SDL_PRESSED);
				 } else if(oldTurn == 2 && i == 1) {
				 
				 	 Key_Event( turnKeysyms[DpadAsturn][RIGHT], SDL_RELEASED);
					 Key_Event( turnKeysyms[DpadAsturn][LEFT], SDL_PRESSED);
				 
				 }
				 oldTurn =	i;
			}
	
			if( OldCoords[pointerId] )
				return 1;
		
			//return processTouchscreenKeyboard(x, y, MOUSE_DOWN, pointerId);
		}
		return 0;
}

static int setupScreenKeyboardButton( int buttonID, Uint8 * charBuf )
{
	// TODO: softstretch with antialiasing
	int w, h,  format;
	GLTexture_t * data = NULL;
	int texture_w, texture_h;
	
	if( buttonID < 1 )
		data = &arrowImages;
	else
		data = &(buttonImages[buttonID-1]);


	memcpy(&w, charBuf, sizeof(int));
	memcpy(&h, charBuf + sizeof(int), sizeof(int));
	memcpy(&format, charBuf + 2*sizeof(int), sizeof(int));
	w = ntohl(w);
	h = ntohl(h);
	format = ntohl(format);
	
	texture_w = power_of_2(w);
	texture_h = power_of_2(h);
	data->w = texture_w;
	data->h = texture_h;
	LOGI("data w:%d, h:%d\n", w, h);

	qglEnable(GL_TEXTURE_2D);

	qglGenTextures(1, &data->id);
	
	qglBindTexture(GL_TEXTURE_2D, data->id);
	LOGI("On-screen keyboard generated OpenGL texture ID %x", data->id);

	qglTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_w, texture_h, 0, GL_RGBA,
					format ? GL_UNSIGNED_SHORT_4_4_4_4 : GL_UNSIGNED_SHORT_5_5_5_1, NULL);
	qglPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	qglTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGBA,
						format ? GL_UNSIGNED_SHORT_4_4_4_4 : GL_UNSIGNED_SHORT_5_5_5_1,
						charBuf + 3*sizeof(int) );

	qglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	qglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	qglDisable(GL_TEXTURE_2D);

	return 3*sizeof(int) + w * h * 2;
}


static inline void drawCharTex(GLTexture_t * tex, SDL_Rect * src, SDL_Rect * dest, 
	Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	GLint cropRect[4];
	/*
	GLfloat texColor[4];
	static const float onediv255 = 1.0f / 255.0f;
	*/
	if( !dest->h || !dest->w )
		return;

	qglBindTexture(GL_TEXTURE_2D, tex->id);

	qglColor4x(r * 0x100, g * 0x100, b * 0x100,  200 * 0x100 );

	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);

	qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	qglEnable(GL_BLEND);
	qglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	qglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	qglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	#if 0
	dest->x = dest->x*320/screen_width;
	dest->y = dest->y*200/screen_height;
	dest->w = dest->w*320/screen_width;
	dest->h = dest->h*200/screen_height;
	DrawQuad(dest->x, 200 - dest->y - dest->h, dest->w, dest->h, 
		0, 0, 1/*tex->w*/, 1/*tex->h*/);
#endif


	cropRect[0] = 0;
	cropRect[1] = tex->h;
	cropRect[2] = tex->w;
	cropRect[3] = -tex->h;
	if(src)
	{//left down width hight
		cropRect[0] = src->x;
		cropRect[1] = src->h; // TODO: check if height works as expected in inverted GL coords
		cropRect[2] = src->w;
		cropRect[3] = -src->h; // TODO: check if height works as expected in inverted GL coords
	}
	qglTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, cropRect);
	glDrawTexiOES(dest->x, screen_height - dest->y - dest->h, 0, dest->w, dest->h);
}
static inline void beginDrawingTex()
{
	qglEnable(GL_TEXTURE_2D);
}

static inline void endDrawingTex()
{

	//glDisable(GL_TEXTURE_2D);
}

//int g_temp = 0xf2;
int drawTouchscreenKeyboard()
{
	int i;
	extern 	clientStatic_t		cls;
	//extern int	keydown[256];
	if(!g_use_touchscreen_icon){
		//LOGI("no need to paint ");
		return 0;
	}
	if(cls.state != CA_ACTIVE) {
		return 0;

	}
	int blendFactor  = 0;
    for(i = 0; i<4; i++) {
		//blendFactor += keydown[arrowkeys[i]]?1:0;
	}

	

	beginDrawingTex();
	//draw the dpad and turn arrow
	if( blendFactor == 0 ) {
      drawCharTex( &arrowImages, NULL, &arrow, 255, 255, 255, transparency );

	}
	else
	{
	  drawCharTex( &arrowImages, NULL, &arrow, 255, 255, 255, transparency / blendFactor );
	}

	for( i = 0; i < nbuttons; i++ )
	{
		if(button_pressed[i] == 0) {//not pressed
			drawCharTex(  &buttonImages[2*i],
						NULL, &buttons[i], 255, 255, 255, transparency );
		} else {
			//pressed
			drawCharTex(  &buttonImages[2*i+1],
						NULL, &buttons[i], 255, 255, 255, transparency );
		}
	}

	if(show_gun) {
      int i;
      SDL_Rect gun_position={0, 0, 32, 32};
      int tap = (screen_width-32*9)/9;
      
      for(i = 0; i < 9; i++) {
        gun_position.x = tap/2+(32+tap)*i;
        drawCharTex(&buttonImages[i+nbuttons*2],
						NULL, &gun_position, 255, 255, 255, transparency);
      }
      
	}
	endDrawingTex();
	return 1;
}

//#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
//set position for a SDL_Rect
//x, y : the centor position of the rect
void setRect(SDL_Rect *r, int x, int y, int w, int h) 
{
	  r->w = w;
	  r->h = h;
	  r->x = x-w/2;
	  r->y = y-h/2;
}
#ifndef MIN
#define MIN(a, b) ((a)<(b)?(a):(b))
#endif

#define jnifuc(name, ...) JNIEXPORT void JNICALL \
JAVA_EXPORT_NAME(name)(__VA_ARGS__)

//jnifuc(Settings_nativeSetupScreenKeyboard, JNIEnv*  env, jobject thiz, jint size, jint _transparency)
void init_screen_key_size( int size, int tt, int _nbuttons, 
int array_invalide, int _transparency, int *body ,int len)
{
	
	nbuttons = _nbuttons;

	if(tt == -1) {
		LOGI("init_screen_key_size, **********************");
		g_use_touchscreen_icon = 0;
		return;
	}
	DpadAsturn = tt;
#if 0
	//not useed
	int _transparency = 8;
	switch(_transparency)
	{
		case 0: transparency = 16; break;
		case 1: transparency = 32; break;
		case 2: transparency = 64; break;
		case 3: transparency = 128; break;
		case 4: transparency = 192; break;
		default: transparency = 128; break;
	}
#endif
	
	if(array_invalide)
	{
		//determine the positon of each icon
		// Arrows to the lower-left part of screen
		float iconscale = (size+1.0)/3.0;
		int tempx, tempy;
		arrow.w = (int)(200.*iconscale);
		arrow.h = (int)(200.*iconscale);
		tempx = screen_width/25+arrow.w/2;
		tempy = screen_height*15/16 - arrow.h/2;
		setRect(&arrow, tempx, tempy, arrow.w, arrow.h);
		
        int w=(int)(75*iconscale);
		
		tempy = screen_height/10+w/2;
		
        //fire
        setRect(&buttons[0], screen_width-(int)(100.*iconscale), screen_height*15/16 - arrow.h/2, (int)(120.*iconscale), (int)(120.*iconscale));
        //switch gun
        setRect(&buttons[1], screen_width/30 + w/2, tempy, w, w);
        //jump
        setRect(&buttons[2], screen_width-(int)(70.*iconscale)/2, screen_height-(int)(80.*iconscale)/2, (int)(70.*iconscale), (int)(80.*iconscale));
        //map
        setRect(&buttons[3], ((screen_width-screen_width/25)-w/2), tempy, w, w);

		//left/right
        setRect(&buttons[4], (screen_width-w*3/2), 
        MIN(screen_height*15/16 - arrow.h/2, screen_height-w-10)-(int)(60.*iconscale)-w/2-15, 2*w, w);
	} else {
		 int i;
		 
		 __android_log_print(ANDROID_LOG_INFO, "quake", "use user set icon");
		 #define SETRect(r) \
		   do {\
		   (r).x = body[i];    \
		   (r).y = body[i+1]; \
		   (r).w = body[i+2]-body[i];  \
		   (r).h = body[i+3] - body[i+1];  \
		   }while(0)
		 for (i=0; i<len; i+=4) {
		 	if(i==0) {
				SETRect(arrow);
			}
			else {
				SETRect(buttons[(i-4)/4]);
			}
        	//sum += body[i];
    	 }
	}
}

//JNIEXPORT void JNICALL
//JAVA_EXPORT_NAME(Settings_nativeSetupScreenKeyboardButtons) ( JNIEnv*  env, jobject thiz, jbyteArray charBufJava )
//jnifuc(Settings_nativeSetupScreenKeyboardButtons,JNIEnv*  env, jobject thiz, jbyteArray charBufJava)
//Java_tk_niuzb_quake3_Settings_nativeSetupScreenKeyboardButtons( JNIEnv*  env, jobject thiz, jbyteArray charBufJava )
void  keybutton(unsigned char * charBuf, int len)
{
	int but, pos;
	
	if(!g_use_touchscreen_icon){
		LOGI("keybutton no need to add icon ");
		return ;
	}
//	int i = 1;
	// __android_log_print(ANDROID_LOG_INFO, "quake","******Settings_nativeSetupScreenKeyboardButtonspos\n");
	for( but = 0, pos = 0; pos < len; but ++ ){
		//LOGI("keyboad:pos %d\n", i++);
		pos += setupScreenKeyboardButton( but, charBuf + pos );
	}
	qglEnable(GL_TEXTURE_2D);
}

