#!/usr/bin/env bash

DOCKER_BUILDKIT=1 docker build \
  -f ./Docker/Dockerfile -t sudomakeinstall/dv-commandline-utils --ssh default .
