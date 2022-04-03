# ensure mysql-backup is present on <REDACTED>X.frdcsa.org

sudo apt-get update
sudo apt-get upgrade

# gnome-tweaks (swap caps and control)
# add ```<IP> <REDACTED>.frdcsa.org <REDACTED>``` to /etc/hosts
cd /home/andrewdo
sudo apt-get install emacs screen git curl
git clone git+ssh://<REDACTED>.frdcsa.org/gitroot/frdcsa-installer
cp frdcsa-installer/Xresources ~/.Xresources
echo "escape ^]^]" > ~/.screenrc

# install perlbrew
curl -L https://install.perlbrew.pl | bash
# add ```source ~/perl5/perlbrew/etc/bashrc``` to end of profile.pl

# # Log out and log back in?
# now install perl with perlbrew (with -Dthreads and  -Duseshrplib

perlbrew init
perlbrew available
# set this to appropriate
export VERSION="5.35.10"
perlbrew list
sudo apt-get install build-essential
perlbrew install -v perl-$VERSION -Dusethreads -Duseshrplib --as perl-"$VERSION"_WITH_THREADS
perlbrew switch perl-"$VERSION"_WITH_THREADS
cpan -i App::cpanminus
# # do manual, don't use local::lib or the other one
# # install ~/.bashrc stuff
# install Language::Prolog::Yaswi

# # update /etc/hosts <- <REDACTED>.frdcsa.org <REDACTED>.frdcsa.org <REDACTED>.frdcsa.org

# # sudo rsync -av --progress root@<REDACTED>.frdcsa.org:/var/lib/myfrdcsa/sandbox/ sandbox/
# # sudo rsync -av --progress root@<REDACTED>.frdcsa.org:/var/lib/myfrdcsa/codebases/data/ data/

# git clone git+ssh://<REDACTED>.frdcsa.org/var/lib/myfrdcsa/capabilities
