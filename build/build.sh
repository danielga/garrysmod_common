#!/bin/bash

# Exit if any command fails and if any unset variable is used
set -eu

DIR="$( cd "$( dirname "${BASH_SOURCE:-$0}" )" && pwd )"

. "${DIR}/install_dependencies.sh"

JOBS=$(getconf _NPROCESSORS_ONLN)

if [ "$(uname -s)" = "Darwin" ]; then
	export SDKROOT="$MACOSX_SDK_DIRECTORY"
fi

if [ "$PROJECT_GENERATOR_VERSION" = "1" ]; then
	pushd "$REPOSITORY_DIR/projects"
else
	pushd "$REPOSITORY_DIR"
fi
echo "Running premake5..."
"$PREMAKE5" "$COMPILER_PLATFORM"
popd

pushd "$REPOSITORY_DIR/projects/$PROJECT_OS/$COMPILER_PLATFORM"
echo "Building module with ${JOBS} job(s)..."
make -j "$JOBS" config=release_x86
popd

if [ "$PROJECT_GENERATOR_VERSION" = "2" ] && [ -z "${DISABLE_X86_64_BUILD+x}" ]; then
	pushd "$REPOSITORY_DIR/projects/$PROJECT_OS/$COMPILER_PLATFORM"
	echo "Building module with ${JOBS} job(s)..."
	make -j "$JOBS" config=release_x86_64
	popd
fi
