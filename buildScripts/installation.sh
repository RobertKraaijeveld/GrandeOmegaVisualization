#!/bin/bash

echo 'export PATH=\"$HOME/.rbenv/bin:$PATH\"' >> ~/.bashrc        
export PATH=$PATH:/usr/local/bin:$HOME/.rbenv/bin:$HOME/.rbenv/shims
eval "$(rbenv init -)"

rbenv local
rbenv rehash
RUBY_CONFIGURE_OPTS=--enable-shared rbenv install 2.2.2 

sudo gem install rails
sudo gem install rice
