package tk.niuzb.game;
/*
 * Copyright (C) 2010 The Android Open Source Project
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



import android.os.Bundle;
import android.preference.Preference;
import android.preference.PreferenceActivity;
import tk.niuzb.soddemo.R;


public class SetPreferencesActivity extends PreferenceActivity  {
	public final static String PREFERENCE_NAME = "wolf3d_sod";
    public static final String PREFERENCE_LEFT_KEY = "keyLeft";
    public static final String PREFERENCE_RIGHT_KEY = "keyRight";
    public static final String PREFERENCE_UP_KEY = "keyUp";
    public static final String PREFERENCE_DOWN_KEY = "keyDown";
    public static final String PREFERENCE_FIRE_KEY = "keyFire";
    public static final String PREFERENCE_DOOR_KEY = "keyDoor";
    public static final String PREFERENCE_TLEFT_KEY = "keyTLeft";
    public static final String PREFERENCE_TRIGHT_KEY = "keyTRight";
	@Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
     
		
        getPreferenceManager().setSharedPreferencesMode(MODE_PRIVATE);
        getPreferenceManager().setSharedPreferencesName(PREFERENCE_NAME);
        
        // Load the preferences from an XML resource
        addPreferencesFromResource(R.xml.preferences);
        
      
        Preference configureKeyboardPref = getPreferenceManager().findPreference("keyconfig");
        if (configureKeyboardPref != null) {
        	KeyboardConfigDialogPreference config = (KeyboardConfigDialogPreference)configureKeyboardPref;
        	config.setPrefs(getSharedPreferences(PREFERENCE_NAME, MODE_PRIVATE));
        	config.setContext(this);
        }
    }

	
}
