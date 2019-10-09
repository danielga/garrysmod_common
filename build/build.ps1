# Fail if any unset variable is used
Set-StrictMode -Version Latest

Import-Module "$PSScriptRoot/functions.psm1"

& "$PSScriptRoot/install_dependencies.ps1"

if ($PROJECT_GENERATOR_VERSION -eq 1) {
	Push-Location "$REPOSITORY_DIR/projects" -ErrorAction Stop
} else {
	Push-Location "$REPOSITORY_DIR" -ErrorAction Stop
}
Write-Output "Running premake5..."
Invoke-Call { & "$PREMAKE5" "$COMPILER_PLATFORM" } -ErrorAction Stop
Pop-Location

Push-Location "$REPOSITORY_DIR/projects/$PROJECT_OS/$COMPILER_PLATFORM" -ErrorAction Stop
Write-Output "Building module..."
Invoke-Call { & "$MSBuild" "$MODULE_NAME.sln" /p:Configuration=Release /p:Platform=Win32 /m } -ErrorAction Stop
Pop-Location

if ($PROJECT_GENERATOR_VERSION -eq 2 -and -not $DISABLE_X86_64_BUILD) {
	Push-Location "$REPOSITORY_DIR/projects/$PROJECT_OS/$COMPILER_PLATFORM" -ErrorAction Stop
	Write-Output "Building module..."
	Invoke-Call { & "$MSBuild" "$MODULE_NAME.sln" /p:Configuration=Release /p:Platform=x64 /m } -ErrorAction Stop
	Pop-Location
}
