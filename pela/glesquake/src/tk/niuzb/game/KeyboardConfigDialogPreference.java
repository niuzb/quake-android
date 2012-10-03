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

import tk.niuzb.quake.R;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnKeyListener;
import android.content.SharedPreferences;
import android.content.res.TypedArray;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.preference.DialogPreference;
import android.util.AttributeSet;
import android.view.KeyEvent;
import android.view.View;
import android.widget.TextView;


public class KeyboardConfigDialogPreference extends DialogPreference implements OnKeyListener {
	private SharedPreferences mSharedPrefs;
	private Context mContext;
	
	//string
//	private String mLeftPrefKey;
//	private String mRightPrefKey;
//	private String mUpPrefKey;
//	private String mDownPrefKey;
//	private String mFirePrefKey;
//	private String mjumpPrefKey;
//	private String mTLeftPrefKey;
//	private String mTRightPrefKey;
//	private String mweaponPrefKey;
//	private String mrunPrefKey;
//	private final static String PREFERENCE_NAME = "quake";
	private static final String PREFERENCE_LEFT_KEY = "keyLeft";
	private static final String PREFERENCE_RIGHT_KEY = "keyRight";
	private static final String PREFERENCE_UP_KEY = "keyUp";
	private static final String PREFERENCE_DOWN_KEY = "keyDown";
	private static final String PREFERENCE_FIRE_KEY = "keyFire";
	private static final String PREFERENCE_JUMP_KEY = "keyjump";
	private static final String PREFERENCE_TLEFT_KEY = "keyTLeft";
	private static final String PREFERENCE_TRIGHT_KEY = "keyTRight";
	private static final String PREFERENCE_WEAPON_KEY = "keyweapon";
	private static final String PREFERENCE_RUN_KEY = "keyrun";
	private String[] mKeyLabels;
	private int mListeningId = 0;
	private View mLeftBorder;
	private View mRightBorder;
	private View mUpBorder;
	private View mDownBorder;
	private View mFireBorder;
	private View mjumpBorder;
	private View mTLeftBorder;
	private View mTRightBorder;
	private View mweaponBorder;
	private View mrunBorder;
	private Drawable mUnselectedBorder;
	private Drawable mSelectedBorder;
	private int mLeftKeyCode;
	private int mRightKeyCode;
	private int mUpKeyCode;
	private int mDownKeyCode;
	private int mFireKeyCode;
	private int mjumpKeyCode;
	private int mTLeftKeyCode;
	private int mTRightKeyCode;
	private int mweaponKeyCode;
	private int mrunKeyCode;
	
	private TextView mLeftText;
	private TextView mRightText;
	private TextView mUpText;
	private TextView mDownText;
	private TextView mFireText;
	private TextView mjumpText;
	private TextView mTLeftText;
	private TextView mTRightText;
	private TextView mweaponText;
	private TextView mrunText;
	private class ConfigClickListener implements View.OnClickListener {
		private int mId;
		public ConfigClickListener(int id) {
			mId = id;
		}
		
		public void onClick(View v) {
			selectId(mId);
		}
		
	}
	
	public KeyboardConfigDialogPreference(Context context, AttributeSet attrs) {
		this(context, attrs, android.R.attr.dialogPreferenceStyle);
	}

	public KeyboardConfigDialogPreference(Context context, AttributeSet attrs,
			int defStyle) {
		super(context, attrs, defStyle);
		
//		TypedArray a = context.obtainStyledAttributes(attrs,
//                R.styleable.KeyConfigPreference, defStyle, 0);
//		mLeftPrefKey = a.getString(R.styleable.KeyConfigPreference_leftKey);
//		mRightPrefKey = a.getString(R.styleable.KeyConfigPreference_rightKey);
//		mUpPrefKey = a.getString(R.styleable.KeyConfigPreference_upKey);
//		mDownPrefKey = a.getString(R.styleable.KeyConfigPreference_downKey);
//		mFirePrefKey = a.getString(R.styleable.KeyConfigPreference_fireKey);
//		mjumpPrefKey = a.getString(R.styleable.KeyConfigPreference_jumpKey);
//		mTLeftPrefKey = a.getString(R.styleable.KeyConfigPreference_tleftKey);
//		mTRightPrefKey = a.getString(R.styleable.KeyConfigPreference_trightKey);
//		mweaponPrefKey= a.getString(R.styleable.KeyConfigPreference_weaponKey);
//		mrunPrefKey= a.getString(R.styleable.KeyConfigPreference_runKey);
//        a.recycle();
	}
	
	public KeyboardConfigDialogPreference(Context context) {
        this(context, null);
    }

