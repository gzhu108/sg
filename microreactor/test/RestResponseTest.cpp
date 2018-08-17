#include "gtest/gtest.h"
#include "RestResponse.h"

using namespace sg::microreactor;


TEST(RestResponseTest, ParseResponse)
{
    auto message = std::make_shared<std::string>("HTTP/1.1 200 OK\r\nDate: Fri, 17 Aug 2018 20:46:47 GMT\r\n\r\n<html></html>");

    RestResponse response;
    response.Parse(message);

    EXPECT_EQ(message, response.mRawMessage);
    EXPECT_EQ(200, response.mStatusCode);
    EXPECT_EQ("OK", response.mStatusText);
    EXPECT_EQ("<html></html>", std::string(response.mBody.mOffset, response.mBody.mLength));
}
