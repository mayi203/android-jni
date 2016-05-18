#include "com_mayi_jni_l3_JNILib.h"
#include <android/log.h>
#include <stdlib.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <android/asset_manager_jni.h>
#include <android/asset_manager.h>

static AAssetManager* mgr = NULL;
static jobject cameraObj = NULL;
static jobject surface = NULL;
#define  LOG_TAG    "JNI_L3"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

GLuint textureId = -1;
GLuint mProgram;
GLuint mPositionHandle;
GLuint mTextureCoordHandle;

const GLfloat squareCoords[] = { -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f,
		1.0f };

const GLfloat textureVertices[] = { 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.0f };

const GLshort drawOrder[] = { 0, 1, 2, 0, 2, 3 };

auto vertexShader = "attribute vec4 vPosition;\n"
		"attribute vec2 inputTextureCoordinate;\n"
		"varying vec2 textureCoordinate;\n"
		"void main(){\n"
		"gl_Position = vPosition;\n"
		"textureCoordinate = inputTextureCoordinate;\n"
		"}";
auto fragShader = "#extension GL_OES_EGL_image_external : require\n"
		"precision mediump float;\n"
		"varying vec2 textureCoordinate;\n"
		"uniform samplerExternalOES s_texture;\n"
		"void main() {\n"
		"  gl_FragColor = texture2D( s_texture, textureCoordinate );\n"
		"}";

static void checkGlError(const char* op) {
	for (GLint error = glGetError(); error; error = glGetError()) {
		LOGI("after %s() glError (0x%x)\n", op, error);
	}
}

GLuint createTextureID() {
	GLuint textureId;

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_EXTERNAL_OES, textureId);
	glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S,
			GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T,
			GL_CLAMP_TO_EDGE);
	return textureId;
}

GLuint loadShader(GLenum shaderType, const char* pSource) {
	GLuint shader = glCreateShader(shaderType);
	if (shader) {
		glShaderSource(shader, 1, &pSource, NULL);
		glCompileShader(shader);
		GLint compiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			GLint infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen) {
				char* buf = (char*) malloc(infoLen);
				if (buf) {
					glGetShaderInfoLog(shader, infoLen, NULL, buf);
					LOGE("Could not compile shader %d:\n%s\n", shaderType, buf);
					free(buf);
				}
				glDeleteShader(shader);
				shader = 0;
			}
		}
	}
	return shader;
}

GLuint createProgram(const char* pVertexSource, const char* pFragmentSource) {
	GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
	if (!vertexShader) {
		return 0;
	}

	GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
	if (!pixelShader) {
		return 0;
	}

	GLuint program = glCreateProgram();
	if (program) {
		glAttachShader(program, vertexShader);
		checkGlError("glAttachShader");
		glAttachShader(program, pixelShader);
		checkGlError("glAttachShader");
		glLinkProgram(program);
		GLint linkStatus = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
		if (linkStatus != GL_TRUE) {
			GLint bufLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
			if (bufLength) {
				char* buf = (char*) malloc(bufLength);
				if (buf) {
					glGetProgramInfoLog(program, bufLength, NULL, buf);
					LOGE("Could not link program:\n%s\n", buf);
					free(buf);
				}
			}
			glDeleteProgram(program);
			program = 0;
		}
	}
	return program;
}
bool setupGraphics(int w, int h) {
	mProgram = createProgram(vertexShader, fragShader);
	mPositionHandle = glGetAttribLocation(mProgram, "vPosition");
	checkGlError("glGetAttribLocation");
	mTextureCoordHandle = glGetAttribLocation(mProgram,
			"inputTextureCoordinate");
	glViewport(0, 0, w, h);
	checkGlError("glViewport");
	return true;
}

void renderFrame() {
	glUseProgram(mProgram);
	//checkGlError("glUseProgram");
	glActiveTexture(GL_TEXTURE0);
	//checkGlError("glActiveTexture");
	glBindTexture(GL_TEXTURE_EXTERNAL_OES, textureId);
	//checkGlError("glBindTexture");
	// Enable a handle to the triangle vertices
	glEnableVertexAttribArray(mPositionHandle);
	//checkGlError("glEnableVertexAttribArray");
	glVertexAttribPointer(mPositionHandle, 2, GL_FLOAT, GL_FALSE, 0,
			squareCoords);
	checkGlError("glVertexAttribPointer1");
	glEnableVertexAttribArray(mTextureCoordHandle);
	//checkGlError("glEnableVertexAttribArray");
	glVertexAttribPointer(mTextureCoordHandle, 2, GL_FLOAT, GL_FALSE, 0,
			textureVertices);
	//checkGlError("glVertexAttribPointer2");
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, drawOrder);
	//checkGlError("glDrawElements");
	glDisableVertexAttribArray(mPositionHandle);
	glDisableVertexAttribArray(mTextureCoordHandle);
}

