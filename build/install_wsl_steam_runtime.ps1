param ([string]$RuntimeName = "scout", [string]$InstallationDirectory = "C:/")

# Fail if any unset variable is used
Set-StrictMode -Version Latest

# Stop at first error
$ErrorActionPreference = "Stop"

function Use-Object([Object]$InputObject, [scriptblock]$ScriptBlock) {
    try {
        . $ScriptBlock
    } finally {
        if ($null -ne $InputObject -and $InputObject -is [System.IDisposable]) {
            $InputObject.Dispose()
        }
    }
}

function DownloadFileWithProgressBar([string]$URL, [string]$TargetPath) {
    Use-Object ($Client = [System.Net.Http.HttpClient]::new()) {
    Use-Object ($ResponseStream = $Client.GetStreamAsync($URL).GetAwaiter().GetResult()) {
    Use-Object ($TargetStream = [System.IO.FileStream]::new($TargetPath, [System.IO.FileMode]::Create)) {
        $Buffer = [byte[]]::new(1MB)
        $DownloadedBytes = 0

        while ($true) {
            $Count = $ResponseStream.Read($Buffer, 0, $Buffer.Length)
            if ($Count -eq 0) {
                break
            }

            $TargetStream.Write($Buffer, 0, $Count)
            $DownloadedBytes = $DownloadedBytes + $Count
            Write-Progress -Activity "Downloading file '$($URL.Split('/') | Select-Object -Last 1)'" -Status "Downloaded ${DownloadedBytes}B"
        }

        Write-Progress -Activity "Finished downloading file '$($URL.Split('/') | Select-Object -Last 1)'"
    }
    }
    }
}

function VerifyFileHash([string]$FilePath, [string]$FileHash) {
    return (Get-FileHash $FilePath -Algorithm SHA256).Hash.ToLower() -eq $FileHash.ToLower()
}

$AcceptableRuntimes = @("scout", "soldier", "heavy")
if (!$AcceptableRuntimes.Contains($RuntimeName)) {
    throw "Invalid runtime ""${RuntimeName}"". Acceptable values are ""scout"", ""soldier"" and ""heavy""."
}

$FullRuntimeName = "steam-runtime-${RuntimeName}"

if (!$InstallationDirectory.EndsWith('/') -and !$InstallationDirectory.EndsWith('\')) {
    $InstallationDirectory = $InstallationDirectory + '/'
}

$RuntimeVersion = (Invoke-WebRequest "https://repo.steampowered.com/steamrt-images-${RuntimeName}/snapshots/latest-steam-client-general-availability/VERSION.txt").Content.Trim()

$Checksum = ((Invoke-WebRequest "https://repo.steampowered.com/steamrt-images-${RuntimeName}/snapshots/latest-steam-client-general-availability/SHA256SUMS").Content | `
    Select-String -Pattern "(\w{64}) \*com\.valvesoftware\.SteamRuntime\.Sdk-amd64,i386-${RuntimeName}-sysroot\.tar\.gz").Matches.Groups[1].Value

$TempFilePath = [System.IO.Path]::GetTempPath() + "com.valvesoftware.SteamRuntime.Sdk-amd64,i386-${RuntimeName}-${RuntimeVersion}-sysroot.tar.gz"
if ((Test-Path -Path $TempFilePath -PathType Leaf) -and !(VerifyFileHash -FilePath $TempFilePath -FileHash $Checksum)) {
    Remove-Item -Path $TempFilePath
}

if (!(Test-Path -Path $TempFilePath -PathType Leaf)) {
    DownloadFileWithProgressBar `
        -URL "https://repo.steampowered.com/steamrt-images-${RuntimeName}/snapshots/latest-steam-client-general-availability/com.valvesoftware.SteamRuntime.Sdk-amd64,i386-${RuntimeName}-sysroot.tar.gz" `
        -TargetPath $TempFilePath
}

if (!(VerifyFileHash -FilePath $TempFilePath -FileHash $Checksum)) {
    Remove-Item -Path $TempFilePath
    throw "Checksum mismatch for downloaded file `"https://repo.steampowered.com/steamrt-images-${RuntimeName}/snapshots/latest-steam-client-general-availability/com.valvesoftware.SteamRuntime.Sdk-amd64,i386-${RuntimeName}-sysroot.tar.gz`"."
}

$WSLDistros = wsl -l
if (!$WSLDistros.Contains($FullRuntimeName)) {
    wsl --import "${FullRuntimeName}" "${InstallationDirectory}/${FullRuntimeName}" "${TempFilePath}"
}

Write-Output "Downloading and installing dependencies..."
wsl -d "${FullRuntimeName}" -e bash -c "cd ~ && wget -q `"https://github.com/danielga/garrysmod_common/releases/download/steam-runtime-bootstrap/steam-runtime-bootstrap.tar.xz`" && tar -xf steam-runtime-bootstrap.tar.xz && ./steam-runtime-bootstrap.sh"
