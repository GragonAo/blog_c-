#include "services/BlogService.h"
#include <drogon/HttpAppFramework.h>

// 获取列表
drogon::Task<std::vector<BlogModel>> BlogService::getBlogList(size_t page, size_t pageSize) 
{
    auto db = drogon::app().getDbClient();
    long long limit = static_cast<long long>(pageSize);
    long long offset = (page > 0) ? static_cast<long long>((page - 1) * pageSize) : 0;

    std::string sql = "SELECT * FROM blog ORDER BY id DESC LIMIT $1 OFFSET $2";

    try {
        auto result = co_await db->execSqlCoro(sql, limit, offset);
        
        std::vector<BlogModel> blogs;
        for (const auto &row : result) {
            blogs.emplace_back(row);
        }
        co_return blogs;
    } catch (const std::exception &e) {
        LOG_ERROR << "List fetch error: " << e.what();
        co_return std::vector<BlogModel>{};
    }
}

// 创建博客
drogon::Task<BlogModel> BlogService::createBlog(Json::Value json) 
{
    auto db = drogon::app().getDbClient();
    std::string sql = "INSERT INTO blog (title, content, summary, created_at) "
                      "VALUES ($1, $2, $3, $4) RETURNING *";

    std::string title = json.get("title", "").asString();
    std::string content = json.get("content", "").asString();
    std::string summary = json.get("summary", "").asString();
    auto now = trantor::Date::now();

    auto result = co_await db->execSqlCoro(sql, title, content, summary, now);

    co_return BlogModel(result[0]);
}