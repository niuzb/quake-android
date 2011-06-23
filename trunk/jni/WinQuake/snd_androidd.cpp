/*
Copyright (C) 1996-1997 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <jni.h>
#include <android/log.h>
#include <string.h> // for memset()

#include "quakedef.h"
#include "snd_androidd.h"
int snd_inited;

static int tryrates[] = { 11025, 22051, 44100, 8000 };
static unsigned char * audioBuffer = NULL;
static size_t audioBufferSize = 0;
extern JavaVM * jniVM;
// Extremely wicked JNI environment to call Java functions from C code
static jbyteArray audioBufferJNI = NULL;
static jobject JavaAudioThread = NULL;
static jmethodID JavaInitAudio = NULL;
static jmethodID JavaDeinitAudio = NULL;
static jmethodID JavagetPlayOffset = NULL;
static jmethodID JavaPauseAudioPlayback = NULL;
static jmethodID JavaResumeAudioPlayback = NULL;




static void ANDROIDAUD_CloseAudio()
{
	//__android_log_print(ANDROID_LOG_INFO, "quake", "ANDROIDAUD_CloseAudio()");
	JNIEnv * jniEnv = NULL;
	(jniVM)->AttachCurrentThread(&jniEnv, NULL);

	(jniEnv)->DeleteGlobalRef(audioBufferJNI);
	audioBufferJNI = NULL;
	audioBuffer = NULL;
	audioBufferSize = 0;
	
	(jniEnv)->CallIntMethod( JavaAudioThread, JavaDeinitAudio );

	/* We cannot call DetachCurrentThread() from main thread or we'll crash */
	/* (jniVM)->DetachCurrentThread(jniVM); */
	
}
static JNIEnv * jniEnvPlaying = NULL;
static jmethodID JavaFillBuffer = NULL;

static void ANDROIDAUD_ThreadInit()
{
	jclass JavaAudioThreadClass = NULL;
	jmethodID JavaInitThread = NULL;
	jmethodID JavaGetBuffer = NULL;
	jboolean isCopy = JNI_TRUE;

	(jniVM)->AttachCurrentThread(&jniEnvPlaying, NULL);

	JavaAudioThreadClass = (jniEnvPlaying)->GetObjectClass( JavaAudioThread);
	JavaFillBuffer = (jniEnvPlaying)->GetMethodID( JavaAudioThreadClass, "fillBuffer", "()I");

	/* HACK: raise our own thread priority to max to get rid of "W/AudioFlinger: write blocked for 54 msecs" errors */
	JavaInitThread = (jniEnvPlaying)->GetMethodID( JavaAudioThreadClass, "initAudioThread", "()I");
	(jniEnvPlaying)->CallIntMethod(JavaAudioThread, JavaInitThread );

	JavaGetBuffer = (jniEnvPlaying)->GetMethodID( JavaAudioThreadClass, "getBuffer", "()[B");
	audioBufferJNI = (jbyteArray)(jniEnvPlaying)->CallObjectMethod(  JavaAudioThread, JavaGetBuffer );
	audioBufferJNI = (jbyteArray)(jniEnvPlaying)->NewGlobalRef( audioBufferJNI);
	shm->buffer = (unsigned char *) (jniEnvPlaying)->GetByteArrayElements( audioBufferJNI, &isCopy);
	if( !shm->buffer )
	{
		__android_log_print(ANDROID_LOG_ERROR, "quake", "ANDROIDAUD_ThreadInit() JNI::GetByteArrayElements() failed! we will crash now");
		return;
	}
	if( isCopy == JNI_TRUE )
		__android_log_print(ANDROID_LOG_ERROR, "quake", "ANDROIDAUD_ThreadInit(): JNI returns a copy of byte array - no audio will be played");

	
	//__android_log_print(ANDROID_LOG_INFO, "quake", "ANDROIDAUD_ThreadInit()");
	//SDL_memset(audioBuffer, this->spec.silence, this->spec.size);
};
#if 0
void  InitJavaCallbacks ()
{
	JNIEnv * jniEnv;
	jclass JavaAudioThreadClass = NULL;
	
	(jniVM)->AttachCurrentThread(&jniEnv, NULL);
	__android_log_print(ANDROID_LOG_ERROR, "quake", "init audio1");
	JavaAudioThread = (jniEnv)->NewGlobalRef( thiz);
	JavaAudioThreadClass = (jniEnv)->GetObjectClass( JavaAudioThread);
	JavaInitAudio = (jniEnv)->GetMethodID( JavaAudioThreadClass, "initAudio", "(IIII)I");
	JavaDeinitAudio = (jniEnv)->GetMethodID( JavaAudioThreadClass, "deinitAudio", "()I");
	JavaPauseAudioPlayback = (jniEnv)->GetMethodID( JavaAudioThreadClass, "pauseAudioPlayback", "()I");
	JavaResumeAudioPlayback = (jniEnv)->GetMethodID( JavaAudioThreadClass, "resumeAudioPlayback", "()I");
	JavagetPlayOffset = (jniEnv)->GetMethodID( JavaAudioThreadClass, "getPlayOffset", "()I");
}
#endif

