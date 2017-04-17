sudo fallocate -l 1G /swapfile
ls -lh /swapfile
sudo chmod 600 /swapfile
ls -lh /swapfile
sudo mkswap /swapfile
free
sudo swapon /swapfile
free
sudo cp /etc/fstab /etc/fstab.bak
echo '/swapfile none swap sw 0 0' | sudo tee -a /etc/fstab
free
ls
cd opencv_contrib/
make -j5
mkdir ./ssh
chmod 700 .ssh
cd ./ssh/
ls
cd ..
ls
git clone https://github.com/opencv/opencv.git
cd opencv/
ls
mkdir release
cd release
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local ..
make
sudo make install
cd ../../
ls
git clone https://github.com/opencv/opencv_contrib.git
cd opencv_contrib/
cmake -DOPENCV_EXTRA_MODULES_PATH=/modules <opencv_source_directory>
pwd
cmake -DOPENCV_EXTRA_MODULES_PATH=/home/deploy/opencv_contrib/modules /home/deploy/opencv
make -j5
ls
cd opencv_contrib/
make -j5
sudo apt-get update
sudo apt-get install -y curl gnupg build-essential
sudo gpg --keyserver hkp://keys.gnupg.net --recv-keys 409B6B1796C275462A1703113804BB82D39DC0E3
$ curl -sSL https://get.rvm.io | sudo bash -s stable
if sudo grep -q secure_path /etc/sudoers; then sudo sh -c "echo export rvmsudo_secure_path=1 >> /etc/profile.d/rvm_secure_path.sh" && echo Environment variable installed; fi
rvm install ruby
$ ~.
rvm install ruby
rvm install ruby-2.3.0
sudo rvm install ruby-2.3.0
rvm install ruby-2.3.0
sudo apt-get install curl git-core nginx -y
gpg --keyserver hkp://keys.gnupg.net --recv-keys 409B6B1796C275462A1703113804BB82D39DC0E3
curl -sSL https://get.rvm.io | bash -s stable
sudo curl -sSL https://get.rvm.io | bash -s stable
source  ~/.rvm/scripts/rvm
source ~/.rvm/scripts/rvm
cd
wget http://ftp.ruby-lang.org/pub/ruby/2.4/ruby-2.4.0.tar.gz
tar -xzvf ruby-2.4.0.tar.gz
cd ruby-2.4.0/
./configure
ruby --v
ruby
cd ..
ls
rm -rf ruby-2.4.0 ruby-2.4.0.tar.gz 
ls
aptitude purge ruby
sudo apt-get install libgdbm-dev libncurses5-dev automake libtool bison libffi-dev
gpg --keyserver hkp://keys.gnupg.net --recv-keys 409B6B1796C275462A1703113804BB82D39DC0E3
curl -sSL https://get.rvm.io | bash -s stable
sudo curl -sSL https://get.rvm.io | bash -s stable
su -
cd ..
ls
cd home/
s
ls
cd deploy/
ls
ruby -v
~.
ruby -v
cd ..
cd
git clone https://github.com/rbenv/rbenv.git ~/.rbenv
echo 'export PATH="$HOME/.rbenv/bin:$PATH"' >> ~/.bashrc
echo 'eval "$(rbenv init -)"' >> ~/.bashrc
exec $SHELL
git clone https://github.com/rbenv/ruby-build.git ~/.rbenv/plugins/ruby-build
echo 'export PATH="$HOME/.rbenv/plugins/ruby-build/bin:$PATH"' >> ~/.bashrc
exec $SHELL
rbenv install 2.3.0
rbenv global  2.3.0
ruby --v
ruby -v
