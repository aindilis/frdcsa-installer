#!/usr/bin/env bash

echo "Script first needs to be updated with techniques from frdcsa-installer-panoply-git-20180217.sh"
exit

# THIS IS OFFICIAL PANOPLY (TO METAL OR DOCKER) INSTALLER (NOT YET
# FINISHED)

# As of [2022-04-03,12:40:31] I am fixing it up to properly install
# panoply on regular machine (or possibly Docker) .  Some other use
# cases that need to be eventually fixed and maintained are installing
# panoply on a Vagrant VM, and installing FRDCSA-private.

# ensure user is root
if [[ $EUID -ne 0 ]]; then
    echo "This script must be run as root"
    exit 1
fi

# make sure you approvelist the IP of the target server with fail2ban
# https://www.howtoforge.com/how-to-whitelist-an-ip-in-fail2ban-on-debian-wheezy

set -v

echo "Installer for Debian Bullseye (11)"
# FIXME: use lsb_release to ensure we're on bullseye

# FIXME: Among the very first things this system should do is make
# sure there is a user by the name of andrewdo, with adduser andrewdo,
# and adduser andrewdo sudo.

export UPDATE_REPOS=false
# export USE_FORCE_YES="--force-yes"
export USE_FORCE_YES=""

export NONINTERACTIVE=true
export PERL_MM_USE_DEFAULT=1

export PRIVATE_INSTALL=false

export THE_SOURCE="/var/lib/myfrdcsa/codebases/releases/myfrdcsa-0.1/myfrdcsa-0.1/frdcsa.bashrc"

export USE_PERLBREW=true
if [ $USE_PERLBREW == true ]; then
    # export CPANM_COMMAND="su $SUDO_USER -c"
    export CPANM_COMMAND="echo"
    export FRDCSA_PERLBREW_VERSION="5.35.10"
    export FRDCSA_PERLBREW_FULL="perl-5.35.10_WITH_THREADS"
else
    export CPANM_COMMAND="sudo"
fi

# FIXME: Update vvv
export CPAN_MIRROR_DIR="20150322-1/minicpan"

# FIXME: make the installer idempotent

export INSTALL_TO_VAGRANT=false
export INSTALL_TO_HOST=true

export INSTALL_WITHOUT_NETWORK=false

export INSTALL_SCRIPT_DEPENDENCIES="/var/lib/myfrdcsa/codebases/internal/packager/scripts/install-script-dependencies-perlbrew"

if [ $INSTALL_WITHOUT_NETWORK == true ]; then
    # FIXME enter location of  data here
    export WITHOUT_NETWORK_DATADIR="201504014-1/var/lib/myfrdcsa/codebases/internal/classify/data/.incoming"
fi

if [[ $INSTALL_TO_VAGRANT == true && $INSTALL_TO_HOST == true ]] ||
       [[ $INSTALL_TO_VAGRANT == false && $INSTALL_TO_HOST == false ]]; then
    echo "ERROR: Currently only one must be true at a time of INSTALL_TO_VAGRANT and INSTALL_TO_HOST"
    exit 1
fi

if $INSTALL_TO_VAGRANT == true; then
    export USER="vagrant"
    export GROUP="vagrant"
    export DATA_DIR="/vagrant/data"
elif $INSTALL_TO_HOST == true; then
    export USER="andrewdo"
    export GROUP="andrewdo"
    export DATA_DIR="/home/$USER/frdcsa-installer/data"
    if ! [ -d "/home/$USER" ]; then
	# FIXME: does this even work? vvv
	adduser $USER
	adduser $USER sudo
    fi
fi

# # This is all deprecated by upgrades to vagrant, so commenting out
# # setup a secure .ssh environment

# if $INSTALL_TO_VAGRANT == true; then
#     if [ ! -d /home/vagrant/.ssh.old ]; then
# 	mv /home/vagrant/.ssh /home/vagrant/.ssh.old
# 	mkdir -p /home/vagrant/.ssh
# 	chmod 700 /home/vagrant/.ssh
# 	chown -R vagrant.vagrant /home/vagrant/.ssh
#     fi
# elif $INSTALL_TO_HOST == true; then
#     echo nothing to do for now
# fi


if $INSTALL_TO_VAGRANT == true; then

    # FIXME: add something here to make it idempotent
    if ! grep -q 'andrewdo@justin' /home/$USER/.ssh/authorized_keys; then
	cat $DATA_DIR/id_rsa.pub >> /home/$USER/.ssh/authorized_keys
    fi
fi

if ! $INSTALL_WITHOUT_NETWORK == true; then
    if ! grep -q "Host posi.frdcsa.org" /home/$USER/.ssh/config; then
	echo -e "Host posi.frdcsa.org\n\tStrictHostKeyChecking no\n" >> /home/$USER/.ssh/config
	chown $USER.$GROUP /home/$USER/.ssh/authorized_keys /home/$USER/.ssh/config
	cp $DATA_DIR/frdcsa_git_id_rsa /home/$USER/.ssh/id_rsa
	chown $USER.$GROUP /home/$USER/.ssh/id_rsa
	chmod 600 /home/$USER/.ssh/id_rsa
    fi
    if ! grep -q "BEGIN RSA PRIVATE KEY" /home/$USER/.ssh/id_rsa; then
	echo "ERROR: do not possess the key"
	# exit 1;
    fi
fi

# setup a proper sources.list
# cp $DATA_DIR/sources.list /etc/apt

export APT_UPDATED=0

if [ $APT_UPDATED == 0 ]; then
    sudo apt-get update
    export export APT_UPDATED=1
fi
sudo apt-get install -y $USE_FORCE_YES build-essential git emacs apg libclass-methodmaker-perl w3m-el mew bbdb nmap super libssl-dev chase libxml2-dev link-grammar liblink-grammar5 liblink-grammar-dev screen cpanminus perl-doc libssl-dev bbdb openjdk-11-jdk libxml-atom-perl namazu2 namazu2-index-tools apt-file x11-apps dlocate locate telnet xclip libb-utils-perl libcal-dav-perl libconfig-general-perl libdata-dump-streamer-perl libfile-slurp-perl libfile-which-perl libgetopt-declare-perl libgraph-perl libpadwalker-perl libproc-processtable-perl libstring-shellquote-perl libstring-similarity-perl libtask-weaken-perl libterm-readkey-perl libtie-ixhash-perl libtk-perl libunicode-map8-perl libunicode-string-perl libxml-atom-perl libxml-dumper-perl libxml-perl libxml-twig-perl libnet-telnet-perl link-grammar link-grammar-dictionaries-en libuima-addons-java libuima-as-java libuima-as-java-doc libuima-adapter-vinci-java libuima-core-java libuima-cpe-java libuima-document-annotation-java libuima-tools-java libuima-vinci-java uima-doc uima-examples uima-utils wordnet wordnet-base wordnet-gui  festival wamerican-insane libevent-perl libfile-pid-perl libxml-smart-perl libnet-dbus-perl libschedule-cron-perl libfile-nfslock-perl libinline-java-perl ssh-askpass ssh-askpass-gnome ssh-askpass-fullscreen elpa-hydra wmctrl zlib1g-dev lib32z1-dev libexpat1-dev graphviz libmariadb-dev-compat libdb-dev imagemagick libmagickcore-6-headers libmagickcore-dev tesseract-ocr freeglut3-dev libxi-dev libxmu-dev


