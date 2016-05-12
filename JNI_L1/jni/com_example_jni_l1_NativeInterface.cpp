#include "com_example_jni_l1_NativeInterface.h"
#include <stdlib.h>
#include <android/log.h>
#include <android/asset_manager_jni.h>
#include <android/asset_manager.h>

#define TAG "JNI"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE,TAG,__VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)

extern "C" {
JNIEXPORT jstring JNICALL Java_com_example_jni_1l1_NativeInterface_helloJNI(
		JNIEnv* env, jclass) {
	return env->NewStringUTF("Hello JNI!");
}

JNIEXPORT jstring JNICALL Java_com_example_jni_1l1_NativeInterface_readFromAssets(
		JNIEnv* env, jclass, jobject assetManager, jstring filename) {
	jstring resultStr;
	AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);
	if (mgr == NULL) {
		LOGI("%s", "AssetManager=NULL");
		return 0;
	}
	jboolean iscopy;
	const char *mfile = env->GetStringUTFChars(filename, &iscopy);
	AAsset* asset = AAssetManager_open(mgr, mfile, AASSET_MODE_UNKNOWN);
	env->ReleaseStringUTFChars(filename, mfile);
	if (asset == NULL) {
		LOGI("%s", "asset=NULL");
		return 0;
	}
	off_t bufferSize = AAsset_getLength(asset);
	char *buffer = (char *) malloc(bufferSize + 1);
	buffer[bufferSize] = 0;
	int numByteRead = AAsset_read(asset, buffer, bufferSize);
	resultStr = env->NewStringUTF(buffer);
	LOGI("%s", buffer);
	free(buffer);
	AAsset_close(asset);
	return resultStr;
}
JNIEXPORT jobjectArray JNICALL Java_com_example_jni_1l1_NativeInterface_getTwoArray(
		JNIEnv *env, jclass, jint dimon) {
	jclass intArrayClass = env->FindClass("[I");
	jobjectArray objectIntArray = env->NewObjectArray(dimon, intArrayClass,
			NULL);
	for (int i = 0; i < dimon; i++) {
		jintArray intArray = env->NewIntArray(dimon);
		jint temp[10];
		for (int j = 0; j < dimon; j++) {
			temp[j] = i + j;
		}
		env->SetIntArrayRegion(intArray, 0, dimon, temp);
		env->SetObjectArrayElement(objectIntArray, i, intArray);
		env->DeleteLocalRef(intArray);
	}
	return objectIntArray;
}
}

