#!/usr/bin/env bash

# See <http://perltricks.com/article/57/2014/1/1/Shazam-Use-Image-Magick-with-Perlbrew-in-minutes>

# may need to have the perl configured with -Duseshrplib
#
#     perl -V | grep -- '-Duseshrplib'

# compile perl with perlbrew:
#
#     perlbrew install $PERL_VERSION_HERE -D useshrplib
#
# may need to --force
#
# see <https://github.com/gugod/App-perlbrew/issues/131>


# TOP="$HOME/local"
# 
# if [ -n "$1" ]; then
#  TOP=$1
# fi
# 
# mkdir -p $TOP
# cd "$TOP"
# wget -c http://www.imagemagick.org/download/ImageMagick.tar.gz
# tar xzvf ImageMagick.tar.gz
# cd ImageMagick* # this isn't exactly clean

cd /home/andrewdo/local/ImageMagick-7.0.11-4

export TOP="$HOME/local"
export PERL_CORE=$(perl -e 'print grep { -d } map { "$_/CORE" } @INC')
export PERL_BIN=$(which perl)

export PERL_THREADS=$(perl -V | grep -c 'useithreads=define')

export THREAD_FLAG="--with-threads"

if [ $PERL_THREADS = 0 ]; then
    export THREAD_FLAG="--without-threads"
fi

echo LDFLAGS=-L$PERL_CORE \
    ./configure --prefix $TOP \
    --with-perl=$PERL_BIN \
    --enable-shared $THREAD_FLAG

LDFLAGS=-L$PERL_CORE \
    ./configure --prefix $TOP \
    --with-perl=$PERL_BIN \
    --enable-shared $THREAD_FLAG

make install