# libwordnet-querydata-perl

if ! dpkg -l | grep wamerican-insane | grep -q '^ii'; then
    echo "ERROR: first major group of packages did not install"
    exit 1
fi

if [ ! -d "/etc/myfrdcsa/config" ]; then
    if [ $PRIVATE_INSTALL == true ]; then
	cd /etc
	if [ $INSTALL_WITHOUT_NETWORK == true ]; then
	    su -c "git clone $WITHOUT_NETWORK_DATADIR/myfrdcsa"
	else
	    su -c "git clone ssh://andrewdo@ai2.frdcsa.org/etc/myfrdcsa"
	fi
	su -c "chown -R $USER.$USER myfrdcsa"
	su -c "rm /etc/myfrdcsa/config/perllib"
    fi
fi

# FIXME: We will want this to work with the new way of doing
# authentication with mariadb [2022-04-04,09:38:52]
if ! dpkg -l | grep -E 'mariadb-server-[0-9]+' | grep -q '^ii'; then
    

    # FIXME: add something here to abort installation if it detects that it will remove any packages
    
    # http://stackoverflow.com/questions/1202347/how-can-i-pass-a-password-from-a-bash-script-to-aptitude-for-installing-mysql

    # emacs install twittering-mode, ert

    if [ ! -f "/etc/myfrdcsa/config/perllib" ]; then
	if [ ! -d "/etc/myfrdcsa/config" ]; then
	    mkdir -p /etc/myfrdcsa/config/
	fi
	# FIXME have it write the mysql password to /etc/myfrdcsa/config/perllib
	apg -n 1 -m 16 > /etc/myfrdcsa/config/perllib
	chown $USER.$GROUP /etc/myfrdcsa/config/perllib
	chmod 600 /etc/myfrdcsa/config/perllib
    fi

    export PASS=`cat /etc/myfrdcsa/config/perllib`

    export DEBIAN_FRONTEND=noninteractive

    # debconf-set-selections <<< "mysql-server-5.1 mysql-server/root_password password $PASS"
    # debconf-set-selections <<< "mysql-server-5.1 mysql-server/root_password_again password $PASS"

    if [ $APT_UPDATED == 0 ]; then
	apt-get update
	export export APT_UPDATED=1
    fi
    apt-get -q -y $USE_FORCE_YES install mariadb-server mariadb-client

    echo "Give mysql server time to start up before we try to set a password..."
    sleep 5

    mysql -uroot <<EOF
UPDATE mysql.user SET Password=PASSWORD('$PASS') WHERE User='root'; FLUSH PRIVILEGES;
EOF

    # echo "Give mysql server time to start up before we try to set a password..."
    # sleep 5
    # mysql -uroot -e <<EOF
    # EOSQL "UPDATE mysql.user SET Password=PASSWORD('$PASS') WHERE User='root'; FLUSH PRIVILEGES;"
    # EOSQL
    # echo "Done setting mysql password."
    # EOF

    unset DEBIAN_FRONTEND
fi

if [ ! -d "/var/lib/myfrdcsa/codebases" ]; then
    mkdir -p /var/lib/myfrdcsa/codebases
    chown -R $USER.$GROUP /var/lib/myfrdcsa
fi

cd /var/lib/myfrdcsa/codebases

if [ ! -d "/var/lib/myfrdcsa/codebases/releases" ]; then
    if [ $INSTALL_WITHOUT_NETWORK == true ]; then
	echo SKIPPING for now
	# su $USER -c "git clone $WITHOUT_NETWORK_DATADIR/releases"
    else
	# echo skipped
	su $USER -c "git clone ssh://andrewdo@posi.frdcsa.org/gitroot/releases"
    fi
else
    if [ $UPDATE_REPOS == true ]; then
	pushd /var/lib/myfrdcsa/codebases/releases
	# su $USER -c "git pull"
	popd
    fi
fi
if [ ! -d "/var/lib/myfrdcsa/codebases/releases" ]; then
    echo "ERROR: Didn't check out /var/lib/myfrdcsa/codebases/releases"
    # exit 1
fi

if [ ! -d "/var/lib/myfrdcsa/codebases/minor" ]; then
    if [ $INSTALL_WITHOUT_NETWORK == true ]; then
	echo SHIPPING for now
	# su $USER -c "git clone $WITHOUT_NETWORK_DATADIR/minor"
    else
	# echo skipped
	su $USER -c "git clone ssh://andrewdo@posi.frdcsa.org/gitroot/minor"
    fi
else
    if [ $UPDATE_REPOS == true ]; then
	pushd /var/lib/myfrdcsa/codebases/minor
	su $USER -c "git pull"
	popd
    fi
fi
if [ ! -d "/var/lib/myfrdcsa/codebases/minor" ]; then
    echo "ERROR: Didn't check out /var/lib/myfrdcsa/codebases/minor"
    # exit 1
fi

if [ ! -d "/var/lib/myfrdcsa/codebases/minor-data" ]; then
    if [ $INSTALL_WITHOUT_NETWORK == true ]; then
	echo SHIPPING for now
	# su $USER -c "git clone $WITHOUT_NETWORK_DATADIR/minor"
    else
	# echo skipped
	# su $USER -c "git clone ssh://andrewdo@ai.frdcsa.org/Meredith/gitroot/minor-data"
	# su $USER -c "git clone ssh://andrewdo@ai2.frdcsa.org/var/lib/myfrdcsa/codebases/minor-data"
	echo "FIXME 1"
	exit
	# su $USER -c "rsync -av -progress andrewdo@ai2.frdcsa.org/var/lib/myfrdcsa/codebases/minor-data minor-data/"
    fi
else
    if [ $UPDATE_REPOS == true ]; then
	pushd /var/lib/myfrdcsa/codebases/minor-data
	su $USER -c "git pull"
	popd
    fi
fi
if [ ! -d "/var/lib/myfrdcsa/codebases/minor-data" ]; then
    echo "ERROR: Didn't check out /var/lib/myfrdcsa/codebases/minor-data"
    # exit 1
fi


cd /home/$USER

if [ ! -d "/home/$USER/.myconfig" ]; then
    if [ $INSTALL_WITHOUT_NETWORK == true ]; then
	su $USER -c "git clone $WITHOUT_NETWORK_DATADIR/myconfig"
	su $USER -c "mv myconfig .myconfig"
    else
	su $USER -c "git clone ssh://andrewdo@posi.frdcsa.org/gitroot/.myconfig"
    fi
else
    if [ $UPDATE_REPOS == true ]; then
	pushd /home/$USER/.myconfig
	su $USER -c "git pull"
	popd
    fi
fi
if [ ! -d "/home/$USER/.myconfig" ]; then
    echo "ERROR: Didn't check out /home/$USER/.myconfig"
    exit 1
fi

export DATETIMESTAMP=`date "+%Y%m%d%H%M%S"`

if ! grep myfrdcsa .bashrc; then
    mv /home/$USER/.bashrc /home/$USER/.bashrc.$DATETIMESTAMP
    mv /home/$USER/.emacs.d /home/$USER/.emacs.d.$DATETIMESTAMP
    su $USER -c "/home/$USER/.myconfig/bin/install-myconfig.pl"
fi
if ! grep myfrdcsa .bashrc; then
    echo "ERROR: Didn't install /home/$USER/.myconfig correctly"
    exit 1
fi

cd /home/$USER

