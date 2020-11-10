package org.huihui.meidaplayer.demo;

import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import org.huihui.meidaplayer.MediaPlayer;
import org.huihui.meidaplayer.test.R;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

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
    }

}
