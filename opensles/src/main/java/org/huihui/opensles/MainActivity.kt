package org.huihui.opensles

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import org.huihui.opensles.databinding.ActivityMainBinding
import java.io.File
import java.io.FileOutputStream

class MainActivity : AppCompatActivity() {
    companion object {
        init {
            System.loadLibrary("opensles_demo")
        }
    }

    lateinit var bind: ActivityMainBinding
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        resources.openRawResource(R.raw.output).copyTo(FileOutputStream(File(filesDir, "output.pcm")))
        bind = ActivityMainBinding.inflate(layoutInflater)
        setContentView(bind.root)
        bind.btn.setOnClickListener {
            play(File(filesDir, "output.pcm").absolutePath)
        }

    }

    private external fun play(path: String)

}