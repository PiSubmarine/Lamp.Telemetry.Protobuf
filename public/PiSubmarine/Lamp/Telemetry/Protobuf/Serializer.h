#pragma once

#include "PiSubmarine/Lamp/Telemetry/Api/IProvider.h"
#include "PiSubmarine/Telemetry/Api/IRawSource.h"

namespace PiSubmarine::Lamp::Telemetry::Protobuf
{
    class Serializer final : public ::PiSubmarine::Telemetry::Api::IRawSource
    {
    public:
        explicit Serializer(const Api::IProvider& provider);

        [[nodiscard]] Error::Api::Result<std::vector<std::byte>> GetRaw() const override;

    private:
        const Api::IProvider& m_Provider;
    };
}
