/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package tk.niuzb.quake3;

import java.io.IOException;
import java.nio.ByteBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import tk.niuzb.game.SetPreferencesActivity;
import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.opengl.GLSurfaceView;
import android.os.SystemClock;
import android.util.AttributeSet;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;

/**
 * An implementation of SurfaceView that uses the dedicated surface for
 * displaying an OpenGL animation. This allows the animation to run in a
 * separate thread, without requiring that it be driven by the update mechanism
 * of the view hierarchy.
 * 
 * The application-specific rendering code is delegated to a GLView.Renderer
 * instance.
 */
class QuakeView extends GLSurfaceView {
	QuakeRenderer mRenderer;
	Activity mContext;

	QuakeView(Context context, QuakeActivity p) {
		super(context);
		this.mContext = p;
		this.parent = p;
		init();
	}

	public QuakeView(Context context, AttributeSet attrs) {
		super(context, attrs);
		this.mContext = (Activity)context;
		init();
	}

	private void init() {
		touchInput = DifferentTouchInput.getInstance();
		//accelerometer = new AccelerometerReader(this.mContext);
		// We want events.
		Context c = parent.getApplicationContext();
		SharedPreferences prefs = c.getSharedPreferences(
				SetPreferencesActivity.PREFERENCE_NAME, Context.MODE_PRIVATE);
		Globals.leftKey = prefs.getInt(
				SetPreferencesActivity.PREFERENCE_LEFT_KEY,
				KeyEvent.KEYCODE_DPAD_LEFT);
		Globals.rightKey = prefs.getInt(
				SetPreferencesActivity.PREFERENCE_RIGHT_KEY,
				KeyEvent.KEYCODE_DPAD_RIGHT);
		Globals.upKey = prefs.getInt(SetPreferencesActivity.PREFERENCE_UP_KEY,
				KeyEvent.KEYCODE_DPAD_UP);
		Globals.downKey = prefs.getInt(
				SetPreferencesActivity.PREFERENCE_DOWN_KEY,
				KeyEvent.KEYCODE_DPAD_DOWN);
		Globals.fireKey = prefs.getInt(
				SetPreferencesActivity.PREFERENCE_FIRE_KEY,
				KeyEvent.KEYCODE_SEARCH);
		Globals.doorKey = prefs.getInt(
				SetPreferencesActivity.PREFERENCE_DOOR_KEY,
				KeyEvent.KEYCODE_SPACE);
		Globals.tleftKey = prefs
				.getInt(SetPreferencesActivity.PREFERENCE_TLEFT_KEY,
						KeyEvent.KEYCODE_Q);
		Globals.trightKey = prefs.getInt(
				SetPreferencesActivity.PREFERENCE_TRIGHT_KEY,
				KeyEvent.KEYCODE_E);
		Globals.UseTouchscreenKeyboard = prefs.getBoolean(
				"toggle_use_touchscreen", true);
		setFocusable(true);
		setFocusableInTouchMode(true);
		requestFocus();
	}

	public void setQuakeLib(QuakeLib quakeLib) {
		mQuakeLib = quakeLib;
		mRenderer = new QuakeRenderer(parent);
		setRenderer(mRenderer);
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		if (!weWantThisKeyCode(keyCode)) {
			return super.onKeyDown(keyCode, event);
		}
		switch (keyCode) {
		case KeyEvent.KEYCODE_ALT_RIGHT:
		case KeyEvent.KEYCODE_ALT_LEFT:
			mAltKeyPressed = true;
			break;
		case KeyEvent.KEYCODE_SHIFT_RIGHT:
		case KeyEvent.KEYCODE_SHIFT_LEFT:
			mShiftKeyPressed = true;
			break;
		}
		// queueKeyEvent(QuakeLib.KEY_PRESS,
		// keyCodeToQuakeCode(keyCode));

		queueKeyEvent(QuakeLib.KEY_PRESS,
				keyCodeToQuakeCode(Globals.TranslateScancode(keyCode, true)));
		return true;
	}

	@Override
	public boolean onKeyUp(int keyCode, KeyEvent event) {
		if (!weWantThisKeyCode(keyCode)) {
			return super.onKeyUp(keyCode, event);
		}
		switch (keyCode) {
		case KeyEvent.KEYCODE_ALT_RIGHT:
		case KeyEvent.KEYCODE_ALT_LEFT:
			mAltKeyPressed = false;
			break;
		case KeyEvent.KEYCODE_SHIFT_RIGHT:
		case KeyEvent.KEYCODE_SHIFT_LEFT:
			mShiftKeyPressed = false;
			break;
		}
		// queueKeyEvent(QuakeLib.KEY_RELEASE,
		// keyCodeToQuakeCode(keyCode));
		queueKeyEvent(QuakeLib.KEY_RELEASE,
				keyCodeToQuakeCode(Globals.TranslateScancode(keyCode, false)));
		return true;
	}

