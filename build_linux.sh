#!/bin/bash

set -xe

mkdir -p build
cmake . -B build &&
cmake --build build
