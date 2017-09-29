unix {
    !exists($$PWD/../webrtc-audio-processing-src/webrtc/modules/audio_processing/.libs/libwebrtc_audio_processing.a) {
        system("cd $$PWD/../webrtc-audio-processing-src && ./autogen.sh && make clean && make")
    }

    !exists($$PWD/../webrtc-audio-processing-src/webrtc/modules/audio_processing/.libs/libwebrtc_audio_processing.a) {
        message("Cannot build 3rdparty/webrtc-audio-processing-src. Please fix.")
        error("Aborting configuration.")
    }
}

TEMPLATE =aux 
OBJECTS_DIR = ./
DESTDIR = ./

macx|win32 {
    message("No support for webrtc-audio-processing on macx or win32 yet.")
    error("Aborting configuration")
}

