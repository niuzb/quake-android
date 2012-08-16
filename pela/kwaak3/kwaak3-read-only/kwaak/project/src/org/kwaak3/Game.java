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

import java.io.File;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.Window;
import android.view.WindowManager;
import android.view.ViewGroup.LayoutParams;
import android.widget.TextView;

public class Game extends Activity {
	private KwaakAudio mKwaakAudio;
	private KwaakView mGLSurfaceView;
	public static Activity MAIN;

	private void showError(String s) {
//		AlertDialog.Builder builder = new AlertDialog.Builder(this);
//		builder.setMessage(s);
//		builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
//			public void onClick(DialogInterface dialog, int id) {
//				Game.this.finish();
//			}
//		});
//		AlertDialog dialog = builder.create();
//		dialog.show();
		setUpStatusLabel(s);
	}

	public boolean checkGameFiles(boolean show) {
		File quake3_dir = new File("/sdcard/quake3");
		if (quake3_dir.exists() == false) {
      if(show)
			showError("Unable to locate /sdcard/quake3.");
			return false;
		}

		File baseq3_dir = new File("/sdcard/quake3/baseq3");
		if (baseq3_dir.exists() == false) {
			if(show)
			showError("Unable to locate /sdcard/quake3/baseq3.");
			return false;
		}

		/* Check if pak0.pk3-pak8.pk3 are around */
		for (int i = 0; i < 8; i++) {
			String pak_filename = "pak" + i + ".pk3";
			File quake3_pak_file = new File("/sdcard/quake3/baseq3/"
					+ pak_filename);
			if (quake3_pak_file.exists() == false) {
        if(show)
				showError("Unable to locate /sdcard/quake3/baseq3/"
						+ pak_filename+"\n,Please get data files pak0.pk3 to pak8.pk3 and move to this directory");
				return false;
			}
		}
		return true;
	}

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		/* We like to be fullscreen */
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		super.onCreate(savedInstanceState);
		MAIN = this;
		ExtractData();
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
	}

	@Override
	protected void onPause() {
		// Log.d("Quake_JAVA", "onPause");
		super.onPause();

		if (mKwaakAudio != null)
			mKwaakAudio.pause();
	}

	@Override
	protected void onResume() {
		/*
		 * Resume doesn't always seem to work well. On my Milestone it works but
		 * not on the G1. The native code seems to be running but perhaps we
		 * need to issue a 'vid_restart'.
		 */
		// Log.d("Quake_JAVA", "onResume");
		super.onResume();
		if (mGLSurfaceView != null) {
			mGLSurfaceView.onResume();
		}

		if (mKwaakAudio != null)
			mKwaakAudio.resume();
	}
    public void startGame() {
    	/* Don't initialize the game engine when the game files aren't around */
		if (checkGameFiles(true)) {
			mGLSurfaceView = new KwaakView(this);
			setContentView(mGLSurfaceView,new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.
		            FILL_PARENT));
			Log.v("quake", "set content view");
			mGLSurfaceView.requestFocus();
			mGLSurfaceView.setId(1);

			/*
			 * The KwaakAudio object is owned by the Game class but is only used
			 * by the game library using JNI calls. It is not that pretty but it
			 * is the only way without using the (unstable) AudioTrack API from
			 * C++.
			 */
			mKwaakAudio = new KwaakAudio();
			KwaakJNI.setAudio(mKwaakAudio);

			Bundle extras = getIntent().getExtras();
			if (extras != null) {
				/*
				 * We use a separate call for disabling audio because a user can
				 * reactivate audio from the q3 console by issuing 's_initsound
				 * 1' + 'snd_restart'
				 */
				KwaakJNI.enableAudio(extras.getBoolean("sound"));

				/*
				 * Enable lightmaps for rendering (default=disabled). Enabling
				 * causes a performance hit of typically 25% (on Milestone +
				 * Nexus One).
				 */
				KwaakJNI.enableLightmaps(extras.getBoolean("lightmaps"));

				/* Run a timedemo */
				KwaakJNI.enableBenchmark(extras.getBoolean("benchmark"));

				/* Show the framerate */
				KwaakJNI.showFramerate(extras.getBoolean("showfps"));
			}
		} else {
			// setContentView(R.layout.main);
		//	finish();
			Log.v("quake", "no data find");
		}
    }
	void ExtractData() {

		class Callback implements Runnable {
			Game p;

			Callback(Game _p) {
				p = _p;
			}

			public void run() {
			try {
					Thread.sleep(300);
				} catch (InterruptedException e) {
				}
				;
				p.startDownloader();
			}
		}
		;
		Thread changeConfigAlertThread = null;
		changeConfigAlertThread = new Thread(new Callback(this));
		changeConfigAlertThread.start();

	}

	public void setUpStatusLabel(String s) {
		Game Parent = this; // Too lazy to rename

		if (Parent._tv == null) {
			Parent._tv = new TextView(Parent);
		//	Parent._tv.setMaxLines(1);
			Parent._tv.setTextSize (20);
			// Parent._layout2.addView(Parent._tv);
		}
		Parent._tv.setText(s);
		Parent.setContentView(Parent._tv);
	}

	/* download the data to sdcard */
	public void startDownloader() {
		class Callback implements Runnable {
			public Game Parent;

			public void run() {
				setUpStatusLabel("init...");
				if (Parent.downloader == null)
					Parent.downloader = new DataDownloader(Parent, Parent._tv);
			}
		}
		Callback cb = new Callback();
		cb.Parent = this;
		this.runOnUiThread(cb);
	}

	private static DataDownloader downloader = null;
	private TextView _tv = null;
}
