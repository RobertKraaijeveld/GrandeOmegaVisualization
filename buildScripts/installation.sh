#!/bin/bash

echo 'export PATH=\"$HOME/.rbenv/bin:$PATH\"' >> ~/.bashrc        
export PATH=$PATH:/usr/local/bin:$HOME/.rbenv/bin:$HOME/.rbenv/shims
eval "$(rbenv init -)"

#gem install rails
#gem install rice
