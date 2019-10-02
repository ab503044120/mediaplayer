package org.huihui.musicplayer.demo;

import android.os.Bundle;
import android.os.Environment;

import org.huihui.musicplayer.MusicPlayer;

import java.io.File;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        MusicPlayer musicPlayer = new MusicPlayer();
        musicPlayer.SetDataSource(new File(Environment.getExternalStorageDirectory(), "input.mp3").getAbsolutePath());
        musicPlayer.prepare();
    }

}
