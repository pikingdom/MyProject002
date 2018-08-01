package com.nd.hilauncherdev.launcher;

import android.graphics.Bitmap;

public class LauncherAnimationHelp {
	public static native void native_blur(Bitmap bitmap,int radius,int iterations,int startRow,int endRow);
}
