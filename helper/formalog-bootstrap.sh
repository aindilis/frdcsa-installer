#!/usr/bin/env bash

set -v

export USER=vagrant
export GROUP=vagrant

export DISTRO=`lsb_release -a 2> /dev/null | grep "Distributor ID:" | perl -pe 's/Distributor ID:\s+//'`
export LSB_RELEASE=`lsb_release -a 2> /dev/null | grep "Codename:" | perl -pe 's/Codename:\s+//'`

export USE_FORCE_YES="--force-yes"

killall -9 policyd
killall -9 screen
screen -wipe

if ! grep -q prologmud /etc/hosts; then
    cp /vagrant/data/hosts /etc/hosts
fi

if [ ! -f /home/$USER/.screenrc ]; then
    su $USER -c "cp /vagrant/data/.screenrc /home/$USER"
fi

if [ ! -f /root/.screenrc ]; then
    cp /vagrant/data/.screenrc /root
fi

su $USER -c "/vagrant/data/fix-keys.sh"

if ! dpkg -l | grep 'ii  git ' | grep -q '^ii'; then
    apt-get update
    if [ "$DISTRO" = "Debian" ]; then 
	if [ "$LSB_RELEASE" = "wheezy" ]; then
	    apt-get install -y $USE_FORCE_YES emacs23-nox openjdk-7-jdk screen git subversion dlocate unzip build-essential bison libbison-dev flex libssl-dev openssl apache2 
	elif [ "$LSB_RELEASE" = "jessie" ]; then
	    apt-get install -y $USE_FORCE_YES emacs24-nox openjdk-7-jdk screen git subversion dlocate unzip build-essential bison libbison-dev flex libssl-dev openssl apache2 
	fi
    elif [ "$DISTRO" = "Ubuntu" ]; then
	apt-get install -y $USE_FORCE_YES emacs24-nox openjdk-7-jdk screen git subversion dlocate unzip build-essential bison libbison-dev flex libssl-dev openssl apache2 
    fi
fi

if ! dpkg -l | grep software-properties-common | grep -q '^ii'; then
    if [ "$DISTRO" = "Debian" ]; then
	sudo apt-get install -y $USE_FORCE_YES python-software-properties software-properties-common
    fi
fi

if ! dpkg -l | grep git | grep -q '^ii'; then
    echo "ERROR: packages did not install"
    exit 1
fi

if [ ! [ -f /etc/apt/sources.list.d/swi-prolog-devel-trusty.list || -f /etc/apt/sources.list.d/swi-prolog-devel-wheezy.list ] ]; then
    apt-add-repository -y ppa:swi-prolog/devel
fi
if [ ! -d /vagrant/packages ]; then
    mkdir -p /vagrant/packages
fi
cd /vagrant/packages
if ! dpkg -l | grep swi-prolog | grep -q '^ii'; then
    if [ "$DISTRO" = "Debian" ]; then 
	if ! ls /vagrant/packages | grep swi-prolog | grep -q deb ; then
	    if [ "$LSB_RELEASE" = "wheezy" ]; then
		rm -f /etc/apt/sources.list.d/swi-prolog-devel-wheezy.list*
		echo "# deb http://ppa.launchpad.net/swi-prolog/devel/ubuntu trusty main" > /etc/apt/sources.list.d/swi-prolog-devel-trusty.list
		echo "deb-src http://ppa.launchpad.net/swi-prolog/devel/ubuntu trusty main" >> /etc/apt/sources.list.d/swi-prolog-devel-trusty.list
		apt-get update
		apt-get install -y $USE_FORCE_YES libncursesw5-dev libjpeg8-dev libxpm-dev unixodbc-dev default-jdk junit libxft-dev libxext-dev libxinerama-dev libarchive-dev libunwind8-dev libossp-uuid-dev dh-autoreconf build-essential:native libreadline-dev libgmp-dev dctrl-tools libdb-dev
	    elif [ "$LSB_RELEASE" = "jessie" ]; then
		rm -f /etc/apt/sources.list.d/swi-prolog-devel-jessie.list*
		echo "# deb http://ppa.launchpad.net/swi-prolog/devel/ubuntu vivid main" > /etc/apt/sources.list.d/swi-prolog-devel-vivid.list
		echo "deb-src http://ppa.launchpad.net/swi-prolog/devel/ubuntu vivid main" >> /etc/apt/sources.list.d/swi-prolog-devel-vivid.list

		if ! cat /etc/apt/sources.list | grep -q wheezy ; then
		    echo "deb http://http.us.debian.org/debian wheezy main" >> /etc/apt/sources.list
		fi
		if ! cat /etc/apt/sources.list | grep -q wheezy ; then
		    echo "ERROR: could not add wheezy to sources.list"
		    exit 1
		fi

		apt-get update
		apt-get install -y $USE_FORCE_YES -t wheezy libjpeg8-dev

		if cat /etc/apt/sources.list | grep -q wheezy ; then
		    cat /etc/apt/sources.list | grep -v "deb http://http.us.debian.org/debian wheezy main" > /etc/apt/sources.list.tmp
		    mv /etc/apt/sources.list /etc/apt/sources.list.orig
		    mv /etc/apt/sources.list.tmp /etc/apt/sources.list
		fi
		if cat /etc/apt/sources.list | grep -q wheezy ; then
		    echo "ERROR: could not remove wheezy fromsources.list"
		    exit 1
		fi

		apt-get update
		apt-get install -y $USE_FORCE_YES debhelper libncursesw5-dev libreadline-dev libjpeg8-dev libx11-dev libxpm-dev libxt-dev x11proto-core-dev unixodbc-dev default-jdk junit libxft-dev libxext-dev libice-dev libxinerama-dev dctrl-tools libarchive-dev libunwind8-dev libossp-uuid-dev dh-autoreconf

	    fi
	    apt-get source --allow-unauthenticated --build swi-prolog
	fi
	# FIXME: make this only if it's wheezy, not jessie
	dpkg -i swi-prolog-nox_*.deb
	apt-get -f install -y $USE_FORCE_YES
	dpkg -i swi-prolog-nox_*.deb
	dpkg -i swi-prolog-x_*.deb
	dpkg -i swi-prolog_[0-9]*.deb
	dpkg -i swi-prolog-java_*.deb
	dpkg -i swi-prolog-odbc_*.deb

    elif [ "$DISTRO" = "Ubuntu" ]; then
	apt-get update
	sudo apt-get install -y $USE_FORCE_YES swi-prolog
    fi
fi
if ! dpkg -l | grep swi-prolog | grep -q '^ii'; then
    echo "ERROR: packages did not install"
    exit 1
fi

# source <(curl -s https://raw.githubusercontent.com/TeamSPoon/PrologMUD/master/scripts/web_install.sh)
