package com.mayi.jni_l3;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLSurfaceView;

public class GLRenderer implements GLSurfaceView.Renderer {

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		JNILib.CameraDevice_init(1);
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		JNILib.CameraDevice_start(width, height);
	}

	@Override
	public void onDrawFrame(GL10 gl) {
		JNILib.step();
	}

}
