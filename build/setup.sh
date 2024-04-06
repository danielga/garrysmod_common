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
validate_variable_or_set_default "PROJECT_GENERATOR_VERSION" "1"

case "$(uname -s)" in
    Linux*)
        TARGET="linux"
        validate_variable_or_set_default "PREMAKE5_URL" "https://github.com/premake/premake-core/releases/download/v5.0.0-alpha15/premake-5.0.0-alpha15-linux.tar.gz"
        validate_variable_or_set_default "PROJECT_OS" "linux"
        ;;
    Darwin*)
        TARGET="osx"
        validate_variable_or_set_default "PREMAKE5_URL" "https://github.com/premake/premake-core/releases/download/v5.0.0-alpha15/premake-5.0.0-alpha15-macosx.tar.gz"
        validate_variable_or_set_default "PROJECT_OS" "macosx"
        validate_variable_or_set_default "MACOSX_SDK_URL" "https://github.com/phracker/MacOSX-SDKs/releases/download/10.15/MacOSX10.7.sdk.tar.xz"
        validate_variable_or_set_default "MACOSX_SDK_DIRECTORY" "$DEPENDENCIES/$PROJECT_OS/MacOSX10.7.sdk"
        validate_variable_or_set_default "SDKROOT" "$MACOSX_SDK_DIRECTORY"
        ;;
    *)
        echo "Unknown operating system"
        exit 1
        ;;
esac

validate_variable_or_set_default "PREMAKE5" "$DEPENDENCIES/$PROJECT_OS/premake-core/premake5"
validate_variable_or_set_default "DISABLE_32BIT" "0"
validate_variable_or_set_default "DISABLE_64BIT" "0"

create_directory_forcefully "$DEPENDENCIES"
