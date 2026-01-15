CREATE DATABASE test_db;
USE test_db;
CREATE TABLE test_table (id int PRIMARY KEY, name varchar(20));
SHOW TABLE test_table;
ALTER TABLE test_table ADD COLUMN email varchar(30);
SHOW TABLE test_table;
.exit;