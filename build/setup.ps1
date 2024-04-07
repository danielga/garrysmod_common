# Fail if any unset variable is used
Set-StrictMode -Version Latest

Import-Module "$PSScriptRoot/functions.psm1"

ValidateVariableOrSetDefault "REPOSITORY_DIR" -Default (Split-Path (Split-Path $PSScriptRoot))
ValidateVariableOrSetDefault "MODULE_NAME" -Default (Split-Path $REPOSITORY_DIR -Leaf)
ValidateVariableOrSetDefault "DEPENDENCIES" -Default "$REPOSITORY_DIR/dependencies"
ValidateVariableOrSetDefault "GARRYSMOD_COMMON_REPOSITORY" -Default "https://github.com/danielga/garrysmod_common.git"
ValidateVariableOrSetDefault "GARRYSMOD_COMMON_BRANCH" -Default "master"
ValidateVariableOrSetDefault "GARRYSMOD_COMMON" -Default "$DEPENDENCIES/garrysmod_common"
ValidateVariableOrSetDefault "COMPILER_PLATFORM" -Default "vs2019"
ValidateVariableOrSetDefault "PREMAKE5_URL" -Default "https://github.com/premake/premake-core/releases/download/v5.0.0-alpha15/premake-5.0.0-alpha15-windows.zip"
ValidateVariableOrSetDefault "PREMAKE5" -Default "$DEPENDENCIES/windows/premake-core/premake5.exe"
ValidateVariableOrSetDefault "PROJECT_OS" -Default "windows"
ValidateVariableOrSetDefault "PROJECT_GENERATOR_VERSION" -Default "1"
ValidateVariableOrSetDefault "DISABLE_32BIT" -Default "0"
ValidateVariableOrSetDefault "DISABLE_64BIT" -Default "0"

CreateDirectoryForcefully $DEPENDENCIES
