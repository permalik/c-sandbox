#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: $0 <project_name>"
fi

project_name=$1
build_dir="./build"

if [ -d "$build_dir" ]; then
    echo "tearing down build.."
    rm -rf ./build
    echo "creating build directory.."
    mkdir build
    cd ./build || exit
    echo "generating build assets.."
    cmake ..
    echo "compiling project.."
    cmake --build .
    echo "executing project.."
    ./"${project_name}"
else
    echo "creating build directory.."
    mkdir build
    cd ./build || exit
    echo "generating build assets.."
    cmake ..
    echo "compiling project.."
    cmake --build .
    echo "executing project.."
    ./"${project_name}"
fi
