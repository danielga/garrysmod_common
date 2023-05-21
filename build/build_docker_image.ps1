# Fail if any unset variable is used
Set-StrictMode -Version Latest

# Stop at first error
$ErrorActionPreference = "Stop"

Import-Module "$PSScriptRoot/functions.psm1"

$DockerRepository = "danielga/steamrt-scout"

$CurrentVersion = (Invoke-WebRequest "https://registry.hub.docker.com/v2/repositories/${DockerRepository}/tags?page_size=100").Content | jq -r '[."results"[]["name"] | select(startswith("v")) | .[1:]] | sort | last // 0'
$NextVersion = [int]$CurrentVersion + 1

Invoke-Call { docker pull "${DockerRepository}:latest" }
Invoke-Call { docker build -t "${DockerRepository}:latest" -t "${DockerRepository}:v${NextVersion}" . } -ErrorAction Stop
Invoke-Call { docker login } -ErrorAction Stop
Invoke-Call { docker push "${DockerRepository}:v${NextVersion}" } -ErrorAction Stop
Invoke-Call { docker push "${DockerRepository}:latest" } -ErrorAction Stop
