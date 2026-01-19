#ifndef __TRIVIALDB_LOGGER__
#define __TRIVIALDB_LOGGER__

#include <string>
#include <fstream>
#include <ctime>
#include <mutex>
#include <sstream>

// 日志级别
enum class LogLevel {
    DEBUG,      // 调试信息
    INFO,       // 普通操作信息
    WARNING,    // 警告
    ERROR,      // 错误（catch 到的异常）
    FATAL       // 致命错误
};

// 操作类型
enum class OperationType {
    // 数据库操作
    DB_CREATE,
    DB_DROP,
    DB_USE,
    DB_SHOW,
    
    // 表操作
    TABLE_CREATE,
    TABLE_DROP,
    TABLE_SHOW,
    TABLE_RENAME,
    TABLE_ALTER_ADD,
    TABLE_ALTER_DROP,
    TABLE_ALTER_MODIFY,
    TABLE_ALTER_RENAME,
    
    // 索引操作
    INDEX_CREATE,
    INDEX_DROP,
    
    // 数据操作
    DATA_INSERT,
    DATA_DELETE,
    DATA_UPDATE,
    DATA_SELECT,
    
    // 系统操作
    SYSTEM_START,
    SYSTEM_QUIT,
    SYSTEM_ERROR,
    
    // 未知操作
    UNKNOWN
};

// 日志条目结构
struct LogEntry {
    std::string timestamp;          // 时间戳
    std::string user;               // 用户名（预留）
    LogLevel level;                 // 日志级别
    OperationType op_type;          // 操作类型
    std::string database;           // 当前数据库（可为空）
    std::string table;              // 操作的表（可为空）
    std::string sql_command;        // 原始SQL命令
    bool success;                   // 操作是否成功
    std::string message;            // 附加信息/错误信息
    int affected_rows;              // 影响的行数（针对DML）
};

class Logger {
private:
    std::ofstream log_file;
    std::ofstream error_log_file;   // 单独的错误日志
    std::string log_file_path;
    std::string error_log_file_path;
    
    // ========== 用户/权限系统接口预留 - 开始 ==========
    // 当前用户名，默认为 "admin"
    // TODO: 实现用户认证系统后，通过 set_current_user() 设置当前登录用户
    // 可扩展字段:
    //   - std::string current_role;      // 用户角色 (admin/user/guest)
    //   - std::string session_id;        // 会话ID
    //   - std::string client_ip;         // 客户端IP地址
    //   - time_t login_time;             // 登录时间
    std::string current_user;        // 当前用户（预留接口）
    // ========== 用户/权限系统接口预留 - 结束 ==========
    
    std::string current_database;    // 当前数据库
    LogLevel min_level;              // 最小记录级别
    std::mutex log_mutex;            // 线程安全
    bool console_output;             // 是否同时输出到控制台
    bool initialized;                // 是否已初始化
    
    Logger();  // 单例模式
    std::string get_timestamp();
    std::string level_to_string(LogLevel level);
    std::string op_type_to_string(OperationType op);
    void write_log(const LogEntry& entry);
    void write_error_log(const LogEntry& entry);
    void ensure_initialized();
    
public:
    ~Logger();
    
    // 禁止拷贝
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    // 单例获取
    static Logger* get_instance();
    
    // 配置接口
    void set_log_file(const std::string& path);
    void set_error_log_file(const std::string& path);
    void set_min_level(LogLevel level);
    void set_console_output(bool enabled);
    
    // ========== 用户/权限系统接口预留 - 开始 ==========
    // 设置当前操作用户
    // @param user: 用户名
    // 使用示例（未来实现用户系统后）:
    //   Logger::get_instance()->set_current_user(session.get_username());
    // TODO: 可扩展方法:
    //   void set_user_role(const std::string& role);
    //   void set_session_id(const std::string& session_id);
    //   void set_client_ip(const std::string& ip);
    //   bool check_permission(OperationType op);  // 权限检查
    void set_current_user(const std::string& user);
    std::string get_current_user() const;
    // ========== 用户/权限系统接口预留 - 结束 ==========
    
