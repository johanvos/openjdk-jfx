/*
 * Copyright (c) 2014, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.  Oracle designates this
 * particular file as subject to the "Classpath" exception as provided
 * by Oracle in the LICENSE file that accompanied this code.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 */
#include <EGL/egl.h>
// #include <GLES/gl.h>
#include "jni.h"
#include <android/log.h>
#include <android/native_window_jni.h>
#define  LOG_TAG  "GraalGluon"

#define  LOGD(ignore, ...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(ignore, ...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)



// #include "com_sun_glass_ui_monocle_EGL.h"
#include "Monocle.h"

#include <stdlib.h>
//Builtin library entrypoint
JNIEXPORT jint JNICALL
JNI_OnLoad_glass_monocle(JavaVM *vm, void * reserved) {
#ifdef JNI_VERSION_1_8
    //min. returned JNI_VERSION required by JDK8 for builtin libraries
    JNIEnv *env;
    if ((*vm)->GetEnv(vm, (void **)&env, JNI_VERSION_1_8) != JNI_OK) {
        return JNI_VERSION_1_4;
    }
    return JNI_VERSION_1_8;
#else
    return JNI_VERSION_1_4;
#endif
}

extern int setEGLAttrs(jint *attrs, int *eglAttrs);

/*
int setEGLAttrs(jint *attrs, int *eglAttrs) {
    int index = 0;

    eglAttrs[index++] = EGL_SURFACE_TYPE;
    if (attrs[6] != 0) {
        eglAttrs[index++] = (EGL_WINDOW_BIT);
    } else {
        eglAttrs[index++] = EGL_PBUFFER_BIT;
    }

    // TODO:  We are depending on the order of attributes defined in
    // GLPixelFormat - we need a better way to manage this

    if (attrs[0] == 5 && attrs[1] == 6
            && attrs[2] == 5 && attrs[3] == 0) {
        // Optimization for Raspberry Pi model B. Even though the result
        // of setting EGL_BUFFER_SIZE to 16 should be the same as setting
        // component sizes separately, we get less per-frame overhead if we
        // only set EGL_BUFFER_SIZE.
        eglAttrs[index++] = EGL_BUFFER_SIZE;
        eglAttrs[index++] = 16;
    } else {
        eglAttrs[index++] = EGL_RED_SIZE;
        eglAttrs[index++] = attrs[0];
        eglAttrs[index++] = EGL_GREEN_SIZE;
        eglAttrs[index++] = attrs[1];
        eglAttrs[index++] = EGL_BLUE_SIZE;
        eglAttrs[index++] = attrs[2];
        eglAttrs[index++] = EGL_ALPHA_SIZE;
        eglAttrs[index++] = attrs[3];
    }

    eglAttrs[index++] = EGL_DEPTH_SIZE;
    eglAttrs[index++] = attrs[4];
    eglAttrs[index++] = EGL_RENDERABLE_TYPE;
    eglAttrs[index++] = EGL_OPENGL_ES2_BIT;
    eglAttrs[index] = EGL_NONE;
return index;
}
*/

JNIEXPORT void testGraalGL(ANativeWindow *mywindow);

ANativeWindow* anwindow;

JNIEXPORT jlong JNICALL Java_com_sun_glass_ui_monocle_EGL_eglGetDisplay
    (JNIEnv *env, jclass clazz, jlong display) {
LOGE(stderr, "[EGLJNI] getDisplay, dislay = %ld or %p\n", display, (void *)display);
LOGE(stderr, "anwindow = %p\n",anwindow);
    EGLDisplay dpy = eglGetDisplay(((EGLNativeDisplayType) (unsigned long)(display)));
LOGE(stderr, "[EGLJNI] getDisplay, got dislay = %ld or %p\n", (long)dpy, dpy);
    return asJLong(dpy);
}

JNIEXPORT jboolean JNICALL Java_com_sun_glass_ui_monocle_EGL_eglInitialize
    (JNIEnv *env, jclass clazz, jlong eglDisplayPtr, jintArray majorArray,
     jintArray minorArray){
EGLDisplay eglDisplay = (EGLDisplay)eglDisplayPtr;
LOGE(stderr, "[EGLJNI] eglInitialize, egldislay = %ld or %p\n", eglDisplayPtr, eglDisplay);

    EGLint major, minor;
if (env ==0) return JNI_TRUE;
    if (eglInitialize(eglDisplay, &major, &minor)) {
         (*env)->SetIntArrayRegion(env, majorArray, 0, 1, &major);
         (*env)->SetIntArrayRegion(env, minorArray, 0, 1, &minor);
        return JNI_TRUE;
    } else {
LOGE(stderr, "[EGLJNI] major issue! eglInitialize failes!!!\n\n\n");
        return JNI_FALSE;
    }
}

