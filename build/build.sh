#!/bin/bash

# Exit if any command fails and if any unset variable is used
set -eu

DIR="$( cd "$( dirname "${BASH_SOURCE:-$0}" )" && pwd )"

. "${DIR}/install_dependencies.sh"

JOBS=$(getconf _NPROCESSORS_ONLN)

pushd "$REPOSITORY_DIR/projects"
echo "Running premake5..."
"$PREMAKE5" "$COMPILER_PLATFORM"
popd

pushd "$REPOSITORY_DIR/projects/$PROJECT_OS/$COMPILER_PLATFORM"
echo "Building module with ${JOBS} job(s)..."
make -j "$JOBS"
popd
