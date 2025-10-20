# 获取脚本的绝对路径
SCRIPT_PATH="$(readlink -f "$0")"

# 脚本所在的目录
SCRIPT_DIR="$(dirname "$SCRIPT_PATH")"

# 脚本父目录
ROOT_DIR="$(dirname "$SCRIPT_DIR")"

# 定义关键子目录变量
DEPS_DIR="${ROOT_DIR}/deps"

echo "DEPS_DIR path is: ${DEPS_DIR}"

cd ${DEPS_DIR}

#! 编译boost库
version="1.88.0" 
boost_dir="boost_1_88_0" 

if [ ! -f ${DEPS_DIR}/boost_installed.txt ]; then

	if [ ! -f ${boost_dir}.tar.gz ]; then
		wget https://archives.boost.io/release/${version}/source/${boost_dir}.tar.gz
	fi 
	
	if [ ! -f ${DEPS_DIR}/boost_download.txt ]; then
		tar -zxvf ${boost_dir}.tar.gz
		rm -rf boost
		mv ${boost_dir} boost
		touch ${DEPS_DIR}/boost_download.txt
	fi 
	
	cd boost
	
	if [ ! -f b2 ]; then
		./bootstrap.sh --with-libraries=all --with-toolset=gcc
	fi 
	
	./b2 install --prefix=${DEPS_DIR}/boost/stage/ cxxflags="-std=c++20"
	touch ${DEPS_DIR}/boost_installed.txt
	
fi 

#! 编译hiredis库
if [ ! -f ${DEPS_DIR}/hiredis_installed.txt ]; then

    cd ${DEPS_DIR}/
	
	if [ ! -f ${DEPS_DIR}/hiredis_clone.txt ]; then
	
		rm -rf hiredis

		git clone https://github.com/redis/hiredis
		
		if [ $? -eq 0 ]; then 
		
			cd hiredis
            git checkout v1.3.0
	 
			touch ${DEPS_DIR}/hiredis_clone.txt
	
		fi
	
	fi 
	
	if [ -f ${DEPS_DIR}/hiredis_clone.txt ]; then
	
		cd ${DEPS_DIR}/hiredis
	
		mkdir -p build
		cd build
		mkdir -p debug
		cd debug

		cmake ../.. -DCMAKE_BUILD_TYPE=Debug
		make 
		
		cd ${DEPS_DIR}/hiredis/build
		mkdir -p release
		cd release

		cmake ../.. -DCMAKE_BUILD_TYPE=Release
		make 
	 
		if [ $? -eq 0 ]; then 
	 
			touch ${DEPS_DIR}/hiredis_installed.txt
	
		fi
	
	fi
	
fi

#! 编译mongo库
if [ ! -f ${DEPS_DIR}/mongo-cxx-driver_installed.txt ]; then

    cd ${DEPS_DIR}/
	
	if [ ! -f ${DEPS_DIR}/mongo-cxx-driver_clone.txt ]; then
	
		rm -rf mongo-cxx-driver

		git clone https://github.com/mongodb/mongo-cxx-driver.git
		
		if [ $? -eq 0 ]; then 
		
			cd mongo-cxx-driver
            git checkout releases/v4.1
	 
			touch ${DEPS_DIR}/mongo-cxx-driver_clone.txt
	
		fi
	
	fi 
	
	if [ -f ${DEPS_DIR}/mongo-cxx-driver_clone.txt ]; then
	
		cd ${DEPS_DIR}/mongo-cxx-driver
	
		mkdir -p build
		cd build

		cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=${DEPS_DIR}/mongo
		make
		
		rm -rf ${DEPS_DIR}/mongo
		make install
	 
		if [ $? -eq 0 ]; then 
	 
			touch ${DEPS_DIR}/mongo-cxx-driver_installed.txt
	
		fi
	
	fi
	
fi

#! 编译protobuf库
if [ ! -f ${DEPS_DIR}/protobuf_installed.txt ]; then

    cd ${DEPS_DIR}/

	if [ ! -f ${DEPS_DIR}/protobuf_installed_clone.txt ]; then

		rm -rf protobuf_src

		git clone https://github.com/protocolbuffers/protobuf.git protobuf_src

		if [ $? -eq 0 ]; then

			cd protobuf_src
            git checkout v32.1
			git submodule update --init --recursive
            cd ..

			touch ${DEPS_DIR}/protobuf_installed_clone.txt

		fi

	fi

	if [ -f ${DEPS_DIR}/protobuf_installed_clone.txt ]; then

		cd ${DEPS_DIR}/protobuf_src

		mkdir -p build
		cd build

		cmake .. -DCMAKE_BUILD_TYPE=Release -Dprotobuf_BUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=${DEPS_DIR}/protobuf
		make

		rm -rf ${DEPS_DIR}/protobuf
		make install

		if [ $? -eq 0 ]; then

			touch ${DEPS_DIR}/protobuf_installed.txt

		fi

	fi

fi