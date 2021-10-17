#!/bin/bash

set -euxo pipefail

# Don't use with versions prior to GCC 9!
version="${1:-12.3.0}"
major_version=`echo "$version" | cut -d. -f1`

sudo apt-get -y install wget flex texinfo build-essential m4 bison

if [ ! -f "gcc-${version}.tar.xz" ]; then
	wget "http://ftp.gnu.org/gnu/gcc/gcc-${version}/gcc-${version}.tar.xz"
fi

if [ ! -d "gcc-${version}" ]; then
	tar -xf "gcc-${version}.tar.xz"
fi

pushd "gcc-${version}"

contrib/download_prerequisites

if [ "$major_version" -ge "11" ]; then
	# Patch dwarf_version to default to 4 instead of 5, since steam-runtime's ld doesn't support DWARF 5.
	# This build of GCC will still support -gdwarf-5, but that's up to the user.
	patch -u -p0 -N << EOF
--- gcc/common.opt	2021-09-29 17:10:59.793270902 +0100
+++ gcc/common.opt	2021-09-29 16:46:44.733263653 +0100
@@ -3175,7 +3175,7 @@
 Generate debug information in default version of DWARF format.
 
 gdwarf-
-Common Driver Joined UInteger Var(dwarf_version) Init(5) Negative(gstabs)
+Common Driver Joined UInteger Var(dwarf_version) Init(4) Negative(gstabs)
 Generate debug information in DWARF v2 (or later) format.
 
 gdwarf32
EOF
fi

rm -rf build/
mkdir build
pushd build

export CC=gcc-9
export CXX=g++-9
export AR=gcc-ar-9
export NM=gcc-nm-9
export RANLIB=gcc-ranlib-9

core_count=`getconf _NPROCESSORS_ONLN`
double_core_count="$(( core_count * 2 ))"

../configure -v --with-pkgversion="SteamRT ${version}-1+steamrt1.3+srt1" \
	--enable-languages=c,c++,fortran,lto,objc,obj-c++ \
	--prefix="/usr/lib/gcc-${major_version}" --with-gcc-major-version-only \
	--program-prefix= --program-suffix="-${major_version}" --enable-shared \
	--enable-linker-build-id --with-pic --enable-nls --enable-clocale=gnu \
	--enable-libstdcxx-debug --enable-libstdcxx-time=yes \
	--with-default-libstdcxx-abi=gcc4-compatible --disable-libstdcxx-dual-abi \
	--enable-gnu-unique-object --disable-vtable-verify --enable-plugin --enable-default-pie \
	--with-system-zlib --enable-multiarch --disable-werror --with-arch-32=i686 \
	--with-abi=m64 --with-multilib-list=m32,m64 --enable-multilib --with-tune=generic \
	--enable-checking=release --build=x86_64-linux-gnu --host=x86_64-linux-gnu \
	--target=x86_64-linux-gnu
make -j "$double_core_count"
sudo make install-strip

popd

popd

sudo mkdir -p "/usr/lib/gcc-${major_version}/lib32/backup"
sudo mv "/usr/lib/gcc-${major_version}/lib32/"*.so* "/usr/lib/gcc-${major_version}/lib32/backup"

sudo mkdir -p "/usr/lib/gcc-${major_version}/lib32/debug/backup"
sudo mv "/usr/lib/gcc-${major_version}/lib32/debug/"*.so* "/usr/lib/gcc-${major_version}/lib32/debug/backup"

sudo mkdir -p "/usr/lib/gcc-${major_version}/lib64/backup"
sudo mv "/usr/lib/gcc-${major_version}/lib64/"*.so* "/usr/lib/gcc-${major_version}/lib64/backup"

sudo mkdir -p "/usr/lib/gcc-${major_version}/lib64/debug/backup"
sudo mv "/usr/lib/gcc-${major_version}/lib64/debug/"*.so* "/usr/lib/gcc-${major_version}/lib64/debug/backup"

tar -cvJf "gcc-${version}.tar.xz" "/usr/lib/gcc-${major_version}"
