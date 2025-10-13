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

#! 编译hiredis库
if [ ! -f /data/celeritas/Libs/hiredis_installed.txt ]; then

    cd /data/celeritas/deps/
	
	if [ ! -f /data/celeritas/deps/hiredis_clone.txt ]; then
	
		rm -rf hiredis

		git clone https://github.com/redis/hiredis
		
		if [ $? -eq 0 ]; then 
		
			cd hiredis
            git checkout v1.3.0
	 
			touch /data/celeritas/deps/hiredis_clone.txt
	
		fi
	
	fi 
	
	if [ -f /data/celeritas/deps/hiredis_clone.txt ]; then
	
		cd /data/celeritas/deps/hiredis
	
		mkdir -p build
		cd build
		mkdir -p debug
		cd debug

		cmake ../.. -DCMAKE_BUILD_TYPE=Debug
		make 
		
		cd /data/celeritas/deps/hiredis/build
		mkdir -p release
		cd release

		cmake ../.. -DCMAKE_BUILD_TYPE=Release
		make 
	 
		if [ $? -eq 0 ]; then 
	 
			touch /data/celeritas/deps/hiredis_installed.txt
	
		fi
	
	fi
	
fi

#! 编译mongo库
if [ ! -f /data/celeritas/deps/mongo-cxx-driver_installed.txt ]; then

    cd /data/celeritas/deps/
	
	if [ ! -f /data/celeritas/deps/mongo-cxx-driver_clone.txt ]; then
	
		rm -rf mongo-cxx-driver

		git clone https://github.com/mongodb/mongo-cxx-driver.git
		
		if [ $? -eq 0 ]; then 
		
			cd mongo-cxx-driver
            git checkout releases/v4.1
	 
			touch /data/celeritas/deps/mongo-cxx-driver_clone.txt
	
		fi
	
	fi 
	
	if [ -f /data/celeritas/deps/mongo-cxx-driver_clone.txt ]; then
	
		cd /data/celeritas/deps/mongo-cxx-driver
	
		mkdir -p build
		cd build

		cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/data/celeritas/deps/mongo
		make
		
		rm -rf /data/celeritas/deps/mongo
		make install
	 
		if [ $? -eq 0 ]; then 
	 
			touch /data/celeritas/deps/mongo-cxx-driver_installed.txt
	
		fi
	
	fi
	
fi

#! 编译protobuf库
if [ ! -f /data/celeritas/deps/protobuf_installed.txt ]; then

    cd /data/celeritas/deps/

	if [ ! -f /data/celeritas/deps/protobuf_installed_clone.txt ]; then

		rm -rf protobuf

		git clone https://github.com/protocolbuffers/protobuf.git

		if [ $? -eq 0 ]; then

			cd protobuf
            git checkout v32.1
			git submodule update --init --recursive
            cd ..

			touch /data/celeritas/deps/protobuf_installed_clone.txt

		fi

	fi

	if [ -f /data/celeritas/deps/protobuf_installed_clone.txt ]; then

		cd /data/celeritas/deps/protobuf

		mkdir -p build
		cd build

		cmake .. -DCMAKE_BUILD_TYPE=Release
		make

		if [ $? -eq 0 ]; then

			touch /data/celeritas/deps/protobuf_installed.txt

		fi

	fi

fi