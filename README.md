Webrtc Mumble fork with echo cancelation
===================================================

## Building for Windows

Only cross compilation with mingw tested and supported.

Make sure you have the latest webrtc-audio-processing sources from either
https://github.com/rakete/webrtc-audio-processing
or
https://github.com/hennakin/webrtc-audio-processing
in 3rdparty/webrtc-audio-processing-src submodule path.

# Installing the required MXE packages

From https://wiki.mumble.info/wiki/BuildingWindows#Build_using_MXE_on_Debian_and_derivates

    echo "deb http://pkg.mxe.cc/repos/apt/debian jessie main" > /etc/apt/sources.list.d/mxeapt.list
    apt-key adv --keyserver keyserver.ubuntu.com --recv-keys D43A795B73B16ABE9643FE1AFD8FFF16DB45C6AB
    apt-get update

Install the required packages

    apt-get install \
        mxe-${ARCH}-w64-mingw32.static-qtbase \
        mxe-${ARCH}-w64-mingw32.static-qtsvg \
        mxe-${ARCH}-w64-mingw32.static-qttools \
        mxe-${ARCH}-w64-mingw32.static-qttranslations \
        mxe-${ARCH}-w64-mingw32.static-boost \
        mxe-${ARCH}-w64-mingw32.static-protobuf \
        mxe-${ARCH}-w64-mingw32.static-sqlite \
        mxe-${ARCH}-w64-mingw32.static-flac \
        mxe-${ARCH}-w64-mingw32.static-ogg \
        mxe-${ARCH}-w64-mingw32.static-vorbis \
        mxe-${ARCH}-w64-mingw32.static-libsndfile

# Building

Clone the repository from either rakete or hennakin

    git clone https://github.com/rakete/mumble.git # or https://github.com/hennakin/mumble.git
    cd mumble

Export environment variable to tell QMake where MXE's protobuf compiler is and add MXE's directory to PATH

    export MUMBLE_PROTOC=/usr/lib/mxe/usr/x86_64-unknown-linux-gnu/bin/protoc
    export PATH=$PATH:/usr/lib/mxe/usr/bin

Run QMake to process the project(s) files and run make to start the build

    x86_64-w64-mingw32.static-qmake-qt5 -recursive CONFIG+="release g15-emulator no-overlay no-bonjour no-elevation no-ice"
    make

Afterwards the windows build can be found in the mumble/release directory.
