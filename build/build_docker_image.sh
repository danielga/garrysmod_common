#!/usr/bin/env bash

set -euo pipefail

CUR_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

DOCKER_REPOSITORY='danielga/steamrt-scout'
LATEST_DOCKER_TAG="${DOCKER_REPOSITORY}:latest"
PREVIOUS_DOCKER_TAG_VERSION=$(curl -sL "https://registry.hub.docker.com/v2/repositories/${DOCKER_REPOSITORY}/tags?page_size=100" | jq -r '[."results"[]["name"] | select(startswith("v")) | .[1:]] | sort | last // 0')
NEXT_DOCKER_TAG="${DOCKER_REPOSITORY}:v$((PREVIOUS_DOCKER_TAG_VERSION + 1))"

pushd "$CUR_DIR" > /dev/null
docker pull "$LATEST_DOCKER_TAG" || true
docker build -t "$LATEST_DOCKER_TAG" -t "$NEXT_DOCKER_TAG" .
echo "Logging in to Docker"
docker login
docker push "$NEXT_DOCKER_TAG"
docker push "$LATEST_DOCKER_TAG"
popd > /dev/null