if [ $PRIVATE_INSTALL == true ]; then
    if [ ! -d "/home/$USER/.myconfig-private" ]; then
	if [ $INSTALL_WITHOUT_NETWORK == true ]; then
	    su $USER -c "git clone $WITHOUT_NETWORK_DATADIR/myconfig-private"
	    su $USER -c "mv myconfig-private .myconfig-private"
	else
	    # FIXME this won't work
	    su $USER -c "git clone ssh://andrewdo@posi.frdcsa.org/gitroot/.myconfig-private"
	fi
    else
	if [ $UPDATE_REPOS == true ]; then
	    pushd /home/$USER/.myconfig-private
	    su $USER -c "git pull"
	    popd
	fi
    fi
    if [ ! -d "/home/$USER/.myconfig-private" ]; then
	echo "ERROR: Didn't check out /home/$USER/.myconfig-private"
	exit 1
    fi

    if [ ! -f "/home/$USER/.cpmdb" ]; then
	su $USER -c "/home/$USER/.myconfig-private/bin/install-myconfig-private.pl"
    fi
    if [ ! -f "/home/$USER/.cpmdb" ]; then
	echo "ERROR: Didn't install /home/$USER/.myconfig-private correctly"
	exit 1
    fi
fi


if [ ! -d "/var/lib/myfrdcsa/codebases/internal" ]; then
    # run the script for updating the perl links
    su $USER -c "mkdir -p /var/lib/myfrdcsa/codebases/internal/"
    su $USER -c "/var/lib/myfrdcsa/codebases/releases/myfrdcsa-0.1/myfrdcsa-0.1/scripts/gen-internal-links.pl"
fi
if [ ! -h "/var/lib/myfrdcsa/codebases/internal/myfrdcsa" ]; then
    echo "ERROR: gen internal links failed, exiting."
    exit 1
fi

# ANDY: SEEMS TO BE WHERE I STOPPED


if [ ! -h "/home/andrewdo/perl5/perlbrew/perls/$FRDCSA_PERLBREW_FULL/lib/site_perl/$FRDCSA_PERLBREW_VERSION/MyFRDCSA" ]; then
    /var/lib/myfrdcsa/codebases/internal/myfrdcsa/scripts/gen-perl-links--perlbrew.pl
fi
if [ ! -h "/home/andrewdo/perl5/perlbrew/perls/$FRDCSA_PERLBREW_FULL/lib/site_perl/$FRDCSA_PERLBREW_VERSION/MyFRDCSA" ]; then
    echo "ERROR: gen perl links failed, exiting."
    exit 1
fi

if [ ! -x "/root/.cpan" ]; then
    # setup CPAN
    # have it configure it by default
    # FIXME: switch to cpanm
    cpan -J
fi
if [ ! -x "/root/.cpan" ]; then
    echo "ERROR: didn't configure CPAN."
    exit 1
fi

if [ ! -d "/var/lib/myfrdcsa/codebases/data/" ]; then
    su $USER -c "mkdir -p /var/lib/myfrdcsa/codebases/data/"
    # create all the data dirs as required
    su $USER -c "/var/lib/myfrdcsa/codebases/internal/myfrdcsa/scripts/gen-data-dirs.pl"
fi
if [ ! -d "/var/lib/myfrdcsa/codebases/data/" ]; then
    echo "ERROR: gen data dirs failed, exiting."
    exit 1
fi

if [ $PRIVATE_INSTALL == true ]; then
    if [ ! -h /home/$USER/.config/frdcsa ]; then
	cd /home/$USER
	mkdir -p .config
	chown $USER.$GROUP .config
	cd .config
	if [ $INSTALL_WITHOUT_NETWORK == true ]; then
	    su $USER -c "git clone $WITHOUT_NETWORK_DATADIR/frdcsa-private"
	else
	    su $USER -c "git clone ssh://andrewdo@ai2.frdcsa.org/home/andrewdo/.config/frdcsa-private"
	fi
	ln -s /home/$USER/.config/frdcsa-private /home/$USER/.config/frdcsa
    else
	if [ $UPDATE_REPOS == true ]; then
	    pushd /home/$USER/.config/frdcsa-private
	    su $USER -c "git pull"
	    popd
	fi
    fi
    if [ ! -h /home/$USER/.config/frdcsa ]; then
	echo "ERROR: didn't checkout /home/$USER/.config/frdcsa properly, exiting."
	exit 1
    fi
else
    if [ ! -h /home/$USER/.config/frdcsa ]; then
	cd /home/$USER
	mkdir -p .config
	chown $USER.$GROUP .config
	cd .config
	if [ $INSTALL_WITHOUT_NETWORK == true ]; then
	    su $USER -c "git clone $WITHOUT_NETWORK_DATADIR/frdcsa-public"
	else
	    su $USER -c "git clone ssh://andrewdo@ai2.frdcsa.org/home/andrewdo/.config/frdcsa"
	fi
	ln -s /home/$USER/.config/frdcsa-public /home/$USER/.config/frdcsa
    else
	if [ $UPDATE_REPOS == true ]; then
	    pushd /home/$USER/.config/frdcsa-public
	    su $USER -c "git pull"
	    popd
	fi
    fi
    if [ ! -h /home/$USER/.config/frdcsa ]; then
	echo "ERROR: didn't checkout /home/$USER/.config/frdcsa properly, exiting."
	exit 1
    fi
fi

cd /var/lib/myfrdcsa/codebases/internal/kbfs/data

if [ ! -d "/var/lib/myfrdcsa/codebases/internal/kbfs/data/mysql-backups" ]; then
    su $USER -c "mkdir /var/lib/myfrdcsa/codebases/internal/kbfs/data/mysql-backups"
fi
if [ ! -d "/var/lib/myfrdcsa/codebases/internal/kbfs/data/mysql-backups" ]; then
    echo "ERROR: didn't make /var/lib/myfrdcsa/codebases/internal/kbfs/data/mysql-backups properly, exiting."
    exit 1
fi

cd /var/lib/myfrdcsa/codebases/internal/kbfs/data/mysql-backups

if [ ! -d "/var/lib/myfrdcsa/codebases/internal/kbfs/data/mysql-backups/mysql-backup" ]; then

    if [ $PRIVATE_INSTALL == true ]; then
	if [ $INSTALL_WITHOUT_NETWORK == true ]; then
	    su $USER -c "git clone $WITHOUT_NETWORK_DATADIR/mysql-private-backup"
	else
	    # FIXME this is not going to work, wrong
	    su $USER -c "git clone ssh://andrewdo@ai2.frdcsa.org/gitroot/mysql-private-backup"
	fi
	su $USER -c "mv mysql-private-backup mysql-backup"
    else
	if [ $INSTALL_WITHOUT_NETWORK == true ]; then
	    su $USER -c "git clone $WITHOUT_NETWORK_DATADIR/mysql-backup"
	else
	    su $USER -c "git clone ssh://andrewdo@ai.frdcsa.org/gitroot/mysql-backup"
	fi
    fi
else
    if [ $UPDATE_REPOS == true ]; then
	pushd /var/lib/myfrdcsa/codebases/internal/kbfs/data/mysql-backups/mysql-backup
	su $USER -c "git pull"
	popd
    fi
fi
if ! [ -d "/var/lib/myfrdcsa/codebases/internal/kbfs/data/mysql-backups/mysql-backup" ]; then
    echo "ERROR: failed to clone mysql-backup, exiting."
    exit 1
