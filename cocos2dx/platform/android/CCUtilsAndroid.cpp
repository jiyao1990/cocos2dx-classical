#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#include "CCUtilsAndroid.h"
#include "ccTypes.h"
#include "CCUtils.h"
#include <jni.h>

NS_CC_BEGIN

static jobject gContext = NULL;

JNIEnv* CCUtilsAndroid::getJNIEnv() {
    JavaVM* vm = JniHelper::getJavaVM();
    JNIEnv* env = NULL;
    if(vm) {
        jint ret = vm->GetEnv((void**) &env, JNI_VERSION_1_4);
        if (ret != JNI_OK) {
            jint status = vm->AttachCurrentThread(&env, NULL);
            if(status < 0) {
                CCLOGERROR("getJNIEnv: failed to attach current thread");
                env = NULL;
            }
        }
    }
    
    return env;
}

jobject CCUtilsAndroid::getContext() {
    // if global context is not saved, try to get one from java side
    if(gContext == NULL) {
        JniMethodInfo t;
        JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/Cocos2dxActivity", "getContext", "()Landroid/content/Context;");
        jobject ctx = t.env->CallStaticObjectMethod(t.classID, t.methodID);
        
        // save a global ref
        gContext = t.env->NewGlobalRef(ctx);
        
        // release
        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(ctx);
    }
    
    // return
    return gContext;
}

int CCUtilsAndroid::getVersionCode() {
    // get package manager
    JniMethodInfo t;
    JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/CCUtils", "getVersionCode", "()I");
    int ret = t.env->CallStaticIntMethod(t.classID, t.methodID);
    
    // release
    t.env->DeleteLocalRef(t.classID);
    
    // return
    return ret;
}

string CCUtilsAndroid::getMainExpansionPath(int versionCode) {
    // call java method
    JniMethodInfo t;
    JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/CCUtils", "getMainExpansionPath", "(I)Ljava/lang/String;");
    jstring jPath = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID, versionCode);
    string path = JniHelper::jstring2string(jPath);
    
    // release
    t.env->DeleteLocalRef(jPath);
    t.env->DeleteLocalRef(t.classID);
    
    // return
    return path;
}

string CCUtilsAndroid::getPatchExpansionPath(int versionCode) {
    // call java method
    JniMethodInfo t;
    JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/CCUtils", "getPatchExpansionPath", "(I)Ljava/lang/String;");
    jstring jPath = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID, versionCode);
    string path = JniHelper::jstring2string(jPath);
    
    // release
    t.env->DeleteLocalRef(jPath);
    t.env->DeleteLocalRef(t.classID);
    
    // return
    return path;
}

jobject CCUtilsAndroid::newIntent(const char* activityName) {
    // get context
    jobject context = getContext();
    
    // find constructor
    JniMethodInfo t;
    JniHelper::getMethodInfo(t, "android/content/Intent", "<init>", "(Landroid/content/Context;Ljava/lang/Class;)V");
    
    // create activity name
    size_t len = strlen(activityName);
    char* jniName = (char*)calloc(len + 1, sizeof(char));
    for(int i = 0; i < len; i++) {
        if(activityName[i] == '.')
            jniName[i] = '/';
        else
            jniName[i] = activityName[i];
    }
    jclass actClass = t.env->FindClass(jniName);
    
    // new intent
    jobject intent = t.env->NewObject(t.classID, t.methodID, context, actClass);
    
    // clear
    t.env->DeleteLocalRef(actClass);
    t.env->DeleteLocalRef(context);
    CC_SAFE_FREE(jniName);
    t.env->DeleteLocalRef(t.classID);
    
    return intent;
}

jobject CCUtilsAndroid::newIntentByAction(const char* action) {
    JniMethodInfo t;
    JniHelper::getMethodInfo(t, "android/content/Intent", "<init>", "(Ljava/lang/String;)V");
    jstring a = t.env->NewStringUTF(action);
    jobject intent = t.env->NewObject(t.classID, t.methodID, a);
    t.env->DeleteLocalRef(a);
    t.env->DeleteLocalRef(t.classID);
    return intent;
}

void CCUtilsAndroid::setIntentUri(jobject intent, const char* uri) {
    JniMethodInfo t;
    JniHelper::getStaticMethodInfo(t, "android/net/Uri", "parse", "(Ljava/lang/String;)Landroid/net/Uri;");
    jstring jUriStr = t.env->NewStringUTF(uri);
    jobject jUri = t.env->CallStaticObjectMethod(t.classID, t.methodID, jUriStr);
    
    // release
    t.env->DeleteLocalRef(t.classID);
    
    JniHelper::getMethodInfo(t, "android/content/Intent", "setData", "(Landroid/net/Uri;)Landroid/content/Intent;");
    t.env->CallObjectMethod(intent, t.methodID, jUri);
    t.env->DeleteLocalRef(jUri);
    t.env->DeleteLocalRef(jUriStr);
    t.env->DeleteLocalRef(t.classID);
}

void CCUtilsAndroid::putBooleanExtra(jobject intent, const char* name, bool value) {
    JniMethodInfo t;
    JniHelper::getMethodInfo(t, "android/content/Intent", "putExtra", "(Ljava/lang/String;Z)Landroid/content/Intent;");
    jstring s = t.env->NewStringUTF(name);
    t.env->CallObjectMethod(intent, t.methodID, s, value);
    t.env->DeleteLocalRef(s);
    t.env->DeleteLocalRef(t.classID);
}

