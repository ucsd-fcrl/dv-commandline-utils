#!/usr/bin/env bash

mkdir -p ../bin/releases/

tar -czvf ../bin/releases/dv-commandline-utils-${OSTYPE}-$(date +"%Y-%m-%d").tar.gz ../bin/dv-*