qboolean SNDDMA_Init(void)
{

	int rc;
    int fmt;
	int tmp;
    int i;
    char *s;
	int caps;
	JNIEnv * jniEnv = NULL;

	snd_inited = 0;

	shm = &sn;
    shm->splitbuffer = 0;
	shm->samplebits = 8;
	shm->channels = 1;
	shm->speed = 11025;
	(jniVM)->AttachCurrentThread( &jniEnv, NULL);
	// The returned audioBufferSize may be huge, up to 100 Kb for 44100 because user may have selected large audio buffer to get rid of choppy sound
	audioBufferSize = (jniEnv)->CallIntMethod( JavaAudioThread, JavaInitAudio, 
					(jint)shm->speed, (jint)shm->channels, 
					(jint)(0), (jint)(1<<5) );

	if( audioBufferSize == 0 )
	{
		__android_log_print(ANDROID_LOG_INFO, "quake", "ANDROIDAUD_OpenAudio(): failed to get audio buffer from JNI");
		ANDROIDAUD_CloseAudio();
		return(-1);
	}
	shm->samples = audioBufferSize;
	shm->submission_chunk = 1;
	ANDROIDAUD_ThreadInit();
	//shm->buffer = audioBuffer;
	shm->soundalive = true;
	//(jniEnv)->CallIntMethod(JavaAudioThread, JavaResumeAudioPlayback );

	snd_inited = 1;
	return 1;

}
void ANDROIDAUD_PlayAudio()
{
	jboolean isCopy = JNI_TRUE;

	//(jniEnvPlaying)->ReleaseByteArrayElements(audioBufferJNI, (jbyte *)shm->buffer, 0);
	//shm->buffer = NULL;

	(jniEnvPlaying)->CallIntMethod(JavaAudioThread, JavaFillBuffer );

	//shm->buffer = (unsigned char *) (jniEnvPlaying)->GetByteArrayElements(audioBufferJNI, &isCopy);
	if( !shm->buffer )
		__android_log_print(ANDROID_LOG_ERROR, "quake", "ANDROIDAUD_PlayAudio() JNI::GetByteArrayElements() failed! we will crash now");

	//if( isCopy == JNI_TRUE )
	//	__android_log_print(ANDROID_LOG_INFO, "quake", "ANDROIDAUD_PlayAudio() JNI returns a copy of byte array - that's slow");
}

int SNDDMA_GetDMAPos(void)
{
	int postion;
#if 0
	struct count_info count;

	if (!snd_inited) return 0;

	if (ioctl(audio_fd, SNDCTL_DSP_GETOPTR, &count)==-1)
	{
		perror("/dev/dsp");
		Con_Printf("Uh, sound dead.\n");
		close(audio_fd);
		snd_inited = 0;
		return 0;
	}
//	shm->samplepos = (count.bytes / (shm->samplebits / 8)) & (shm->samples-1);
//	fprintf(stderr, "%d    \r", count.ptr);
	shm->samplepos = count.ptr / (shm->samplebits / 8);
#endif
	JNIEnv * jniEnv = NULL;
	(jniVM)->AttachCurrentThread(&jniEnv, NULL);

	
	postion = (jniEnv)->CallIntMethod( JavaAudioThread, JavagetPlayOffset );

	shm->samplepos = postion / (shm->samplebits / 8);

	return shm->samplepos;

}

void SNDDMA_Shutdown(void)
{
	if (snd_inited)
	{
		ANDROIDAUD_CloseAudio();
	}
}

/*
==============
SNDDMA_Submit

Send sound to device if buffer isn't really the dma buffer
===============
*/
void SNDDMA_Submit(void)
{
	//ANDROIDAUD_PlayAudio();

}
void SNDDMA_ReportWrite(size_t lengthBytes) {
  
}


JNIEXPORT jint JNICALL Java_com_android_quake_AudioThread_nativeAudioInitJavaCallbacks (JNIEnv * jniEnv, jobject thiz)
{
	jclass JavaAudioThreadClass = NULL;
	
	__android_log_print(ANDROID_LOG_ERROR, "quake", "init audio1");
	JavaAudioThread = (jniEnv)->NewGlobalRef( thiz);
	JavaAudioThreadClass = (jniEnv)->GetObjectClass( JavaAudioThread);
	JavaInitAudio = (jniEnv)->GetMethodID( JavaAudioThreadClass, "initAudio", "(IIII)I");
	JavaDeinitAudio = (jniEnv)->GetMethodID( JavaAudioThreadClass, "deinitAudio", "()I");
	JavaPauseAudioPlayback = (jniEnv)->GetMethodID( JavaAudioThreadClass, "pauseAudioPlayback", "()I");
	JavaResumeAudioPlayback = (jniEnv)->GetMethodID( JavaAudioThreadClass, "resumeAudioPlayback", "()I");
	JavagetPlayOffset = (jniEnv)->GetMethodID( JavaAudioThreadClass, "getPlayOffset", "()I");
}


