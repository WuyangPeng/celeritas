# 测试环境安装

## 安装 docker

### 执行下面三条命令。

- yum install -y yum-utils device-mapper-persistent-data lvm2

- yum-config-manager --add-repo https://mirrors.aliyun.com/docker-ce/linux/centos/docker-ce.repo

- yum install docker-ce

- 执行docker version，查看是否安装docker成功。

### 将docker默认安装路径从/var/lib下移动到/data下。

#### 开启然后关闭docker，docker必须开启过才会在/var/lib下生成目录。

- systemctl start docker

- systemctl stop docker

#### 迁移数据

- mv /var/lib/docker /data/

#### 制作目录链接

- ln -s /data/docker /var/lib/docker

#### 查看是否成功

- cd /var/lib
- ll
- 其中一条会出现
- lrwxrwxrwx 1 root root 12 Feb 17 21:40 docker -> /data/docker

### 启动 docker

- systemctl start docker

### 设置docker开机自启动

- systemctl enable docker.service

## 安装 portainer

docker run --name portainer -d \
-p 35000:9000 \
-v /var/run/docker.sock:/var/run/docker.sock \
--env TZ=:/etc/localtime \
-v /etc/localtime:/etc/localtime:ro \
--restart=always \
--log-driver json-file --log-opt max-size=10m \
portainer/portainer

## 安装 svnadmin

docker run -d --name svnadmin \
-p 35080:80 -p 33690:3690 \
-v /data/svnadmin/:/home/svnadmin/ \
-v /data/svnadmin/conf.d/:/etc/httpd/conf.d/ \
-v /data/svnadmin/sasl2/:/etc/sasl2/ \
--restart=always \
--log-driver json-file --log-opt max-size=10m \
--privileged witersencom/svnadmin

## 安装 redis

docker run -d --name redis \
--network host \
-v /data/redis-data/node:/data \
-v /data/redis-data/conf/redis.conf:/etc/redis/redis.conf \
-v /data/redis-data/conf/users.acl:/etc/redis/users.acl \
--env TZ=:/etc/localtime -v /etc/localtime:/etc/localtime:ro  \
--restart=always \
--log-driver json-file --log-opt max-size=10m \
redis redis-server /etc/redis/redis.conf --aclfile /etc/redis/users.acl --port 7001

## 安装 mongo

docker run -itd --name mongo \
-p 27017:27017 \
-v /data/mongodb/data:/data/db \
-v /data/mongodb/mongodump:/data/mongodump \
--env TZ=:/etc/localtime -v /etc/localtime:/etc/localtime:ro  \
--restart=always \
--log-driver json-file --log-opt max-size=10m mongo --auth

## 安装 mysql

docker run -d --name mysql \
-p 3306:3306 \
-e MYSQL_ROOT_PASSWORD=celeritas_pass \
-v /data/mysql/data:/var/lib/mysql \
-v /data/mysql/conf:/etc/mysql/conf.d \
-v /data/mysql/logs:/var/log/mysql \
--env TZ=:/etc/localtime -v /etc/localtime:/etc/localtime:ro  \
--restart=always \
--log-driver json-file --log-opt max-size=10m \
mysql
