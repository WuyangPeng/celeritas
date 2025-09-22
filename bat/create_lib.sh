cd /data/celeritas/deps

#! 编译boost库
version="1.88.0" 
boost_dir="boost_1_88_0" 

if [ ! -f /data/celeritas/deps/boost_installed.txt ]; then

	if [ ! -f ${boost_dir}.tar.gz ]; then
		wget https://archives.boost.io/release/${version}/source/${boost_dir}.tar.gz
	fi 
	
	if [ ! -f /data/celeritas/deps/boost_download.txt ]; then
		tar -zxvf ${boost_dir}.tar.gz
		rm -rf boost
		mv ${boost_dir} boost
		touch /data/celeritas/deps/boost_download.txt
	fi 
	
	cd boost
	
	if [ ! -f b2 ]; then
		./bootstrap.sh --with-libraries=all --with-toolset=gcc
	fi 
	
	./b2 install --prefix=/data/celeritas/deps/boost/stage/ cxxflags="-std=c++20"
	touch /data/celeritas/deps/boost_installed.txt
	
fi 