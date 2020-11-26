package org.huihui.opensles

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import org.huihui.opensles.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {
    lateinit var bind: ActivityMainBinding
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        bind = ActivityMainBinding.inflate(layoutInflater)
        setContentView(bind.root)
        bind.btn.setOnClickListener {

        }
    }
}