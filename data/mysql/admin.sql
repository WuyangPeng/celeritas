CREATE TABLE `admin_account` (
  `account_id` BIGINT NOT NULL COMMENT '后台账号ID',
  `username` VARCHAR(255) NOT NULL COMMENT '用户名',
  `app_id` BIGINT NOT NULL COMMENT '应用ID',
  `password_hash` VARCHAR(255) NOT NULL DEFAULT '' COMMENT '密码哈希',
  `salt` VARCHAR(255) NOT NULL DEFAULT '' COMMENT '盐值',
  `role` INT NOT NULL DEFAULT 0 COMMENT '角色权限',
  `nickname` VARCHAR(255) NOT NULL DEFAULT '' COMMENT '用户昵称',
  `avatar` VARCHAR(255) NOT NULL DEFAULT '' COMMENT '头像地址',
  `creator_account_id` BIGINT NOT NULL DEFAULT 0 COMMENT '创建者的账号ID',
  `status` INT NOT NULL DEFAULT 0 COMMENT '账号状态 （1:正常 0:禁用）',
  `create_time` BIGINT NOT NULL DEFAULT 0 COMMENT '创建时间戳',
  `update_time` BIGINT NOT NULL DEFAULT 0 COMMENT '更新时间戳',
  `last_login_time` BIGINT NOT NULL DEFAULT 0 COMMENT '最后登录时间戳',
  `last_login_ip` VARCHAR(255) NOT NULL DEFAULT '' COMMENT '最后登录IP',
  PRIMARY KEY (`account_id`),
  KEY `app_id_index` (`app_id`),
  UNIQUE KEY `username_app_id_unique` (`username`,`app_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

CREATE TABLE `announcement` (
  `id` BIGINT NOT NULL COMMENT '公告ID',
  `app_id` BIGINT NOT NULL DEFAULT 0 COMMENT '应用ID',
  `title` VARCHAR(255) NOT NULL DEFAULT '' COMMENT '公告标题',
  `tag` INT NOT NULL DEFAULT 0 COMMENT '标签 （0:无 1:新 2:热 3:维）',
  `banner_url` VARCHAR(255) NOT NULL DEFAULT '' COMMENT '横幅图片URL',
  `content` VARCHAR(255) NOT NULL DEFAULT '' COMMENT '公告正文',
  `priority` INT NOT NULL DEFAULT 0 COMMENT '优先级 （0-99, 越大越靠前）',
  `publish_time` BIGINT NOT NULL DEFAULT 0 COMMENT '发布时间戳（毫秒）',
  `status` INT NOT NULL DEFAULT 0 COMMENT '状态 （0:draft 1:published 2:archived）',
  `server` VARCHAR(255) NOT NULL DEFAULT '' COMMENT '目标区服 （all 或具体服ID如 s1001）',
  `create_time` BIGINT NOT NULL DEFAULT 0 COMMENT '创建时间戳',
  `update_time` BIGINT NOT NULL DEFAULT 0 COMMENT '更新时间戳',
  PRIMARY KEY (`id`),
  KEY `app_id_index` (`app_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

CREATE TABLE `feedback` (
  `id` BIGINT NOT NULL COMMENT '反馈ID',
  `app_id` BIGINT NOT NULL DEFAULT 0 COMMENT '应用ID',
  `type` INT NOT NULL DEFAULT 0 COMMENT '反馈类型 （0:问题反馈 1:意见建议 2:举报他人）',
  `content` VARCHAR(255) NOT NULL DEFAULT '' COMMENT '反馈内容',
  `device_info` VARCHAR(255) NOT NULL DEFAULT '' COMMENT '设备信息',
  `server` VARCHAR(255) NOT NULL DEFAULT '' COMMENT '服务器名称',
  `player_id` VARCHAR(255) NOT NULL DEFAULT '' COMMENT '玩家ID',
  `is_anonymous` TINYINT(1) NOT NULL DEFAULT 0 COMMENT '是否匿名',
  `image_data` MEDIUMBLOB NOT NULL COMMENT '图片二进制数据',
  `create_time` BIGINT NOT NULL DEFAULT 0 COMMENT '创建时间戳',
  PRIMARY KEY (`id`),
  KEY `app_id_type_index` (`app_id`,`type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

