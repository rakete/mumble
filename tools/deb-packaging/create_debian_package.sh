#!/bin/bash

#
# since there is are no make install targets this script will create a debian package
#

if [ $# -ne 1 ]; then
    echo "Wrong number of arguments."
    echo "Help: $0 <mumble-home-dir>"
    
    exit 1
fi

set -e

MUMBLE_HOME="$1"

tmp=mumble_files #$(mktemp -d)

clean_up() {
    echo "cleaning up..."
    rm -rf "${tmp}" 2> /dev/null
}

mkdir -p "${tmp}/usr/bin"
mkdir -p "$tmp/usr/lib/mumble"
mkdir -p "$tmp/usr/share/icons/hicolor/scalable/apps"
mkdir -p "$tmp/usr/share/pixmaps"
mkdir -p "$tmp/usr/share/applications"
mkdir -p "$tmp/usr/share/services/"

echo "copying ..."
echo "release/mumble"
cp "${MUMBLE_HOME}/release/mumble" "$tmp/usr/bin/"
echo "release/libmumble.so*"
cp "${MUMBLE_HOME}/release/libmumble.so"* "$tmp/usr/lib/mumble/"
echo release/libcelt0.so.0.7.0
cp "${MUMBLE_HOME}/release/libcelt0.so.0.7.0" "$tmp/usr/lib/mumble/"
echo release/libcelt0.so.0.11.0
cp "${MUMBLE_HOME}/release/libcelt0.so.0.11.0" "$tmp/usr/lib/mumble/"
echo "release/plugins/lib*"
cp "${MUMBLE_HOME}/release/plugins/lib"*".so" "$tmp/usr/lib/mumble/"
echo icons/mumble.xpm
cp "${MUMBLE_HOME}/icons/mumble.xpm" "$tmp/usr/share/pixmaps/"
echo icons/mumble.svg
cp "${MUMBLE_HOME}/icons/mumble.svg" "$tmp/usr/share/icons/hicolor/scalable/apps/"
echo scripts/mumble.desktop
cp "${MUMBLE_HOME}/scripts/mumble.desktop" "$tmp/usr/share/applications/"
echo scripts/mumble.protocol
cp "${MUMBLE_HOME}/scripts/mumble.protocol" "$tmp/usr/share/services/"
echo /scripts/mumble-overlay
cp "${MUMBLE_HOME}/scripts/mumble-overlay" "$tmp/usr/bin/"
echo "DEBIAN"
cp -r DEBIAN "$tmp/"

dpkg-deb -b mumble_files . 

clean_up

echo "finished successfully."
