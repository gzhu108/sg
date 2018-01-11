#include "gtest/gtest.h"
#include "RestMessageDecoder.h"

using namespace sg::microreactor;


class RestMessageDecoderMock : public RestMessageDecoder
{
public:
    void GetRegisteredRestFactorys(std::map<std::string, RestMessageDecoder::RestFactoryList>& registeredRestFactorys)
    {
        registeredRestFactorys = mRestFactoryTable;
    }

    virtual std::shared_ptr<RestFactory> GetRestFactory(std::shared_ptr<RestRequest> restRequest) override
    {
        return RestMessageDecoder::GetRestFactory(restRequest);
    }
};

class RestFactoryMock : public RestFactory
{
public:
    RestFactoryMock(const std::string& method, const std::string& path, const std::string& contentType)
        : RestFactory(method, path, contentType)
    {
    }

    virtual std::shared_ptr<sg::microreactor::Reactor> CreateReactor(std::shared_ptr<RestRequest> request, sg::microreactor::Connection& connection) override
    {
        return nullptr;
    }
};


TEST(RestMessageDecoderTest, RegisterRestFactoryNormal)
{
    auto api_1 = std::make_shared<RestFactoryMock>("GET", "/api_1/data", "ContentType: text/plain");
    auto api_2 = std::make_shared<RestFactoryMock>("POST", "/api_2/save", "ContentType: application/json");
    
    RestMessageDecoderMock decoder;
    decoder.RegisterRestFactory(api_1);
    decoder.RegisterRestFactory(api_2);

    auto restRequest_1 = std::make_shared<RestRequest>();
    restRequest_1->mMethod = "GET";
    restRequest_1->mUri = "/api_1/data";

    auto found_api_1 = decoder.GetRestFactory(restRequest_1);
    EXPECT_NE(nullptr, found_api_1);
    EXPECT_EQ("/api_1/data", found_api_1->mPath);
    EXPECT_EQ("GET", found_api_1->mMethod);

    auto restRequest_2 = std::make_shared<RestRequest>();
    restRequest_2->mMethod = "POST";
    restRequest_2->mUri = "/api_2/save";

    auto found_api_2 = decoder.GetRestFactory(restRequest_2);
    EXPECT_NE(nullptr, found_api_2);
    EXPECT_EQ("/api_2/save", found_api_2->mPath);
    EXPECT_EQ("POST", found_api_2->mMethod);


#if 0
    std::map<std::string, RestMessageDecoder::RestFactoryList> registeredRestFactorys;
    codec.GetRegisteredRestFactorys(registeredRestFactorys);
    
    auto found_1 = registeredRestFactorys.find("/api_1/data");
    EXPECT_NE(registeredRestFactorys.end(), found_1.first);
    EXPECT_EQ("/api_1/data", found_1.first->first);
    EXPECT_EQ("/api_1/data", found_1.first->second->mPath);
    EXPECT_EQ("GET", found_1.first->second->mMethod);
    EXPECT_EQ("test/plain", found_1.first->second->mContentType);
    
    auto found_2 = registeredRestFactorys.find("/api_2/save");
    EXPECT_NE(registeredRestFactorys.end(), found_2.first);
    EXPECT_EQ("/api_2/save", found_2.first->first);
    EXPECT_EQ("/api_2/save", found_2.first->second->mPath);
    EXPECT_EQ("POST", found_2.first->second->mMethod);
    EXPECT_EQ("application/json", found_2.first->second->mContentType);
#endif
}

