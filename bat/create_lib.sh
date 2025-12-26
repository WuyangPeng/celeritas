#!/bin/bash

# 获取脚本的绝对路径
script_path="$(readlink -f "$0")"

# 脚本所在的目录
script_dir="$(dirname "$script_path")"

# 脚本父目录
root_dir="$(dirname "$script_dir")"

# 定义关键子目录变量
deps_dir="${root_dir}/deps"

echo "deps_dir path is: ${deps_dir}"

cd ${deps_dir}

#! 编译boost库
boost_version="1.88.0"
boost_dir="boost_$(echo ${boost_version} | sed 's/\./_/g')"

echo "boost version: ${boost_version}"

boost_installed_txt="${deps_dir}/boost_installed.txt"
boost_download_txt="${deps_dir}/boost_download.txt"

if [ ! -f ${boost_installed_txt} ]; then

	if [ ! -f ${boost_dir}.tar.gz ]; then
		wget https://archives.boost.io/release/${boost_version}/source/${boost_dir}.tar.gz
	fi 
	
	if [ ! -f ${boost_download_txt} ]; then
		tar -zxvf ${boost_dir}.tar.gz
		rm -rf boost
		mv ${boost_dir} boost
		touch ${boost_download_txt}
	fi 
	
	cd boost

	if [ ! -f b2 ]; then
		./bootstrap.sh --with-libraries=all --with-toolset=gcc
	fi 
	
	./b2 install --prefix=${deps_dir}/boost/stage/ cxxflags="-std=c++20"
	touch ${boost_installed_txt}
	
fi 

#! 编译hiredis库
hiredis_version="v1.3.0"

echo "hiredis version: ${hiredis_version}"

hiredis_installed_txt="${deps_dir}/hiredis_installed.txt"
hiredis_clone_txt="${deps_dir}/hiredis_clone.txt"

if [ ! -f ${hiredis_installed_txt} ]; then

    cd ${deps_dir}/
	
	if [ ! -f ${hiredis_clone_txt} ]; then
	
		rm -rf hiredis

		git clone https://github.com/redis/hiredis
		
		if [ $? -eq 0 ]; then 
		
			cd hiredis
      git checkout ${hiredis_version}
	 
			touch ${hiredis_clone_txt}
	
		fi
	
	fi 
	
	if [ -f ${hiredis_clone_txt} ]; then
	
		cd ${deps_dir}/hiredis
	
		mkdir -p build
		cd build
		mkdir -p debug
		cd debug

		cmake ../.. -DCMAKE_BUILD_TYPE=Debug
		make 
		
		cd ${deps_dir}/hiredis/build
		mkdir -p release
		cd release

		cmake ../.. -DCMAKE_BUILD_TYPE=Release
		make 
	 
		if [ $? -eq 0 ]; then 
	 
			touch ${hiredis_installed_txt}
	
		fi
	
	fi
	
fi

#! 编译mongo库
mongo_version="v4.1"

echo "mongo version: ${mongo_version}"

mongo_installed_txt="${deps_dir}/mongo-cxx-driver_installed.txt"
mongo_clone_txt="${deps_dir}/mongo-cxx-driver_clone.txt"

if [ ! -f ${mongo_installed_txt} ]; then

    cd ${deps_dir}/
	
	if [ ! -f ${mongo_clone_txt} ]; then
	
		rm -rf mongo-cxx-driver

		git clone https://github.com/mongodb/mongo-cxx-driver.git
		
		if [ $? -eq 0 ]; then 
		
			cd mongo-cxx-driver
      git checkout releases/${mongo_version}
	 
			touch ${mongo_clone_txt}
	
		fi
	
	fi 
	
	if [ -f ${mongo_clone_txt} ]; then
	
		cd ${deps_dir}/mongo-cxx-driver
	
		mkdir -p build
		cd build

		cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=${deps_dir}/mongo
		make
		
		rm -rf ${deps_dir}/mongo
		make install
	 
		if [ $? -eq 0 ]; then 
	 
			touch ${mongo_installed_txt}
	
		fi
	
	fi
	
fi

#! 编译protobuf库
protobuf_version="v32.1"

echo "protobuf version: ${protobuf_version}"

protobuf_installed_txt="${deps_dir}/protobuf_installed.txt"
protobuf_clone_txt="${deps_dir}/protobuf_clone.txt"

if [ ! -f ${protobuf_installed_txt} ]; then

    cd ${deps_dir}/

	if [ ! -f ${protobuf_clone_txt} ]; then

		rm -rf protobuf_src

		git clone https://github.com/protocolbuffers/protobuf.git protobuf_src

		if [ $? -eq 0 ]; then

			cd protobuf_src
      git checkout ${protobuf_version}
			git submodule update --init --recursive
      cd ..

			touch ${protobuf_clone_txt}

		fi

	fi

	if [ -f ${protobuf_clone_txt} ]; then

		cd ${deps_dir}/protobuf_src

		mkdir -p build
		cd build

		cmake .. -DCMAKE_BUILD_TYPE=Release -Dprotobuf_BUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=${deps_dir}/protobuf
		make

		rm -rf ${deps_dir}/protobuf
		make install

		if [ $? -eq 0 ]; then

			touch ${protobuf_installed_txt}

		fi

	fi

fi