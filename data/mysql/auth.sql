CREATE TABLE `apps` (
  `app_id` BIGINT NOT NULL COMMENT '应用id',
  `game_name` VARCHAR(255) NOT NULL COMMENT '游戏名字',
  `app_secret` VARCHAR(255) NOT NULL COMMENT '签名',
  `sms_provider_id` BIGINT NOT NULL DEFAULT 0 COMMENT '短信服务商id',
  `email_provider_id` BIGINT NOT NULL DEFAULT 0 COMMENT '邮件服务商id',
  `status` INT NOT NULL DEFAULT 0 COMMENT '游戏状态',
  PRIMARY KEY (`app_id`),
  KEY `game_name_index` (`game_name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

CREATE TABLE `sms_providers` (
  `provider_id` BIGINT NOT NULL COMMENT '服务商id',
  `provider_name` VARCHAR(255) NOT NULL COMMENT '服务商名称',
  `base_url` VARCHAR(255) NOT NULL COMMENT '基础 API 地址',
  `api_key` VARCHAR(255) NOT NULL COMMENT '密钥 ID',
  `api_secret` VARCHAR(255) NOT NULL COMMENT '密钥 Secret',
  `decryption_key` VARCHAR(255) NOT NULL COMMENT '解密密钥',
  `active` TINYINT(1) NOT NULL DEFAULT 0 COMMENT '是否启用',
  `process_type` INT NOT NULL DEFAULT 0 COMMENT '处理类型',
  PRIMARY KEY (`provider_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

CREATE TABLE `email_providers` (
  `provider_id` BIGINT NOT NULL COMMENT '服务商id',
  `provider_name` VARCHAR(255) NOT NULL COMMENT '服务商名称',
  `base_url` VARCHAR(255) NOT NULL COMMENT '基础 API 地址',
  `api_key` VARCHAR(255) NOT NULL COMMENT '密钥 ID',
  `api_secret` VARCHAR(255) NOT NULL COMMENT '密钥 Secret',
  `decryption_key` VARCHAR(255) NOT NULL COMMENT '解密密钥',
  `active` TINYINT(1) NOT NULL DEFAULT 0 COMMENT '是否启用',
  `process_type` INT NOT NULL DEFAULT 0 COMMENT '处理类型',
  PRIMARY KEY (`provider_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

CREATE TABLE `sdk_providers` (
  `sdk_id` BIGINT NOT NULL,
  `app_id` BIGINT NOT NULL,
  `process_type` INT NOT NULL COMMENT '处理类型',
  `provider_name` VARCHAR(255) NOT NULL COMMENT '服务商名称',
  `base_url` VARCHAR(255) NOT NULL COMMENT '基础 API 地址',
  `api_key` VARCHAR(255) NOT NULL COMMENT '密钥 ID',
  `api_secret` VARCHAR(255) NOT NULL COMMENT '密钥 Secret',
  `decryption_key` VARCHAR(255) NOT NULL COMMENT '解密密钥',
  `active` TINYINT(1) NOT NULL DEFAULT 0 COMMENT '是否启用',
  PRIMARY KEY (`sdk_id`),
  UNIQUE KEY `app_id_process_type_unique` (`app_id`,`process_type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

CREATE TABLE `account` (
  `account_id` BIGINT NOT NULL COMMENT '账号id，雪花算法生成',
  `account_name` VARCHAR(255) NOT NULL COMMENT '正式账号的用户名',
  `password_hash` VARCHAR(255) NOT NULL COMMENT '加密后的密码',
  `salt` VARCHAR(255) NOT NULL COMMENT '密码盐值(视加密算法而定)',
  `device_id` VARCHAR(255) NOT NULL COMMENT '游客登录凭证',
  `app_id` BIGINT NOT NULL COMMENT '应用id',
  `create_time` BIGINT NOT NULL DEFAULT 0 COMMENT '创建时间',
  `status` INT NOT NULL DEFAULT 0 COMMENT '账号状态',
  PRIMARY KEY (`account_id`),
  UNIQUE KEY `account_name_unique` (`account_name`),
  UNIQUE KEY `device_id_app_id_unique` (`device_id`,`app_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

CREATE TABLE `account_bind` (
  `account_bind_id` BIGINT NOT NULL COMMENT '账号绑定id，雪花算法生成',
  `account_id` BIGINT NOT NULL DEFAULT 0 COMMENT '账号id，雪花算法生成',
  `account_type` INT NOT NULL COMMENT '账号类型',
  `app_id` BIGINT NOT NULL COMMENT '应用id',
  `auth_key` VARCHAR(255) NOT NULL COMMENT '具体的身份凭证',
  PRIMARY KEY (`account_bind_id`),
  KEY `account_id_index` (`account_id`),
  UNIQUE KEY `account_type_app_id_auth_key_unique` (`account_type`,`app_id`,`auth_key`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