fi

export TEMP_IDENTIFIER=`cat /etc/myfrdcsa/config/perllib`
cd mysql-backup
if ! echo show databases | mysql -u root --password="$TEMP_IDENTIFIER" | grep unilang; then
    for it in `ls -1 *.sql | sed -e 's/\.sql//'`; do
	echo $it ; 
	echo "create database \`$it\`;" | mysql -u root --password="$TEMP_IDENTIFIER" ; 
	cat $it.sql | mysql -u root --password="$TEMP_IDENTIFIER" $it ; 
    done
fi
if ! echo show databases | mysql -u root --password="$TEMP_IDENTIFIER" | grep unilang; then
    echo "ERROR: databases apparently didn't load"
    exit 1
fi

if [ ! -d "$DATA_DIR/frdcsa-misc/etc" ]; then
    cd $DATA_DIR

    if [ $INSTALL_WITHOUT_NETWORK == true ]; then
	su $USER -c "git clone $WITHOUT_NETWORK_DATADIR/frdcsa-misc"
    else
	su $USER -c "git clone ssh://andrewdo@posi.frdcsa.org/gitroot/frdcsa-misc"
    fi
else
    if [ $UPDATE_REPOS == true ]; then
	pushd $DATA_DIR/frdcsa-misc
	su $USER -c "git pull"
	popd
    fi
fi
if [ ! -d "$DATA_DIR/frdcsa-misc/etc" ]; then
    echo "ERROR: didn't checkout frdcsa-misc properly"
    exit 1
fi

export TMP_PERL_MM_OPT=$PERL_MM_OPT
export PERL_MM_OPT=

if [ $INSTALL_WITHOUT_NETWORK == true ]; then
    export CPANMADDITIONAL="--mirror $CPAN_MIRROR_DIR"
else
    export CPANMADDITIONAL=""
fi

# FIXME: RPC::XML is hanging on test, install with no tests
# FIXME: Text::Capitalize fails to install

$CPANM_COMMAND "cpanm $CPANMADDITIONAL Try::Tiny Path::Class ExtUtils::CBuilder Crypt::SSLeay Module::Load ExtUtils::MakeMaker PPI File::Remove Test::Object Test::NoWarnings Test::Tester Test::NoWarnings Test::SubCalls Hook::LexWrap Test::SubCalls IO::String Clone Class::Inspector PPI Data::SExpression Test::Deep Data::SExpression Data::URIEncode Lingua::EN::Sentence X11::WMCtrl Mail::Box IO::stringy Object::Realize::Later Test::Pod Digest::HMAC User::Identity MIME::Types Devel::GlobalDestruction Sub::Exporter::Progressive Devel::GlobalDestruction Mail::Box Ubigraph Frontier::RPC Ubigraph Net::Google::Calendar DateTime::Format::ICal DateTime::Event::ICal DateTime::Event::Recurrence DateTime::Set Set::Infinite DateTime::Set DateTime::Event::Recurrence DateTime::Event::ICal DateTime::Format::ICal Net::Google::AuthSub Date::ICal Date::Leapyear Date::ICal IPC::Run3" # Net::SSL Crypt::SSLeay

$CPANM_COMMAND "cpanm $CPANMADDITIONAL -n Net::Google"

export PERL_MM_OPT=$TMP_PERL_MM_OPT

if ! perldoc -l Crypt::SSLeay; then
    echo "ERROR: CPANM modules did not install"
    exit 1
fi

export TMP_PERL_MM_OPT=$PERL_MM_OPT
export PERL_MM_OPT=

if ! perldoc -l File::Signature; then
    $CPANM_COMMAND "cpanm $CPANMADDITIONAL -f File::Signature"
fi
export PERL_MM_OPT=$TMP_PERL_MM_OPT

if ! perldoc -l File::Signature; then
    echo "ERROR: didn't install File::Signature correctly"
    # exit 1
fi

export TMP_PERL_MM_OPT=$PERL_MM_OPT
export PERL_MM_OPT=

if ! perldoc -l Dir::List; then
    $CPANM_COMMAND "cpanm $CPANMADDITIONAL -f Dir::List"
fi

export PERL_MM_OPT=$TMP_PERL_MM_OPT

if ! perldoc -l Dir::List; then
    echo "ERROR: didn't install Dir::List correctly"
    # exit 1
fi

if ! perldoc -l File::Stat; then
    # DONE: FIXME: manually install File::Stat
    cd /tmp && tar xzf $DATA_DIR/frdcsa-misc/File-Stat-0.01.tar.gz 
    cd File-Stat-0.01/
    perl Makefile.PL
    make
    make install
fi
if ! perldoc -l File::Stat; then
    echo "ERROR: didn't install File::Stat correctly"
    # exit 1
fi

export SUDO_ADDENDUM="$USER ALL = NOPASSWD: /usr/bin/apt-get, /usr/bin/cpanm"
echo $SUDO_ADDENDUM
if ! cat /etc/sudoers | grep /usr/bin/apt-get; then
    echo "$SUDO_ADDENDUM" >> /etc/sudoers
    echo >> /etc/sudoers
fi

if [ ! -d "/root/.Inline" ]; then
    mkdir /root/.Inline
fi

if [ ! -d "/home/$USER/.Inline" ]; then
    su $USER -c "mkdir /home/$USER/.Inline"
fi

#####################################################################################

echo "STARTING INSTALLATION OF UNILANG: PROCESS IS CONFUSING, PLEASE WAIT UP TO 15 MINUTES FOR IT TO COMPLETE (until you see 'INSTALLATION OF UNILANG COMPLETE')"

if [ ! -f "/etc/init.d/unilang" ]; then
    cat /var/lib/myfrdcsa/codebases/internal/unilang/systems/etc/init.d/unilang | perl -pe "s/<USERNAME>/$USER/sg" > /etc/init.d/unilang
    sudo chmod 755 /etc/init.d/unilang 
    sudo update-rc.d unilang defaults
fi

cd /var/lib/myfrdcsa/codebases/internal/myfrdcsa && /var/lib/myfrdcsa/codebases/internal/myfrdcsa/scripts/install-multi-agent-system.pl

cd /var/lib/myfrdcsa/codebases/internal/unilang
if ! $INSTALL_SCRIPT_DEPENDENCIES "./start -s -u localhost 9000 -c -W 5000"; then
    echo "ERROR: Installation of Unilang failed"
    exit 1
fi

echo "INSTALLATION OF UNILANG COMPLETE"

#####################################################################################

export UNILANG_START_DURATION=15

echo "UniLang starting up again to install subsequent agents"
# su $USER -c "/etc/init.d/unilang start"
/etc/init.d/unilang start
sleep $UNILANG_START_DURATION

echo "Starting web-services"
su $USER -c "source $THE_SOURCE && cd /var/lib/myfrdcsa/codebases/internal/unilang && NONINTERACTIVE=true $INSTALL_SCRIPT_DEPENDENCIES \"./scripts/web-services/server -u -t XMLRPC -W 10000\""

echo "Starting Manager"
su $USER -c "source $THE_SOURCE && cd /var/lib/myfrdcsa/codebases/internal/manager && NONINTERACTIVE=true $INSTALL_SCRIPT_DEPENDENCIES \"./manager -u --scheduler -W 10000\""

