CREATE TABLE `time_refresh`
(
    `id`                BIGINT NOT NULL,
    `time_refresh_type` INT    NOT NULL DEFAULT 0 COMMENT '时间刷新类型',
    `parameter`         INT    NOT NULL DEFAULT 0 COMMENT '时间刷新参数',
    PRIMARY KEY (`id`)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8mb4
  COLLATE = utf8mb4_0900_ai_ci;

INSERT INTO `time_refresh`(`id`, `time_refresh_type`, `parameter`)
VALUES (1, 1, 0);

INSERT INTO `time_refresh`(`id`, `time_refresh_type`, `parameter`)
VALUES (36001, 1, 3600000);

INSERT INTO `time_refresh`(`id`, `time_refresh_type`, `parameter`)
VALUES (72001, 1, 7200000);

INSERT INTO `time_refresh`(`id`, `time_refresh_type`, `parameter`)
VALUES (108001, 1, 10800000);

INSERT INTO `time_refresh`(`id`, `time_refresh_type`, `parameter`)
VALUES (144001, 1, 14400000);

INSERT INTO `time_refresh`(`id`, `time_refresh_type`, `parameter`)
VALUES (180001, 1, 18000000);

INSERT INTO `time_refresh`(`id`, `time_refresh_type`, `parameter`)
VALUES (216001, 1, 21600000);

INSERT INTO `time_refresh`(`id`, `time_refresh_type`, `parameter`)
VALUES (288001, 1, 28800000);

INSERT INTO `time_refresh`(`id`, `time_refresh_type`, `parameter`)
VALUES (324001, 1, 32400000);

INSERT INTO `time_refresh`(`id`, `time_refresh_type`, `parameter`)
VALUES (360001, 1, 36000000);

INSERT INTO `time_refresh`(`id`, `time_refresh_type`, `parameter`)
VALUES (396001, 1, 39600000);

INSERT INTO `time_refresh`(`id`, `time_refresh_type`, `parameter`)
VALUES (432001, 1, 43200000);

INSERT INTO `time_refresh`(`id`, `time_refresh_type`, `parameter`)
VALUES (468001, 1, 46800000);

INSERT INTO `time_refresh`(`id`, `time_refresh_type`, `parameter`)
VALUES (504001, 1, 50400000);

INSERT INTO `time_refresh`(`id`, `time_refresh_type`, `parameter`)
VALUES (540001, 1, 54000000);

INSERT INTO `time_refresh`(`id`, `time_refresh_type`, `parameter`)
VALUES (576001, 1, 57600000);

INSERT INTO `time_refresh`(`id`, `time_refresh_type`, `parameter`)
VALUES (612001, 1, 61200000);

INSERT INTO `time_refresh`(`id`, `time_refresh_type`, `parameter`)
VALUES (648001, 1, 64800000);

INSERT INTO `time_refresh`(`id`, `time_refresh_type`, `parameter`)
VALUES (684001, 1, 68400000);

INSERT INTO `time_refresh`(`id`, `time_refresh_type`, `parameter`)
VALUES (720001, 1, 72000000);

INSERT INTO `time_refresh`(`id`, `time_refresh_type`, `parameter`)
VALUES (756001, 1, 75600000);

INSERT INTO `time_refresh`(`id`, `time_refresh_type`, `parameter`)
VALUES (792001, 1, 79200000);

INSERT INTO `time_refresh`(`id`, `time_refresh_type`, `parameter`)
VALUES (828001, 1, 82800000);

INSERT INTO `time_refresh`(`id`, `time_refresh_type`, `parameter`)
VALUES (2, 2, 0);

INSERT INTO `time_refresh`(`id`, `time_refresh_type`, `parameter`)
VALUES (5184002, 2, 518400000);

INSERT INTO `time_refresh`(`id`, `time_refresh_type`, `parameter`)
VALUES (3, 3, 0);