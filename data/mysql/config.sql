CREATE TABLE `time_refresh` (
  `id` BIGINT NOT NULL,
  `time_refresh_type` INT NOT NULL DEFAULT 0 COMMENT '时间刷新类型',
  `parameter` INT NOT NULL DEFAULT 0 COMMENT '时间刷新参数',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

