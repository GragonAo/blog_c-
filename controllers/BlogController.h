#pragma once
#include <drogon/HttpController.h>
#include <services/BlogService.h>

using namespace drogon;

class BlogController : public HttpController<BlogController>
{
  public:
    METHOD_LIST_BEGIN
        // 定义路由：GET /blogs?page=1&size=10
        ADD_METHOD_TO(BlogController::getList, "/blogs?page={1}&size={2}", Get);
        // 定义路由：POST /blogs
        ADD_METHOD_TO(BlogController::create, "/blogs", Post);
    METHOD_LIST_END

    // 获取列表
    Task<HttpResponsePtr> getList(HttpRequestPtr req, size_t page, size_t size);

    // 创建文章
    Task<HttpResponsePtr> create(HttpRequestPtr req);

private:
    std::shared_ptr<BlogService> blogService = BlogService::instance();
};