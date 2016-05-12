package com.mayi.jni_l2;

import android.app.Activity;
import android.os.Bundle;

public class MainActivity extends Activity {

	GL2JNIView mView;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		mView = new GL2JNIView(this);
		setContentView(mView);
	}
}
