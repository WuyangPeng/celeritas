CREATE TABLE `session_token` (
  `token` VARCHAR(255) NOT NULL,
  `account_id` BIGINT NOT NULL COMMENT '账号id',
  PRIMARY KEY (`token`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

