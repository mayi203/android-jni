package com.example.jni_l1;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

public class MainActivity extends Activity {

	String TAG = "JNI";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		TextView tv = new TextView(this);
		tv.setText(NativeInterface.readFromAssets(getAssets(), "test.txt"));
		setContentView(tv);
		int[][] array = NativeInterface.getTwoArray(10);
		for (int i = 0, j = array.length; i < j; i++) {
			int[] a = array[i];
			for (int m = 0, n = a.length; m < n; m++) {
				Log.i(TAG, "array: " + a[m]);
			} 
		}
	}
}
