mkdir "$env:DEPENDENCIES" -ErrorAction SilentlyContinue
cd "$env:DEPENDENCIES"

if( !( ( Get-Item "$env:GARRYSMOD_COMMON/premake5.lua" -ErrorAction SilentlyContinue ) -is [System.IO.FileInfo] ) )
{
	echo "garrysmod_common directory is empty, doing git clone of the remote repo"
	git clone --recursive https://github.com/danielga/garrysmod_common.git
}
else
{
	echo "garrysmod_common directory is good, pulling any latest changes"
	cd "$env:GARRYSMOD_COMMON"
	git pull
	git submodule update --init --recursive
}

cd "$env:DEPENDENCIES"

if( !( ( Get-Item "$env:SOURCE_SDK/LICENSE" -ErrorAction SilentlyContinue ) -is [System.IO.FileInfo] ) )
{
	echo "sourcesdk-minimal directory is empty, doing git clone of the remote repo"
	git clone https://github.com/danielga/sourcesdk-minimal.git
}
else
{
	echo "sourcesdk-minimal directory is good, pulling any latest changes"
	cd "$env:SOURCE_SDK"
	git pull
}

cd "$env:DEPENDENCIES"

if( !( ( Get-Item "$env:DEPENDENCIES/premake-core/premake5.lua" -ErrorAction SilentlyContinue ) -is [System.IO.FileInfo] ) )
{
	echo "premake-core directory is empty, doing git clone of the remote repo"
	git clone --recursive https://github.com/premake/premake-core.git
}
else
{
	echo "premake-core directory is good, pulling any latest changes"
	cd "$env:DEPENDENCIES/premake-core"
	git pull
	git submodule update --init --recursive
}

mkdir "$env:DEPENDENCIES/windows" -ErrorAction SilentlyContinue

pushd "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools"
cmd /c "VsDevCmd.bat&set" |
foreach {
	if( $_ -match "=" )
	{
		$v = $_.split( "=" )
		Set-Item -force -path "ENV:\$($v[0])" -value "$($v[1])"
	}
}
popd
Write-Host "`nVisual Studio 2017 command prompt variables set." -ForegroundColor Yellow

if( !( ( Get-Item "$env:PREMAKE5" -ErrorAction SilentlyContinue ) -is [System.IO.FileInfo] ) )
{
	echo "premake-core directory is empty, bootstrapping"
	cd "$env:DEPENDENCIES/premake-core"
	nmake -f Bootstrap.mak windows
	cd "$env:DEPENDENCIES"
	mkdir "$env:DEPENDENCIES/windows/premake-core" -ErrorAction SilentlyContinue
	cp "$env:DEPENDENCIES/premake-core/bin/release/premake5.exe" "$env:DEPENDENCIES/windows/premake-core"
}

cd "$env:CI_PROJECT_DIR/projects"
& "$env:PREMAKE5" vs2017
cd "$env:CI_PROJECT_DIR/projects/windows/vs2017"

msbuild serversecure.sln /p:Configuration=Release

cp "$env:CI_PROJECT_DIR/projects/windows/vs2017/release/gm*_${env:MODULE_NAME}_win32.dll" "$env:CI_PROJECT_DIR"
cd "$env:CI_PROJECT_DIR"
