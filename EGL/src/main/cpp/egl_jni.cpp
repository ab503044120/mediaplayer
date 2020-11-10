#include <jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "egl/EGLThread.h"
#include <android/bitmap.h>
#include "Log.h"
#include "JNI_Initializer.h"

void egl_init(JavaVM *vm, void *reserved);

static jclass gEGLRenderClass;
static jfieldID gEGLRenderClassNativeObjID;
static int mediaplayerRegister = registInitFun(&egl_init);

void egl_init(JavaVM *vm, void *reserved) {
  JNIEnv *env = nullptr;
  vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6);
  jclass classEGLRender = env->FindClass("org/huihui/egl/EGLRender");
  gEGLRenderClass = reinterpret_cast<jclass>(env->NewGlobalRef(classEGLRender));
  gEGLRenderClassNativeObjID = env->GetFieldID(classEGLRender, "nativeObj", "J");
}

extern "C"
JNIEXPORT void JNICALL
Java_org_huihui_egl_EGLRender_nInit(JNIEnv *env, jobject thiz) {
  env->SetLongField(thiz, gEGLRenderClassNativeObjID, reinterpret_cast<jlong>(new EGLThread()));
}

extern "C"
JNIEXPORT void JNICALL
Java_org_huihui_egl_EGLRender_nRelease(JNIEnv *env, jobject thiz) {
  auto *render = reinterpret_cast<EGLThread *>(env->GetLongField(thiz, gEGLRenderClassNativeObjID));
  delete render;
  env->SetLongField(thiz, gEGLRenderClassNativeObjID, -1L);
}

extern "C"
JNIEXPORT void JNICALL
Java_org_huihui_egl_EGLRender_nSurfaceCreated(JNIEnv *env, jobject thiz, jobject surface) {
  ANativeWindow *mWindow = ANativeWindow_fromSurface(env, surface);
  auto *render = reinterpret_cast<EGLThread *>(env->GetLongField(thiz, gEGLRenderClassNativeObjID));
  render->surfaceCreate(mWindow);
}

extern "C"
JNIEXPORT void JNICALL
Java_org_huihui_egl_EGLRender_nSurfaceChanged(JNIEnv *env, jobject thiz, jint width, jint height) {
  auto *render = reinterpret_cast<EGLThread *>(env->GetLongField(thiz, gEGLRenderClassNativeObjID));
  render->surfaceChange(height, width);
}

extern "C"
JNIEXPORT void JNICALL
Java_org_huihui_egl_EGLRender_nSurfaceDestroyed(JNIEnv *env, jobject thiz) {
  auto *render = reinterpret_cast<EGLThread *>(env->GetLongField(thiz, gEGLRenderClassNativeObjID));
  render->surfaceDestory();
}
extern "C"
JNIEXPORT void JNICALL
Java_org_huihui_egl_EGLRender_setBitmap(JNIEnv *env, jobject thiz, jobject bitmap) {
  auto *render = reinterpret_cast<EGLThread *>(env->GetLongField(thiz, gEGLRenderClassNativeObjID));
  AndroidBitmapInfo info; // create a AndroidBitmapInfo
  int result;
  // 获取图片信息
  result = AndroidBitmap_getInfo(env, bitmap, &info);
  if (result != ANDROID_BITMAP_RESULT_SUCCESS) {
    LOGE("JNI", "AndroidBitmap_getInfo failed, result: %d", result);
    return;
  }
  LOGD("JNI", "bitmap width: %d, height: %d, format: %d, stride: %d", info.width, info.height,
       info.format, info.stride);
  // 获取像素信息
  uint8_t *addrPtr = nullptr;
  result = AndroidBitmap_lockPixels(env, bitmap, reinterpret_cast<void **>(&addrPtr));
  auto *data = static_cast<uint8_t *>(malloc(info.stride * info.height));
  memcpy(data, addrPtr, info.stride * info.height);
  render->setBitmap(data, info.width, info.height, info.format);
  if (result != ANDROID_BITMAP_RESULT_SUCCESS) {
    LOGE("JNI", "AndroidBitmap_lockPixels failed, result: %d", result);
    return;
  }
  // 执行图片操作的逻辑

  // 像素信息不再使用后需要解除锁定
  result = AndroidBitmap_unlockPixels(env, bitmap);
  if (result != ANDROID_BITMAP_RESULT_SUCCESS) {
    LOGE("JNI", "AndroidBitmap_unlockPixels failed, result: %d", result);
  }
}