    // 数据库上下文
    void set_current_database(const std::string& db);
    std::string get_current_database() const;
    
    // 核心日志方法
    void log(LogLevel level, OperationType op_type, 
             const std::string& sql_command,
             bool success,
             const std::string& message = "",
             const std::string& table = "",
             int affected_rows = -1);
    
    // 便捷方法
    void log_debug(const std::string& message);
    void log_info(OperationType op_type, const std::string& sql_command, 
                  bool success, const std::string& message = "");
    void log_warning(const std::string& message);
    void log_error(OperationType op_type, const std::string& sql_command,
                   const std::string& error_message);
    void log_fatal(const std::string& message);
    
    // 特化的日志方法（带SQL命令样貌）
    void log_database_op(OperationType op, const char* db_name, bool success, 
                         const std::string& message = "");
    void log_table_op(OperationType op, const char* table_name, bool success,
                      const std::string& message = "");
    void log_data_op(OperationType op, const char* table_name, 
                     const std::string& sql_preview, bool success,
                     int affected_rows, const std::string& message = "");
    
    // 异常捕获日志
    void log_exception(const char* location, const char* exception_msg);
    void log_exception(const char* location, const std::exception& e);
    
    // 生成SQL命令字符串的辅助方法
    static std::string format_create_db_sql(const char* db_name);
    static std::string format_drop_db_sql(const char* db_name);
    static std::string format_use_db_sql(const char* db_name);
    static std::string format_show_db_sql(const char* db_name);
    static std::string format_create_table_sql(const char* table_name);
    static std::string format_drop_table_sql(const char* table_name);
    static std::string format_show_table_sql(const char* table_name);
    static std::string format_rename_table_sql(const char* old_name, const char* new_name);
    static std::string format_alter_add_sql(const char* table_name, const char* col_name);
    static std::string format_alter_drop_sql(const char* table_name, const char* col_name);
    static std::string format_alter_modify_sql(const char* table_name, const char* col_name);
    static std::string format_alter_rename_sql(const char* table_name, const char* old_col, const char* new_col);
    static std::string format_insert_sql(const char* table_name, int row_count);
    static std::string format_delete_sql(const char* table_name);
    static std::string format_update_sql(const char* table_name, const char* col_name);
    static std::string format_select_sql(const char* table_name);
    static std::string format_create_index_sql(const char* table_name, const char* col_name);
    static std::string format_drop_index_sql(const char* table_name, const char* col_name);
};

// 便捷宏定义
#define LOG_DEBUG(msg) Logger::get_instance()->log_debug(msg)
#define LOG_INFO(op, sql, succ) Logger::get_instance()->log_info(op, sql, succ)
#define LOG_WARNING(msg) Logger::get_instance()->log_warning(msg)
#define LOG_ERROR(op, sql, err) Logger::get_instance()->log_error(op, sql, err)
#define LOG_FATAL(msg) Logger::get_instance()->log_fatal(msg)
#define LOG_EXCEPTION(loc, e) Logger::get_instance()->log_exception(loc, e)

// 数据库操作日志宏
#define LOG_DB_OP(op, db, succ) Logger::get_instance()->log_database_op(op, db, succ)
#define LOG_DB_OP_MSG(op, db, succ, msg) Logger::get_instance()->log_database_op(op, db, succ, msg)

// 表操作日志宏
#define LOG_TABLE_OP(op, tb, succ) Logger::get_instance()->log_table_op(op, tb, succ)
#define LOG_TABLE_OP_MSG(op, tb, succ, msg) Logger::get_instance()->log_table_op(op, tb, succ, msg)

// 数据操作日志宏
#define LOG_DATA_OP(op, tb, sql, succ, rows) Logger::get_instance()->log_data_op(op, tb, sql, succ, rows)
#define LOG_DATA_OP_MSG(op, tb, sql, succ, rows, msg) Logger::get_instance()->log_data_op(op, tb, sql, succ, rows, msg)

#endif
