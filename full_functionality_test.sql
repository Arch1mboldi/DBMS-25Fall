-- 全面数据库功能测试
-- 1. 数据库操作
CREATE DATABASE company_db;
USE company_db;

-- 2. 表操作 - 创建表
CREATE TABLE departments (
    dept_id int PRIMARY KEY,
    dept_name varchar(50) NOT NULL,
    location varchar(100)
);

CREATE TABLE employees (
    emp_id int PRIMARY KEY,
    name varchar(50) NOT NULL,
    age int,
    salary float,
    dept_id int,
    hire_date varchar(10)
);

CREATE TABLE projects (
    project_id int PRIMARY KEY,
    project_name varchar(100),
    budget float,
    dept_id int
);

-- 显示所有表
SHOW TABLE departments;
SHOW TABLE employees;
SHOW TABLE projects;

-- 3. 数据插入操作
INSERT INTO departments VALUES (1, 'Technology', 'New York');
INSERT INTO departments VALUES (2, 'Marketing', 'Los Angeles');
INSERT INTO departments VALUES (3, 'Finance', 'Chicago');

INSERT INTO employees VALUES (101, 'John Smith', 30, 75000.0, 1, '2020-01-15');
INSERT INTO employees VALUES (102, 'Jane Doe', 28, 68000.0, 2, '2021-03-22');
INSERT INTO employees VALUES (103, 'Mike Johnson', 35, 82000.0, 1, '2019-05-10');
INSERT INTO employees VALUES (104, 'Sarah Wilson', 32, 78000.0, 3, '2020-08-05');
INSERT INTO employees VALUES (105, 'Tom Brown', 29, 65000.0, 2, '2022-01-30');

INSERT INTO projects VALUES (1001, 'Website Redesign', 50000.0, 1);
INSERT INTO projects VALUES (1002, 'Marketing Campaign', 30000.0, 2);
INSERT INTO projects VALUES (1003, 'Financial System', 75000.0, 3);

-- 4. 查询操作 - 单表查询
-- 4.1 选择所有列
SELECT * FROM departments;
SELECT * FROM employees;
SELECT * FROM projects;

-- 4.2 条件查询
SELECT * FROM employees WHERE age > 30;
SELECT * FROM employees WHERE salary > 70000;
SELECT * FROM employees WHERE dept_id = 1;
SELECT * FROM employees WHERE name LIKE 'J%';

-- 4.3 投影查询
SELECT emp_id, name, salary FROM employees;
SELECT dept_name, location FROM departments;

-- 5. 多表连接查询
-- 5.1 内连接
SELECT e.emp_id, e.name, e.salary, d.dept_name 
FROM employees e, departments d 
WHERE e.dept_id = d.dept_id;

SELECT e.name, p.project_name, p.budget
FROM employees e, departments d, projects p
WHERE e.dept_id = d.dept_id AND d.dept_id = p.dept_id;

-- 6. 数据修改操作
-- 6.1 更新数据
UPDATE employees SET salary = 80000.0 WHERE emp_id = 101;
SELECT * FROM employees WHERE emp_id = 101;

-- 6.2 删除数据
DELETE FROM employees WHERE emp_id = 105;
SELECT * FROM employees;

-- 7. 表结构修改操作
-- 7.1 重命名表
RENAME TABLE projects TO company_projects;
SHOW TABLE company_projects;

-- 7.2 修改表结构
ALTER TABLE employees ADD COLUMN email varchar(100);
ALTER TABLE employees ADD COLUMN phone varchar(15);
SHOW TABLE employees;

ALTER TABLE employees DROP COLUMN phone;
SHOW TABLE employees;

-- 8. 聚合查询
SELECT dept_id, COUNT(*) as employee_count, AVG(salary) as avg_salary
FROM employees 
GROUP BY dept_id;

-- 9. 清理测试环境
DROP TABLE employees;
DROP TABLE departments;
DROP TABLE company_projects;
DROP DATABASE company_db;

EXIT;