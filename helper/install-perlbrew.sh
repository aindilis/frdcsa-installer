sudo apt-get upgrade
sudo apt-get update
sudo apt-get install curl
curl -L https://install.perlbrew.pl | bash
# # add to .profile: 'source ~/perl5/perlbrew/etc/bashrc'
# # restart shell
perlbrew init
perlbrew available
perlbrew list
sudo apt-get install build-essential
perlbrew install -v perl-5.35.10 -Dusethreads --as perl-5.35.10_WITH_THREADS
perlbrew switch perl-5.35.10_WITH_THREADS
cpan -i App::cpanminus
# # do manual, don't use local::lib or the other one
# # install ~/.bashrc stuff
# install Language::Prolog::Yaswi

# # update /etc/hosts <- <REDACTED>2.frdcsa.org <REDACTED>.frdcsa.org <REDACTED>.frdcsa.org
