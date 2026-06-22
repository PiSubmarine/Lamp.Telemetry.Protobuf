#include <array>

#include <gtest/gtest.h>

#include "Lamp.pb.h"
#include "PiSubmarine/Lamp/Telemetry/Protobuf/Deserializer.h"
#include "PiSubmarine/Lamp/Telemetry/Protobuf/ErrorCode.h"
#include "PiSubmarine/Telemetry/Api/IRawSourceMock.h"

namespace PiSubmarine::Lamp::Telemetry::Protobuf
{
    TEST(DeserializerTest, RejectsInvalidPayload)
    {
        ::PiSubmarine::Telemetry::Api::IRawSourceMock rawSourceMock;
        const std::array<std::byte, 3> payload{std::byte{0x01}, std::byte{0x02}, std::byte{0x03}};

        EXPECT_CALL(rawSourceMock, GetRaw())
            .WillOnce(testing::Return(Error::Api::Result<std::vector<std::byte>>(
                std::vector<std::byte>(payload.begin(), payload.end()))));

        Deserializer deserializer(rawSourceMock);
        const auto result = deserializer.GetStatus();

        ASSERT_FALSE(result.has_value());
        EXPECT_EQ(result.error().Cause, make_error_code(ErrorCode::DeserializationFailed));
    }

    TEST(DeserializerTest, RejectsOutOfRangeIntensity)
    {
        ::pisubmarine::lamp::telemetry::protobuf::Status protoStatus;
        protoStatus.set_intensity(1.5);

        std::string serialized;
        ASSERT_TRUE(protoStatus.SerializeToString(&serialized));

        std::vector<std::byte> payload;
        payload.reserve(serialized.size());
        for (const char character : serialized)
        {
            payload.push_back(static_cast<std::byte>(character));
        }

        ::PiSubmarine::Telemetry::Api::IRawSourceMock rawSourceMock;
        EXPECT_CALL(rawSourceMock, GetRaw())
            .WillOnce(testing::Return(Error::Api::Result<std::vector<std::byte>>(payload)));

        Deserializer deserializer(rawSourceMock);
        const auto result = deserializer.GetStatus();

        ASSERT_FALSE(result.has_value());
        EXPECT_EQ(result.error().Cause, make_error_code(ErrorCode::InvalidPayload));
    }
}
