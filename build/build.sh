#!/bin/bash

# Exit if any command fails and if any unset variable is used
set -eu

DIR="$( cd "$( dirname "${BASH_SOURCE:-$0}" )" && pwd )"

. "${DIR}/install_dependencies.sh"

JOBS=$(getconf _NPROCESSORS_ONLN)

if [ "$PROJECT_GENERATOR_VERSION" = "1" ]; then
	pushd "$REPOSITORY_DIR/projects"
else
	pushd "$REPOSITORY_DIR"
fi
echo "Running premake5..."
"$PREMAKE5" "$COMPILER_PLATFORM"
popd

BUILD_32BIT=false
if [ "$PROJECT_GENERATOR_VERSION" -le "2" ] || [ ! "$(uname -s)" = "Darwin" ]; then
	BUILD_32BIT=true
fi
if [ "${DISABLE_32BIT#false}" = "true" ]; then
	BUILD_32BIT=false
fi

BUILD_64BIT=false
if [ "$PROJECT_GENERATOR_VERSION" -ge "3" ]; then
	BUILD_64BIT=true
fi
if [ "${DISABLE_64BIT#false}" = "true" ]; then
	BUILD_64BIT=false
fi

if [ "${BUILD_32BIT}" = "true" ]; then
	pushd "$REPOSITORY_DIR/projects/$PROJECT_OS/$COMPILER_PLATFORM"
	echo "Building module with ${JOBS} job(s)..."
	make -j "$JOBS" config=release_x86
	popd
fi

if [ "${BUILD_64BIT}" = "true" ]; then
	pushd "$REPOSITORY_DIR/projects/$PROJECT_OS/$COMPILER_PLATFORM"
	echo "Building module with ${JOBS} job(s)..."
	make -j "$JOBS" config=release_x86_64
	popd
fi
