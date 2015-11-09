//
// Created by nedearb on 11/8/15.
//

#include <string.h>
#include <jni.h>

jstring Java_takp_nedearb_underneath_MainActivity_stringFromJNI(JNIEnv* env, jobject thiz) {
    return (*env)->NewStringUTF(env, "Hello JNI");
}


