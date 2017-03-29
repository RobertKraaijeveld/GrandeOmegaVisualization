git clone https://github.com/rbenv/rbenv.git ~/.rbenv 
echo 'export PATH=\"$HOME/.rbenv/bin:$PATH\"' >> ~/.bashrc 
     
git clone https://github.com/rbenv/ruby-build.git ~/.rbenv/plugins/ruby-build 

RUBY_CONFIGURE_OPTS=--enable-shared rbenv install 2.2.2 

sudo gem install rails
sudo gem install rice
