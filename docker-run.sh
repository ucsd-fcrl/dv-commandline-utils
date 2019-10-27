#!/usr/bin/env bash

docker run \
  --rm \
  -it \
  -v "${PWD}":/code/src \
  -w /code/bin \
  sudomakeinstall/dv-commandline-utils zsh
