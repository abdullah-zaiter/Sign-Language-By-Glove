# Conda Prerequisites
sudo apt install glib2.0-dev -y

## Go to home directory
cd ~

## You can change what anaconda version you want at 
# https://repo.continuum.io/archive/
wget https://repo.continuum.io/archive/Anaconda3-5.0.1-Linux-x86_64.sh
bash Anaconda3-5.0.1-Linux-x86_64.sh -b -p ~/anaconda
#rm Anaconda3-5.0.1-Linux-x86_64.sh
echo 'export PATH="~/anaconda/bin:$PATH"' >> ~/.bashrc 

## Refresh basically
source .bashrc

#Set PATH
export PATH=~/anaconda/bin:$PATH

sudo echo 'export PATH=~/anaconda/bin:$PATH' >> ~/.profile
conda update conda -y


#Clone Repository to Desktop
cd ~/Desktop
git clone https://github.com/abdullah-zaiter/Sign-Language-By-Glove.git

#Conda Environment Set Up
cd Sign-Language-By-Glove
conda env create -f environment.yml