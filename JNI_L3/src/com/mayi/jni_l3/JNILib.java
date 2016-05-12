package com.mayi.jni_l3;

import android.content.res.AssetManager;

public class JNILib {
	static {
		System.loadLibrary("JNI_L3");
	}

	public static native void init(AssetManager asset);

	public static native void readAsset();

	public static native boolean CameraDevice_init(int cameraDierction);

	public static native boolean CameraDevice_start(int width, int height);

	public static native boolean CameraDevice_stop();

	public static native void newFrameAvailable(int width, int height, byte[] cameraData);

	public static native void step();
}
