#include <jni.h>
#include <android/bitmap.h>
#include <stdio.h>
#include <math.h>

#include<malloc.h>

#define ABS(a) ((a)<(0)?(-a):(a))
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

void Boxblur2(int *inPixels, int*outPixels, int width, int height, int Radius,int iterations);
void blur2(int* in, int* out, int width, int height, int radius);
void native_blur(JNIEnv *env, jobject* object, jobject srcBitmap,jint radius, int iterations) {
	void *buff1;
	void *buff2;
	AndroidBitmapInfo info;
	AndroidBitmap_getInfo(env, srcBitmap, &info);
	AndroidBitmap_lockPixels(env, srcBitmap, &buff1);
	buff2=new int[info.width*info.height];
	Boxblur2((int*) buff1, (int*) buff2, info.width, info.height,radius,iterations);
	AndroidBitmap_unlockPixels(env, srcBitmap);
	free(buff2);
}
static const JNINativeMethod method_table[] = { { "native_blur",
		"(Landroid/graphics/Bitmap;II)V",
		(void*) native_blur } };

static int clamp(int value, int start, int end) {
	if (value < start) {
		value = start;
	} else if (value >= end) {
		value = end - 1;
	}
	return value;
}
void Boxblur2(int *inPixels, int*outPixels, int width, int height, int Radius,int iterations) {
	int hRadius = 5;
	int vRadius = 5;
	for (int i = 0; i < iterations; i++) {
		blur2(inPixels, outPixels, width, height, hRadius);
		blur2(outPixels, inPixels, height, width, vRadius);
	}
}

void blur2(int* in, int* out, int width, int height, int radius) {
	int widthMinus1 = width - 1;
	int tableSize = 2 * radius + 1;
	int *divide = new int[256 * tableSize];

	for (int i = 0; i < 256 * tableSize; i++)
		divide[i] = i / tableSize;

	int inIndex = 0;

	for (int y = 0; y < height; y++) {
		int outIndex = y;
		int ta = 0, tr = 0, tg = 0, tb = 0;

		for (int i = -radius; i <= radius; i++) {
			int rgb = in[inIndex + clamp(i, 0, width - 1)];
			ta += (rgb >> 24) & 0xff;
			tr += (rgb >> 16) & 0xff;
			tg += (rgb >> 8) & 0xff;
			tb += rgb & 0xff;
		}

		for (int x = 0; x < width; x++) {
			out[outIndex] = (divide[ta] << 24) | (divide[tr] << 16)
					| (divide[tg] << 8) | divide[tb];

			int i1 = x + radius + 1;
			if (i1 > widthMinus1)
				i1 = widthMinus1;
			int i2 = x - radius;
			if (i2 < 0)
				i2 = 0;
			int rgb1 = in[inIndex + i1];
			int rgb2 = in[inIndex + i2];

			ta += ((rgb1 >> 24) & 0xff) - ((rgb2 >> 24) & 0xff);
			tr += ((rgb1 & 0xff0000) - (rgb2 & 0xff0000)) >> 16;
			tg += ((rgb1 & 0xff00) - (rgb2 & 0xff00)) >> 8;
			tb += (rgb1 & 0xff) - (rgb2 & 0xff);
			outIndex += height;
		}
		inIndex += width;
	}
	free(divide);
}
void JNI_OnUnload(JavaVM* vm, void* reserved) {

	//__android_log_print(ANDROID_LOG_ERROR, "zhou", "JNI_OnLoad");
}



jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	//__android_log_print(ANDROID_LOG_ERROR, "zhou", "JNI_OnLoad");

	JNIEnv* env = NULL;
	jint result = -1;

	if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		return result;
	}

	jclass imageutil = env->FindClass("com/nd/hilauncherdev/launcher/LauncherAnimationHelp");

	result = env->RegisterNatives(imageutil, method_table, 1);
	if (result == JNI_OK) {
		//__android_log_print(ANDROID_LOG_ERROR, "zhou","RegisterNatives succeed");
	} else {
		//__android_log_print(ANDROID_LOG_ERROR, "zhou","RegisterNatives fail R=%d", result);
	}
	return JNI_VERSION_1_4;
}