JNIEXPORT void JNICALL Java_com_mayi_jni_1l3_JNILib_init(JNIEnv * env, jclass,
		jobject assetManager) {
	mgr = AAssetManager_fromJava(env, assetManager);
	if (mgr == NULL) {
		LOGI("%s", "AssetManager=NULL");
	}
}
JNIEXPORT void JNICALL Java_com_mayi_jni_1l3_JNILib_readAsset(JNIEnv * env,
		jclass) {
	jboolean iscopy;
	jstring filename = env->NewStringUTF("Shader/test.txt");
	const char *mfile = env->GetStringUTFChars(filename, &iscopy);
	if (mgr == NULL) {
		LOGI("%s", "AssetManager=NULL");
		return;
	}
	AAsset* asset = AAssetManager_open(mgr, mfile, AASSET_MODE_UNKNOWN);
	env->ReleaseStringUTFChars(filename, mfile);
	if (asset == NULL) {
		LOGI("%s", "Asset=NULL");
		return;
	}
	off_t bufferSize = AAsset_getLength(asset);
	char *buffer = (char *) malloc(bufferSize + 1);
	buffer[bufferSize] = 0;
	int numByteRead = AAsset_read(asset, buffer, bufferSize);
	LOGI("%s", buffer);
	free(buffer);
	AAsset_close(asset);
}
JNIEXPORT jboolean JNICALL Java_com_mayi_jni_1l3_JNILib_CameraDevice_1init(
		JNIEnv * env, jclass, jint direction) {
	jclass class_CameraPre = env->FindClass("com/mayi/jni_l3/CameraPreview");
	if (class_CameraPre == NULL) {
		LOGI("%s", "class_CameraPre=NULL");
	}
	LOGI("find class");
	jmethodID camera_open = env->GetMethodID(class_CameraPre, "open", "(I)I");
	if (camera_open == NULL) {
		LOGI("%s", "camera_open=NULL");
	}
	LOGI("find method");
	if (cameraObj == NULL) {
		jobject obj = env->AllocObject(class_CameraPre);
		cameraObj = env->NewGlobalRef(obj);
		LOGI("AllocObject");
	}
	jint result = env->CallIntMethod(cameraObj, camera_open, direction);
	LOGI("call method open");
	if (result == 0) {
		return true;
	} else {
		return false;
	}
}
JNIEXPORT jboolean JNICALL Java_com_mayi_jni_1l3_JNILib_CameraDevice_1start(
		JNIEnv * env, jclass, jint width, jint height) {
	jclass class_CameraPre = env->FindClass("com/mayi/jni_l3/CameraPreview");
	if (class_CameraPre == NULL) {
		LOGI("%s", "class_CameraPre=NULL");
	}
	LOGI("find class");
	if (textureId == -1) {
		textureId = createTextureID();
	}
	LOGI("textureId = %d", textureId);
	jclass class_surface = env->FindClass("android/graphics/SurfaceTexture");
	if (class_surface == NULL) {
		LOGI("%s", "class_surface=NULL");
	}
	LOGI("find class surface");
	jmethodID sufaceTexture = env->GetMethodID(class_surface, "<init>", "(I)V");

	if (surface == NULL) {
		jobject surfaceObj = env->NewObject(class_surface, sufaceTexture,
				textureId);
		surface = env->NewGlobalRef(surfaceObj);
	}

	jmethodID camera_start = env->GetMethodID(class_CameraPre, "start",
			"(Landroid/graphics/SurfaceTexture;)Z");
	if (camera_start == NULL) {
		LOGI("%s", "camera_start=NULL");
	}
	LOGI("find method");
	if (cameraObj == NULL) {
		jobject obj = env->AllocObject(class_CameraPre);
		cameraObj = env->NewGlobalRef(obj);
		LOGI("AllocObject");
	}
	jboolean result = env->CallBooleanMethod(cameraObj, camera_start, surface);

	setupGraphics(width, height);
	LOGI("call method start");
	return result;
}
JNIEXPORT jboolean JNICALL Java_com_mayi_jni_1l3_JNILib_CameraDevice_1stop(
		JNIEnv * env, jclass) {
	jclass class_CameraPre = env->FindClass("com/mayi/jni_l3/CameraPreview");
	if (class_CameraPre == NULL) {
		LOGI("%s", "class_CameraPre=NULL");
	}
	LOGI("find class");
	jmethodID camera_stop = env->GetMethodID(class_CameraPre, "stop", "()Z");
	if (camera_stop == NULL) {
		LOGI("%s", "camera_stop=NULL");
	}
	LOGI("find method");
	if (cameraObj == NULL) {
		jobject obj = env->AllocObject(class_CameraPre);
		cameraObj = env->NewGlobalRef(obj);
		LOGI("AllocObject");
	}
	jboolean result = env->CallBooleanMethod(cameraObj, camera_stop);
	if (surface != NULL) {
		env->DeleteGlobalRef(surface);
		surface = NULL;
	}
	if (cameraObj != NULL) {
		env->DeleteGlobalRef(cameraObj);
		cameraObj = NULL;
	}
	if (mgr != NULL) {
		mgr = NULL;
	}
	LOGI("call method");
	return result;
}
JNIEXPORT void JNICALL Java_com_mayi_jni_1l3_JNILib_newFrameAvailable(
		JNIEnv * env, jclass, jint width, jint height, jbyteArray data) {
}
JNIEXPORT void JNICALL Java_com_mayi_jni_1l3_JNILib_step(JNIEnv * env, jclass) {
	jclass class_surface = env->FindClass("android/graphics/SurfaceTexture");
	if (class_surface == NULL) {
		LOGI("%s", "class_surface=NULL");
	}
	jmethodID updateTexImage = env->GetMethodID(class_surface, "updateTexImage",
			"()V");
	if (updateTexImage == NULL) {
		LOGI("%s", "updateTexImage==NULL");
	}
	if (surface != NULL)
		env->CallVoidMethod(surface, updateTexImage);
	jmethodID getTransformMatrix = env->GetMethodID(class_surface,
			"getTransformMatrix", "([F)V");
	if (getTransformMatrix == NULL) {
		LOGI("%s", "getTransformMatrix==NULL");
	}
	jfloatArray array = env->NewFloatArray(16);
	if (surface != NULL)
		env->CallVoidMethod(surface, getTransformMatrix, array);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderFrame();
}
