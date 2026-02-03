db.user_role.createIndex({ "full_name": 1 }, { unique: true });
db.user_mail.createIndex({ "user_id": 1 });
