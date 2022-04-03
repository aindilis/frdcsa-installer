su root
/sbin/adduser andrewdo sudo
scp -r <REDACTED>:/var/lib/myfrdcsa/codebases/minor/frdcsa-installer/frdcsa-installer/ .
sudo nano /etc/apt/sources.list
sudo apt-get update
scp <REDACTED>:.Xresources .
xrdb -merge .Xresources
# log into terms again
gnome-tweaks
# Keyboard and Mouse -> Additional Layout Options -> Ctrl Position -> Swap Ctrl and Caps Lock
echo "escape ^]^]" > .screenrc
sudo cp .screenrc /root
sudo chown root.root /root/.screenrc
sudo apt-get install emacs screen

# do the frdcsa-installer-20... stuff

sudo apt-get install sshfs

# fusermount: option allow_other only allowed if 'user_allow_other' is set in /etc/fuse.conf
# edit /etc/fuse.conf
./mount-<REDACTE>2-filesystems.sh

sudo apt-get install perlbrew

# modify .profile append:
# source ~/perl5/perlbrew/etc/bashrc

perlbrew install-patchperl
perlbrew init
perlbrew -v install perl-5.35.10

# gnome-control-center
# Power -> Power Saving = Black screen
# Power -> Suspend & Power Button = Off


perlbrew switch perl-5.35.10
cpan -i App::cpanminus
