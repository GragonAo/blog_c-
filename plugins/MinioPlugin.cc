#include <drogon/plugins/Plugin.h>
#include <json/json.h>
#include <drogon/HttpClient.h>

using namespace drogon;

class MinioPlugin : public drogon::Plugin<MinioPlugin>
{
public:
    MinioPlugin() {}

    void initAndStart(const Json::Value &config) override
    {
        LOG_INFO << "MinioPlugin initializing...";
        // 读取配置
        _endpoint = config.get("endpoint", "127.0.0.1:9000").asString();
        _accessKey = config.get("access_key", "admin").asString();
        _secretKey = config.get("secret_key", "your_strong_password").asString();
        _useSSL = config.get("use_ssl", false).asBool();
        _bucketName = config.get("bucket_name", "blog").asString();

        _client =  HttpClient::newHttpClient((_useSSL ? "https://" : "http://") + _endpoint);
        
        LOG_INFO << "MinioPlugin connected to: " << _endpoint << ", Bucket: " << _bucketName;
    }

    void shutdown() override 
    {
        LOG_INFO << "MinioPlugin shutting down.";
    }

    /**
     *  PUT 上传 (使用 Drogon HttpClient)
     * 生产中建议结合 S3 V4 签名算法或使用 AWS SDK C++
     */
    void uploadFile(const std::string &objectName, 
                    const std::string &content, 
                    const std::string &contentType,
                    std::function<void(bool)> callback)
    {
        auto req = drogon::HttpRequest::newHttpRequest();
        req->setMethod(drogon::HttpMethod::Put);
        req->setPath("/" + _bucketName + "/" + objectName);
        req->setBody(content);
        req->addHeader("Content-Type", contentType);

        // 注意：Minio 默认需要 S3 签名
        // 如果开启了 Public 策略，简单 PUT 可能成功，否则需要计算 Authorization Header
        _client->sendRequest(req, [callback](drogon::ReqResult result, const drogon::HttpResponsePtr &resp) {
            if (result == drogon::ReqResult::Ok && resp->statusCode() == drogon::k200OK) {
                callback(true);
            } else {
                LOG_ERROR << "Minio upload failed: " << (resp ? resp->body() : "No response");
                callback(false);
            }
        });
    }

private:
    std::string _endpoint;
    std::string _accessKey;
    std::string _secretKey;
    std::string _bucketName;
    bool _useSSL;
    drogon::HttpClientPtr _client;
};