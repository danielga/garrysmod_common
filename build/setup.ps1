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
ValidateVariableOrSetDefault "PREMAKE5_URL" -Default "https://github.com/premake/premake-core/releases/download/v5.0.0-alpha14/premake-5.0.0-alpha14-windows.zip"
ValidateVariableOrSetDefault "PREMAKE5_EXECUTABLE" -Default "$DEPENDENCIES/windows/premake-core/premake5.exe"
ValidateVariableOrSetDefault "PREMAKE5" -Default "premake5.exe"
ValidateVariableOrSetDefault "PROJECT_OS" -Default "windows"
ValidateVariableOrSetDefault "PROJECT_GENERATOR_VERSION" -Default "1"
ValidateVariableOrSetDefault "TARGET_OS" -Default "win32"
ValidateVariableOrSetDefault "TARGET_OS_64" -Default "win64"
ValidateVariableOrSetDefault "TARGET_ARCHITECTURE" -Default "x86"
ValidateVariableOrSetDefault "TARGET_ARCHITECTURE_64" -Default "x86_64"
ValidateVariableOrSetDefault "SOURCE_SDK_REPOSITORY" -Default "https://github.com/danielga/sourcesdk-minimal.git"
ValidateVariableOrSetDefault "SOURCE_SDK_BRANCH" -Default "master"

if (Test-Path env:SOURCE_SDK) {
	Set-Variable SOURCE_SDK -Value (Get-Item env:SOURCE_SDK).Value -Scope Global -ErrorAction Stop -Confirm:$false
}

if (Test-Path env:DISABLE_X86_64_BUILD) {
	Set-Variable DISABLE_X86_64_BUILD -Value (Get-Item env:DISABLE_X86_64_BUILD).Value -Scope Global -ErrorAction Stop -Confirm:$false
}

CreateDirectoryForcefully $DEPENDENCIES