	@Override
	protected void onBindDialogView(View view) {
		super.onBindDialogView(view);
		if (mSharedPrefs != null) {
			mLeftKeyCode = mSharedPrefs.getInt(PREFERENCE_LEFT_KEY, KeyEvent.KEYCODE_DPAD_LEFT);
			mRightKeyCode = mSharedPrefs.getInt(PREFERENCE_RIGHT_KEY, KeyEvent.KEYCODE_DPAD_RIGHT);
			mUpKeyCode = mSharedPrefs.getInt(PREFERENCE_UP_KEY, KeyEvent.KEYCODE_DPAD_UP);
			mDownKeyCode = mSharedPrefs.getInt(PREFERENCE_DOWN_KEY, KeyEvent.KEYCODE_DPAD_DOWN);
			mFireKeyCode = mSharedPrefs.getInt(PREFERENCE_FIRE_KEY, KeyEvent.KEYCODE_F);
			mjumpKeyCode = mSharedPrefs.getInt(PREFERENCE_JUMP_KEY, KeyEvent.KEYCODE_SPACE);
			mTLeftKeyCode = mSharedPrefs.getInt(PREFERENCE_TLEFT_KEY, KeyEvent.KEYCODE_Q);
			
			mTRightKeyCode = mSharedPrefs.getInt(PREFERENCE_TRIGHT_KEY, KeyEvent.KEYCODE_E);
			mweaponKeyCode= mSharedPrefs.getInt(PREFERENCE_WEAPON_KEY, KeyEvent.KEYCODE_W);
			mrunKeyCode= mSharedPrefs.getInt(PREFERENCE_RUN_KEY, KeyEvent.KEYCODE_R);
			mLeftText = (TextView)view.findViewById(R.id.key_left);
			mLeftText.setText(getKeyLabel(mLeftKeyCode));
			
			mRightText = (TextView)view.findViewById(R.id.key_right);
			mRightText.setText(getKeyLabel(mRightKeyCode));
			
			mUpText = (TextView)view.findViewById(R.id.key_up);
			mUpText.setText(getKeyLabel(mUpKeyCode));
			
			mDownText = (TextView)view.findViewById(R.id.key_down);
			mDownText.setText(getKeyLabel(mDownKeyCode));
			
			mFireText = (TextView)view.findViewById(R.id.key_fire);
			mFireText.setText(getKeyLabel(mFireKeyCode));
			mjumpText = (TextView)view.findViewById(R.id.key_jump);
			mjumpText.setText(getKeyLabel(mjumpKeyCode));
			mTLeftText = (TextView)view.findViewById(R.id.key_turnleft);
			mTLeftText.setText(getKeyLabel(mTLeftKeyCode));
			mTRightText = (TextView)view.findViewById(R.id.key_turnright);
			mTRightText.setText(getKeyLabel(mTRightKeyCode));
			mweaponText = (TextView)view.findViewById(R.id.key_weapon);
			mweaponText.setText(getKeyLabel(mweaponKeyCode));
			mrunText = (TextView)view.findViewById(R.id.key_run);
			mrunText.setText(getKeyLabel(mrunKeyCode));
			
			mLeftBorder = view.findViewById(R.id.left_border);
			mRightBorder = view.findViewById(R.id.right_border);
			mUpBorder = view.findViewById(R.id.up_border);
			mDownBorder = view.findViewById(R.id.down_border);
			mFireBorder = view.findViewById(R.id.fire_border);
			mjumpBorder = view.findViewById(R.id.jump_border);
			mTLeftBorder = view.findViewById(R.id.turnleft_border);
			mTRightBorder = view.findViewById(R.id.turnright_border);
			mweaponBorder = view.findViewById(R.id.weapon_border);
			mrunBorder= view.findViewById(R.id.run_border);
			
			mLeftBorder.setOnClickListener(new ConfigClickListener(R.id.key_left));
			mRightBorder.setOnClickListener(new ConfigClickListener(R.id.key_right));
			mUpBorder.setOnClickListener(new ConfigClickListener(R.id.key_up));
			mDownBorder.setOnClickListener(new ConfigClickListener(R.id.key_down));
			mFireBorder.setOnClickListener(new ConfigClickListener(R.id.key_fire));
			mjumpBorder.setOnClickListener(new ConfigClickListener(R.id.key_jump));
			mTLeftBorder.setOnClickListener(new ConfigClickListener(R.id.key_turnleft));
			mTRightBorder.setOnClickListener(new ConfigClickListener(R.id.key_turnright));
			mweaponBorder.setOnClickListener(new ConfigClickListener(R.id.key_weapon));
			mrunBorder.setOnClickListener(new ConfigClickListener(R.id.key_run));
			
			mUnselectedBorder = mContext.getResources().getDrawable(R.drawable.key_config_border);
			mSelectedBorder = mContext.getResources().getDrawable(R.drawable.key_config_border_active);
		}
		
		mListeningId = 0;
		
		
	}
	
	@Override
	protected void showDialog(Bundle state) {
		super.showDialog(state);
		getDialog().setOnKeyListener(this);
		getDialog().takeKeyEvents(true);
	}

	protected String getKeyLabel(int keycode) {
		String result = "Unknown Key";
		if (mKeyLabels == null) {
			mKeyLabels = mContext.getResources().getStringArray(R.array.keycode_labels);
		}
		
		if (keycode > 0 && keycode < mKeyLabels.length) {
			result = mKeyLabels[keycode - 1];
		} else {
			result = SDL_Keys.names[keycode];
		}
		return result;
	}
	
