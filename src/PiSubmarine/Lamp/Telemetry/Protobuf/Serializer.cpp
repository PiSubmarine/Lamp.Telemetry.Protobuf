#include "PiSubmarine/Lamp/Telemetry/Protobuf/Serializer.h"

#include <string>

#include "Lamp.pb.h"
#include "PiSubmarine/Lamp/Telemetry/Protobuf/ErrorCode.h"
#include "PiSubmarine/Error/Api/MakeError.h"

namespace PiSubmarine::Lamp::Telemetry::Protobuf
{
    Serializer::Serializer(const Api::IProvider& provider)
        : m_Provider(provider)
    {
    }

    Error::Api::Result<std::vector<std::byte>> Serializer::GetRaw() const
    {
        const auto statusResult = m_Provider.GetStatus();
        if (!statusResult.has_value())
        {
            return std::unexpected(statusResult.error());
        }

        ::pisubmarine::lamp::telemetry::protobuf::Status protoStatus;
        protoStatus.set_intensity(static_cast<double>(statusResult->Intensity));
        protoStatus.set_has_open_load_fault(statusResult->HasOpenLoadFault);
        protoStatus.set_has_overcurrent_fault(statusResult->HasOvercurrentFault);
        protoStatus.set_has_overtemperature_shutdown_fault(statusResult->HasOvertemperatureShutdownFault);
        protoStatus.set_has_undervoltage_fault(statusResult->HasUndervoltageFault);
        protoStatus.set_has_overvoltage_fault(statusResult->HasOvervoltageFault);
        protoStatus.set_has_overtemperature_warning(statusResult->HasOvertemperatureWarning);

        std::string serialized;
        if (!protoStatus.SerializeToString(&serialized))
        {
            return std::unexpected(Error::Api::MakeError(
                Error::Api::ErrorCondition::DeviceError,
                make_error_code(ErrorCode::SerializationFailed)));
        }

        std::vector<std::byte> bytes;
        bytes.reserve(serialized.size());
        for (const char character : serialized)
        {
            bytes.push_back(static_cast<std::byte>(character));
        }

        return bytes;
    }
}
