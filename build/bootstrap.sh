#!/bin/bash

# Exit if any command fails
set -e

# Checkout with the correct line endings on plain text files, depending on the host OS
git config --global core.autocrlf true >/dev/null 2>/dev/null

function validate_variable_or_set_default {
	local NAME="$1"
	local DEFAULT="$2"

	local VALUE="$DEFAULT"
	if [ "${!NAME}" ]; then
		local VALUE="${!NAME}"
	fi

	if [ -z "$VALUE" ] && [ -z "$DEFAULT" ]; then
		exit 1
	fi

	export "$NAME"="$VALUE"
}

function update_local_git_repository {
	local DIRECTORY="$1"
	local REPOSITORY="$2"
	local BRANCH="${3:-master}"

	local UPDATED=0
	if ! git -C "$DIRECTORY" rev-parse --is-inside-work-tree >/dev/null 2>&1; then
		echo "Cloning repository \"${REPOSITORY}\" into \"${DIRECTORY}\"..."
		rm -rf "$DIRECTORY"
		git clone --quiet --recursive --branch "$BRANCH" "$REPOSITORY" "$DIRECTORY"
		local UPDATED=1
	else
		pushd "$DIRECTORY"

		echo "Fetching all branches from remote in \"${DIRECTORY}\"..."
		git fetch --quiet --all --prune

		local CURBRANCH=$(git symbolic-ref --quiet --short HEAD)
		if [ ! "$BRANCH" = "$CURBRANCH" ]; then
			echo "Checking out branch \"${BRANCH}\" in \"${DIRECTORY}\"..."
			git checkout --quiet --force "$BRANCH"
			local UPDATED=1
		fi

		local LOCAL=$(git rev-parse @)
		local REMOTE=$(git rev-parse @{u})
		local BASE=$(git merge-base @ @{u})

		if [ "$LOCAL" = "$BASE" ]; then
			echo "Branch \"${BRANCH}\" in \"${DIRECTORY}\" needs updating..."
			local UPDATED=1
		elif [ ! "$LOCAL" = "$REMOTE" ]; then
			echo "Hard resetting branch \"${BRANCH}\" in \"${DIRECTORY}\"..."
			git reset --quiet --hard "origin/${BRANCH}"
			git clean --quiet --force -dx
			local UPDATED=1
		fi

		if [ "$UPDATED" = "1" ]; then
			echo "Updating branch \"${BRANCH}\" in \"${DIRECTORY}\"..."
			git pull --quiet --recurse-submodules
		fi

		popd
	fi

	echo "$UPDATED"
}

validate_variable_or_set_default "GARRYSMOD_COMMON_REPOSITORY" "https://github.com/danielga/garrysmod_common.git"
validate_variable_or_set_default "GARRYSMOD_COMMON_BRANCH" "master"
validate_variable_or_set_default "GARRYSMOD_COMMON" "./garrysmod_common"

update_local_git_repository "$GARRYSMOD_COMMON" "$GARRYSMOD_COMMON_REPOSITORY" "$GARRYSMOD_COMMON_BRANCH" >/dev/null
