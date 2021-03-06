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

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.view.Window;
import android.view.WindowManager;

import android.widget.TextView;
import java.lang.Thread;
import java.util.concurrent.locks.ReentrantLock;
import android.os.Build;
import android.util.Log;
import android.app.Activity;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;



/**
 * An implementation of SurfaceView that uses the dedicated surface for
 * displaying an OpenGL animation.  This allows the animation to run in a
 * separate thread, without requiring that it be driven by the update mechanism
 * of the view hierarchy.
 *
 * The application-specific rendering code is delegated to a GLView.Renderer
 * instance.
 */
class QuakeView extends GLSurfaceView {
    QuakeRenderer mRenderer;

    QuakeView(Context context, Activity p) {
        super(context);
        this.parent = p;
        init();
    }

    public QuakeView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    private void init() {
        touchInput = DifferentTouchInput.getInstance();
        // We want events.
        setFocusable(true);
        setFocusableInTouchMode(true);
        requestFocus();
    }

    public void setQuakeLib(QuakeLib quakeLib) {
        mQuakeLib = quakeLib;
        mRenderer = new QuakeRenderer();
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
        queueKeyEvent(QuakeLib.KEY_PRESS,
                keyCodeToQuakeCode(keyCode));
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
            break;        }
        queueKeyEvent(QuakeLib.KEY_RELEASE,
                keyCodeToQuakeCode(keyCode));
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
        return (keyCode != KeyEvent.KEYCODE_VOLUME_UP) &&
            (keyCode != KeyEvent.KEYCODE_VOLUME_DOWN) &&
            (keyCode != KeyEvent.KEYCODE_SEARCH);
    }
/*
    @Override
    public boolean dispatchTouchEvent(MotionEvent ev) {
        queueMotionEvent(ev);
        return true;
    }
    */
    
        @Override
        public boolean onTouchEvent(final MotionEvent event) 
        {
            touchInput.process(event);
            // Wait a bit, and try to synchronize to app framerate, or event thread 
    //will eat all CPU and we'll lose FPS
    /*
            synchronized (mRenderer) {
                try {
                    mRenderer.wait(100L);
                } catch (InterruptedException e) { }
            }
    */
            return true;
        };

    private int keyCodeToQuakeCode(int keyCode) {
        int key = 0;
        if (key >= sKeyCodeToQuakeCode.length) {
            return 0;
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
        queueEvent(
            new Runnable() {
                public void run() {
                    mQuakeLib.event(type, keyCode);
                }
            });
    }


    public void queueTrackballEvent(final MotionEvent ev) {
        queueEvent(
            new Runnable() {
                public void run() {
                    mQuakeLib.trackballEvent(ev.getEventTime(),
                            ev.getAction(),
                            ev.getX(), ev.getY());
                }
            });
    }

    private boolean mShiftKeyPressed;
    private boolean mAltKeyPressed;

    private static final int[] sKeyCodeToQuakeCode = {
        '$', QuakeLib.K_ESCAPE, '$', '$',  QuakeLib.K_ESCAPE, '$', '$', '0', //  0.. 7
        '1', '2', '3', '4',  '5', '6', '7', '8', //  8..15
        '9', '$', '$', QuakeLib.K_UPARROW,  QuakeLib.K_DOWNARROW, QuakeLib.K_LEFTARROW, QuakeLib.K_RIGHTARROW, QuakeLib.K_ENTER, // 16..23
        '$', '$', '$', QuakeLib.K_HOME,  '$', 'a', 'b', 'c', // 24..31

        'd', 'e', 'f', 'g',  'h', 'i', 'j', 'k', // 32..39
        'l', 'm', 'n', 'o',  'p', 'q', 'r', 's', // 40..47
        't', 'u', 'v', 'w',  'x', 'y', 'z', ',', // 48..55
        '.', QuakeLib.K_ALT, QuakeLib.K_ALT, QuakeLib.K_SHIFT,  QuakeLib.K_SHIFT, QuakeLib.K_TAB, ' ', '$', // 56..63
        '$', '$', QuakeLib.K_ENTER, QuakeLib.K_BACKSPACE, '`', '-',  '=', '[', // 64..71
        ']', '\\', ';', '\'', '/', QuakeLib.K_CTRL,  '#', '$', // 72..79
        QuakeLib.K_HOME, '$', QuakeLib.K_ENTER, '$',  '$'                      // 80..84
    };

    private static final int sKeyCodeToQuakeCodeShift[] =
    {
        0, 0, 0, 0,  0, 0, 0, ')', //  0.. 7
        '!', '@', '#', '$',  '%', '^', '&', '*', //  8..15
        '(', 0, 0, 0,  0, 0, 0, 0, // 16..23
        0, 0, 0, 0,  0, 0, ']', 0, // 24..31

        '\\', '_', '{', '}',  ':', '-', ';', '"', // 32..39
        '\'', '>', '<', '+',  '=', 0, 0, '|', // 40..47
        0, 0, '[', '`',  0, 0, QuakeLib.K_PAUSE, ';', // 48..55
        0, 0, 0, 0,  0, 0, 0, 0, // 56..63
        0, 0, 0, 0,  0, 0, 0, 0, // 64..71
        0, 0, '?', '0',  0, QuakeLib.K_CTRL, 0, 0, // 72..79
        0, 0, 0, 0,  0                       // 80..84
    };

    private static final int sKeyCodeToQuakeCodeAlt[] =
    {
        0, 0, 0, 0,  0, 0, 0, QuakeLib.K_F10, //  0.. 7
        QuakeLib.K_F1, QuakeLib.K_F2, QuakeLib.K_F3, QuakeLib.K_F4,  QuakeLib.K_F5, QuakeLib.K_F6, QuakeLib.K_F7, QuakeLib.K_F8, //  8..15
        QuakeLib.K_F9, 0, 0, 0,  0, 0, 0, 0, // 16..23
        0, 0, 0, 0,  0, 0, 0, 0, // 24..31

        0, 0, 0, 0,  0, 0, 0, 0, // 32..39
        0, 0, 0, 0,  0, 0, 0, 0, // 40..47
        QuakeLib.K_F11, 0, 0, 0,  0, QuakeLib.K_F12, 0, 0, // 48..55
        0, 0, 0, 0,  0, 0, 0, 0, // 56..63
        0, 0, 0, 0,  0, 0, 0, 0, // 64..71
        0, 0, 0, 0,  0, 0, 0, 0, // 72..79
        0, 0, 0, 0,  0           // 80..83
    };

    private class QuakeRenderer implements GLSurfaceView.Renderer {
        public void onDrawFrame(GL10 gl) {
            if (mWidth != 0 &&  mHeight != 0) {
                mGameMode = mQuakeLib.step(mWidth, mHeight);
            }
        }

        public void onSurfaceChanged(GL10 gl, int width, int height) {
            mWidth = width;
            mHeight = height;
            mQuakeLib.init();
            Settings.Apply(parent);
            
        }

        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            // Do nothing.
        }
        private int mWidth;
        private int mHeight;
    }
    
	DifferentTouchInput touchInput = null;
    private QuakeLib mQuakeLib;
    private boolean mGameMode;
    Activity parent;
}

