#!/bin/bash

# Exit if any command fails
set -e

if [ ! -z ${SOURCE_SDK+x} ]; then
	if [ ! -f "$SOURCE_SDK/LICENSE" ]; then
		echo "sourcesdk-minimal local repository is empty, doing git clone of the remote repo"
		git clone --depth 1 --shallow-submodules "https://github.com/danielga/sourcesdk-minimal.git" "$SOURCE_SDK"
	else
		LOCAL=$(git -C "$SOURCE_SDK" rev-parse @)
		REMOTE=$(git -C "$SOURCE_SDK" rev-parse @{u})
		BASE=$(git -C "$SOURCE_SDK" merge-base @ @{u})

		if [ $LOCAL = $REMOTE ]; then
			echo "sourcesdk-minimal local repository is good and needs no updates"
		elif [ $LOCAL = $BASE ]; then
			echo "sourcesdk-minimal local repository is good but needs updates"
			git -C "$SOURCE_SDK" pull
		else
			echo "sourcesdk-minimal local repository is bad, cloning again"
			rm -rf "$SOURCE_SDK"
			git clone --depth 1 --shallow-submodules "https://github.com/danielga/sourcesdk-minimal.git" "$SOURCE_SDK"
		fi
	fi
fi

BUILD_PREMAKE5=false;
if [ ! -f "$DEPENDENCIES/premake-core/premake5.lua" ]; then
	echo "premake-core local repository is empty, doing git clone of the remote repo"
	git clone --depth 1 --shallow-submodules "https://github.com/premake/premake-core.git" "$DEPENDENCIES/premake-core"
	BUILD_PREMAKE5=true
else
	LOCAL=$(git -C "$DEPENDENCIES/premake-core" rev-parse @)
	REMOTE=$(git -C "$DEPENDENCIES/premake-core" rev-parse @{u})
	BASE=$(git -C "$DEPENDENCIES/premake-core" merge-base @ @{u})

	if [ $LOCAL = $REMOTE ]; then
		echo "premake-core local repository is good and needs no updates"
	elif [ $LOCAL = $BASE ]; then
		echo "premake-core local repository is good but needs updates"
		git -C "$DEPENDENCIES/premake-core" pull
		BUILD_PREMAKE5=true
	else
		echo "premake-core local repository is bad, cloning again"
		rm -rf "$DEPENDENCIES/premake-core"
		git clone --depth 1 --shallow-submodules "https://github.com/premake/premake-core.git" "$DEPENDENCIES/premake-core"
		BUILD_PREMAKE5=true
	fi
fi

mkdir -p "$DEPENDENCIES/$PROJECT_OS"

JOBS=$(getconf _NPROCESSORS_ONLN)

if [ "$BUILD_PREMAKE5" = true ]; then
	echo "premake-core needs building, bootstrapping"
	pushd "$DEPENDENCIES/premake-core"
	make -j "$JOBS" -f Bootstrap.mak "$TARGET_OS"
	popd
	mkdir -p "$DEPENDENCIES/$PROJECT_OS/premake-core"
	cp "$DEPENDENCIES/premake-core/bin/release/$PREMAKE5_EXECUTABLE" "$PREMAKE5"
fi

pushd "$REPOSITORY_DIR/projects"
"$PREMAKE5" "$COMPILER_PLATFORM"
popd

pushd "$REPOSITORY_DIR/projects/$PROJECT_OS/$COMPILER_PLATFORM"
make -j "$JOBS"
popd
