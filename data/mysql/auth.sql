CREATE TABLE `account` (
  `account_id` BIGINT NOT NULL COMMENT '全局唯一ID，雪花算法生成',
  `account_name` VARCHAR(255) NOT NULL COMMENT '正式账号的用户名',
  `account_type` INT NOT NULL COMMENT '账号类型',
  `password_hash` VARCHAR(255) NOT NULL COMMENT '加密后的密码',
  `salt` VARCHAR(255) NOT NULL COMMENT '密码盐值(视加密算法而定)',
  `device_id` VARCHAR(255) NOT NULL COMMENT '游客登录凭证',
  `create_time` BIGINT NOT NULL COMMENT '创建时间',
  `status` INT NOT NULL COMMENT '账号状态',
  PRIMARY KEY (`account_id`),
  UNIQUE KEY `account_name_unique` (`account_name`),
  KEY `device_id_index` (`device_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

