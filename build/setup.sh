#!/bin/bash

# Exit if any command fails and if any unset variable is used
set -eu

DIR="$( cd "$( dirname "${BASH_SOURCE:-$0}" )" && pwd )"

. "${DIR}/functions.sh"

validate_variable_or_set_default "REPOSITORY_DIR" "$(cd "${DIR}/../.." && pwd)"
validate_variable_or_set_default "MODULE_NAME" "$(basename "$REPOSITORY_DIR")"
validate_variable_or_set_default "DEPENDENCIES" "$REPOSITORY_DIR/dependencies"
validate_variable_or_set_default "GARRYSMOD_COMMON_REPOSITORY" "https://github.com/danielga/garrysmod_common.git"
validate_variable_or_set_default "GARRYSMOD_COMMON_BRANCH" "master"
validate_variable_or_set_default "GARRYSMOD_COMMON" "$DEPENDENCIES/garrysmod_common"
validate_variable_or_set_default "COMPILER_PLATFORM" "gmake"
validate_variable_or_set_default "PREMAKE5_EXECUTABLE" "premake5"
validate_variable_or_set_default "PREMAKE5" "premake5"
validate_variable_or_set_default "PROJECT_GENERATOR_VERSION" "1"
validate_variable_or_set_default "SOURCE_SDK_REPOSITORY" "https://github.com/danielga/sourcesdk-minimal.git"
validate_variable_or_set_default "SOURCE_SDK_BRANCH" "master"

case "$(uname -s)" in
    Linux*)
        validate_variable_or_set_default "PREMAKE5_URL" "https://github.com/premake/premake-core/releases/download/v5.0.0-alpha14/premake-5.0.0-alpha14-linux.tar.gz"
        validate_variable_or_set_default "PROJECT_OS" "linux"
        ;;
    Darwin*)
        validate_variable_or_set_default "PREMAKE5_URL" "https://github.com/premake/premake-core/releases/download/v5.0.0-alpha14/premake-5.0.0-alpha14-macosx.tar.gz"
        validate_variable_or_set_default "PROJECT_OS" "osx"
        ;;
    *)
        echo "Unknown operating system"
        exit 1
        ;;
esac

validate_variable_or_set_default "TARGET_OS" "$PROJECT_OS"
validate_variable_or_set_default "TARGET_OS_64" "${PROJECT_OS}64"
validate_variable_or_set_default "TARGET_ARCHITECTURE" "x86"
validate_variable_or_set_default "TARGET_ARCHITECTURE_64" "x86_64"

create_directory_forcefully "$DEPENDENCIES"