JNIEXPORT jboolean JNICALL Java_com_sun_glass_ui_monocle_EGL_eglBindAPI
    (JNIEnv *env, jclass clazz, jint api) {
LOGE(stderr, "[EEEEGGGGGLLLL] bindApi\n");

    if (eglBindAPI(api)) {
        return JNI_TRUE;
    } else {
        return JNI_FALSE;
    }
}

JNIEXPORT jboolean JNICALL Java_com_sun_glass_ui_monocle_EGL_eglChooseConfig
    (JNIEnv *env, jclass clazz, jlong eglDisplayPtr, jintArray attribs,
     jlongArray configs, jint configSize, jintArray numConfigs) {
EGLDisplay eglDisplay = (EGLDisplay)eglDisplayPtr;
    int i=0;

    int eglAttrs[50]; // value, attr pair plus a None
    jint *attrArray;

    attrArray = (*env)->GetIntArrayElements(env, attribs, JNI_FALSE);
    int cnt = setEGLAttrs(attrArray, eglAttrs);
    (*env)->ReleaseIntArrayElements(env, attribs, attrArray, JNI_ABORT);
    EGLConfig *configArray = malloc(sizeof(EGLConfig) * configSize);
    jlong *longConfigArray = malloc(sizeof(long) * configSize);
    EGLint numConfigPtr=0;
    jboolean retval;
     for (int i =0; i < cnt; i++) {
         LOGE(stderr, "attributes for config %d: %d\n", i, eglAttrs[i]);
     }


    if (!eglChooseConfig(eglDisplay, eglAttrs, configArray, configSize,
                               &numConfigPtr)) {
        retval = JNI_FALSE;
    } else {
        retval = JNI_TRUE;
        (*env)->SetIntArrayRegion(env, numConfigs, 0, 1, &numConfigPtr);
        for (i = 0; i < numConfigPtr; i++) {
            longConfigArray[i] = asJLong(configArray[i]);
        }

        (*env)->SetLongArrayRegion(env, configs, 0, configSize, longConfigArray);
    }
    free(configArray);
    free(longConfigArray);
LOGE(stderr, "GLERR after config?  %d\n",eglGetError());
    return retval;
}

EGLSurface mysurface;

JNIEXPORT jlong JNICALL Java_com_sun_glass_ui_monocle_EGL__1eglCreateWindowSurface
    (JNIEnv *env, jclass clazz, jlong eglDisplayPtr, jlong eglConfigPtr,
     jlong nativeWindow, jintArray attribs) {

LOGE(stderr, "[JNIEGL] createWindowSurface\n");
EGLDisplay eglDisplay = (EGLDisplay)eglDisplayPtr;
EGLConfig eglConfig = (EGLConfig)eglConfigPtr;
    EGLSurface eglSurface;
    EGLint *attrArray = NULL;

    if (attribs != NULL) {
        attrArray = (*env)->GetIntArrayElements(env, attribs, JNI_FALSE);
    }

    eglSurface =  eglCreateWindowSurface(eglDisplay, eglConfig,
                                         (EGLNativeWindowType) nativeWindow,
                                         (EGLint *) NULL);
mysurface = eglSurface;
    if (attrArray != NULL) {
        (*env)->ReleaseIntArrayElements(env, attribs, attrArray, JNI_ABORT);
    }
LOGE(stderr, "EGL Surface at %p\n", eglSurface);
LOGE(stderr, "GLERR?  %d\n",eglGetError());
/*
LOGE(stderr, "while we're here, lets create eglContext\n");
LOGE(stderr, "size of EGLDisplay = %ul\n",sizeof(EGLDisplay));
jlong eglContext = Java_com_sun_glass_ui_monocle_EGL_eglCreateContext (env, 0, eglDisplayPtr, eglConfigPtr, 0, 0);
LOGE(stderr, "while we're here, we created eglContext\n");
*/
    return asJLong(eglSurface);
}

JNIEXPORT jlong JNICALL Java_com_sun_glass_ui_monocle_EGL_eglCreateContext
    (JNIEnv *env, jclass clazz, jlong eglDisplayPtr, jlong eglConfigPtr,
      jlong shareContext, jintArray attribs){

EGLDisplay eglDisplay = (EGLDisplay)eglDisplayPtr;
EGLConfig eglConfig = (EGLConfig)eglConfigPtr;

LOGE(stderr, "EGLcretecontext, egldisplay at %p with val %ld\n", eglDisplay, eglDisplayPtr);
LOGE(stderr, "EGLcretecontext, config at %p with val %ld\n", eglConfig, eglConfigPtr);
    // we don't support any passed-in context attributes presently
    // we don't support any share context presently
    EGLint contextAttrs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    EGLContext context = eglCreateContext(eglDisplay, eglConfig,
                                          NULL, contextAttrs);
LOGE(stderr, "EGL Context at %p\n", context);
LOGE(stderr, "GLERR?  %d\n",eglGetError());

// LOGE(stderr, "And now also make current!! surface = %p\n",mysurface);
 // EGLBoolean makec = eglMakeCurrent(eglDisplay, mysurface, mysurface, context);
 // EGLBoolean makec = noJava_com_sun_glass_ui_monocle_EGL_eglMakeCurrent(env, 0, eglDisplayPtr, (long)mysurface, (long)mysurface, (jlong)context);
// LOGE(stderr, "And now also made current!! answer = %d\n",makec);

    return asJLong(context);
}

