CREATE TABLE `apps` (
  `app_id` BIGINT NOT NULL COMMENT 'app id',
  `game_name` VARCHAR(255) NOT NULL DEFAULT "" COMMENT '游戏名字',
  `app_secret` VARCHAR(255) NOT NULL DEFAULT "" COMMENT '签名',
  `status` INT NOT NULL DEFAULT 0 COMMENT '游戏状态',
  PRIMARY KEY (`app_id`),
  KEY `game_name_index` (`game_name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

CREATE TABLE `account` (
  `account_id` BIGINT NOT NULL COMMENT '全局唯一ID，雪花算法生成',
  `account_name` VARCHAR(255) NOT NULL COMMENT '正式账号的用户名',
  `password_hash` VARCHAR(255) NOT NULL DEFAULT "" COMMENT '加密后的密码',
  `salt` VARCHAR(255) NOT NULL DEFAULT "" COMMENT '密码盐值(视加密算法而定)',
  `device_id` VARCHAR(255) NOT NULL COMMENT '游客登录凭证',
  `create_time` BIGINT NOT NULL DEFAULT 0 COMMENT '创建时间',
  `status` INT NOT NULL DEFAULT 0 COMMENT '账号状态',
  PRIMARY KEY (`account_id`),
  UNIQUE KEY `account_name_unique` (`account_name`),
  UNIQUE KEY `device_id_unique` (`device_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

CREATE TABLE `account_bind` (
  `account_bind_id` BIGINT NOT NULL COMMENT '全局唯一ID，雪花算法生成',
  `account_id` BIGINT NOT NULL DEFAULT 0 COMMENT '全局唯一ID，雪花算法生成',
  `account_type` INT NOT NULL COMMENT '账号类型',
  `auth_key` VARCHAR(255) NOT NULL COMMENT '具体的身份凭证',
  `is_primary` TINYINT(1) NOT NULL DEFAULT 0 COMMENT '是否是主登录方式',
  PRIMARY KEY (`account_bind_id`),
  KEY `account_id_index` (`account_id`),
  UNIQUE KEY `account_type_auth_key_unique` (`account_type`,`auth_key`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

