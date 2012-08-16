/*
 * Kwaak3
 * Copyright (C) 2010 Roderick Colenbrander
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

package org.kwaak3;

import java.io.IOException;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;

abstract class DifferentTouchInput {

	public static DifferentTouchInput getInstance() {
		if ((Build.VERSION.SDK_INT) <= 4) {
			return SingleTouchInput.Holder.sInstance;
		} else {
			Log.v("quake", "multi touch");
			return MultiTouchInput.Holder.sInstance;
		}
	}

	public abstract void process( final MotionEvent event);

	private static class SingleTouchInput extends DifferentTouchInput {
		private long mLastTouchTime = 0L;

		private static class Holder {
			private static final SingleTouchInput sInstance = new SingleTouchInput();
		}

		public void process(final MotionEvent event) {
			final long time = System.currentTimeMillis();
			if (event.getAction() == MotionEvent.ACTION_MOVE
					&& time - mLastTouchTime < 32) {
				return;

			}
			mLastTouchTime = time;

			int action = -1;
			if (event.getAction() == MotionEvent.ACTION_DOWN)
				action = 0;
			if (event.getAction() == MotionEvent.ACTION_UP)
				action = 1;
			if (event.getAction() == MotionEvent.ACTION_MOVE)
				action = 2;
			if (action >= 0)
				KwaakJNI.queueMotionEvent(action, event.getX(),  event.getY(),
						 0);
		}
	}

	private static class MultiTouchInput extends DifferentTouchInput {

		private long mLastTouchTime = 0L;

		private static class Holder {
			private static final MultiTouchInput sInstance = new MultiTouchInput();
		}

		public void process( final MotionEvent event) {

			final long time = System.currentTimeMillis();
			if ((event.getAction() & MotionEvent.ACTION_MASK) == MotionEvent.ACTION_MOVE
					&& time - mLastTouchTime < 32) {

				return;
			}
			mLastTouchTime = time;

			for (int i = 0; i < event.getPointerCount(); i++) {
				int action = -1;
				boolean point = false;
				switch (event.getAction() & MotionEvent.ACTION_MASK) {
				case MotionEvent.ACTION_DOWN:
					action = 0;
					break;
				case MotionEvent.ACTION_POINTER_DOWN:
					point = true;
					action = 0;
					break;
				case MotionEvent.ACTION_UP:
					action = 1;
					break;
				case MotionEvent.ACTION_POINTER_UP:
					point = true;
					action = 1;
					break;
				case MotionEvent.ACTION_MOVE:
					action = 2;
					break;
				}
				int pid = event.getPointerId(i);
				int act = event.getAction();
				int id = act >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;

				if (((point && pid == id) || !point) && action >= 0) {
					//Log.v("quake", "motion event,********"+action+"     pid: "+event.getPointerId(i));
					//v.queueMotionEvent
					KwaakJNI.queueMotionEvent( action,event.getX(pid),  event
							.getY(pid),  event.getPointerId(i));
				}
			}
		}
	}
}

class KwaakView extends GLSurfaceView {
	private KwaakRenderer mKwaakRenderer;

	public KwaakView(Context context) {
		super(context);

		/*
		 * We need the path to the library directory for dlopen in our JNI
		 * library
		 */
		String cache_dir, lib_dir;

		touchInput = DifferentTouchInput.getInstance();
		try {
			cache_dir = context.getCacheDir().getCanonicalPath();
			lib_dir = cache_dir.replace("cache", "lib");
		} catch (IOException e) {
			e.printStackTrace();
			lib_dir = "/data/data/tk.niuzb.kkik3/lib";
		}
		KwaakJNI.setLibraryDirectory(lib_dir);

		mKwaakRenderer = new KwaakRenderer();
		setRenderer(mKwaakRenderer);

		setFocusable(true);
		setFocusableInTouchMode(true);
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		int qKeyCode = androidKeyCodeToQuake(Globals.TranslateScancode(keyCode, true), event);
		// Log.d("Quake_JAVA", "onKeyDown=" + keyCode + " " + qKeyCode + " " +
		// event.getDisplayLabel() + " " + event.getUnicodeChar() + " " +
		// event.getNumber());
		return queueKeyEvent(qKeyCode, 1);
	}

	@Override
	public boolean onKeyUp(int keyCode, KeyEvent event) {
		int qKeyCode = androidKeyCodeToQuake(Globals.TranslateScancode(keyCode, false), event);
		// Log.d("Quake_JAVA", "onKeyUp=" + keyCode + " " + qKeyCode + " shift="
		// + event.isShiftPressed() + " =" + event.getMetaState());
		return queueKeyEvent(qKeyCode, 0);
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		// Log.d("Quake_JAVA", "onTouchEvent action=" + event.getAction() +
		// " x=" + event.getX() + " y=" + event.getY() + " pressure=" +
		// event.getPressure() + "size = " + event.getSize());
		/*
		 * Perhaps we should pass integers down to reduce the number of float
		 * computations
		 */
		// return queueMotionEvent(event.getAction(), event.getX(),
		// event.getY(), event.getPressure());
		touchInput.process(event);
		// Wait a bit, and try to synchronize to app framerate, or event thread
		// will eat all CPU and we'll lose FPS

		return true;

	}

	@Override
	public boolean onTrackballEvent(MotionEvent event) {
		// Log.d("Quake_JAVA", "onTrackballEvent action=" + event.getAction() +
		// " x=" + event.getX() + " y=" + event.getY());
		return queueTrackballEvent(event.getAction(), event.getX(), event
				.getY());
	}

	public boolean queueKeyEvent(final int qKeyCode, final int state) {
		if (qKeyCode == 0)
			return true;

		/*
		 * Make sure all communication with Quake is done from the Renderer
		 * thread
		 */
		queueEvent(new Runnable() {
			public void run() {
				KwaakJNI.queueKeyEvent(qKeyCode, state);
			}
		});
		return true;
	}

	public boolean queueMotionEvent(final int action, final float x,
			final float y, final int id) {
		/*
		 * Make sure all communication with Quake is done from the Renderer
		 * thread
		 */
		queueEvent(new Runnable() {
			public void run() {
				KwaakJNI.queueMotionEvent(action, x, y, id);
			}
		});
		return true;
	}

	private boolean queueTrackballEvent(final int action, final float x,
			final float y) {
		/*
		 * Make sure all communication with Quake is done from the Renderer
		 * thread
		 */
		queueEvent(new Runnable() {
			public void run() {
				KwaakJNI.queueTrackballEvent(action, x, y);
			}
		});
		return true;
	}

	DifferentTouchInput touchInput = null;

	private static final int QK_ENTER = 13;
	private static final int QK_ESCAPE = 27;
	private static final int QK_BACKSPACE = 127;
	private static final int QK_LEFT = 134;
	private static final int QK_RIGHT = 135;
	private static final int QK_UP = 132;
	private static final int QK_DOWN = 133;
	private static final int QK_CTRL = 137;
	private static final int QK_SHIFT = 138;
	private static final int QK_CONSOLE = 340;

	private static final int QK_F1 = 145;
	private static final int QK_F2 = 146;
	private static final int QK_F3 = 147;
	private static final int QK_F4 = 148;

	private int androidKeyCodeToQuake(int aKeyCode, KeyEvent event) {
		/* Convert non-ASCII keys by hand */
		switch (aKeyCode) {
		/*
		 * For now map the focus buttons to F1 and let the user remap it in
		 * game. This should allow some basic movement on the Nexus One if
		 * people map it to forward. At least on the Milestone the camera button
		 * itself is shared with the Focus one. You have to press focus first
		 * and then you hit camera, this leads to the following event sequence
		 * which I don't handle right now: focus_down -> camera_down ->
		 * camera_up -> focus_up.
		 */
		case KeyEvent.KEYCODE_FOCUS:
			return QK_F1;
		case KeyEvent.KEYCODE_VOLUME_DOWN:
			return QK_F2;
		case KeyEvent.KEYCODE_VOLUME_UP:
			return QK_F3;
		case KeyEvent.KEYCODE_DPAD_UP:
			return QK_UP;
		case KeyEvent.KEYCODE_DPAD_DOWN:
			return QK_DOWN;
		case KeyEvent.KEYCODE_DPAD_LEFT:
			return QK_LEFT;
		case KeyEvent.KEYCODE_DPAD_RIGHT:
			return QK_RIGHT;
		case KeyEvent.KEYCODE_DPAD_CENTER:
			/* Center is useful for shooting if you only use the keyboard */
			return QK_CTRL;
		case KeyEvent.KEYCODE_ENTER:
    case KeyEvent.KEYCODE_MENU:
			return QK_ENTER;
		case KeyEvent.KEYCODE_SEARCH:
			return QK_CONSOLE;
		case KeyEvent.KEYCODE_BACK:
			return QK_ESCAPE;
		case KeyEvent.KEYCODE_DEL:
			return QK_BACKSPACE;
		case KeyEvent.KEYCODE_ALT_LEFT:
			return QK_CTRL;
		case KeyEvent.KEYCODE_SHIFT_LEFT:
			return QK_SHIFT;
		}

		/*
		 * Let Android do all the character conversion for us. This way we don't
		 * have to care about modifier keys and specific keyboard layouts. TODO:
		 * add some more filtering
		 */
		int uchar = event.getUnicodeChar();
		if (uchar < 256)
			return uchar;

		return 0;
	}

}
