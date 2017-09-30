unix {
    !exists($$PWD/../webrtc-audio-processing-src/webrtc/modules/audio_processing/.libs/libwebrtc_audio_processing.a) {
        system("cd $$PWD/../webrtc-audio-processing-src && ./autogen.sh && make clean && make")
    }

    !exists($$PWD/../webrtc-audio-processing-src/webrtc/modules/audio_processing/.libs/libwebrtc_audio_processing.a) {
        message("Cannot build 3rdparty/webrtc-audio-processing-src. Please fix.")
        error("Aborting configuration.")
    }
}

win32 {
    !exists($$PWD/../webrtc-audio-processing-src/webrtc/modules/audio_processing/.libs/libwebrtc_audio_processing.a) {
        system("cd $$PWD/../webrtc-audio-processing-src && ./autogen.sh && \
                export CC=x86_64-w64-mingw32.static-gcc && \
                export CXX=x86_64-w64-mingw32.static-g++ && \
                export CPP=x86_64-w64-mingw32.static-cpp && \
                export RANLIB=x86_64-w64-mingw32.static-ranlib && \
                ./configure --host=x86_64-w64-mingw32 && \
                make clean && make")
    }

    !exists($$PWD/../webrtc-audio-processing-src/webrtc/modules/audio_processing/.libs/libwebrtc_audio_processing.a) {
        message("Cannot build 3rdparty/webrtc-audio-processing-src. Please fix.")
        error("Aborting configuration.")
    }
}

TEMPLATE =aux 
OBJECTS_DIR = ./
DESTDIR = ./

macx {
    message("No support for webrtc-audio-processing on macx or win32 yet.")
    error("Aborting configuration")
}

