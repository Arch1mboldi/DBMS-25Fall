CREATE DATABASE testdb2;
SET OUTPUT = 'output_test.txt';
USE testdb2;
CREATE TABLE test2 (id int);
INSERT INTO test2 VALUES (42);
SELECT * FROM test2;
EXIT;
