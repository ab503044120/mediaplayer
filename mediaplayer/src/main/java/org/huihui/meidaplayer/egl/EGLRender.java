package org.huihui.meidaplayer.egl;

import android.view.Surface;

/*
 * @Description:
 * @version 1.0
 * @author 陈松辉
 * @date 2020/10/22 22:11
 */
public class EGLRender {

    static {
        System.loadLibrary("mediaplayer");
    }
    private long nativeObj = -1;

    public EGLRender() {
        nInit();
    }

    public void release() {
        nRelease();
    }

    public void onSurfaceCreated(Surface surface) {
        nSurfaceCreated(surface);
    }

    public void onSurfaceChanged(int width, int height) {
        nSurfaceChanged(width, height);
    }

    public void onSurfaceDestroyed() {
        nSurfaceDestroyed();
    }

    private native void nInit();

    private native void nRelease();

    private native void nSurfaceCreated(Surface surface);

    private native void nSurfaceChanged(int width, int height);

    private native void nSurfaceDestroyed();
}

