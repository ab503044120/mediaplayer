package org.huihui.meidaplayer;

public class MediaPlayer {

    static {
        System.loadLibrary("mediaplayer");
    }

    private long nPlayer;

    public MediaPlayer() {
        nPlayer = nInit();
    }

    public void SetDataSource(String source) {
        nSetDataSource(nPlayer, source);
    }

    public void prepare() {
        nPrepare(nPlayer);
    }

    private native long nInit();

    private native void nSetDataSource(long nPlayer, String source);

    private native void nPrepare(long nPlayer);

    private native void nStart(long nPlayer);

    private native void nStop(long nPlayer);

    private native void nPause(long nPlayer);

    private native void nSeek(long nPlayer, long msec);

    private native long nGetPosition(long nPlayer);

    private native long nGetDuration(long nPlayer);

    private native void nReset(long nPlayer);

    private native void nRelease(long nPlayer);

}
