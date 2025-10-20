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

if [ ! -f ${deps_dir}/boost_installed.txt ]; then

	if [ ! -f ${boost_dir}.tar.gz ]; then
		wget https://archives.boost.io/release/${boost_version}/source/${boost_dir}.tar.gz
	fi 
	
	if [ ! -f ${deps_dir}/boost_download.txt ]; then
		tar -zxvf ${boost_dir}.tar.gz
		rm -rf boost
		mv ${boost_dir} boost
		touch ${deps_dir}/boost_download.txt
	fi 
	
	cd boost
	
	if [ ! -f b2 ]; then
		./bootstrap.sh --with-libraries=all --with-toolset=gcc
	fi 
	
	./b2 install --prefix=${deps_dir}/boost/stage/ cxxflags="-std=c++20"
	touch ${deps_dir}/boost_installed.txt
	
fi 

#! 编译hiredis库
if [ ! -f ${deps_dir}/hiredis_installed.txt ]; then

    cd ${deps_dir}/
	
	if [ ! -f ${deps_dir}/hiredis_clone.txt ]; then
	
		rm -rf hiredis

		git clone https://github.com/redis/hiredis
		
		if [ $? -eq 0 ]; then 
		
			cd hiredis
            git checkout v1.3.0
	 
			touch ${deps_dir}/hiredis_clone.txt
	
		fi
	
	fi 
	
	if [ -f ${deps_dir}/hiredis_clone.txt ]; then
	
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
	 
			touch ${deps_dir}/hiredis_installed.txt
	
		fi
	
	fi
	
fi

#! 编译mongo库
if [ ! -f ${deps_dir}/mongo-cxx-driver_installed.txt ]; then

    cd ${deps_dir}/
	
	if [ ! -f ${deps_dir}/mongo-cxx-driver_clone.txt ]; then
	
		rm -rf mongo-cxx-driver

		git clone https://github.com/mongodb/mongo-cxx-driver.git
		
		if [ $? -eq 0 ]; then 
		
			cd mongo-cxx-driver
            git checkout releases/v4.1
	 
			touch ${deps_dir}/mongo-cxx-driver_clone.txt
	
		fi
	
	fi 
	
	if [ -f ${deps_dir}/mongo-cxx-driver_clone.txt ]; then
	
		cd ${deps_dir}/mongo-cxx-driver
	
		mkdir -p build
		cd build

		cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=${deps_dir}/mongo
		make
		
		rm -rf ${deps_dir}/mongo
		make install
	 
		if [ $? -eq 0 ]; then 
	 
			touch ${deps_dir}/mongo-cxx-driver_installed.txt
	
		fi
	
	fi
	
fi

#! 编译protobuf库
if [ ! -f ${deps_dir}/protobuf_installed.txt ]; then

    cd ${deps_dir}/

	if [ ! -f ${deps_dir}/protobuf_installed_clone.txt ]; then

		rm -rf protobuf_src

		git clone https://github.com/protocolbuffers/protobuf.git protobuf_src

		if [ $? -eq 0 ]; then

			cd protobuf_src
            git checkout v32.1
			git submodule update --init --recursive
            cd ..

			touch ${deps_dir}/protobuf_installed_clone.txt

		fi

	fi

	if [ -f ${deps_dir}/protobuf_installed_clone.txt ]; then

		cd ${deps_dir}/protobuf_src

		mkdir -p build
		cd build

		cmake .. -DCMAKE_BUILD_TYPE=Release -Dprotobuf_BUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=${deps_dir}/protobuf
		make

		rm -rf ${deps_dir}/protobuf
		make install

		if [ $? -eq 0 ]; then

			touch ${deps_dir}/protobuf_installed.txt

		fi

	fi

fi