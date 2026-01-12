CREATE TABLE `mysql_test` (
  `user_id` BIGINT NOT NULL,
  `chapter_id` INT NOT NULL DEFAULT 0,
  `chapter_name` VARCHAR(255) NOT NULL DEFAULT '',
  `chance_winning` DOUBLE NOT NULL DEFAULT 0.0,
  `winning` TINYINT(1) NOT NULL DEFAULT 0,
  `currency` BIGINT NOT NULL DEFAULT 0,
  `count` INT NOT NULL DEFAULT 1,
  PRIMARY KEY (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

