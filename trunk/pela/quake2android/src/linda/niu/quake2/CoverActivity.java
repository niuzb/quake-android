package linda.niu.quake2;



import linda.niu.quake2.R;
import linda.niu.quake2.R.id;
import linda.niu.quake2.R.layout;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.Button;

import com.jeyries.quake2.Quake2;


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
//        mFadeInAnimation = AnimationUtils.loadAnimation(this, R.anim.fade_in);

        //mTicker = findViewById(R.id.ticker);
        if (mTicker != null) {
        	mTicker.setFocusable(true);
        	mTicker.requestFocus();
        	mTicker.setSelected(true);
        }
        
		 Button button = (Button) findViewById(R.id.game_quick_start);
         button.setOnClickListener(new View.OnClickListener() {
             public void onClick(View v) {
                 // Perform action on click
            	 Intent i = new Intent(CoverActivity.this, Quake2.class);
            	 CoverActivity.this.startActivity(i);	
             }
         });
//         button = (Button)this.findViewById(R.id.game_options);
//         button.setOnClickListener(new View.OnClickListener() {
//             public void onClick(View v) {
//                 // Perform action on click
//            	 Intent i = new Intent(CoverActivity.this, SetPreferencesActivity.class);
//            	 CoverActivity.this.startActivity(i);	
//             }
//         });

	}

    
    @Override
    protected void onResume() {
        super.onResume();
//        if (mTicker != null) {
//        	mTicker.clearAnimation();
//        	mTicker.setAnimation(mFadeInAnimation);
//        }
    }
}
