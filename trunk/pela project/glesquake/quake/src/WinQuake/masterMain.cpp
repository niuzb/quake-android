/* //device/apps/Quake/quake/src/QW/client/main.c
**
** Copyright 2007, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/
#define LOG_TAG "quake"

#ifdef ANDROID_NDK

#else
#include <nativehelper/jni.h>
#include <utils/Log.h>
#endif

#include <stdio.h>
#include <assert.h>
#include <dlfcn.h>
#include "quakedef.h"
#include "touchscreenkeyboard.h"


int AndroidInit();
int AndroidEvent2(int type, int value);
int AndroidMotionEvent(unsigned long long eventTime, int action,
        float x, float y, float pressure, float size, int deviceId);
int AndroidTrackballEvent(unsigned long long eventTime, int action,
        float x, float y);
int AndroidStep(int width, int height);
void AndroidQuit();
int g_use_touchscreen_icon = 1;
jboolean
qinit(JNIEnv *env, jobject thiz,jboolean use_touch, jstring datadir) {
	jboolean iscopy;
	const char *dir = env->GetStringUTFChars(datadir, &iscopy);
	LOGI("home dir %s\n", dir);
	setenv("HOME", dir, 1);
	env->ReleaseStringUTFChars(datadir, dir);
	LOGI("qinit");
	if(use_touch == JNI_TRUE)
		g_use_touchscreen_icon = 1;
	else 
		g_use_touchscreen_icon = 0;
    return AndroidInit() ? JNI_TRUE : JNI_FALSE;
 }

jboolean
qevent(JNIEnv *env, jobject thiz, jint type, jint value) {
    return AndroidEvent2(type, value) ? JNI_TRUE : JNI_FALSE;
}
static int last_pointer = -1;

void
qmotionevent( JNIEnv*  env, jobject  thiz, jint x, 
jint y, jint action, jint pointerId, jint force, jint radius ) {
	//LOGI("qmotionevent:x:%d, y:%d, action:%d, id:%d",x, y, action, pointerId);
	if(g_use_touchscreen_icon && processTouchscreenKeyboard(x, y, action, pointerId) )
		return;
			if( action == MOUSE_DOWN ){
				
				//LOGI("DOWN, x:%f, y:%f,", x, y);
				last_pointer = pointerId;

			} else if( action == MOUSE_MOVE){
				if(pointerId != last_pointer)
					return;
	
			}else if( action == MOUSE_UP){
				//LOGI("up, x:%f, y:%f,", x, y);
				last_pointer = -1;
			}


     AndroidMotionEvent((unsigned long long) Sys_FloatTime*1000,
            action, x, y, force, radius,
            pointerId);
}

jboolean
qtrackballevent(JNIEnv *env, jobject thiz, jlong eventTime, jint action,
        jfloat x, jfloat y) {
    return AndroidTrackballEvent((unsigned long long) eventTime,
            action, x, y) ? JNI_TRUE : JNI_FALSE;
}

jboolean
qstep(JNIEnv *env, jobject thiz, jint width, jint height) {
    return AndroidStep(width, height)  ? JNI_TRUE : JNI_FALSE;
}

void
qquit(JNIEnv *env, jobject thiz) {
    LOGI("qquit");
    AndroidQuit();
 }
#define EXPORT_ME __attribute__ ((visibility("default")))
extern int paint_audio (void *unused, void * stream, int len);

extern "C" void EXPORT_ME
Java_tk_niuzb_quake3_QuakeLib_Quake2PaintAudio( JNIEnv* env,jobject thiz, jobject buf )
{

	void *stream;
	int len;


	stream = (env)->GetDirectBufferAddress(   buf);
	len = (env)->GetDirectBufferCapacity(  buf);

	//__android_log_print(ANDROID_LOG_DEBUG, "quake2-jni.c", "paint_audio %p %d\n", stream, len );

	 paint_audio ( NULL, stream, len );

}

static float dx = 0.04, dy = 0.1, dz = 0.1; 
void updateOrientation ( float accX, float accY, float accZ )
{
	
	// TODO: ask user for accelerometer precision from Java

	static float midX = 0, midY = 0, midZ = 0;
	static int pressLeft = 0, pressRight = 0, pressUp = 0, pressDown = 0, pressR = 0, pressL = 0;
	

	if( accX < midX - dx )
	{
		if( !pressLeft )
		{
			//__android_log_print(ANDROID_LOG_INFO, "libSDL", "Accelerometer: press left, acc %f mid %f d %f", accX, midX, dx);
			pressLeft = 1;
			AndroidEvent2( SDL_PRESSED, (KEYCODE_DPAD_LEFT) );
		}
	}
	else
	{
		if( pressLeft )
		{
			//__android_log_print(ANDROID_LOG_INFO, "libSDL", "Accelerometer: release left, acc %f mid %f d %f", accX, midX, dx);
			pressLeft = 0;
			AndroidEvent2( SDL_RELEASED, (KEYCODE_DPAD_LEFT) );
		}
	}
	if( accX < midX - dx*2 )
		midX = accX + dx*2;

	if( accX > midX + dx )
	{
		if( !pressRight )
		{
			//__android_log_print(ANDROID_LOG_INFO, "libSDL", "Accelerometer: press right, acc %f mid %f d %f", accX, midX, dx);
			pressRight = 1;
			AndroidEvent2( SDL_PRESSED, (KEYCODE_DPAD_RIGHT) );
		}
	}
	else
	{
		if( pressRight )
		{
			//__android_log_print(ANDROID_LOG_INFO, "libSDL", "Accelerometer: release right, acc %f mid %f d %f", accX, midX, dx);
			pressRight = 0;
			AndroidEvent2( SDL_RELEASED, (KEYCODE_DPAD_RIGHT) );
		}
	}
	if( accX > midX + dx*2 )
		midX = accX - dx*2;

	if( accY < midY - dy )
	{
		if( !pressUp )
		{
			//__android_log_print(ANDROID_LOG_INFO, "libSDL", "Accelerometer: press up, acc %f mid %f d %f", accY, midY, dy);
			pressUp = 1;
			AndroidEvent2( SDL_PRESSED, (KEYCODE_DPAD_DOWN) );
		}
	}
	else
	{
		if( pressUp )
		{
			//__android_log_print(ANDROID_LOG_INFO, "libSDL", "Accelerometer: release up, acc %f mid %f d %f", accY, midY, dy);
			pressUp = 0;
			AndroidEvent2( SDL_RELEASED, (KEYCODE_DPAD_DOWN) );
		}
	}
	if( accY < midY - dy*2 )
		midY = accY + dy*2;

	if( accY > midY + dy )
	{
		if( !pressDown )
		{
			//__android_log_print(ANDROID_LOG_INFO, "libSDL", "Accelerometer: press down, acc %f mid %f d %f", accY, midY, dy);
			pressDown = 1;
			AndroidEvent2( SDL_PRESSED, (KEYCODE_DPAD_UP) );
		}
	}
	else
	{
		if( pressDown )
		{
			//__android_log_print(ANDROID_LOG_INFO, "libSDL", "Accelerometer: release down, acc %f mid %f d %f", accY, midY, dy);
			pressDown = 0;
			AndroidEvent2( SDL_RELEASED, (KEYCODE_DPAD_UP) );
		}
	}
	if( accY > midY + dy*2 )
		midY = accY - dy*2;

	if( accZ < midZ - dz )
	{
		if( !pressL )
		{
			pressL = 1;
			AndroidEvent2( SDL_PRESSED, (KEYCODE_ALT_LEFT) );
		}
	}
	else
	{
		if( pressL )
		{
			pressL = 0;
			AndroidEvent2( SDL_RELEASED, (KEYCODE_ALT_LEFT) );
		}
	}
	if( accZ < midZ - dz*2 )
		midZ = accZ + dz*2;

	if( accZ > midZ + dz )
	{
		if( !pressR )
		{
			pressR = 1;
			AndroidEvent2( SDL_PRESSED, (KEYCODE_ALT_RIGHT) );
		}
	}
	else
	{
		if( pressR )
		{
			pressR = 0;
			AndroidEvent2( SDL_RELEASED, (KEYCODE_ALT_RIGHT) );
		}
	}
	if( accZ > midZ + dz*2 )
		midZ = accZ - dz*2;

}
extern "C" void EXPORT_ME Java_tk_niuzb_quake3_AccelerometerReader_nativeOrientation( JNIEnv*  env, jobject  thiz, jfloat accX, jfloat accY, jfloat accZ )
{
	//LOGI("get accerometer %f %f\n", accX, accY);
	updateOrientation (accX, accY, accZ); // TODO: make values in range 0.0:1.0
}

static const char *classPathName = "tk/niuzb/quake3/QuakeLib";
extern void keybutton(JNIEnv * env, jobject thiz, jbyteArray charBufJava);
static JNINativeMethod methods[] = {
  {"init", "(ZLjava/lang/String;)Z", (void*)qinit },
  {"event", "(II)Z", (void*)qevent },
  {"nativeMouse", "(IIIIII)V", (void*) qmotionevent },
  {"trackballEvent", "(JIFF)Z", (void*) qtrackballevent },
  {"step", "(II)Z", (void*)qstep },
  {"quit", "()V", (void*)qquit },
  {"keybutton", "([B)V", (void*)keybutton},
  //{"Quake2PaintAudio", "(Ljava/lang/Object)V", (void*)Quake2PaintAudio},

};

/*
 * Register several native methods for one class.
 */
