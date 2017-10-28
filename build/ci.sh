#!/bin/bash

mkdir -p "$DEPENDENCIES"
cd "$DEPENDENCIES"

if [ ! -f "$GARRYSMOD_COMMON/premake5.lua" ]; then
	echo "garrysmod_common directory is empty, doing git clone of the remote repo";
	git clone --recursive https://github.com/danielga/garrysmod_common.git;
else
	echo "garrysmod_common directory is good, pulling any latest changes";
	cd "$GARRYSMOD_COMMON";
	git pull;
	git submodule update --init --recursive;
fi

cd "$DEPENDENCIES"

if [ ! -f "$SOURCE_SDK/LICENSE" ]; then
	echo "sourcesdk-minimal directory is empty, doing git clone of the remote repo";
	git clone https://github.com/danielga/sourcesdk-minimal.git;
else
	echo "sourcesdk-minimal directory is good, pulling any latest changes";
	cd "$SOURCE_SDK";
	git pull;
fi

cd "$DEPENDENCIES"

if [ ! -f "$DEPENDENCIES/premake-core/premake5.lua" ]; then
	echo "premake-core directory is empty, doing git clone of the remote repo";
	git clone --recursive https://github.com/premake/premake-core.git;
else
	echo "premake-core directory is good, pulling any latest changes";
	cd "$DEPENDENCIES/premake-core";
	git pull;
	git submodule update --init --recursive;
fi

mkdir -p "$DEPENDENCIES/$PROJECT_OS"

if [ ! -f "$PREMAKE5" ]; then
	echo "premake-core directory is empty, bootstrapping";
	cd "$DEPENDENCIES/premake-core";
	make -f Bootstrap.mak "$TARGET_OS";
	cd "$DEPENDENCIES";
	mkdir -p "$DEPENDENCIES/$PROJECT_OS/premake-core";
	cp "$DEPENDENCIES/premake-core/bin/release/premake5" "$DEPENDENCIES/$PROJECT_OS/premake-core";
fi

cd "$REPOSITORY_DIR/projects"
"$PREMAKE5" gmake
cd "$REPOSITORY_DIR/projects/$PROJECT_OS/gmake"

make

cp "$REPOSITORY_DIR/projects/$PROJECT_OS/gmake/release/"gm*_${MODULE_NAME}_$TARGET_OS.dll "$REPOSITORY_DIR"
cd "$REPOSITORY_DIR"
