-- 全面测试 ALTER TABLE 和 RENAME TABLE 功能
CREATE DATABASE test_db;
USE test_db;

-- 测试1: 创建基础表
CREATE TABLE users (
    id int PRIMARY KEY,
    name varchar(30),
    age int
);
SHOW TABLE users;

-- 测试2: RENAME TABLE 功能
RENAME TABLE users TO customers;
SHOW TABLE customers;

-- 测试3: ALTER TABLE ADD COLUMN 功能  
ALTER TABLE customers ADD COLUMN email varchar(50);
SHOW TABLE customers;

-- 测试4: ALTER TABLE DROP COLUMN 功能
ALTER TABLE customers DROP COLUMN age;
SHOW TABLE customers;

-- 测试5: 再次添加列测试
ALTER TABLE customers ADD COLUMN phone varchar(15);
SHOW TABLE customers;

-- 测试6: 创建第二个表并测试跨表操作
CREATE TABLE orders (
    order_id int PRIMARY KEY,
    customer_id int,
    product_name varchar(50),
    quantity int
);
SHOW TABLE orders;

-- 测试7: 重命名第二个表
RENAME TABLE orders TO customer_orders;
SHOW TABLE customer_orders;

-- 清理测试环境
DROP TABLE customers;
DROP TABLE customer_orders;
DROP DATABASE test_db;

EXIT;