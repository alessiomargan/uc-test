#!/bin/bash

# Use the first argument as GIT_SRC_DIR, or default to the script's directory
if [ -n "$1" ]; then
    GIT_SRC_DIR="$1"
else
    GIT_SRC_DIR="$(dirname "$(realpath "$0")")"
fi

echo "GIT_SRC_DIR: $GIT_SRC_DIR"

# Define the output file
OUTPUT_FILE="$GIT_SRC_DIR/include/build_info.h"

# Add a comment header to the generated file
echo "/*" > "$OUTPUT_FILE"
echo " * $OUTPUT_FILE" >> "$OUTPUT_FILE"
echo " * Auto-generated file. Do not edit manually." >> "$OUTPUT_FILE"
echo " */" >> "$OUTPUT_FILE"

echo "#define BUILD_TIMESTAMP \"$(date +"%Y-%m-%d %H:%M:%S")\"" >> "$OUTPUT_FILE"
echo "#define GIT_HASH \"$(git -C $GIT_SRC_DIR rev-parse --short HEAD)\"" >> "$OUTPUT_FILE"
echo "#define GIT_COMMIT_HASH \"$(git -C $GIT_SRC_DIR rev-parse HEAD)\"" >> "$OUTPUT_FILE"
echo "#define GIT_BRANCH \"$(git -C $GIT_SRC_DIR rev-parse --abbrev-ref HEAD)\"" >> "$OUTPUT_FILE"

# Get the most recent Git tag
GIT_TAG="$(git -C $GIT_SRC_DIR describe --tags --abbrev=0 2>/dev/null)"
if [ -z "$GIT_TAG" ]; then
    GIT_TAG="No Tag"
    echo "#warning \"No Git tag found in the repository!\"" >> "$OUTPUT_FILE"
fi
echo "#define GIT_TAG \"$GIT_TAG\"" >> "$OUTPUT_FILE"

# Check if the repository is dirty
if [ -n "$(git -C $GIT_SRC_DIR status --porcelain)" ]; then
    echo "#define GIT_DIRTY 1" >> "$OUTPUT_FILE"
    echo "#warning \"Repository has uncommitted changes!\"" >> "$OUTPUT_FILE"
else
    echo "#define GIT_DIRTY 0" >> "$OUTPUT_FILE"
fi

# Dynamically detect the remote name and URL
GIT_REMOTE_NAME="$(git -C $GIT_SRC_DIR remote | head -n 1)"
GIT_REPO_URL="$(git -C $GIT_SRC_DIR config --get remote.$GIT_REMOTE_NAME.url)"
if [ -z "$GIT_REPO_URL" ]; then
    GIT_REPO_URL="Unknown"
fi
echo "#define GIT_REPO_URL \"$GIT_REPO_URL\"" >> "$OUTPUT_FILE"

echo "#define BUILD_HOST \"$(hostname)\"" >> "$OUTPUT_FILE"
echo "#define BUILD_USER \"$(whoami)\"" >> "$OUTPUT_FILE"
#echo "#define COMPILER_VERSION \"$(gcc --version | head -n 1)\"" >> "$OUTPUT_FILE"
echo "#define BUILD_OS \"$(uname -sr)\"" >> "$OUTPUT_FILE"