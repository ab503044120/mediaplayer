#include <jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "JNI_Initializer.h"
#include "egl/EGLThread.h"

void egl_init(JavaVM *vm, void *reserved);

static jclass gEGLRenderClass;
static jfieldID gEGLRenderClassNativeObjID;
static int mediaplayerRegister = registInitFun(&egl_init);

void egl_init(JavaVM *vm, void *reserved) {
  JNIEnv *env = nullptr;
  vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6);
  jclass classEGLRender = env->FindClass("org/huihui/meidaplayer/egl/EGLRender");
  gEGLRenderClass = reinterpret_cast<jclass>(env->NewGlobalRef(classEGLRender));
  gEGLRenderClassNativeObjID = env->GetFieldID(classEGLRender, "nativeObj", "J");
}

extern "C"
JNIEXPORT void JNICALL
Java_org_huihui_meidaplayer_egl_EGLRender_nInit(JNIEnv *env, jobject thiz) {
  env->SetLongField(thiz, gEGLRenderClassNativeObjID, reinterpret_cast<jlong>(new EGLThread()));
}

extern "C"
JNIEXPORT void JNICALL
Java_org_huihui_meidaplayer_egl_EGLRender_nRelease(JNIEnv *env, jobject thiz) {
  auto *render = reinterpret_cast<EGLThread *>(env->GetLongField(thiz, gEGLRenderClassNativeObjID));
  delete render;
  env->SetLongField(thiz, gEGLRenderClassNativeObjID, -1L);
}

extern "C"
JNIEXPORT void JNICALL
Java_org_huihui_meidaplayer_egl_EGLRender_nSurfaceCreated(JNIEnv *env, jobject thiz, jobject surface) {
  ANativeWindow *mWindow = ANativeWindow_fromSurface(env, surface);
  ANativeWindow_fromSurfacena
  auto *render = reinterpret_cast<EGLThread *>(env->GetLongField(thiz, gEGLRenderClassNativeObjID));
  render->surfaceCreate(mWindow);
}

extern "C"
JNIEXPORT void JNICALL
Java_org_huihui_meidaplayer_egl_EGLRender_nSurfaceChanged(JNIEnv *env, jobject thiz, jint width, jint height) {
  auto *render = reinterpret_cast<EGLThread *>(env->GetLongField(thiz, gEGLRenderClassNativeObjID));
  render->surfaceChange(height, width);
}

extern "C"
JNIEXPORT void JNICALL
Java_org_huihui_meidaplayer_egl_EGLRender_nSurfaceDestroyed(JNIEnv *env, jobject thiz) {
  auto *render = reinterpret_cast<EGLThread *>(env->GetLongField(thiz, gEGLRenderClassNativeObjID));
  render->surfaceDestory();
}