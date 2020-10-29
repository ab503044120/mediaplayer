package org.huihui.meidaplayer.demo;

import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.TextView;

import org.huihui.meidaplayer.MediaPlayer;
import org.huihui.meidaplayer.egl.EGLRender;
import org.huihui.meidaplayer.test.R;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    EGLRender render;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        TextView viewById = findViewById(R.id.sample_text);
        viewById.setText("11111");
        viewById.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                MediaPlayer mediaPlayer = new MediaPlayer();
//        musicPlayer.SetDataSource(new File(Environment.getExternalStorageDirectory(), "input.mp3").getAbsolutePath());
                mediaPlayer.SetDataSource("http://stream7.iqilu.com/10339/article/202002/17/778c5884fa97f460dac8d90493c451de.mp4");
                mediaPlayer.prepare();
            }

        });
        render = new EGLRender();
        ((SurfaceView) findViewById(R.id.sv)).getHolder().addCallback(new SurfaceHolder.Callback() {
            @Override
            public void surfaceCreated(@NonNull SurfaceHolder holder) {
                render.onSurfaceCreated(holder.getSurface());
            }

            @Override
            public void surfaceChanged(@NonNull SurfaceHolder holder, int format, int width, int height) {
                render.onSurfaceChanged(width, height);
            }

            @Override
            public void surfaceDestroyed(@NonNull SurfaceHolder holder) {
                render.onSurfaceDestroyed();
            }
        });
    }

}
