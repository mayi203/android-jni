package com.mayi.jni_l3;

import java.io.IOException;

import android.graphics.PixelFormat;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.hardware.Camera.Parameters;
import android.hardware.Camera.PreviewCallback;
import android.hardware.Camera.Size;
import android.util.Log;

@SuppressWarnings("deprecation")
public class CameraPreview implements PreviewCallback {

	final static String TAG = "CameraPreview";

	private int num = 5;

	private int preWidth;
	private int preHeight;

	private Camera camera;

	public CameraPreview() {
		Log.i(TAG, "CameraPreview");
	}

	@Override
	public void onPreviewFrame(byte[] data, Camera camera) {
		Log.i(TAG, "onPreviewFrame");
		JNILib.newFrameAvailable(preWidth, preHeight, data);
		camera.addCallbackBuffer(data);
	}

	public int open(int direction) {
		Log.i(TAG, "open: " + num);
		num = 8;
		Log.i(TAG, "Direction: " + direction);

		try {
			camera = Camera.open();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return 0;
	}

	public boolean start(SurfaceTexture texture) {
		Log.i(TAG, "start: " + num);
		num = 6;

		try {
			camera.setPreviewTexture(texture);
			Parameters para = camera.getParameters();
			int pixelformat = para.getPreviewFormat();
			PixelFormat pixelinfo = new PixelFormat();
			PixelFormat.getPixelFormatInfo(pixelformat, pixelinfo);
			Size preSize = para.getPreviewSize();
			preWidth = preSize.width;
			preHeight = preSize.height;
			int bufSize = preWidth * preHeight * pixelinfo.bitsPerPixel / 8;

			for (int i = 0; i < 5; i++) {
				camera.addCallbackBuffer(new byte[bufSize]);
			}
			camera.setDisplayOrientation(90);
			camera.setPreviewCallbackWithBuffer(this);
			camera.startPreview();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return true;
	}

	public boolean stop() {
		Log.i(TAG, "stop: " + num);
		num = 7;
		return true;
	}

}
