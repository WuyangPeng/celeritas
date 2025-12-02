CREATE TABLE `orders` (
  `id` BIGINT NOT NULL,
  `order_id` VARCHAR(255) NOT NULL COMMENT '商户订单号',
  `account_id` BIGINT NOT NULL DEFAULT 0 COMMENT '账号ID',
  `game_server_id` VARCHAR(255) NOT NULL COMMENT '区服id',
  `role_id` BIGINT NOT NULL DEFAULT 0 COMMENT '角色id',
  `product_id` VARCHAR(255) NOT NULL COMMENT '商品id',
  `amount` INT NOT NULL DEFAULT 0 COMMENT '订单金额',
  `platform` INT NOT NULL COMMENT '支付平台',
  `transaction_id` VARCHAR(255) NOT NULL COMMENT '第三方交易流水号',
  `status` INT NOT NULL DEFAULT 0 COMMENT '支付状态',
  `delivery_status` INT NOT NULL DEFAULT 0 COMMENT '发货状态',
  `client_request_id` VARCHAR(255) NOT NULL COMMENT '游戏服务器传入的唯一请求ID',
  `retry_count` INT NOT NULL DEFAULT 0 COMMENT '重试次数',
  `create_time` BIGINT NOT NULL DEFAULT 0 COMMENT '订单创建时间',
  `pay_time` BIGINT NOT NULL DEFAULT 0 COMMENT '支付成功时间',
  `expire_time` BIGINT NOT NULL DEFAULT 0 COMMENT '订单支付超时时间',
  `callback_data` VARCHAR(255) NOT NULL COMMENT '原始回调数据',
  `refund_time` BIGINT NOT NULL DEFAULT 0 COMMENT '退款成功时间',
  `refund_amount` INT NOT NULL DEFAULT 0 COMMENT '退款金额',
  PRIMARY KEY (`id`),
  UNIQUE KEY `order_id_unique` (`order_id`),
  UNIQUE KEY `client_request_id_unique` (`client_request_id`),
  UNIQUE KEY `platform_transaction_id_unique` (`platform`,`transaction_id`),
  KEY `role_id_status_index` (`role_id`,`status`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

CREATE TABLE `sdk_payment_providers` (
  `sdk_id` BIGINT NOT NULL,
  `app_id` BIGINT NOT NULL,
  `platform` INT NOT NULL COMMENT '平台',
  `provider_name` VARCHAR(255) NOT NULL COMMENT '服务商名称',
  `api_key` VARCHAR(255) NOT NULL COMMENT '密钥 ID',
  `api_secret` VARCHAR(255) NOT NULL COMMENT '密钥 Secret',
  `decryption_key` VARCHAR(255) NOT NULL COMMENT '解密密钥',
  `active` TINYINT(1) NOT NULL DEFAULT 0 COMMENT '是否启用',
  PRIMARY KEY (`sdk_id`),
  UNIQUE KEY `app_id_platform_unique` (`app_id`,`platform`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

