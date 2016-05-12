package com.example.jni_l1;

import android.content.res.AssetManager;

public class NativeInterface {
	static{
		System.loadLibrary("JNILIB");
	}
	
	public static native String helloJNI();

	public static native String readFromAssets(AssetManager ass, String filename);
	
	public static native int[][] getTwoArray(int dimion);
}
