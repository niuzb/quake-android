package org.kwaak3;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectInputStream;
import java.util.zip.GZIPInputStream;

import tk.niuzb.game.SetPreferencesActivity;
import tk.niuzb.kkik3.R;
import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;
import android.view.KeyEvent;

class Settings {

	static byte[] loadRaw(Activity p, int res) {
		byte[] buf = new byte[65536 * 2];
		byte[] a = new byte[0];
		try {
			InputStream is = new GZIPInputStream(p.getResources()
					.openRawResource(res));
			int readed = 0;
			while ((readed = is.read(buf)) >= 0) {
				byte[] b = new byte[a.length + readed];
				System.arraycopy(a, 0, b, 0, a.length);
				System.arraycopy(buf, 0, b, a.length, readed);
				a = b;
			}
		} catch (Exception e) {
		}
		;
		return a;
	}

	private static void loadconfig(Activity p) {
  
      Context c = p.getApplicationContext();
      SharedPreferences prefs = c.getSharedPreferences(SetPreferencesActivity.
  PREFERENCE_NAME,Context.MODE_PRIVATE);
      Globals.PhoneHasTrackball = prefs.getBoolean("toggle_has_trackball", true);
      String s  = prefs.getString("list_trackball_dampening", "1");
      Globals.TrackballDampening = Integer.parseInt(s);
      Globals.PhoneHasArrowKeys = prefs.getBoolean("toggle_has_arrowkeys", true);
      Globals.UseAccelerometerAsArrowKeys = prefs.getBoolean("toggle_use_accelerometer_as_arrowkeys", false);
      Globals.UseTouchscreenKeyboard = prefs.getBoolean("toggle_use_touchscreen", 
  true);
          
      Globals.UseDpadAsTurn = prefs.getBoolean("toggle_use_turn", 
      false);
      s = prefs.getString("list_touchscreen_size", "2");
      Globals.TouchscreenKeyboardSize = Integer.parseInt(s);
          s = prefs.getString("list_touchscreen_transparent", "2");
      Globals.TouchscreenKeyboardTransparency = Integer.parseInt(s);
      Globals.leftKey = prefs.getInt(SetPreferencesActivity.PREFERENCE_LEFT_KEY, 
  KeyEvent.KEYCODE_DPAD_LEFT);
      Globals.rightKey = prefs.getInt(SetPreferencesActivity.PREFERENCE_RIGHT_KEY
  , KeyEvent.KEYCODE_DPAD_RIGHT);
      Globals.upKey = prefs.getInt(SetPreferencesActivity.PREFERENCE_UP_KEY, 
  KeyEvent.KEYCODE_DPAD_UP);
      Globals.downKey = prefs.getInt(SetPreferencesActivity.PREFERENCE_DOWN_KEY, 
  KeyEvent.KEYCODE_DPAD_DOWN);
      Globals.fireKey = prefs.getInt(SetPreferencesActivity.PREFERENCE_FIRE_KEY, 
  KeyEvent.KEYCODE_SEARCH);
      Globals.doorKey = prefs.getInt(SetPreferencesActivity.PREFERENCE_DOOR_KEY, 
  KeyEvent.KEYCODE_SPACE);
      Globals.tleftKey = prefs.getInt(SetPreferencesActivity.PREFERENCE_TLEFT_KEY
  , KeyEvent.KEYCODE_Q);
      Globals.trightKey = prefs.getInt(SetPreferencesActivity.
  PREFERENCE_TRIGHT_KEY, KeyEvent.KEYCODE_E);
  }
	
	static void Apply(Activity p) {
		loadconfig(p);
		{
			ObjectInputStream settingsFile = null;
			int[] iconPosition = new int[24];
			int notfind = 1;
    		try {
    			 settingsFile = new ObjectInputStream(new FileInputStream( p.getFilesDir().getAbsolutePath() + "/" + ".touch_config"));
    			 
    			 for(int i = 0; i<24; i++) {
    				 iconPosition[i] = settingsFile.readInt();
    				 if(i == 23)
    					 notfind = 0;
    				// Log.v("quake", "setting "+ iconPosition[i]);
    			 }
    			 if(settingsFile != null) {
    				 settingsFile.close();
    			 }
    		} catch( FileNotFoundException e ) {
    			
    		} catch( SecurityException e ) {
    		} catch ( IOException e ) {}
    		int state = Globals.UseDpadAsTurn?
                1:0;
    		if( !Globals.UseTouchscreenKeyboard ){
    			state = -1;
    		}
			KwaakJNI.nativeSetupScreenKeyboard(	Globals.TouchscreenKeyboardSize, 
										state,
										Globals.AppTouchscreenKeyboardKeysAmount, 
										notfind,
										Globals.TouchscreenKeyboardTransparency,iconPosition);

			
		}
		// Log.d("quake", "apply");
		KwaakJNI.keybutton(loadRaw(p, R.raw.simpletheme));
	}

	// private static native void nativeSetupScreenKeyboard(int size,int trans);
}
