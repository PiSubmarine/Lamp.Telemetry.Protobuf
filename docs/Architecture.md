# Lamp.Telemetry.Protobuf

`PiSubmarine.Lamp.Telemetry.Protobuf` adapts `Lamp.Telemetry.Api` to and from
raw protobuf payloads.

- `Serializer` implements `Telemetry.Api::IRawSource` by querying a local
  `Lamp::Telemetry::Api::IProvider`
- `Deserializer` implements `Lamp::Telemetry::Api::IProvider` by reading bytes
  from a `Telemetry.Api::IRawSource`
