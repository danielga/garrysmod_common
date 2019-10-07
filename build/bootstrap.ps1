# Fail if any unset variable is used
Set-StrictMode -Version Latest

# Checkout with the correct line endings on plain text files, depending on the host OS
git config --global core.autocrlf true >$null 2>$null

function ValidateVariableOrSetDefault([string]$Name, $Default = $null) {
	$value = $Default
	if (Test-Path env:$Name) {
		$value = (Get-Item env:$Name).Value
	}

	if (Test-Path variable:$Name) {
		$value = Get-Variable -ValueOnly $Name
	}

	if (($null -eq $value) -and ($null -eq $Default)) {
		throw "'$Name' was not set"
	}

	Set-Variable $Name -Value $value -Scope Global -ErrorAction Stop -Confirm:$false
}

function Invoke-Call([scriptblock]$ScriptBlock, [string]$ErrorAction = $ErrorActionPreference) {
	& @ScriptBlock
	if (($LastExitCode -ne 0) -and ($ErrorAction -eq "Stop")) {
		exit $LastExitCode
	}
}

function UpdateLocalGitRepository([string]$Repository, [string]$Directory, [string]$Branch = "master") {
	$updated = $false

	if ((Get-Item $Directory -ErrorAction SilentlyContinue) -is [System.IO.DirectoryInfo]) {
		git -C "$Directory" rev-parse --is-inside-work-tree >$null 2>$null
		$shouldclone = ($LastExitCode -ne 0)
	} else {
		$shouldclone = $true
	}

	if ($shouldclone) {
		Write-Output "Cloning repository `"${Repository}`" into `"${Directory}`"..."
		Remove-Item -Recurse -Force -ErrorAction SilentlyContinue $Directory
		Invoke-Call { git clone --quiet --depth 1 --shallow-submodules --recursive --branch "$Branch" "$Repository" "$Directory" } -ErrorAction Stop
		$updated = $true
	} else {
		Push-Location $Directory -ErrorAction Stop

		Write-Output "Fetching all branches from remote in `"${Directory}`"..."
		Invoke-Call { git fetch --quiet --all --prune --prune-tags } -ErrorAction Stop

		$curbranch = git symbolic-ref --quiet --short HEAD
		if ($LastExitCode -ne 0) {
			exit $LastExitCode
		}

		if ($curbranch -ne $Branch) {
			Write-Output "Checking out branch `"${Branch}`" in `"${Directory}`"..."
			Invoke-Call { git checkout --quiet --force "$Branch" } -ErrorAction Stop
			$updated = $true
		}

		$local = git rev-parse "@"
		if ($LastExitCode -ne 0) {
			exit $LastExitCode
		}

		$remote = git rev-parse "@{u}"
		if ($LastExitCode -ne 0) {
			exit $LastExitCode
		}

		$base = git merge-base "@" "@{u}"
		if ($LastExitCode -ne 0) {
			exit $LastExitCode
		}

		if ($local -eq $base) {
			Write-Output "Branch `"${Branch}`" in `"${Directory}`" needs updating..."
			$updated = $true
		} elseif ($local -ne $remote) {
			Write-Output "Hard resetting branch `"${Branch}`" in `"${Directory}`"..."
			Invoke-Call { git reset --quiet --hard "origin/${Branch}" } -ErrorAction Stop
			Invoke-Call { git clean --quiet --force -dx } -ErrorAction Stop
			$updated = $true
		}

		if ($updated) {
			Write-Output "Updating branch `"${Branch}`" in `"${Directory}`"..."
			Invoke-Call { git pull --quiet --recurse-submodules } -ErrorAction Stop
		}

		Pop-Location
	}

	return $updated
}

ValidateVariableOrSetDefault "GARRYSMOD_COMMON_REPOSITORY" -Default "https://github.com/danielga/garrysmod_common.git"
ValidateVariableOrSetDefault "GARRYSMOD_COMMON" -Default "./garrysmod_common"

UpdateLocalGitRepository $GARRYSMOD_COMMON -Repository $GARRYSMOD_COMMON_REPOSITORY
