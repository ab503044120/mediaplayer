package org.huihui.musicplayer.demo;

import android.os.Bundle;
import android.os.Environment;
import android.widget.TextView;
import org.huihui.musicplayer.MusicPlayer;
import org.huihui.musicplayer.test.R;
import java.io.File;
import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        TextView viewById = findViewById(R.id.sample_text);
        viewById.setText("11111");
        MusicPlayer musicPlayer = new MusicPlayer();
        musicPlayer.SetDataSource(new File(Environment.getExternalStorageDirectory(), "input.mp3").getAbsolutePath());
        musicPlayer.prepare();
    }

}