echo "Current batch of agents installed, stopping UniLang for now"
# su $USER -c "/etc/init.d/unilang stop"
/etc/init.d/unilang stop
killall start unilang unilang-client

if ! [ -d "/var/lib/myfrdcsa/codebases/data/freekbs2/theorem-provers" ]; then
    # scp -r andrewdo@justin.frdcsa.org:/var/lib/myfrdcsa/codebases/data/freekbs2/theorem-provers .
    cd /var/lib/myfrdcsa/codebases/data/freekbs2
    cp -ar $DATA_DIR/frdcsa-misc/theorem-provers .
fi
if ! [ -d "/var/lib/myfrdcsa/codebases/data/freekbs2/theorem-provers" ]; then
    echo "ERROR: Didn't set up freekbs2 theorem-provers properly"
    exit 1
fi

# # get SPSE2 running

if ! [ -d "/var/lib/myfrdcsa/sandbox" ]; then
    # su $USER -c "mkdir /var/lib/myfrdcsa/sandbox"
    mkdir /var/lib/myfrdcsa/sandbox
    chown -R $USER.$GROUP /var/lib/myfrdcsa/sandbox
fi
if ! [ -d "/var/lib/myfrdcsa/sandbox" ]; then
    echo "ERROR: Didn't set up sandbox properly"
    exit 1
fi

if ! [ -d "/var/lib/myfrdcsa/sandbox/opencyc-4.0/opencyc-4.0" ]; then
    su $USER -c "mkdir /var/lib/myfrdcsa/sandbox/opencyc-4.0"
    cd /var/lib/myfrdcsa/sandbox/opencyc-4.0
    su $USER -c "cp -ar $DATA_DIR/frdcsa-misc/opencyc-4.0 ."
fi
if ! [ -d "/var/lib/myfrdcsa/sandbox/opencyc-4.0/opencyc-4.0" ]; then
    echo "ERROR: Didn't set up opencyc-4.0 properly"
    exit 1
fi


# FIXME: do everything to properly install inline java
# have JAVA_HOME correctly asserted

# INSTALL Inline::Java

export TMP_PERL_MM_OPT=$PERL_MM_OPT
export PERL_MM_OPT=

if ! perldoc -l Text::Quote; then
    $CPANM_COMMAND "cpanm $CPANMADDITIONAL --force Text::Quote"
fi

export PERL_MM_OPT=$TMP_PERL_MM_OPT

if ! perldoc -l Text::Quote; then
    echo "ERROR: Text::Quote did not install"
    exit 1
fi


export TMP_PERL_MM_OPT=$PERL_MM_OPT
export PERL_MM_OPT=

if ! perldoc -l Tk::GraphViz; then
    $CPANM_COMMAND "cpanm $CPANMADDITIONAL --force Tk::GraphViz"
fi

export PERL_MM_OPT=$TMP_PERL_MM_OPT

if ! perldoc -l Tk::GraphViz; then
    echo "ERROR: Tk::GraphViz did not install"
    exit 1
fi

export TMP_PERL_MM_OPT=$PERL_MM_OPT
export PERL_MM_OPT=

export JAVA_HOME=/usr/lib/jvm/java-7-openjdk-amd64

if ! perldoc -l Inline::Java; then
    cd /tmp && tar xzf $DATA_DIR/frdcsa-misc/Inline-Java-0.53.tar.gz
    cd Inline-Java-0.53
    perl Makefile.PL J2SDK=$JAVA_HOME
    make
    make install    
fi

export PERL_MM_OPT=$TMP_PERL_MM_OPT

if ! perldoc -l Inline::Java; then
    echo "ERROR: Inline::Java did not install"
    exit 1
fi

export TMP_PERL_MM_OPT=$PERL_MM_OPT
export PERL_MM_OPT=

if ! perldoc -l AI::Prolog; then
    $CPANM_COMMAND "cpanm $CPANMADDITIONAL --force AI::Prolog"
fi

export PERL_MM_OPT=$TMP_PERL_MM_OPT

if ! perldoc -l AI::Prolog; then
    echo "ERROR: AI::Prolog did not install"
    exit 1
fi

cd /var/lib/myfrdcsa/codebases/minor/spse

if ! perldoc -l Graph::Directed; then
    su $USER -c "/var/lib/myfrdcsa/codebases/minor/package-installation-manager/scripts/install-cpan-modules Graph::Directed"
fi
if ! perldoc -l Graph::Directed; then
    echo "ERROR: Graph::Directed did not install"
    exit 1
fi

if ! perldoc -l Ubigraph; then
    su $USER -c "/var/lib/myfrdcsa/codebases/minor/package-installation-manager/scripts/install-cpan-modules Ubigraph"
fi
if ! perldoc -l Ubigraph; then
    echo "ERROR: Ubigraph did not install"
    exit 1
fi

if ! perldoc -l Cal::DAV; then
    su $USER -c "/var/lib/myfrdcsa/codebases/minor/package-installation-manager/scripts/install-cpan-modules Cal::DAV"
fi
if ! perldoc -l Cal::DAV; then
    echo "ERROR: Cal::DAV did not install"
    exit 1
fi

if ! perldoc -l Net::Google::Calendar; then
    su $USER -c "/var/lib/myfrdcsa/codebases/minor/package-installation-manager/scripts/install-cpan-modules Net::Google::Calendar"
fi
if ! perldoc -l Net::Google::Calendar; then
    echo "ERROR: Net::Google::Calendar did not install"
    exit 1
fi

# su $USER -c "/etc/init.d/unilang start"
/etc/init.d/unilang start
sleep $UNILANG_START_DURATION

## FIXME: Andy [2022-04-04,09:53:10]

# echo "Starting Test-use"
# su $USER -c "source $THE_SOURCE && cd /var/lib/myfrdcsa/codebases/minor/spse/scripts/ && NONINTERACTIVE=true $INSTALL_SCRIPT_DEPENDENCIES ./test-use.pl"

# if ! /var/lib/myfrdcsa/codebases/internal/freekbs2/scripts/kbs2 -l | grep -q Org::FRDCSA::Verber::PSEx2::Do; then
#     # NOTE THIS must be preceded by $INSTALL_SCRIPT_DEPENDENCIES ./test-use.pl
#     su $USER -c "source $THE_SOURCE && cd /var/lib/myfrdcsa/codebases/internal/freekbs2/scripts && NONINTERACTIVE=true $INSTALL_SCRIPT_DEPENDENCIES \"./kbs2 -y -c Org::FRDCSA::Verber::PSEx2::Do fast-import /var/lib/myfrdcsa/codebases/minor/spse/kbs/do2.kbs\""
# fi
# if ! /var/lib/myfrdcsa/codebases/internal/freekbs2/scripts/kbs2 -l | grep -q Org::FRDCSA::Verber::PSEx2::Do; then
#     echo "ERROR: Org::FRDCSA::Verber::PSEx2::Do did not load"
#     exit 1
# fi

# # do something about having to constantly close the GUI to get
# # this to work, add some kill switch inside or preinstall the
# # modules.