void CCUtilsAndroid::putByteExtra(jobject intent, const char* name, unsigned char value) {
    JniMethodInfo t;
    JniHelper::getMethodInfo(t, "android/content/Intent", "putExtra", "(Ljava/lang/String;B)Landroid/content/Intent;");
    jstring s = t.env->NewStringUTF(name);
    t.env->CallObjectMethod(intent, t.methodID, s, value);
    t.env->DeleteLocalRef(s);
    t.env->DeleteLocalRef(t.classID);
}

void CCUtilsAndroid::putCharExtra(jobject intent, const char* name, unsigned short value) {
    JniMethodInfo t;
    JniHelper::getMethodInfo(t, "android/content/Intent", "putExtra", "(Ljava/lang/String;C)Landroid/content/Intent;");
    jstring s = t.env->NewStringUTF(name);
    t.env->CallObjectMethod(intent, t.methodID, s, value);
    t.env->DeleteLocalRef(s);
    t.env->DeleteLocalRef(t.classID);
}

void CCUtilsAndroid::putDoubleExtra(jobject intent, const char* name, double value) {
    JniMethodInfo t;
    JniHelper::getMethodInfo(t, "android/content/Intent", "putExtra", "(Ljava/lang/String;D)Landroid/content/Intent;");
    jstring s = t.env->NewStringUTF(name);
    t.env->CallObjectMethod(intent, t.methodID, s, value);
    t.env->DeleteLocalRef(s);
    t.env->DeleteLocalRef(t.classID);
}

void CCUtilsAndroid::putFloatExtra(jobject intent, const char* name, float value) {
    JniMethodInfo t;
    JniHelper::getMethodInfo(t, "android/content/Intent", "putExtra", "(Ljava/lang/String;F)Landroid/content/Intent;");
    jstring s = t.env->NewStringUTF(name);
    t.env->CallObjectMethod(intent, t.methodID, s, value);
    t.env->DeleteLocalRef(s);
    t.env->DeleteLocalRef(t.classID);
}

void CCUtilsAndroid::putIntExtra(jobject intent, const char* name, int value) {
    JniMethodInfo t;
    JniHelper::getMethodInfo(t, "android/content/Intent", "putExtra", "(Ljava/lang/String;I)Landroid/content/Intent;");
    jstring s = t.env->NewStringUTF(name);
    t.env->CallObjectMethod(intent, t.methodID, s, value);
    t.env->DeleteLocalRef(s);
    t.env->DeleteLocalRef(t.classID);
}

void CCUtilsAndroid::putStringExtra(jobject intent, const char* name, const char* value) {
    JniMethodInfo t;
    JniHelper::getMethodInfo(t, "android/content/Intent", "putExtra", "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;");
    jstring s = t.env->NewStringUTF(name);
    t.env->CallObjectMethod(intent, t.methodID, s, value);
    t.env->DeleteLocalRef(s);
    t.env->DeleteLocalRef(t.classID);
}

void CCUtilsAndroid::putLongExtra(jobject intent, const char* name, long value) {
    JniMethodInfo t;
    JniHelper::getMethodInfo(t, "android/content/Intent", "putExtra", "(Ljava/lang/String;J)Landroid/content/Intent;");
    jstring s = t.env->NewStringUTF(name);
    t.env->CallObjectMethod(intent, t.methodID, s, value);
    t.env->DeleteLocalRef(s);
    t.env->DeleteLocalRef(t.classID);
}

void CCUtilsAndroid::putShortExtra(jobject intent, const char* name, short value) {
    JniMethodInfo t;
    JniHelper::getMethodInfo(t, "android/content/Intent", "putExtra", "(Ljava/lang/String;S)Landroid/content/Intent;");
    jstring s = t.env->NewStringUTF(name);
    t.env->CallObjectMethod(intent, t.methodID, s, value);
    t.env->DeleteLocalRef(s);
    t.env->DeleteLocalRef(t.classID);
}

void CCUtilsAndroid::putParcelableExtra(jobject intent, const char* name, jobject value) {
    JniMethodInfo t;
    JniHelper::getMethodInfo(t, "android/content/Intent", "putExtra", "(Ljava/lang/String;Landroid/os/Parcelable;)Landroid/content/Intent;");
    jstring s = t.env->NewStringUTF(name);
    t.env->CallObjectMethod(intent, t.methodID, s, value);
    t.env->DeleteLocalRef(s);
    t.env->DeleteLocalRef(t.classID);
}

void CCUtilsAndroid::startActivity(jobject intent) {
    jobject ctx = getContext();
    JniMethodInfo t;
    JniHelper::getMethodInfo(t, "android/content/Context", "startActivity", "(Landroid/content/Intent;)V");
    t.env->CallVoidMethod(ctx, t.methodID, intent);
    t.env->DeleteLocalRef(ctx);
    t.env->DeleteLocalRef(t.classID);
}

void CCUtilsAndroid::sendBroadcast(jobject intent) {
    jobject ctx = getContext();
    JniMethodInfo t;
    JniHelper::getMethodInfo(t, "android/content/Context", "sendBroadcast", "(Landroid/content/Intent;)V");
    t.env->CallVoidMethod(ctx, t.methodID, intent);
    t.env->DeleteLocalRef(ctx);
    t.env->DeleteLocalRef(t.classID);
}

NS_CC_END

#endif // #if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID