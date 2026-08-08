-- 后台默认管理员初始化数据 (默认账号: admin, 初始密码: 123456, app_id: 1)
INSERT INTO `admin_account` (
  `account_id`,
  `username`,
  `app_id`,
  `password_hash`,
  `salt`,
  `role`,
  `nickname`,
  `avatar`,
  `creator_account_id`,
  `status`,
  `create_time`,
  `update_time`,
  `last_login_time`,
  `last_login_ip`
) VALUES (
  10001,
  'admin',
  1,
  '0876722e2184218b4c2e0c9cc383b892115c0d66d1fe038b03316de8505afc58',
  'celeritas_salt',
  '0',
  '系统管理员',
  '/avatars/avatar1.svg',
  0,
  1,
  1775923200000, -- 2026-04-12 00:00:00
  1775923200000, -- 2026-04-12 00:00:00
  0,
  ''
) ON DUPLICATE KEY UPDATE
  `app_id` = VALUES(`app_id`),
  `role` = VALUES(`role`),
  `nickname` = VALUES(`nickname`),
  `avatar` = VALUES(`avatar`),
  `creator_account_id` = VALUES(`creator_account_id`),
  `status` = VALUES(`status`),
  `create_time` = VALUES(`create_time`),
  `update_time` = VALUES(`update_time`);
