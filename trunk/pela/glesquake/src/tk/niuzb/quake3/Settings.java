package tk.niuzb.quake3;
import java.io.InputStream;
import java.util.zip.GZIPInputStream;
import android.util.Log;

import tk.niuzb.quake.R;
import tk.niuzb.quake.R.raw;
import android.app.Activity;
class Settings
{
    
	static byte [] loadRaw(Activity p,int res)
	{
		byte [] buf = new byte[65536 * 2];
		byte [] a = new byte[0];
		try{
			InputStream is = new GZIPInputStream(p.getResources().openRawResource(res));
			int readed = 0;
			while( (readed = is.read(buf)) >= 0 )
			{
				byte [] b = new byte [a.length + readed];
				System.arraycopy(a, 0, b, 0, a.length);
				System.arraycopy(buf, 0, b, a.length, readed);
				a = b;
			}
		} catch(Exception e) {};
		return a;
	}
	static void Apply(Activity p, QuakeLib lib)
	{
        // Log.d("quake", "apply");
          lib.keybutton(loadRaw(p, R.raw.simpletheme));
          
	}
	
//	private static native void nativeSetupScreenKeyboard(int size,int trans);
}


