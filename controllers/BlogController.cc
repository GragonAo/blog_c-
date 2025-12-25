#include "BlogController.h"

Task<HttpResponsePtr> BlogController::getList(HttpRequestPtr req, size_t page, size_t size)
{
    auto blogs = co_await blogService->getBlogList(page, size);
    Json::Value ret;
    ret["code"] = 200;
    Json::Value data(Json::arrayValue);
    for (auto &b : blogs)
    {
        data.append(b.toJson());
    }
    ret["data"] = data;
    co_return HttpResponse::newHttpJsonResponse(ret);
}

Task<HttpResponsePtr> BlogController::create(HttpRequestPtr req)
{
    auto json = req->getJsonObject();
    if (!json)
    {
        Json::Value err;
        err["error"] = "Invalid JSON";
        co_return HttpResponse::newHttpJsonResponse(err);
    }
    try 
    {
        auto newBlog = co_await blogService->createBlog(*json);
        Json::Value ret;
        ret["code"] = 201;
        ret["data"] = newBlog.toJson();
        co_return HttpResponse::newHttpJsonResponse(ret);
    }
    catch (const std::exception &e)
    {
        Json::Value ret;
        ret["code"] = 500;
        ret["error"] = e.what();
        co_return HttpResponse::newHttpJsonResponse(ret);
    }
}