#include <gtest/gtest.h>

#include "PiSubmarine/Lamp/Telemetry/Api/IProviderMock.h"
#include "PiSubmarine/Lamp/Telemetry/Protobuf/Deserializer.h"
#include "PiSubmarine/Lamp/Telemetry/Protobuf/Serializer.h"
#include "PiSubmarine/Telemetry/Api/IRawSourceMock.h"

namespace PiSubmarine::Lamp::Telemetry::Protobuf
{
    TEST(SerializerTest, RoundTripsLampStatus)
    {
        Api::IProviderMock providerMock;
        const Api::Status expectedStatus{
            .Intensity = NormalizedFraction{0.7},
            .HasOpenLoadFault = false,
            .HasOvercurrentFault = true,
            .HasOvertemperatureShutdownFault = false,
            .HasUndervoltageFault = true,
            .HasOvervoltageFault = false,
            .HasOvertemperatureWarning = true};

        EXPECT_CALL(providerMock, GetStatus())
            .WillOnce(testing::Return(Error::Api::Result<Api::Status>(expectedStatus)));

        Serializer serializer(providerMock);
        const auto rawResult = serializer.GetRaw();

        ASSERT_TRUE(rawResult.has_value());
        EXPECT_FALSE(rawResult->empty());

        ::PiSubmarine::Telemetry::Api::IRawSourceMock rawSourceMock;
        EXPECT_CALL(rawSourceMock, GetRaw())
            .WillOnce(testing::Return(Error::Api::Result<std::vector<std::byte>>(*rawResult)));

        Deserializer deserializer(rawSourceMock);
        const auto statusResult = deserializer.GetStatus();

        ASSERT_TRUE(statusResult.has_value());
        EXPECT_EQ(*statusResult, expectedStatus);
    }
}
