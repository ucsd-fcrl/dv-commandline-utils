#!/usr/bin/env bash

DOCKER_WD=/Developer/repositories/dv-commandline-utils/src
docker run \
  --rm \
  -it \
  -v "${PWD}":"${DOCKER_WD}" \
  -v ${HOME}/.gitconfig:/etc/gitconfig \
  -w "${DOCKER_WD}" \
  sudomakeinstall/dv-commandline-utils zsh
