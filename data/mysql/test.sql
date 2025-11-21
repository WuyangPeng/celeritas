CREATE TABLE `test` (
  `user_id` BIGINT NOT NULL,
  `chapter_id` INT NOT NULL,
  `chapter_name` VARCHAR(255) NOT NULL,
  `chance_winning` DOUBLE NOT NULL,
  `winning` TINYINT(1) NOT NULL,
  `currency` BIGINT NOT NULL,
  `count` INT NOT NULL,
  PRIMARY KEY (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