	@Override
	public boolean onTrackballEvent(MotionEvent event) {
		if (!mGameMode) {
			return super.onTrackballEvent(event);
		}
		queueTrackballEvent(event);
		return true;
	}

	private boolean weWantThisKeyCode(int keyCode) {
		if (((keyCode == KeyEvent.KEYCODE_VOLUME_DOWN || keyCode == KeyEvent.KEYCODE_VOLUME_UP) && !Globals
				.keyBindingUseVolumeButton()))
			return false;
		return true;
	}

	/*
	 * @Override public boolean dispatchTouchEvent(MotionEvent ev) {
	 * queueMotionEvent(ev); return true; }
	 */

	@Override
	public boolean onTouchEvent(final MotionEvent event) {
		touchInput.process(event);
		// Wait a bit, and try to synchronize to app framerate, or event thread
		// will eat all CPU and we'll lose FPS
		/*
		 * synchronized (mRenderer) { try { mRenderer.wait(100L); } catch
		 * (InterruptedException e) { } }
		 */
		return true;
	};

	private int keyCodeToQuakeCode(int keyCode) {
		int key = 0;
		//just return keycode niuzb
		if(true)
		return keyCode;
		if (keyCode >= sKeyCodeToQuakeCode.length) {
			if(keyCode == 102 ){//l1
				return 'a';
			}else if(keyCode == 103){//r1
				return 'd';
			}else if(keyCode == 99){//x
				return QuakeLib.K_CTRL;
			}else if(keyCode == 100){//y
				return QuakeLib.K_ENTER;
			}else if(keyCode == 108){//start
				return QuakeLib.K_ESCAPE;
			}
			return keyCode;
		}
		if (mAltKeyPressed) {
			key = sKeyCodeToQuakeCodeAlt[keyCode];
			if (key == 0) {
				key = sKeyCodeToQuakeCodeShift[keyCode];
				if (key == 0) {
					key = sKeyCodeToQuakeCode[keyCode];
				}
			}
		} else if (mShiftKeyPressed) {
			key = sKeyCodeToQuakeCodeShift[keyCode];
			if (key == 0) {
				key = sKeyCodeToQuakeCode[keyCode];
			}
		} else {
			key = sKeyCodeToQuakeCode[keyCode];
		}
		if (key == 0) {
			key = '$';
		}
		return key;
	}

	public void queueKeyEvent(final int type, final int keyCode) {
		queueEvent(new Runnable() {
			public void run() {
				mQuakeLib.event(type, keyCode);
			}
		});
	}

	public void queueTrackballEvent(final MotionEvent ev) {
		queueEvent(new Runnable() {
			public void run() {
				mQuakeLib.trackballEvent(ev.getEventTime(), ev.getAction(),
						ev.getX(), ev.getY());
			}
		});
	}

	private boolean mShiftKeyPressed;
	private boolean mAltKeyPressed;

	private static final int[] sKeyCodeToQuakeCode = { '$', QuakeLib.K_ESCAPE,
			'$', '$', QuakeLib.K_ESCAPE,
			'$',
			'$',
			'0', // 0.. 7
			'1', '2', '3', '4', '5',
			'6',
			'7',
			'8', // 8..15
			'9', '$', '$', QuakeLib.K_UPARROW, QuakeLib.K_DOWNARROW,
			QuakeLib.K_LEFTARROW,
			QuakeLib.K_RIGHTARROW,
			QuakeLib.K_ENTER, // 16..23
			'$', '$', '$', QuakeLib.K_HOME, '$', 'a',
			'b',
			'c', // 24..31

			'd', 'e', 'f', 'g', 'h', 'i',
			'j',
			'k', // 32..39
			'l', 'm', 'n', 'o', 'p', 'q',
			'r',
			's', // 40..47
			't', 'u', 'v', 'w', 'x', 'y',
			'z',
			',', // 48..55
			'.', QuakeLib.K_ALT, QuakeLib.K_ALT, QuakeLib.K_SHIFT,
			QuakeLib.K_SHIFT, QuakeLib.K_TAB, ' ',
			'$', // 56..63
			'$', '$', QuakeLib.K_ENTER, QuakeLib.K_BACKSPACE, '`', '-', '=',
			'[', // 64..71
			']', '\\', ';', '\'', '/', QuakeLib.K_CTRL, '#', '$', // 72..79
			QuakeLib.K_HOME, '$', QuakeLib.K_ENTER, '$', '$' // 80..84
			
	};

	private static final int sKeyCodeToQuakeCodeShift[] = { 0, 0, 0, 0, 0, 0,
			0, ')', // 0.. 7
			'!', '@', '#', '$', '%', '^', '&', '*', // 8..15
			'(', 0, 0, 0, 0, 0, 0, 0, // 16..23
			0, 0, 0, 0, 0, 0, ']', 0, // 24..31

			'\\', '_', '{', '}', ':', '-', ';', '"', // 32..39
			'\'', '>', '<', '+', '=', 0, 0, '|', // 40..47
			0, 0, '[', '`', 0, 0, QuakeLib.K_PAUSE, ';', // 48..55
			0, 0, 0, 0, 0, 0, 0, 0, // 56..63
			0, 0, 0, 0, 0, 0, 0, 0, // 64..71
			0, 0, '?', '0', 0, QuakeLib.K_CTRL, 0, 0, // 72..79
			0, 0, 0, 0, 0 // 80..84
	};

