#include "PiSubmarine/Lamp/Telemetry/Protobuf/Deserializer.h"

#include "Lamp.pb.h"
#include "PiSubmarine/Lamp/Telemetry/Protobuf/ErrorCode.h"
#include "PiSubmarine/Error/Api/MakeError.h"

namespace PiSubmarine::Lamp::Telemetry::Protobuf
{
    namespace
    {
        [[nodiscard]] Error::Api::Error MakeLampTelemetryError(
            const Error::Api::ErrorCondition condition,
            const ErrorCode code)
        {
            return Error::Api::MakeError(condition, make_error_code(code));
        }
    }

    Deserializer::Deserializer(const ::PiSubmarine::Telemetry::Api::IRawSource& rawSource)
        : m_RawSource(rawSource)
    {
    }

    Error::Api::Result<Api::Status> Deserializer::GetStatus() const
    {
        const auto rawResult = m_RawSource.GetRaw();
        if (!rawResult.has_value())
        {
            return std::unexpected(rawResult.error());
        }

        ::pisubmarine::lamp::telemetry::protobuf::Status protoStatus;
        if (!protoStatus.ParseFromArray(reinterpret_cast<const char*>(rawResult->data()), static_cast<int>(rawResult->size())))
        {
            return std::unexpected(MakeLampTelemetryError(
                Error::Api::ErrorCondition::ContractError,
                ErrorCode::DeserializationFailed));
        }

        Api::Status status{
            .IsActive = protoStatus.is_active(),
            .HasOpenLoadFault = protoStatus.has_open_load_fault(),
            .HasOvercurrentFault = protoStatus.has_overcurrent_fault(),
            .HasOvertemperatureShutdownFault = protoStatus.has_overtemperature_shutdown_fault(),
            .HasUndervoltageFault = protoStatus.has_undervoltage_fault(),
            .HasOvervoltageFault = protoStatus.has_overvoltage_fault(),
            .HasOvertemperatureWarning = protoStatus.has_overtemperature_warning()};

        return status;
    }
}
