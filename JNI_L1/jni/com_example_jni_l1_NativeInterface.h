/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_example_jni_l1_NativeInterface */

#ifndef _Included_com_example_jni_l1_NativeInterface
#define _Included_com_example_jni_l1_NativeInterface
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_example_jni_l1_NativeInterface
 * Method:    helloJNI
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_example_jni_1l1_NativeInterface_helloJNI
  (JNIEnv *, jclass);

/*
 * Class:     com_example_jni_l1_NativeInterface
 * Method:    readFromAssets
 * Signature: (Landroid/content/res/AssetManager;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_example_jni_1l1_NativeInterface_readFromAssets
  (JNIEnv *, jclass, jobject, jstring);

/*
 * Class:     com_example_jni_l1_NativeInterface
 * Method:    getTwoArray
 * Signature: (I)[[I
 */
JNIEXPORT jobjectArray JNICALL Java_com_example_jni_1l1_NativeInterface_getTwoArray
  (JNIEnv *, jclass, jint);

#ifdef __cplusplus
}
#endif
#endif