	private static final int sKeyCodeToQuakeCodeAlt[] = { 0, 0, 0, 0, 0, 0,
			0,
			QuakeLib.K_F10, // 0.. 7
			QuakeLib.K_F1, QuakeLib.K_F2, QuakeLib.K_F3, QuakeLib.K_F4,
			QuakeLib.K_F5, QuakeLib.K_F6, QuakeLib.K_F7, QuakeLib.K_F8, // 8..15
			QuakeLib.K_F9, 0, 0, 0, 0, 0, 0, 0, // 16..23
			0, 0, 0, 0, 0, 0, 0, 0, // 24..31

			0, 0, 0, 0, 0, 0, 0, 0, // 32..39
			0, 0, 0, 0, 0, 0, 0, 0, // 40..47
			QuakeLib.K_F11, 0, 0, 0, 0, QuakeLib.K_F12, 0, 0, // 48..55
			0, 0, 0, 0, 0, 0, 0, 0, // 56..63
			0, 0, 0, 0, 0, 0, 0, 0, // 64..71
			0, 0, 0, 0, 0, 0, 0, 0, // 72..79
			0, 0, 0, 0, 0 // 80..83
	};
	private static Object quake2Lock = new Object();

	class QuakeRenderer implements GLSurfaceView.Renderer {
		public QuakeRenderer(QuakeActivity p) {
//			System.loadLibrary("sdl-1.2");
//			mAudioThread = new AudioThread(p);
			System.loadLibrary("quake");
		}

		public void onDrawFrame(GL10 gl) {
			synchronized (quake2Lock) {
				if (mWidth != 0 && mHeight != 0) {
					mGameMode = mQuakeLib.step(mWidth, mHeight );
				}
			}
			// if(mGameMode != preMode) {
			// preMode = mGameMode;
			// post(new Runnable() {
			// public void run() {
			// parent.setOverlayView(mGameMode);
			// }
			// });
			//
			// }
		}

		public void onSurfaceChanged(GL10 gl, int width, int height) {
			mWidth = width;
			mHeight = height;
			
			new Thread(new Runnable() {
							public void run() {
								try {
									audio_thread();
								} catch (IOException e) {
									e.printStackTrace();
								}
							}
						}).start();
			mQuakeLib.init(Globals.UseTouchscreenKeyboard, Globals.DataDir);
			// Log.d("quake", "###########quake renderer apply");
			Settings.Apply(parent, mQuakeLib);
			//accelerometer = new AccelerometerReader(parent);

		}
		

		public void audio_thread() throws IOException {

			int audioSize = (2048 * 4);
			
			ByteBuffer audioBuffer = ByteBuffer.allocateDirect(audioSize);
			
			byte[] audioData = new byte[audioSize];
			
			// output to a PCM file
			// adb pull /sdcard/quake2.pcm .
			// sox -L -s -2 -c 2 -r 44100 -t raw quake2.pcm -t wav quake2.wav
			// FileOutputStream out = new FileOutputStream( new File(
			// "/sdcard/quake2.pcm") );
			
			AudioTrack oTrack = new AudioTrack(AudioManager.STREAM_MUSIC,
					22050, // 44100,
					AudioFormat.CHANNEL_CONFIGURATION_STEREO,
					AudioFormat.ENCODING_PCM_16BIT, 4 * (22050 / 5), // 200
																		// millisecond
																		// buffer
																		// => impact
																		// on audio
																		// latency
					AudioTrack.MODE_STREAM);

			

			// Start playing data that is written
			try {
				oTrack.play();
				
				while (!please_exit) {
	
					sQuake2PaintAudio(audioBuffer);
	
					audioBuffer.position(0);
					audioBuffer.get(audioData);
	
					// Write the byte array to the track
					oTrack.write(audioData, 0, audioData.length);

			}
			} catch(Exception e){
				
				
			}
			Log.i("Quake2", "stop audio");

			// Done writting to the track
			oTrack.stop();

		}

		private int sQuake2PaintAudio(ByteBuffer buf) {
			int ret = 0;
			synchronized (quake2Lock) {
				QuakeLib.Quake2PaintAudio(buf);
			}
			return ret;
		}

		public void onSurfaceCreated(GL10 gl, EGLConfig config) {

			// Log.d("quake", "*********************onSurfaceCreated ");

		}

		private int mWidth;
		private int mHeight;
		private AccelerometerReader accelerometer = null;

		AudioThread mAudioThread;
	}

	boolean please_exit = false;
	DifferentTouchInput touchInput = null;
	private QuakeLib mQuakeLib;
	public boolean mGameMode = false;// false means not in play stage
	public boolean preMode = false;
	QuakeActivity parent;
}
