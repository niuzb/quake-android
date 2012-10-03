// DO NOT EDIT THIS FILE - it is automatically generated, edit file under project/java dir
// This string is autogenerated by ChangeAppSettings.sh, do not change spaces amount
package tk.niuzb.game;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.KeyEvent;
import android.view.Window;
import android.view.WindowManager;
import android.os.Vibrator;
import android.hardware.SensorManager;
import android.hardware.SensorEventListener;
import android.hardware.Sensor;
import android.hardware.SensorEvent;

import android.widget.TextView;


class AccelerometerReader implements SensorEventListener {

	private SensorManager _manager = null;

	public AccelerometerReader(Activity context) {
		System.out.println("libSDL: accelerometer start required: " + String.valueOf(Globals.UseAccelerometerAsArrowKeys));
		if( Globals.UseAccelerometerAsArrowKeys )
		{
			_manager = (SensorManager) context.getSystemService(Context.SENSOR_SERVICE);
			if( _manager != null )
			{
				System.out.println("libSDL: starting accelerometer");
				// TODO: orientation allows for 3rd axis - azimuth, but it will be way too hard to the user
				// if( ! _manager.registerListener(this, _manager.getDefaultSensor(Sensor.TYPE_ORIENTATION), SensorManager.SENSOR_DELAY_GAME) )
				_manager.registerListener(this, _manager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER), SensorManager.SENSOR_DELAY_GAME);
			}
		}
	}
	
	public synchronized void stop() {
		if( _manager != null )
		{
			_manager.unregisterListener(this);
		}
	}

	public synchronized void onSensorChanged(SensorEvent event) {

		if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER) 
		{
			if( Globals.HorizontalOrientation )
				nativeAccelerometer(event.values[1], -event.values[0], event.values[2]);
			else
				nativeAccelerometer(event.values[0], event.values[1], event.values[2]); // TODO: not tested!
		}
		else
		{
			if( Globals.HorizontalOrientation )
				nativeOrientation(event.values[1], -event.values[2], event.values[0]);
			else
				nativeOrientation(event.values[2], event.values[1], event.values[0]);
		}
		
	}

	public synchronized void onAccuracyChanged(Sensor s, int a) {
	}
	

	private native void nativeAccelerometer(float accX, float accY, float accZ);
	private native void nativeOrientation(float accX, float accY, float accZ);
}


