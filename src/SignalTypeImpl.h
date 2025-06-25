#pragma once

#include <cstdint>
#include "dbcppp/SignalType.h"

namespace dbcppp
{
    class SignalTypeImpl final
        : public ISignalType
    {
    public:
        SignalTypeImpl(
              std::string&& name
            , uint64_t signal_size
            , ISignal::EByteOrder byte_order
            , ISignal::EValueType value_type
            , double factor
            , double offset
            , double minimum
            , double maximum
            , std::string&& unit
            , double default_value
            , std::string&& value_table);

        std::unique_ptr<ISignalType> Clone() const override;

        const std::string& Name() const override;
        uint64_t SignalSize() const override;
        ISignal::EByteOrder ByteOrder() const override;
        ISignal::EValueType ValueType() const override;
        double Factor() const override;
        double Offset() const override;
        double Minimum() const override;
        double Maximum() const override;
        const std::string& Unit() const override;
        double DefaultValue() const override;
        const std::string& ValueTable() const override;
        
        bool operator==(const ISignalType& rhs) const override;
        bool operator!=(const ISignalType& rhs) const override;

    private:
        std::string _name;
        uint64_t _signal_size;
        ISignal::EByteOrder _byte_order;
        ISignal::EValueType _value_type;
        double _factor;
        double _offset;
        double _minimum;
        double _maximum;
        std::string _unit;
        double _default_value;
        std::string _value_table;
    };
}  // namespace dbcppp
