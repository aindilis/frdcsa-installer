cpanm --look Crypt::SSLeay

patch -p0 < <PATH_TO>/Use_TLS_client_method-with-OpenSSL-1.1.1.patch

edit Makefile.PL

# use inc::IO::Interactive::Tiny;
use lib 'inc';
use IO::Interactive::Tiny;

perl Makefile.PL
make
make test
make install


(make a script for setting up git repo readonly on git server)
