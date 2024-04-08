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

if value_is_falsy "$DISABLE_32BIT" && { [ "$PROJECT_GENERATOR_VERSION" -le "2" ] || [ "$(uname -s)" != "Darwin" ]; }; then
	pushd "$REPOSITORY_DIR/projects/$PROJECT_OS/$COMPILER_PLATFORM"
	echo "Building x86 module with ${JOBS} job(s)..."
	make -j "$JOBS" config=release_x86
	popd
fi

if value_is_falsy "$DISABLE_64BIT" && [ "$PROJECT_GENERATOR_VERSION" -ge "3" ]; then
	pushd "$REPOSITORY_DIR/projects/$PROJECT_OS/$COMPILER_PLATFORM"
	echo "Building x86-64 module with ${JOBS} job(s)..."
	make -j "$JOBS" config=release_x86_64
	popd
fi
