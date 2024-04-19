#!/bin/bash

PRESET_NAME=$1
HOST_OS=$OS$OSTYPE
HOST_TYPE=$HOSTTYPE
HOST_ID="${HOST_OS}-${HOST_TYPE}"
HOST_ID=${HOST_ID,,} # Convert to lowercase

echo "Host OS: $HOST_OS"
echo "Host Type: $HOST_TYPE"
echo "Host Id: $HOST_ID"

if [[ $1 == "" ]]; then
  case "$HOST_ID" in
    "windows_"*)
      # Windows
      PRESET_NAME="msvc-win64-release"
      ;;
    "darwin"*)
      # Mac OSX
      PRESET_NAME="clang-macos-release"
      ;;
    "linux-"*)
      # Linux
      PRESET_NAME="gcc-linux-release"
      ;;
    *)
      # Default case
      echo "Unsupported OS or architecture"
      exit 1
      ;;
  esac
fi

echo "Building ${PRESET_NAME}"

cmake --preset "${PRESET_NAME}"&&cmake --build --preset "${PRESET_NAME}"
