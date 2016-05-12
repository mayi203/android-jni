package com.mayi.jni_l2;

public class GL2JNILib {

	static{
		System.loadLibrary("GL2JNILib");
	}
	public static native void init(int width,int height);
	public static native void step();
}
