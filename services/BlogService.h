#pragma once
#include <services/Service.h>
#include <drogon/utils/coroutine.h>
#include <vector>
#include <json/json.h>
#include <models/Blog.h>

using BlogModel = drogon_model::blog_v2::Blog;

class BlogService : public Service<BlogService> 
{
public:
    drogon::Task<std::vector<BlogModel>> getBlogList(size_t page, size_t pageSize);

    drogon::Task<BlogModel> getBlogById(int id);

    drogon::Task<BlogModel> createBlog(Json::Value json);

    drogon::Task<bool> deleteBlog(int id);
};