JNIEXPORT jboolean JNICALL Java_com_sun_glass_ui_monocle_EGL_eglMakeCurrent
   (JNIEnv *env, jclass clazz, jlong eglDisplayPtr, jlong drawSurfacePtr,
    jlong readSurfacePtr, jlong eglContextPtr) {
EGLDisplay eglDisplay = (EGLDisplay)eglDisplayPtr;
EGLSurface eglDrawSurface = (EGLSurface)drawSurfacePtr;
EGLSurface eglReadSurface = (EGLSurface)readSurfacePtr;
EGLContext eglContext = (EGLSurface)eglContextPtr;
LOGE(stderr, "EGL MakeCurrent! disp = %p, surface = %p, readS = %p, context = %p\n", eglDisplay, eglDrawSurface, eglReadSurface, eglContext);
LOGE(stderr, "and makeCurrent = %p\n", &eglMakeCurrent);
LOGE(stderr, "GLERR?  %d\n",eglGetError());


    if (eglMakeCurrent(eglDisplay, eglDrawSurface, eglReadSurface,
                   eglContext)) {

/*
 glClearColor(0.0f, 1.0f, 0.0f, 1.0f); // Set background color to black and opaque
  glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer
   glFlush();
eglSwapBuffers(eglDisplay, eglDrawSurface);
LOGE(stderr, "DONE SWAPPING\n");
*/

        return JNI_TRUE;
    } else {
        return JNI_FALSE;
    }
}

JNIEXPORT jboolean JNICALL Java_com_sun_glass_ui_monocle_EGL_eglSwapBuffers
    (JNIEnv *env, jclass clazz, jlong eglDisplayPtr, jlong eglSurfacePtr) {
EGLDisplay eglDisplay = (EGLDisplay)eglDisplayPtr;
EGLSurface eglSurface = (EGLSurface)eglSurfacePtr;
LOGE(stderr, "[MonocleGL] swapBuffers, display = %p and surface = %p\n", eglDisplay, eglSurface);
    if (eglSwapBuffers(eglDisplay, eglSurface)) {
        return JNI_TRUE;
    } else {
        return JNI_FALSE;
    }
}

JNIEXPORT jint  JNICALL Java_com_sun_glass_ui_monocle_EGL_eglGetError
    (JNIEnv *env, jclass clazz) {
    return (jint)eglGetError();
}

JNIEXPORT void testGraalGL(ANativeWindow *mywindow) {
// void testGraalGL(ANativeWindow* mywindow) {
    LOGE(stderr, "TEST GL (from monocle), mywindow at %p !\n\n\n\n\n\n", mywindow);
    EGLDisplay eglDisplay = eglGetDisplay((EGLNativeDisplayType) 0);
    LOGE(stderr, "GLERR? after disp %d\n",eglGetError());
    LOGE(stderr, "DISPLAY = %p\n", eglDisplay);
    int* major = malloc(4);
    int minor = 0;
    EGLBoolean initial = eglInitialize(eglDisplay, major, &minor);
    LOGE(stderr, "GLERR afterinit? %d\n",eglGetError());
    LOGE(stderr, "init, %d -- %d\n", *major, minor);
    eglBindAPI(EGL_OPENGL_ES_API);
    LOGE(stderr, "GLERR afterbindapi? %d\n",eglGetError());
int eglAttrs[50];
int jniAttrs[50];
jniAttrs[0] = 8;
jniAttrs[1] = 8;
jniAttrs[2] = 8;
jniAttrs[3] = 8;
jniAttrs[4] = 16;
jniAttrs[5] = 1;
jniAttrs[6] = 1;
int cnt = setEGLAttrs(jniAttrs, eglAttrs);
for (int i =0; i < cnt; i++) {
    LOGE(stderr, "attributes for config %d: %d\n", i, eglAttrs[i]);
}
int configSize = 1;
EGLConfig *configArray = malloc(sizeof(EGLConfig) * configSize);
    jlong *longConfigArray = malloc(sizeof(long) * configSize);
    EGLint numConfigPtr=0;
EGLBoolean configChoose = eglChooseConfig(eglDisplay, eglAttrs, configArray, configSize,
                               &numConfigPtr);
    LOGE(stderr, "GLERR afterconfigchoose? %d\n",eglGetError());
    LOGE(stderr, "result? %d\n", configChoose);

EGLSurface eglSurface = eglCreateWindowSurface(eglDisplay, configArray[0], (EGLNativeWindowType)mywindow, NULL);
    LOGE(stderr, "GLERR eglSurface? %d\n",eglGetError());
    LOGE(stderr, "surface =  %p\n", eglSurface);


EGLint contextAttrs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
EGLContext eglContext = eglCreateContext(eglDisplay, configArray[0], 0, contextAttrs);
    LOGE(stderr, "GLERR eglContext? %d\n",eglGetError());
    LOGE(stderr, "context =  %p\n", eglContext);
EGLBoolean makec = eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
    LOGE(stderr, "GLERR egmakecurrent? %d\n",eglGetError());
    LOGE(stderr, "current =  %d\n", makec);

}

