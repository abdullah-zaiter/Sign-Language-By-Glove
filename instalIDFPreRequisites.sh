#!/bin/sh

sudo apt-get update  # To get the latest package lists

#install prerequisites
sudo apt-get install git wget libncurses-dev flex bison gperf python python-pip python-setuptools cmake ninja-build ccache libffi-dev libssl-dev -y
sudo apt-get install git wget flex bison gperf python python-pip python-setuptools python-serial python-click python-cryptography python-future python-pyparsing python-pyelftools cmake ninja-build ccache
#Setting up Python 3 as default for Ubuntu and Debian
sudo apt-get install python3 python3-pip python3-setuptools

#Making Python 3 the default interpreter is possible by running:
sudo update-alternatives --install /usr/bin/python python /usr/bin/python3 10


# Install Esp IDF v3.2 from git
cd ~/
mkdir esp
cd ~/esp
git clone -b release/v3.2 --recursive https://github.com/espressif/esp-idf.git

cd ~/esp
wget https://dl.espressif.com/dl/xtensa-esp32-elf-linux64-1.22.0-80-g6c4433a-5.2.0.tar.gz
tar -xzf ./xtensa-esp32-elf-linux64-1.22.0-80-g6c4433a-5.2.0.tar.gz

# Environment Variables

sudo echo 'export IDF_PATH=~/esp/esp-idf' >> ~/.profile
sudo echo 'export PATH="$HOME/esp/xtensa-esp32-elf/bin:$PATH"' >> ~/.profile