# if ! /var/lib/myfrdcsa/codebases/internal/freekbs2/scripts/kbs2 -l | grep -q Org::PICForm::PIC::Vis::Metadata; then
#     cd /var/lib/myfrdcsa/codebases/minor/spse/kbs
#     echo y | /var/lib/myfrdcsa/codebases/internal/freekbs2/scripts/kbs2 -c Org::PICForm::PIC::Vis::Metadata fast-import metadata.kbs
# fi
# if ! /var/lib/myfrdcsa/codebases/internal/freekbs2/scripts/kbs2 -l | grep -q Org::PICForm::PIC::Vis::Metadata; then
#     echo "ERROR: Org::PICForm::PIC::Vis::Metadata did not load"
#     exit 1
# fi

# # if ! /var/lib/myfrdcsa/codebases/internal/freekbs2/scripts/kbs2 -l | grep -q Org::FRDCSA::Verber::PSEx2::Do; then
# #     cd /var/lib/myfrdcsa/codebases/minor/spse/kbs
# #     echo y | /var/lib/myfrdcsa/codebases/internal/freekbs2/scripts/kbs2 -c Org::FRDCSA::Verber::PSEx2::Do fast-import do2.kbs
# # fi
# # if ! /var/lib/myfrdcsa/codebases/internal/freekbs2/scripts/kbs2 -l | grep -q Org::FRDCSA::Verber::PSEx2::Do; then
# #     echo "ERROR: Org::FRDCSA::Verber::PSEx2::Do did not load"
# #     exit 1
# # fi

# echo "Starting SPSE2"
# su $USER -c "source $THE_SOURCE && cd /var/lib/myfrdcsa/codebases/minor/spse && XAUTHORITY=/home/$USER/.Xauthority NONINTERACTIVE=true $INSTALL_SCRIPT_DEPENDENCIES \"spse2 -c Org::FRDCSA::Verber::PSEx2::Do -W 10000\""
# # update this patch to work with the latest version of Tk::GraphViz
# # FIXME: copy the modified patch to the gitroot, along with other modifications
cd /usr/local/share/perl
# if [ ! -f /usr/local/share/perl/5.20.2/Tk/GraphViz.pm ] || ! grep -q 'push @{$self->{layout}}, join("",@item);' /usr/local/share/perl/5.20.2/Tk/GraphViz.pm; then 
#     patch -p0 -i /var/lib/myfrdcsa/codebases/minor/spse/Tk-GraphViz.pm.patch
# fi
# if [ ! -f /usr/local/share/perl/5.20.2/Tk/GraphViz.pm ] || ! grep -q 'push @{$self->{layout}}, join("",@item);' /usr/local/share/perl/5.20.2/Tk/GraphViz.pm; then 
#     echo "ERROR: couldn't patch Tk::GraphViz"
#     exit 1
# fi

export TMP_PERL_MM_OPT=$PERL_MM_OPT
export PERL_MM_OPT=

if ! perldoc -l Tk::Month; then
    $CPANM_COMMAND "cpanm $CPANMADDITIONAL -f Tk::Month"
fi
export PERL_MM_OPT=$TMP_PERL_MM_OPT

if ! perldoc -l Tk::Month; then
    echo "ERROR: didn't install Tk::Month correctly"
    exit 1
fi

# echo "Starting SPSE2"
# su $USER -c "source $THE_SOURCE && cd /var/lib/myfrdcsa/codebases/minor/spse && NONINTERACTIVE=true $INSTALL_SCRIPT_DEPENDENCIES \"spse2 -c Org::FRDCSA::Verber::PSEx2::Do -W 20000\""

# # su $USER -c "/etc/init.d/unilang stop"
# /etc/init.d/unilang stop
# killall start unilang unilang-client

su $USER -c "source $THE_SOURCE && cd /var/lib/myfrdcsa/codebases/internal/boss && NONINTERACTIVE=true $INSTALL_SCRIPT_DEPENDENCIES \"./boss\""
su $USER -c "mkdir -p /var/lib/myfrdcsa/codebases/internal/boss/data/namazu"


# sudo netstat -tulpn
# start unilang

# # figure out how to get the script files into the path

# #### only do this if we didn't install a private install

if [ $ARCH == "x86_64" ]; then
    # dpkg --add-architecture ia32
    dpkg --add-architecture i386
fi

# # get FRDCSA Applet working
if ! [ -f "/var/lib/myfrdcsa/codebases/minor/frdcsa-dashboard/data/scater.gif" ]; then
    # scp -r andrewdo@justin.frdcsa.org:/var/lib/myfrdcsa/codebases/minor/frdcsa-dashboard/data .
    # su $USER -c "mkdir /var/lib/myfrdcsa/sandbox/opencyc-4.0"
    cd /var/lib/myfrdcsa/codebases/minor/frdcsa-dashboard/data/
    su $USER -c "cp -ar $DATA_DIR/frdcsa-misc/frdcsa-dashboard/data/* ."
fi
if ! [ -f "/var/lib/myfrdcsa/codebases/minor/frdcsa-dashboard/data/scater.gif" ]; then
    echo "ERROR: Did not properly copy frdcsa-dashboard"
    exit 1
fi

# sudo apt-get install gtk2.0

su $USER -c "source $THE_SOURCE && cd /var/lib/myfrdcsa/codebases/minor/frdcsa-dashboard/ && NONINTERACTIVE=true $INSTALL_SCRIPT_DEPENDENCIES \"./frdcsa-applet -W\""

# put the stuff in /etc/init.d
# @ (frdcsa-applet &) <- as of Sun Sep 14 21:58:06 CDT 2014, not sure what this is about

# for corpus

if [ ! -d "/var/lib/myfrdcsa/sandbox/meteor-0.6" ]; then
    su $USER -c "mkdir /var/lib/myfrdcsa/sandbox/meteor-0.6"
    cd /var/lib/myfrdcsa/sandbox/meteor-0.6
    su $USER -c "cp -ar $DATA_DIR/frdcsa-misc/meteor-0.6 ."
fi
if [ ! -d "/var/lib/myfrdcsa/sandbox/meteor-0.6" ]; then
    echo "ERROR: Did not copy meteor-0.6 properly"
    exit 1
fi

# # fix the username for this file, or fix the whole concept

# copy ?the?
# add frdcsa-applet to the startup applications


# FIXME: run unilang tests and report on success

# # # sudo shutdown -r now

cd /var/lib/myfrdcsa/
mkdir -p datasets
chown $USER.$GROUP datasets

su $USER -c "mkdir -p /var/lib/myfrdcsa/codebases/minor/corpus-manager/data/corpora/gutenberg"

if [ ! -d "/usr/local/include/link-grammar" ]; then
    cp -ar /usr/include/link-grammar/ /usr/local/include
fi
if [ ! -d "/usr/local/include/link-grammar" ]; then
    echo "ERROR: link-grammar did not copy"
    exit 1
fi

export TMP_PERL_MM_OPT=$PERL_MM_OPT
export PERL_MM_OPT=

if ! perldoc -l Lingua::LinkParser; then
    $CPANM_COMMAND "cpanm $CPANMADDITIONAL -f Lingua::LinkParser"
fi
export PERL_MM_OPT=$TMP_PERL_MM_OPT

# if ! perldoc -l Lingua::LinkParser; then
#     echo "ERROR: didn't install Lingua::LinkParser correctly"
#     exit 1
# fi

su $USER -c "mkdir -p /var/lib/myfrdcsa/codebases/external"

