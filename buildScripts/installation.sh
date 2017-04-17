#!/bin/bash

echo "Installing rbenv and ruby 2.2.2 with enable shared"

sudo apt-get update
sudo apt-get install autoconf bison build-essential libssl-dev libyaml-dev libreadline6-dev zlib1g-dev libncurses5-dev libffi-dev libgdbm3 libgdbm-dev


git clone https://github.com/rbenv/rbenv.git ~/.rbenv
echo 'export PATH="$HOME/.rbenv/bin:$PATH"' >> ~/.bashrc
echo 'eval "$(rbenv init -)"' >> ~/.bashrc
source ~/.bashrc

git clone https://github.com/rbenv/ruby-build.git ~/.rbenv/plugins/ruby-build

export RUBY_CONFIGURE_OPTS="--enable-shared"
rbenv install 2.2.2
rbenv global 2.2.2

gem install rails
gem install rice

echo "Installing postgres"
sudo apt-get update
sudo apt-get install postgresql postgresql-contrib

echo "Installing cpp libs libpqxx, lib-cppunit"
sudo apt-get install libpqxx-dev
sudo apt-get install lib-cppunit


