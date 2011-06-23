package tk.niuzb.quake3;
import java.io.ByteArrayOutputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectOutputStream;
import java.util.Locale;
import java.util.zip.GZIPInputStream;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.os.Environment;
import android.os.StatFs;
import android.view.KeyEvent;
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
	static void Apply(Activity p)
	{

        nativeSetupScreenKeyboardButtons(loadRaw(p, R.raw.simpletheme));
	}
	
//	private static native void nativeSetupScreenKeyboard(int size,int trans);
	private static native void nativeSetupScreenKeyboardButtons(byte[] img);
}


