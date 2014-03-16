LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/GameMediator.cpp \
                   ../../Classes/MapManager.cpp \
                   ../../Classes/Variable.cpp \
                   ../../Classes/HelpScene.cpp \
                   ../../Classes/ScoreScene.cpp \
                   ../../Classes/SelectDifficuldScene.cpp \
                   ../../Classes/SelectMapScene.cpp \
                   ../../Classes/SelectSprite.cpp \
                   ../../Classes/GameScene.cpp \
                   ../../Classes/LevelLayer.cpp \
                   ../../Classes/MenuScene.cpp \
                   ../../Classes/GameHUD.cpp \
                   ../../Classes/Enemy.cpp \
                   ../../Classes/Tower.cpp \
                   ../../Classes/Projectile.cpp \
                   ../../Classes/Wave.cpp \
                   ../../Classes/HelloWorldScene.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static


include $(BUILD_SHARED_LIBRARY)

$(call import-module,2d)
$(call import-module,audio/android)
$(call import-module,Box2D)