static int registerNativeMethods(JNIEnv* env, const char* className,
    JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz;

    clazz = env->FindClass(className);
    if (clazz == NULL) {
        fprintf(stderr,
            "Native registration unable to find class '%s'\n", className);
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        fprintf(stderr, "RegisterNatives failed for '%s'\n", className);
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

/*
 * Register native methods for all classes we know about.
 */
static int registerNatives(JNIEnv* env)
{
  if (!registerNativeMethods(env, classPathName,
                 methods, sizeof(methods) / sizeof(methods[0]))) {
    return JNI_FALSE;
  }

  return JNI_TRUE;
}

/*
 * Set some test stuff up.
 *
 * Returns the JNI version on success, -1 on failure.
 */
 typedef union {
		JNIEnv* env;
		void* venv;
 } UnionJNIEnvToVoid;

JavaVM* jniVM;

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	
	UnionJNIEnvToVoid uenv;
    uenv.venv = NULL;
    jint result = -1;
    JNIEnv* env = NULL;
	jniVM = vm;
    if (vm->GetEnv(&uenv.venv, JNI_VERSION_1_2) != JNI_OK) {
        fprintf(stderr, "ERROR: GetEnv failed\n");
        goto bail;
    }
    env = uenv.env;

    assert(env != NULL);

    printf("In mgmain JNI_OnLoad\n");

    if (!registerNatives(env)) {
        fprintf(stderr, "ERROR: quakemaster native registration failed\n");
        goto bail;
    }

    /* success -- return valid version number */
    result = JNI_VERSION_1_2;

bail:
    return result;
}
