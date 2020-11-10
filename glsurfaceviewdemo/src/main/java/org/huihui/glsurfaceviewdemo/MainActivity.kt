package org.huihui.glsurfaceviewdemo

import android.opengl.GLES20
import android.opengl.GLSurfaceView
import android.opengl.GLSurfaceView.RENDERMODE_CONTINUOUSLY
import android.opengl.GLSurfaceView.RENDERMODE_WHEN_DIRTY
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Handler
import android.util.Log
import kotlinx.android.synthetic.main.activity_main.*
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class MainActivity : AppCompatActivity() {
    var handler = Handler()
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        gv.setRenderer(object : GLSurfaceView.Renderer {
            override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {

            }

            override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {

            }

            override fun onDrawFrame(gl: GL10?) {
                var last = System.currentTimeMillis()
                GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT or GLES20.GL_DEPTH_BUFFER_BIT)
                Log.e("haha", "onDrawFrame" + (System.currentTimeMillis() - last))
                Thread.sleep(10)
                GLES20.glClearColor(1f, 0f, 0f, 1f);
            }
        })
        gv.renderMode = RENDERMODE_WHEN_DIRTY
        handler.postDelayed(object :Runnable {
            override fun run() {
                gv.requestRender()
                handler.postDelayed(this,2)
            }
        }, 2)
    }
}
