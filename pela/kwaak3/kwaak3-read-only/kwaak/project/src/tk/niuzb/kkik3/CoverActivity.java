package tk.niuzb.kkik3;

import org.kwaak3.Game;
import org.kwaak3.Globals;

import tk.niuzb.game.SetPreferencesActivity;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.Button;

public class CoverActivity extends Activity {
	private View mTicker;

	private Animation mFadeInAnimation;

	@Override
	public void onCreate(Bundle bundle) {
		super.onCreate(bundle);
		this.requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);

		setContentView(R.layout.cover);
		mFadeInAnimation = AnimationUtils.loadAnimation(this, R.anim.fade_in);

		// mTicker = findViewById(R.id.ticker);
		if (mTicker != null) {
			mTicker.setFocusable(true);
			mTicker.requestFocus();
			mTicker.setSelected(true);
		}

		Button button = (Button) findViewById(R.id.game_quick_start);
		button.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				// Perform action on click
				// Utils.showLauncherDialog(CoverActivity.this, false);
				// Intent i = new Intent(CoverActivity.this, Launcher.class);
				// i.putExtra("wad", wad);
				// CoverActivity.this.startActivity(i);
				Intent intent = new Intent(v.getContext(), Game.class);
				SharedPreferences prefs = CoverActivity.this
						.getApplicationContext().getSharedPreferences(
								SetPreferencesActivity.PREFERENCE_NAME,
								Context.MODE_PRIVATE);
				Globals.PhoneHasTrackball = prefs.getBoolean(
						"toggle_has_trackball", true);

				intent
						.putExtra("sound", prefs.getBoolean("enable_music",
								false));

				// chk = (CheckBox)findViewById(R.id.chkShowFramerate);
				intent.putExtra("showfps", prefs.getBoolean("show_fps", false));

				// chk = (CheckBox)findViewById(R.id.chkEnableLightmaps);
				intent.putExtra("lightmaps", prefs.getBoolean(
						"enable_light_map", false));

				startActivity(intent);
			}
		});
		button = (Button) this.findViewById(R.id.game_options);
		button.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				// Perform action on click
				Intent i = new Intent(CoverActivity.this,
						SetPreferencesActivity.class);
				CoverActivity.this.startActivity(i);
			}
		});

	}

	@Override
	protected void onResume() {
		super.onResume();
		if (mTicker != null) {
			mTicker.clearAnimation();
			mTicker.setAnimation(mFadeInAnimation);
		}
	}
}