if ! [ -d "/var/lib/myfrdcsa/sandbox/termex-1.49/termex-1.49" ]; then
    su $USER -c "mkdir /var/lib/myfrdcsa/sandbox/termex-1.49"
    cd /var/lib/myfrdcsa/sandbox/termex-1.49
    su $USER -c "cp -ar $DATA_DIR/frdcsa-misc/termex-1.49 ."
    cd termex-1.49
    su $USER -c "perl Makefile.PL"
    make install
fi
if ! [ -d "/var/lib/myfrdcsa/sandbox/termex-1.49/termex-1.49" ]; then
    echo "ERROR: termex did not install"
    exit 1
fi

if ! [ -d "/var/lib/myfrdcsa/sandbox/stanford-ner-20080306/stanford-ner-20080306" ]; then
    su $USER -c "mkdir /var/lib/myfrdcsa/sandbox/stanford-ner-20080306"
    cd /var/lib/myfrdcsa/sandbox/stanford-ner-20080306
    su $USER -c "cp -ar $DATA_DIR/frdcsa-misc/stanford-ner-20080306 ."
fi
if ! [ -d "/var/lib/myfrdcsa/sandbox/stanford-ner-20080306/stanford-ner-20080306" ]; then
    echo "ERROR: stanford-ner-20080306 did not copy"
    exit 1
fi

if ! [ -d "/var/lib/myfrdcsa/sandbox/stanford-parser-20110627/stanford-parser-20110627" ]; then
    su $USER -c "mkdir /var/lib/myfrdcsa/sandbox/stanford-parser-20110627"
    cd /var/lib/myfrdcsa/sandbox/stanford-parser-20110627
    su $USER -c "cp -ar $DATA_DIR/frdcsa-misc/var/lib/myfrdcsa/sandbox/stanford-parser-20110627 ."
fi
if ! [ -d "/var/lib/myfrdcsa/sandbox/stanford-parser-20110627/stanford-parser-20110627" ]; then
    echo "ERROR: stanford-parser-20110627 did not copy"
    exit 1
fi


if ! [ -d "/var/lib/myfrdcsa/sandbox/montylingua-2.1/montylingua-2.1" ]; then
    su $USER -c "mkdir /var/lib/myfrdcsa/sandbox/montylingua-2.1"
    cd /var/lib/myfrdcsa/sandbox/montylingua-2.1
    su $USER -c "cp -ar $DATA_DIR/frdcsa-misc/montylingua-2.1 ."
fi
if ! [ -d "/var/lib/myfrdcsa/sandbox/montylingua-2.1/montylingua-2.1" ]; then
    echo "ERROR: MontyLingua 2.1 did not copy"
    exit 1
fi

# FIXME: do we need to add a cabinet here?
su $USER -c "source $THE_SOURCE && cd /var/lib/myfrdcsa/codebases/minor/paperless-office && NONINTERACTIVE=true $INSTALL_SCRIPT_DEPENDENCIES \"./paperless-office -W\""
su $USER -c "/var/lib/myfrdcsa/codebases/minor/package-installation-manager/scripts/install-cpan-modules Module::Build"
su $USER -c "/var/lib/myfrdcsa/codebases/minor/package-installation-manager/scripts/install-cpan-modules WWW::Mechanize::Cached"
su $USER -c "/var/lib/myfrdcsa/codebases/minor/package-installation-manager/scripts/install-cpan-modules -f Yahoo::Search"
su $USER -c "source $THE_SOURCE && cd /var/lib/myfrdcsa/codebases/minor/workhorse/scripts/ && NONINTERACTIVE=true $INSTALL_SCRIPT_DEPENDENCIES \"./process-corpus.pl -W\""
su $USER -c "/var/lib/myfrdcsa/codebases/minor/package-installation-manager/scripts/install-cpan-modules Archive::Zip"
su $USER -c "source $THE_SOURCE && cd /var/lib/myfrdcsa/codebases/internal/corpus && NONINTERACTIVE=true $INSTALL_SCRIPT_DEPENDENCIES \"./corpus -h\""

# for clear
if [ ! -d "/etc/clear" ]; then
    cd /etc
    cp -ar $DATA_DIR/frdcsa-misc/etc/clear .
fi
if [ ! -d "/etc/clear" ]; then
    echo "ERROR: /etc/clear did not copy"
    exit 1
fi  

if [ ! -d /usr/local/WordNet-3.0/dict ]; then
    mkdir -p /usr/local/WordNet-3.0/dict/
fi
if [ ! -d /usr/local/WordNet-3.0/dict ]; then
    echo "ERROR: didn't create /usr/local/WordNet-3.0/dict"
fi
if [ ! -h /usr/local/WordNet-3.0/dict/data.noun ]; then
    for it in `ls /usr/share/wordnet`; do sudo ln -s /usr/share/wordnet/$it /usr/local/WordNet-3.0/dict$it ; done
    for it in `ls /usr/share/wordnet`; do sudo ln -s /usr/share/wordnet/$it /usr/local/WordNet-3.0/dict/ ; done 
fi
if [ ! -h /usr/local/WordNet-3.0/dict/data.noun ]; then
    echo "ERROR: didn't create symlinks /usr/local/WordNet-3.0/* or /usr/local/WordNet-3.0/dict/*"
fi

$CPANM_COMMAND "cpanm $CPANMADDITIONAL WordNet::Tools WordNet::SenseRelate::AllWords WordNet::QueryData "

# su $USER -c "source $THE_SOURCE && cd /var/lib/myfrdcsa/codebases/minor/nlu/systems/annotation && NONINTERACTIVE=true USE_FORCE_YES=--force-yes $INSTALL_SCRIPT_DEPENDENCIES \"./process-2.pl -W\""

# su $USER -c "source $THE_SOURCE && cd /var/lib/myfrdcsa/codebases/internal/clear && NONINTERACTIVE=true USE_FORCE_YES=--force-yes $INSTALL_SCRIPT_DEPENDENCIES \"cla -r /var/lib/myfrdcsa/codebases/minor/action-planner/OConnor.pdf -W\""

echo "Stopping UniLang"

# su $USER -c "/etc/init.d/unilang stop"
/etc/init.d/unilang stop
killall start unilang unilang-client

# generally useful but optional

apt-file update
update-dlocatedb
updatedb
su $USER -c "source $THE_SOURCE && cd /var/lib/myfrdcsa/codebases/internal/boss/ && ./boss updatedb -y"
su $USER -c "source $THE_SOURCE && cd /var/lib/myfrdcsa/codebases/internal/boss/ && ./boss etags -y"

echo "Finished FRDCSA Install"

#####################################################################################

# TODO

# replace all instances of cpan installation with cpanm (including in
# helper applications)

# factor out a version of the install which primarily uses
# $INSTALL_SCRIPT_DEPENDENCIES in order to generate a more efficient
# installer using apt-get and cpanm, in order to avoid installing
# packages that are no longer required, and periodically build the
# efficient installer.



# FIXME: get the patch working

# now need to copy over all files related to getting various
# textanalysis items working, such as montylingua, etc

# areas to improve

# the .myconfig stuff
# the database copying and installation
# the installation of SPSE2-related perl modules

# cd /var/lib/myfrdcsa/codebases/internal/verber/data
# scp -r andrewdo@justin.frdcsa.org:/var/lib/myfrdcsa/codebases/internal/verber/data/ .

# get academician
# verber
# NONINTERACTIVE=true $INSTALL_SCRIPT_DEPENDENCIES /var/lib/myfrdcsa/codebases/internal/verber


