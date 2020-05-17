#!/bin/bash

# Exit if any command fails and if any unset variable is used
set -eu

DIR="$( cd "$( dirname "${BASH_SOURCE:-$0}" )" && pwd )"

. "${DIR}/functions.sh"
. "${DIR}/setup.sh"

# Checkout with the correct line endings on plain text files, depending on the host OS
git config --global core.autocrlf true >/dev/null 2>/dev/null

update_local_git_repository "$GARRYSMOD_COMMON" "$GARRYSMOD_COMMON_REPOSITORY" "$GARRYSMOD_COMMON_BRANCH" >/dev/null

if [ ! -z "${SOURCE_SDK+x}" ]; then
	update_local_git_repository "$SOURCE_SDK" "$SOURCE_SDK_REPOSITORY" "$SOURCE_SDK_BRANCH" >/dev/null
fi

if [ ! -f "$PREMAKE5" ]; then
	echo "Installing premake5..."
	PREMAKE_DIRECTORY="$DEPENDENCIES/$PROJECT_OS/premake-core"
	PREMAKE_TAR_PATH="$PREMAKE_DIRECTORY/premake-core.tar.gz"
	create_directory_forcefully "$PREMAKE_DIRECTORY"
	curl -s -L "$PREMAKE5_URL" -o "$PREMAKE_TAR_PATH"
	tar -xf "$PREMAKE_TAR_PATH" -C "$PREMAKE_DIRECTORY"
	rm -f "$PREMAKE_TAR_PATH"
fi

if [ "$(uname -s)" = "Darwin" ] && [ ! -d "$MACOSX_SDK_DIRECTORY" ]; then
	echo "Installing MacOSX SDK..."
	MACOSX_SDK_DOWNLOAD_DIR="$DEPENDENCIES/$PROJECT_OS"
	MACOSX_SDK_TAR_PATH="$MACOSX_SDK_DOWNLOAD_DIR/macosx_sdk.tar.xz"
	curl -s -L "$MACOSX_SDK_URL" -o "$MACOSX_SDK_TAR_PATH"
	tar -xf "$MACOSX_SDK_TAR_PATH" -C "$MACOSX_SDK_DOWNLOAD_DIR"
	rm -f "$MACOSX_SDK_TAR_PATH"
fi
