package tk.niuzb.quake3;



import android.content.Context;
import android.view.MotionEvent;
import android.view.KeyEvent;
import android.view.Window;
import android.view.WindowManager;
import android.os.Environment;
import java.io.File;

import android.widget.TextView;
import java.lang.Thread;
import android.os.Build;
import android.util.Log;

abstract class DifferentTouchInput
	{
	    
		public static DifferentTouchInput getInstance()
		{
			if ((Build.VERSION.SDK_INT) <= 4) {
				return SingleTouchInput.Holder.sInstance;}
			else {
                Log.v("quake", "multi touch");
				return MultiTouchInput.Holder.sInstance;}
		}
		public abstract void process(final MotionEvent event);
		private static class SingleTouchInput extends DifferentTouchInput
		{
		    private long mLastTouchTime = 0L;
			private static class Holder 
			{
				private static final SingleTouchInput sInstance = new SingleTouchInput();
			}
			public void process(final MotionEvent event)
			{
			     final long time = System.currentTimeMillis();
	        if (event.getAction() == MotionEvent.ACTION_MOVE && time - 
mLastTouchTime < 32) {
		      return;
               
	        }
	        mLastTouchTime = time;

            
				int action = -1;
				if( event.getAction() == MotionEvent.ACTION_DOWN )
					action = 0;
				if( event.getAction() == MotionEvent.ACTION_UP )
					action = 1;
				if( event.getAction() == MotionEvent.ACTION_MOVE )
					action = 2;
				if ( action >= 0 )
					QuakeLib.nativeMouse( (int)event.getX(), (int)event.getY(), 
action, 0, 
													(int)(event.getPressure() * 1000.0),
													(int)(event.getSize() * 1000.0) );
			}
		}
		private static class MultiTouchInput extends DifferentTouchInput
		{
		
            private long mLastTouchTime = 0L;
			private static class Holder 
			{
				private static final MultiTouchInput sInstance = new MultiTouchInput();
			}
			public void process(final MotionEvent event)
			{

                 final long time = System.currentTimeMillis();
                 if (event.getAction() == MotionEvent.ACTION_MOVE && time - 
                mLastTouchTime < 50) {
                    // Sleep so that the main thread doesn't get flooded with UI events.
                   return;
                }
                mLastTouchTime = time;     
				int act = event.getAction();
					 for( int i = 0; i < event.getPointerCount(); i++ )
					 {
						 int action = -1;
				 int pointerIndex = -1;
				 boolean point = false;
				 switch (event.getAction() & MotionEvent.ACTION_MASK) {
				 case MotionEvent.ACTION_DOWN:
					 action = 0;
					 break;
				 case MotionEvent.ACTION_POINTER_DOWN:
					 point = true;
					 pointerIndex = (act & MotionEvent.ACTION_POINTER_INDEX_MASK) 
							 >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;
					 action = 0;
					 break;
				 case MotionEvent.ACTION_UP:
					 action = 1;
					 break;
				 case MotionEvent.ACTION_POINTER_UP:
					 point = true;
					 pointerIndex = (act & MotionEvent.ACTION_POINTER_INDEX_MASK) 
							 >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;
					 action = 1;
					 break;
				 case MotionEvent.ACTION_MOVE:
					 action = 2;
					 break;  
				 default:
						 break;
				 }
				 int pid = event.getPointerId(i);
				  int pointerId = -1 ;
				 if(point)
					 pointerId = event.getPointerId(pointerIndex);
						 try {
							 if (((point && pid == pointerId)||!point) && 
								 action >= 0 ) {
								
								QuakeLib.nativeMouse((int) event.getX(pid),
							(int) event.getY(pid), action,
							event.getPointerId(i),
							(int) (1),
							(int) (1));
							 }
						 } catch (Exception e) {
							 
						 }
					 
				 }

				
			}
		}
	}