# FIXME: note that we have to figure out with the installer what must
#  have gone wrong that it was not able to do 'kbs2 -l' using
#  /etc/init.d/unilang's unilang

# FIXME: also figure  out how we can get the  latest perl version and
#  update the patch to match - or just submit the patch





# # copy over sandbox

# sudo rsync -av --progress root@ai.frdcsa.org:/var/lib/myfrdcsa/sandbox/ /var/lib/myfrdcsa/sandbox/

# # copy over external
# git clone ssh://andrewdo@ai.frdcsa.org/Meredith/gitroot/external


# # copy over collections
# git clone ssh://andrewdo@ai.frdcsa.org/Meredith/gitroot/collections

# download swipl
# $INSTALL_SCRIPT_DEPENDENCIES "/var/lib/myfrdcsa/codebases/internal/packager/packager swipl-8.1.11.tar.gz"


# sudo apt-get install \
#      build-essential cmake ninja-build pkg-config \
#      ncurses-dev libreadline-dev libedit-dev \
#      libunwind-dev \
#      libgmp-dev \
#      libssl-dev \
#      unixodbc-dev \
#      zlib1g-dev libarchive-dev \
#      libossp-uuid-dev \
#      libxext-dev libice-dev libjpeg-dev libxinerama-dev libxft-dev \
#      libxpm-dev libxt-dev \
#      libdb-dev \
#      libpcre3-dev \
#      libyaml-dev \
#              openjdk-11-jdk junit

# cd /var/lib/myfrdcsa/sandbox/swipl-8.1.11/swipl-8.1.11/

# mkdir build
# cd build
# cmake -DCMAKE_INSTALL_PREFIX=/usr/local ..
# make
# ctest -j 4
# sudo make install


# /var/lib/myfrdcsa/codebases/minor/formalog

# (
#  $CPANM_COMMAND "cpanm --reinstall --look Language::Prolog::Yaswi"
#  patch -p1 < /var/lib/myfrdcsa/codebases/minor/formalog/Language-Prolog-Yaswi-0.21-Adjust-list-parsing-to-SWI-Prolog-7.patch 
#  export PERL_MM_OPT=
#  # edit Low/Makefile.PL and replace @plvars=`$pl -dump-runtime-variables`; with @plvars=`$pl --dump-runtime-variables=sh`;
#  perl Makefile.PL
#  make
#  make uninstall
#  # manually clear out the entries
#  make install
#  # meet your uncle Bob
#  )


# cd /var/lib/myfrdcsa/codebases/minor/formalog
# $INSTALL_SCRIPT_DEPENDENCIES "./start -u"


# cd /var/lib/myfrdcsa/codebases/minor/fcms
# git clone ssh://andrewdo@posi.frdcsa.org/gitroot/FCMS

# # edit begin-fcms, add your system in there
# # copy begin-frdcsa to begin-<SYSTEM>, edit the IP address and anything else


# $CPANM_COMMAND "cpanm -f Net::Server"

# comment out this:
# /var/lib/myfrdcsa/codebases/minor/fcms/FCMS/lib/ShinyCMS.pm

# __PACKAGE__->apply_request_class_roles
#   (qw/
#        Catalyst::TraitFor::Request::ProxyBase
#      /);

# edit begin-optimus, put /usr/bin/timeout 5s in front
# $INSTALL_SCRIPT_DEPENDENCIES ./begin-optimus



# now set up the database

# create user 'shinycms_$FLPNAME'@'localhost' identified by '$PASSWD';
# grant all privileges on shinycms_$FLPNAME.* to 'shinycms_$FLPNAME'@'localhost';
# flush privilegse;

# $CPANM_COMMAND "cpanm MooseX/NonMoose.pm"

# DBIx/Class/TimeStamp.pm

# Cookie::Baker::XS

# WWW::Form::UrlEncoded::XS 


# remove the timeout 5s from it

# pack_install(regex).
# pack_install(julian).
# pack_install(genutils).

# mkdir /var/lib/myfrdcsa/codebases/cats
# mkdir /var/lib/myfrdcsa/codebases/internal/akahige/data/people


# cd /var/lib/myfrdcsa/codebases/internal/freekbs2/data-git/
# git clone ssh://andrewdo@ai.frdcsa.org/var/lib/myfrdcsa/codebases/internal/freekbs2/data-git/MyKBs


# # add this to site-wrapper.tt

# [%- ELSIF c.config.site_name == 'Optimus' -%]
#     [%- INCLUDE 'sites/freelifeplanner/site-wrapper.tt' -%]

# $CPANM_COMMAND "cpanm Template/Plugin/DBI.pm"
# sudo apt-get install elpa-ace-window


# $CPANM_COMMAND "cpanm Catalyst::TraitFor::Request::ProxyBase"

# fix verber and verber/data symlinks

# cd /var/lib/myfrdcsa/codebases/internal/verber && $INSTALL_SCRIPT_DEPENDENCIES "./verber -p LPG -w sleep/3/sleep --skip-templates"

# $CPANM_COMMAND "cpanm --force Tk::GraphViz::parseRecordLabel "

# sudo dpkg --add-architecture i386
# sudo apt-get update
# sudo apt-get dist-upgrade
# sudo apt-get install libc6:i386
# sudo apt-get install -f


# for candc-1.00
# install libreadline.so.5:i386, then 
# ln -s /lib/i386-linux-gnu/libreadline.so.5 /lib/i386-linux-gnu/libreadline.so.4


# fix the PerlTK hacks in SPSE2 and PaperlessOffice libraries

# add perlbrew install

# use venv as needed




################################ FIXME Add to proper place


# cd /var/lib/myfrdcsa/codebases/internal/myfrdcsa/bin && install-script-dependencies--perlbrew execution-engine

# # create user 'andrewdo'@'localhost' identified by '';yes

# # grant all privileges on *.* to 'andrewdo'@'localhost';
# # flush privileges;

# cpanm Throwable
# cpanm -f Inline::Java

# apt-get build-dep libimage-magick-perl

# (imagemagick install instructions:
#  (download https://download.imagemagick.org/ImageMagick/download/ImageMagick.tar.gz)
#  (extract, follow install instructions: ./configure: make: make install)
#  (cpanm Image::Magick)
#  (sudo /usr/sbin/ldconfig /usr/local/lib)
# )


# cpanm String::Random
# cpanm OpenGL
# cpanm OpenGL::Image

# libxi-dev
# libxmu-dev


# cd /var/lib/myfrdcsa/codebases/minor/sensor-network/scripts/ && install-script-dependencies--perlbrew ./sensor-network.pl


# swipl
# pack_install('regex').
# pack_install('julian').
# pack_install('genutils').


# scp -r andrewdo@10.0.0.104:/var/lib/myfrdcsa/sandbox/javapengine-20200506/ /var/lib/myfrdcsa/sandbox
# cd /var/lib/myfrdcsa/codebases/minor/formalog/scripts/ && ./test.pl

# scp -r andrewdo@10.0.0.104:/home/andrewdo/.m2/ ~/.m2/

# mkdir -p /var/lib/myfrdcsa/codebases/cats
# mkdir -p /var/lib/myfrdcsa/codebases/internal/akahige/data/people

# scp -r andrewdo@10.0.0.104:/var/lib/myfrdcsa/sandbox/prolog-pddl-3-0-parser-20140825/ /var/lib/myfrdcsa/sandbox/


# sudo apt-get install elpa-helm
