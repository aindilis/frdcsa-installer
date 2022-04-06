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

apt-get install -y git

cd /home/$USER
git clone https://github.com/aindilis/frdcsa-installer

echo "Now run as root on Debian 11:"
echo "./frdcsa-installer/frdcsa-panoply-installer-20220403.sh"
