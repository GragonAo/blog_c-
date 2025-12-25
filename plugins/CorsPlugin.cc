#include <drogon/plugins/Plugin.h>
#include <drogon/HttpAppFramework.h>
#include <drogon/HttpResponse.h>

class CorsPlugin : public drogon::Plugin<CorsPlugin>
{
    
public:
    CorsPlugin() {}

    void initAndStart(const Json::Value &config) override
    {
        LOG_INFO << "CorsPlugin initialized!";
        // 从配置文件读取参数 (类似 Spring Boot 的 @Value)
        _allowCredentials = config.get("allow_credentials", "true").asString();
        _allowedOrigin = config.get("allowed_origin", "*").asString();

        // 1. 注册同步建议 (Sync Advice) 处理 OPTIONS 预检请求
        drogon::app().registerSyncAdvice([this](const drogon::HttpRequestPtr &req)
                                             -> drogon::HttpResponsePtr {
            if (req->method() == drogon::HttpMethod::Options)
            {
                auto resp = drogon::HttpResponse::newHttpResponse();
                
                // 动态获取或使用配置的 Origin
                const auto &origin = req->getHeader("Origin");
                resp->addHeader("Access-Control-Allow-Origin", _allowedOrigin == "*" ? (origin.empty() ? "*" : origin) : _allowedOrigin);
                
                const auto &requestMethod = req->getHeader("Access-Control-Request-Method");
                if (!requestMethod.empty())
                {
                    resp->addHeader("Access-Control-Allow-Methods", requestMethod);
                }

                resp->addHeader("Access-Control-Allow-Credentials", _allowCredentials);

                const auto &requestHeaders = req->getHeader("Access-Control-Request-Headers");
                if (!requestHeaders.empty())
                {
                    resp->addHeader("Access-Control-Allow-Headers", requestHeaders);
                }
                return resp;
            }
            return nullptr; // 返回空以继续执行后续 Handler
        });

        // 2. 注册处理后建议 (Post Handling Advice)
        drogon::app().registerPostHandlingAdvice(
            [this](const drogon::HttpRequestPtr &req, const drogon::HttpResponsePtr &resp) {
                const auto &origin = req->getHeader("Origin");
                if (!origin.empty())
                {
                    resp->addHeader("Access-Control-Allow-Origin", _allowedOrigin == "*" ? origin : _allowedOrigin);
                    resp->addHeader("Access-Control-Allow-Credentials", _allowCredentials);
                }
            });
    }

    void shutdown() override {}

private:
    std::string _allowCredentials;
    std::string _allowedOrigin;
};