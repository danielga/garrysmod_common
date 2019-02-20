#!/bin/bash

# Exit if any command fails and if any unset variable is used
set -eu

DIR="$( cd "$( dirname "${BASH_SOURCE:-$0}" )" && pwd )"

. "${DIR}/functions.sh"

validate_variable_or_set_default "MODULE_NAME"
validate_variable_or_set_default "REPOSITORY_DIR"
validate_variable_or_set_default "DEPENDENCIES" "$REPOSITORY_DIR/dependencies"
validate_variable_or_set_default "GARRYSMOD_COMMON_REPOSITORY" "https://github.com/danielga/garrysmod_common.git"
validate_variable_or_set_default "GARRYSMOD_COMMON" "$DEPENDENCIES/garrysmod_common"
validate_variable_or_set_default "TARGET_OS"
validate_variable_or_set_default "COMPILER_PLATFORM" "gmake"
case "$(uname -s)" in
    Linux*)  validate_variable_or_set_default "PREMAKE5_URL" "https://github.com/premake/premake-core/releases/download/v5.0.0-alpha13/premake-5.0.0-alpha13-linux.tar.gz";;
    Darwin*) validate_variable_or_set_default "PREMAKE5_URL" "https://github.com/premake/premake-core/releases/download/v5.0.0-alpha13/premake-5.0.0-alpha13-macosx.tar.gz";;
    *)       echo "Unknown operating system"; exit 1;;
esac
validate_variable_or_set_default "PREMAKE5_EXECUTABLE" "premake5"
validate_variable_or_set_default "PREMAKE5" "premake5"
validate_variable_or_set_default "PROJECT_OS"

create_directory_forcefully "$DEPENDENCIES"
