package com.mayi.jni_l3;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;

public class MainActivity extends Activity {

	GLSurfaceView surfaceView;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		// JNILib.init(getAssets());
		// JNILib.readAsset();
		surfaceView = new GLSurfaceView(this);
		surfaceView.setEGLContextClientVersion(2);
		surfaceView.setRenderer(new GLRenderer());
		surfaceView.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
		setContentView(surfaceView);
	}

	@Override
	protected void onResume() {
		super.onResume();
		surfaceView.onResume();
	}

	@Override
	protected void onPause() {
		super.onPause();
		JNILib.CameraDevice_stop();
		surfaceView.onPause();
	}

}
