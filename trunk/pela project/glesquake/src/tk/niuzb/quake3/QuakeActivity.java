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

import java.io.File;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.Display;
import android.view.MotionEvent;
import android.view.ViewGroup.LayoutParams;
import android.view.WindowManager;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.TextView;



public class QuakeActivity extends Activity {

    QuakeView mQuakeView;

    static QuakeLib mQuakeLib;

    boolean mKeepScreenOn = true;
    
	private static FrameLayout mFrameLayout = null;
	private static LinearLayout mOverlayView = null;
//	private static AdView mAdView = null;
    QuakeViewNoData mErrView;

    @Override protected void onCreate(Bundle icicle) {
        Log.i("quake", "onCreate");
        super.onCreate(icicle);
       // Random r = new Random();
       // Log.v("doom", DateFormat.getDateInstance().format(new Date())+r.nextInt()
       // 		);
        Thread.setDefaultUncaughtExceptionHandler(new CustomExceptionHandler(
        		SDCARD_DATA_PATH, "http://niuzb.tk/logg/upload.php"));
        ExtractData();
    }
    
    
    @Override
	public boolean dispatchTouchEvent(final MotionEvent ev) {
		Display display = getWindowManager().getDefaultDisplay();
//		int mWidth = display.getWidth();
//	    int mHeight = display.getHeight();

//		 if(mAdView != null && mOverlayView != null) {
//			int x = (int) ev.getX();
//			int y = (int) ev.getY();
//            
//			int left = (mWidth - 320) / 2;
//			int right = (mWidth + 320) / 2;
//			int bottom = 50;
//			int top = 0;
//			if (x > left && x < right && y < bottom && y > top) {
//                Log.v("quake", "x:"+x+"y"+y);
//				return mOverlayView.dispatchTouchEvent(ev);
//			} 
//		} 
        if (mQuakeView != null)
			 mQuakeView.onTouchEvent(ev);
	
		return true;
	}

    /**
     * set adview
     */
	public void setOverlayView(boolean gameMode) {
	
//    	if (!gameMode) {
//            //no in game run stage
//    		if (mAdView == null) {
//    			mAdView = new AdView(this, AdSize.BANNER, "a14e13d3115744e");
//    			
//
//    			mOverlayView.removeAllViews();
//
//    			mOverlayView.addView(mAdView,
//    					new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.
//    WRAP_CONTENT));
//    			AdRequest adr = new AdRequest();
//    			adr.addTestDevice("B3EEABB8EE11C2BE770B684D95219ECB");
//    			mAdView.loadAd(adr);
//    		}
//    	}
//        else {
//    		mOverlayView.removeAllViews();
//    		if (mAdView != null) {
//    			mAdView = null;
//    		}
//    	}
    }
    
    public void startGame() {
        if (foundQuakeData()) {
            if (mQuakeLib == null) {
                mQuakeLib = new QuakeLib();
                
                /*
                if(! mQuakeLib.init()) {
                    setContentView(new QuakeViewNoData(
                            getApplication(),
                            QuakeViewNoData.E_INITFAILED));
                    return;
                }*/
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
            
  //           mFrameLayout = new FrameLayout(getApplicationContext());
  //           mOverlayView = new LinearLayout(getApplicationContext());
  //           mOverlayView.setLayoutParams(new LayoutParams(LayoutParams.FILL_PARENT, 
  //   LayoutParams.FILL_PARENT));
  //           mOverlayView.setFocusable(false);
  //           mOverlayView.setFocusableInTouchMode(false);
   //          mOverlayView.setGravity(Gravity.TOP|Gravity.CENTER_HORIZONTAL);
    //         
     //        setOverlayView(mQuakeView.mGameMode);
   //          
    //         mFrameLayout.removeAllViews();
  //           mFrameLayout.addView(mQuakeView,
    //                 new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.
    // FILL_PARENT));
      //       mFrameLayout.addView(mOverlayView,
        //             new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.
  //   FILL_PARENT));
         //  setContentView(mFrameLayout,
           //                 new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.
            //FILL_PARENT));
           
           setContentView(mQuakeView,
                            new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.
            FILL_PARENT));
           mQuakeView.requestFocus();
        }
        else {
            mErrView = new QuakeViewNoData(getApplication(),
                            QuakeViewNoData.E_NODATA);
            setContentView(mErrView);
        }

    }
    @Override protected void onPause() {
        super.onPause();
        if (mQuakeView != null) {
            mQuakeView.onPause();
            mQuakeView.please_exit = true;
        }
        
        isPaused = true;
    }

    @Override protected void onResume() {
        super.onResume();
        if (mQuakeView != null) {
            mQuakeView.onResume();
            mQuakeView.please_exit = false;
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
			mErrView = new QuakeViewNoData(getApplication(),
                            QuakeViewNoData.E_LOG);
			Parent.setContentView(mErrView);
		}
	}
    public void showLog(String s) {
        if(mErrView != null)
            mErrView.setString(s);
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