JNIEXPORT void testGraalGLWithEnv(JNIEnv *env, ANativeWindow *mywindow, jintArray majorArray, jintArray minorArray, jintArray attributes, jlongArray configs, jintArray configCount) {
/*
// void testGraalGL(ANativeWindow* mywindow) {
    LOGE(stderr, "TEST GLwith env (from monocle), mywindow at %p !\n\n\n\n\n\n", mywindow);
    // EGLDisplay eglDisplay = eglGetDisplay((EGLNativeDisplayType) 0);
jlong eglDisplay = Java_com_sun_glass_ui_monocle_EGL_eglGetDisplay (env, 0,0);
    LOGE(stderr, "GLERR? after disp %d\n",eglGetError());
    LOGE(stderr, "DISPLAY = %lx\n", eglDisplay);
    // int* major = malloc(4);
    // int minor = 0;
jboolean initial = Java_com_sun_glass_ui_monocle_EGL_eglInitialize (env, 0, eglDisplay, majorArray, minorArray);

    // EGLBoolean initial = eglInitialize(eglDisplay, major, &minor);

    LOGE(stderr, "GLERR afterinit? %d\n",eglGetError());
    // LOGE(stderr, "init, %d -- %d\n", *major, minor);

jboolean binded = Java_com_sun_glass_ui_monocle_EGL_eglBindAPI (env, 0, 12448);

 jboolean configChoose = Java_com_sun_glass_ui_monocle_EGL_eglChooseConfig (env, 0, eglDisplay, attributes, configs, 1,configCount );

    LOGE(stderr, "GLERR afterconfigchoose? %d\n",eglGetError());
    LOGE(stderr, "result? %d\n", configChoose);
    long* rawConfig = (*env)->GetLongArrayElements(env, configs, JNI_FALSE);

LOGE(stderr, "rawconfig at %ld or %p\n", rawConfig[0], rawConfig);

 jlong eglSurface = Java_com_sun_glass_ui_monocle_EGL__1eglCreateWindowSurface (env, 0, eglDisplay, rawConfig[0], (jlong)mywindow, 0);

    LOGE(stderr, "GLERR eglSurface? %d\n",eglGetError());
    LOGE(stderr, "surface =  %lx\n", eglSurface);

jlong eglContext = Java_com_sun_glass_ui_monocle_EGL_eglCreateContext (env, 0, eglDisplay, rawConfig[0], 0, 0);
    LOGE(stderr, "context =  %lx\n", eglContext);

EGLBoolean makec = Java_com_sun_glass_ui_monocle_EGL_eglMakeCurrent (env, 0, eglDisplay, eglSurface, eglSurface,  eglContext);
    LOGE(stderr, "make current answer =  %d\n", makec);

/*

EGLint contextAttrs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
EGLContext eglContext = eglCreateContext(eglDisplay, configArray[0], 0, contextAttrs);
    LOGE(stderr, "GLERR eglContext? %d\n",eglGetError());
    LOGE(stderr, "context =  %p\n", eglContext);
EGLBoolean makec = eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
    LOGE(stderr, "GLERR egmakecurrent? %d\n",eglGetError());
    LOGE(stderr, "current =  %d\n", makec);
*/

}

JNIEXPORT void  JNICALL Java_com_sun_glass_ui_monocle_EGL_testGraal
    (JNIEnv *env, jclass clazz, jlong nativeWindow, jintArray major, jintArray minor, jintArray attributes, jlongArray eglConfigs, jintArray configcounts) {
LOGE(stderr, "JNI testGraal asked, let's do that. Set window though to %lx.\n", nativeWindow);
anwindow = (ANativeWindow*)nativeWindow;
    // return testGraalGLWithEnv(env, nativeWindow, major, minor, attributes, eglConfigs, configcounts);
}


