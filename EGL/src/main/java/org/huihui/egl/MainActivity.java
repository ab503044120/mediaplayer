package org.huihui.egl;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.TextView;

import org.huihui.egl.test.R;

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

            }

        });
        render = new EGLRender();
        Bitmap bitmap
                = BitmapFactory.decodeResource(getResources(), R.drawable.test);
        render.setBitmap(bitmap);
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
