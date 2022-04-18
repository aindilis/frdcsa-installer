#!/bin/bash

# ensure user is root
if [[ $EUID -ne 0 ]]; then
    echo "This script must be run as root"
    exit 1
fi

export USER="andrewdo"
export GROUP="andrewdo"

adduser $USER
adduser $USER sudo
