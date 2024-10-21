#!/bin/bash

# Check if git is installed
if ! command -v git &> /dev/null
then
    echo "git could not be found. Please install git to continue."
    exit
fi

# Initialize and update submodules
echo "Initializing and updating submodules..."
git submodule update --init --recursive

echo "Submodules updated successfully!"
