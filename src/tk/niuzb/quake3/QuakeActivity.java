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

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.WindowManager;

import java.io.File;
import tk.niuzb.quake3.*;
import android.widget.TextView;
public class QuakeActivity extends Activity {

    QuakeView mQuakeView;

    static QuakeLib mQuakeLib;

    boolean mKeepScreenOn = true;
    AudioThread mAudioThread;

    @Override protected void onCreate(Bundle icicle) {
        Log.i("quake", "onCreate");
        super.onCreate(icicle);
        ExtractData();


    }
    public void startGame() {
        if (foundQuakeData()) {
            
            System.loadLibrary("sdl-1.2");
            mAudioThread = new AudioThread(this);

            if (mQuakeLib == null) {
                mQuakeLib = new QuakeLib();
                
                
                if(! mQuakeLib.init()) {
                    setContentView(new QuakeViewNoData(
                            getApplication(),
                            QuakeViewNoData.E_INITFAILED));
                    return;
                }
            }
            if (mKeepScreenOn) {
                getWindow().setFlags(
                        WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON,
                        WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
            }

            if (mQuakeView == null) {
                mQuakeView = new
                QuakeView(getApplication(), this);
                //start game niuzb
                mQuakeView.setQuakeLib(mQuakeLib);
            }
            setContentView(mQuakeView);
        }
        else {
            setContentView(new QuakeViewNoData(getApplication(),
                            QuakeViewNoData.E_NODATA));
        }

    }
    @Override protected void onPause() {
        super.onPause();
        if (mQuakeView != null) {
            mQuakeView.onPause();
        }
        isPaused = true;
    }

    @Override protected void onResume() {
        super.onResume();
        if (mQuakeView != null) {
            mQuakeView.onResume();
        }
        isPaused = false;
    }
    public boolean isPaused()
    {
            return isPaused;
    }

    @Override protected void onDestroy() {
        super.onDestroy();
        if (mQuakeLib != null) {
            mQuakeLib.quit();
        }
    }

    private boolean foundQuakeData() {
        return fileExists(SDCARD_DATA_PATH + PAK0_PATH)
		|| fileExists(INTERNAL_DATA_PATH + PAK0_PATH);
    }

    private boolean fileExists(String s) {
        File f = new File(s);
        return f.exists();
    }
    void ExtractData() {
        class Callback implements Runnable
        {
            QuakeActivity p;
            Callback( QuakeActivity _p ) { p = _p; }
            public void run()
            {
                try {
                    Thread.sleep(300);
                } catch( InterruptedException e ) {};
                p.startDownloader();
            }
        };
        Thread changeConfigAlertThread = null;
        changeConfigAlertThread = new Thread(new Callback(this));
        changeConfigAlertThread.start();

    }
	public void setUpStatusLabel()
	{
		QuakeActivity Parent = this; // Too lazy to rename
	
		if( Parent._tv == null )
		{
			Parent._tv = new TextView(Parent);
			Parent._tv.setMaxLines(1);
			Parent._tv.setText("init");
			//Parent._layout2.addView(Parent._tv);
			Parent.setContentView(Parent._tv);
		}
	}

    /*download the data to sdcard*/
    public void startDownloader()
	{
		class Callback implements Runnable
		{
			public QuakeActivity Parent;
			public void run()
			{
				setUpStatusLabel();
				if( Parent.downloader == null )
					Parent.downloader = new DataDownloader(Parent, Parent._tv);
			}
		}
		Callback cb = new Callback();
		cb.Parent = this;
		this.runOnUiThread(cb);
	}
    private static DataDownloader downloader = null;
	private TextView _tv = null;
    private boolean isPaused;
    private final static String SDCARD_DATA_PATH = "/sdcard/quake";
    private final static String INTERNAL_DATA_PATH = "/data/quake";
    private final static String PAK0_PATH = "/id1/pak0.pak";
}
