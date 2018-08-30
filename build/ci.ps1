# Exit if any command fails
$ErrorActionPreference = "Stop"

if( "$env:SOURCE_SDK" ) {
	if( !( ( Get-Item "$env:SOURCE_SDK/LICENSE" -ErrorAction SilentlyContinue ) -is [System.IO.FileInfo] ) ) {
		Write-Output "sourcesdk-minimal local repository is empty, doing git clone of the remote repo"
		git clone --depth 1 --shallow-submodules "https://github.com/danielga/sourcesdk-minimal.git" "$env:SOURCE_SDK"
	} else {
		$LOCAL = git -C "$env:SOURCE_SDK" rev-parse "@"
		$REMOTE = git -C "$env:SOURCE_SDK" rev-parse "@{u}"
		$BASE = git -C "$env:SOURCE_SDK" merge-base "@" "@{u}"

		if( $LOCAL -eq $REMOTE ) {
			Write-Output "sourcesdk-minimal local repository is good and needs no updates"
		} elseif( $LOCAL -eq $BASE ) {
			Write-Output "sourcesdk-minimal local repository is good but needs updates"
			git -C "$env:SOURCE_SDK" pull
		} else {
			Write-Output "sourcesdk-minimal local repository is bad, cloning again"
			Remove-Item -Force -Recurse "$env:SOURCE_SDK"
			git clone --depth 1 --shallow-submodules "https://github.com/danielga/sourcesdk-minimal.git" "$env:SOURCE_SDK"
		}
	}
}

$BUILD_PREMAKE5 = $FALSE
if( !( ( Get-Item "$env:DEPENDENCIES/premake-core/premake5.lua" -ErrorAction SilentlyContinue ) -is [System.IO.FileInfo] ) ) {
	Write-Output "premake-core local repository is empty, doing git clone of the remote repo"
	git clone --depth 1 --shallow-submodules "https://github.com/premake/premake-core.git" "$env:DEPENDENCIES/premake-core"
	$BUILD_PREMAKE5 = $TRUE
} else {
	$LOCAL = git -C "$env:DEPENDENCIES/premake-core" rev-parse "@"
	$REMOTE = git -C "$env:DEPENDENCIES/premake-core" rev-parse "@{u}"
	$BASE = git -C "$env:DEPENDENCIES/premake-core" merge-base "@" "@{u}"

	if( $LOCAL -eq $REMOTE ) {
		Write-Output "premake-core local repository is good and needs no updates"
	} elseif( $LOCAL -eq $BASE ) {
		Write-Output "premake-core local repository is good but needs updates"
		git -C "$env:DEPENDENCIES/premake-core" pull
		$BUILD_PREMAKE5 = $TRUE
	} else {
		Write-Output "premake-core local repository is bad, cloning again"
		Remove-Item -Force -Recurse "$env:DEPENDENCIES/premake-core"
		git clone --depth 1 --shallow-submodules "https://github.com/premake/premake-core.git" "$env:DEPENDENCIES/premake-core"
		$BUILD_PREMAKE5 = $TRUE
	}
}

New-Item "$env:DEPENDENCIES/$env:PROJECT_OS" -ItemType Directory -ErrorAction SilentlyContinue

if( $BUILD_PREMAKE5 ) {
	Write-Output "premake-core needs building, bootstrapping"
	Push-Location "$env:DEPENDENCIES/premake-core"
	$env:CL = "/MP"
	nmake -f Bootstrap.mak "$env:PROJECT_OS"
	Pop-Location
	New-Item "$env:DEPENDENCIES/$env:PROJECT_OS/premake-core" -ItemType Directory -ErrorAction SilentlyContinue
	Copy-Item "$env:DEPENDENCIES/premake-core/bin/release/$env:PREMAKE5_EXECUTABLE" "$env:PREMAKE5"
}

Push-Location "$env:REPOSITORY_DIR/projects"
& "$env:PREMAKE5" "$env:COMPILER_PLATFORM"
Pop-Location

Push-Location "$env:REPOSITORY_DIR/projects/$env:PROJECT_OS/$env:COMPILER_PLATFORM"
msbuild "$env:MODULE_NAME.sln" /p:Configuration=Release /p:Platform=Win32 /m
Pop-Location
