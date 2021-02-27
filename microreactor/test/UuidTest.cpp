#include "gtest/gtest.h"
#include "Uuid.h"

using namespace microreactor;


TEST(UuidTest, GenerateUuid)
{
    Uuid uuid = Uuid::GenerateUuid();
    EXPECT_FALSE(uuid.IsNull());
}

TEST(UuidTest, Clear)
{
    Uuid uuid = Uuid::GenerateUuid();
    EXPECT_FALSE(uuid.IsNull());
    
    uuid.Clear();
    EXPECT_TRUE(uuid.IsNull());
}

TEST(UuidTest, ToString)
{
    Uuid uuid = Uuid::GenerateUuid();
    EXPECT_FALSE(uuid.IsNull());
    
    std::string uuidString = uuid.ToString();
    EXPECT_FALSE(uuidString.empty());

    std::string testUuid("5100b3be-d2d5-4ff3-b655-9e1bb9b7e3b0");
    Uuid uuid2(testUuid);
    EXPECT_FALSE(uuid2.IsNull());
    
    std::string uuid2String = uuid2.ToString();
    EXPECT_EQ(uuid2String, testUuid);
}