	/**
	 * change background of each key setting panel
	 * @param id the key id
	 */
	public void selectId(int id) {
		if (mListeningId != 0) {
			// unselect the current box
			View border = getConfigViewById(mListeningId);
			border.setBackgroundDrawable(mUnselectedBorder);
		}
		
		if (id == mListeningId || id == 0) {
			mListeningId = 0; // toggle off and end.
		} else {
			// select the new box
			View border = getConfigViewById(id);
			border.setBackgroundDrawable(mSelectedBorder);
			mListeningId = id;
		}
	}
	
	/**
	 * get the board layout view  from the textview id  
	 */
	private View getConfigViewById(int id) {
		View config = null;
		switch(id) {
		case R.id.key_left:
			config = mLeftBorder;
			break;
		case R.id.key_right:
			config = mRightBorder;
			break;
		case R.id.key_up:
			config = mUpBorder;
			break;
		case R.id.key_down:
			config = mDownBorder;
			break;
		case R.id.key_fire:
			config = mFireBorder;
			break;
		case R.id.key_jump:
			config = mjumpBorder;
			break;
		case R.id.key_turnleft:
			config = mTLeftBorder;
			break;
		case R.id.key_turnright:
			config = mTRightBorder;
			break;
		case R.id.key_weapon:
			config = mweaponBorder;
			break;
		case R.id.key_run:
			config = mrunBorder;
			break;
		}
		
		return config;
	}

	@Override
	protected void onDialogClosed(boolean positiveResult) {
		super.onDialogClosed(positiveResult);
		
		if (positiveResult) {
			// save changes
			SharedPreferences.Editor editor = mSharedPrefs.edit();
//			editor.putInt(mLeftPrefKey, mLeftKeyCode);
//			editor.putInt(mRightPrefKey, mRightKeyCode);
//			editor.putInt(mUpPrefKey, mUpKeyCode);
//			editor.putInt(mDownPrefKey, mDownKeyCode);
//			editor.putInt(mFirePrefKey, mFireKeyCode);
//			editor.putInt(mjumpPrefKey, mjumpKeyCode);
//			editor.putInt(mTLeftPrefKey, mTLeftKeyCode);
//			editor.putInt(mTRightPrefKey, mTRightKeyCode);
//			editor.putInt(mweaponPrefKey, mweaponKeyCode);
//			editor.putInt(mrunPrefKey, mrunKeyCode);
			editor.putInt(PREFERENCE_LEFT_KEY, mLeftKeyCode);
			editor.putInt(PREFERENCE_RIGHT_KEY, mRightKeyCode);
			editor.putInt(PREFERENCE_UP_KEY, mUpKeyCode);
			editor.putInt(PREFERENCE_DOWN_KEY, mDownKeyCode);
			editor.putInt(PREFERENCE_FIRE_KEY, mFireKeyCode);
			editor.putInt(PREFERENCE_JUMP_KEY, mjumpKeyCode);
			editor.putInt(PREFERENCE_TLEFT_KEY, mTLeftKeyCode);
			editor.putInt(PREFERENCE_TRIGHT_KEY, mTRightKeyCode);
			editor.putInt(PREFERENCE_WEAPON_KEY, mweaponKeyCode);
			editor.putInt(PREFERENCE_RUN_KEY, mrunKeyCode);
			
			editor.commit();
		}
	}

	public void setPrefs(SharedPreferences sharedPreferences) {
		mSharedPrefs = sharedPreferences;
	}
	
	public void setContext(Context context) {
		mContext = context;
	}

	public boolean onKey(DialogInterface dialog, int keyCode, KeyEvent event) {
		boolean eatKey = false;
		if (mListeningId != 0) {
			eatKey = true;
			switch (mListeningId) {
			case R.id.key_left:
				mLeftText.setText(getKeyLabel(keyCode));
				mLeftKeyCode = keyCode;
				break;
			case R.id.key_right:
				mRightText.setText(getKeyLabel(keyCode));
				mRightKeyCode = keyCode;
				break;
			case R.id.key_up:
				mUpText.setText(getKeyLabel(keyCode));
				mUpKeyCode = keyCode;
				break;
			case R.id.key_down:
				mDownText.setText(getKeyLabel(keyCode));
				mDownKeyCode = keyCode;
				break;
			case R.id.key_fire:
				mFireText.setText(getKeyLabel(keyCode));
				mFireKeyCode = keyCode;
				break;
			case R.id.key_jump:
				mjumpText.setText(getKeyLabel(keyCode));
				mjumpKeyCode = keyCode;
				break;
			case R.id.key_turnleft:
				mTLeftText.setText(getKeyLabel(keyCode));
				mTLeftKeyCode = keyCode;
				break;
			case R.id.key_turnright:
				mTRightText.setText(getKeyLabel(keyCode));
				mTRightKeyCode = keyCode;
				break;
			case R.id.key_weapon:
				mweaponText.setText(getKeyLabel(keyCode));
				mweaponKeyCode = keyCode;
				break;
			case R.id.key_run:
				mrunText.setText(getKeyLabel(keyCode));
				mrunKeyCode = keyCode;
				break;
			}
			
			selectId(0);	// deselect the current config box;
		}
		return eatKey;
	}

}
