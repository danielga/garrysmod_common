# Fail if any unset variable is used
Set-StrictMode -Version Latest

Import-Module "$PSScriptRoot/functions.psm1"

& "$PSScriptRoot/setup.ps1"

Add-Type -AssemblyName System.IO.Compression.FileSystem

# Checkout with the correct line endings on plain text files, depending on the host OS
git config --global core.autocrlf true >$null 2>$null

UpdateLocalGitRepository $GARRYSMOD_COMMON -Repository $GARRYSMOD_COMMON_REPOSITORY -Branch "improve-build-scripts"

if (Test-Path variable:SOURCE_SDK) {
	UpdateLocalGitRepository $SOURCE_SDK -Repository "https://github.com/danielga/sourcesdk-minimal.git"
}

if (!((Get-Item $PREMAKE5 -ErrorAction SilentlyContinue) -is [System.IO.FileInfo])) {
	Write-Output "Installing premake5..."
	$PremakeDirectory = "$DEPENDENCIES/$PROJECT_OS/premake-core"
	$PremakeZipPath = "$PremakeDirectory/premake-core.zip"
	CreateDirectoryForcefully $PremakeDirectory
	(New-Object System.Net.WebClient).DownloadFile("$PREMAKE5_URL", $PremakeZipPath)
	[System.IO.Compression.ZipFile]::ExtractToDirectory($PremakeZipPath, $PremakeDirectory)
	Remove-Item $PremakeZipPath -Force -ErrorAction Stop -Confirm:$false
}
