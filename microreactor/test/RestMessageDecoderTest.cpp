#include "gtest/gtest.h"
#include "RestMessageDecoder.h"

using namespace sg::microreactor;


class RestMessageDecoderMock : public RestMessageDecoder
{
public:
    void GetRestReactorFactoryTable(MethodMap& restReactorFactoryTable)
    {
        restReactorFactoryTable = mRestReactorFactoryTable;
    }

    virtual RestReactorFactory GetRestReactorFactory(std::shared_ptr<RestRequest> restRequest) override
    {
        return RestMessageDecoder::GetRestReactorFactory(restRequest);
    }
};

class ReactorFactoryMock
{
public:
    std::shared_ptr<Reactor> CreateReactor(std::shared_ptr<RestRequest>, Connection&)
    {
        return nullptr;
    }
};


TEST(RestMessageDecoderTest, RegisterRestFactoryNormal)
{
    ReactorFactoryMock factory;   
    RestMessageDecoderMock decoder;
    decoder.RegisterRestReactorFactory("GET", "/api_1/data", std::bind(&ReactorFactoryMock::CreateReactor, &factory, std::placeholders::_1, std::placeholders::_2));
    decoder.RegisterRestReactorFactory("POST", "/api_2/save", std::bind(&ReactorFactoryMock::CreateReactor, &factory, std::placeholders::_1, std::placeholders::_2));

    auto restRequest_1 = std::make_shared<RestRequest>();
    restRequest_1->mMethod = "GET";
    restRequest_1->mUri = "/api_1/data";

    auto found_api_1 = decoder.GetRestReactorFactory(restRequest_1);
    EXPECT_NE(nullptr, found_api_1);

    auto restRequest_2 = std::make_shared<RestRequest>();
    restRequest_2->mMethod = "POST";
    restRequest_2->mUri = "/api_2/save";

    auto found_api_2 = decoder.GetRestReactorFactory(restRequest_2);
    EXPECT_NE(nullptr, found_api_2);

    auto restRequest_3 = std::make_shared<RestRequest>();
    restRequest_3->mMethod = "POST";
    restRequest_3->mUri = "/api_3/save";

    auto found_api_3 = decoder.GetRestReactorFactory(restRequest_3);
    EXPECT_EQ(nullptr, found_api_3);

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

