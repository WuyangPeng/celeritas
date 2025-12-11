CREATE TABLE `user` (
  `user_id` BIGINT NOT NULL,
  `account_id` BIGINT NOT NULL COMMENT '账号id',
  `game_server_id` VARCHAR(255) NOT NULL COMMENT '服务器id',
  `is_overload_db` TINYINT(1) NOT NULL DEFAULT 0 COMMENT '是否重载数据库',
  PRIMARY KEY (`user_id`),
  KEY `account_id_index` (`account_id`),
  UNIQUE KEY `account_id_game_server_id_unique` (`account_id`,`game_server_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

