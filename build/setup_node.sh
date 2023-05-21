#!/bin/bash

set -euo pipefail

NODE_VERSION='14.21.3'
curl -L https://github.com/nodejs/node/archive/refs/tags/v${NODE_VERSION}.zip -o node.zip
unzip -a node.zip
rm -f node.zip
pushd "node-${NODE_VERSION}"
patch -s << EOM
--- node.gypi
+++ node.gypi
@@ -308,7 +308,7 @@
       'ldflags': [ '-Wl,-M,/usr/lib/ld/map.noexstk' ],
     }],
     [ 'OS=="linux"', {
-      'libraries!': [
+      'libraries': [
         '-lrt'
       ],
     }],
EOM
CC=gcc-9 CXX=g++-9 AR=gcc-ar-9 NM=gcc-nm-9 RANLIB=gcc-ranlib-9 ./configure
make -j12
make install
popd
rm -rf "node-${NODE_VERSION}"
