
#pragma once

#include <dbcppp/SignalMultiplexerValue.h>

namespace dbcppp
{
    class SignalMultiplexerValueImpl
        : public ISignalMultiplexerValue
    {
    public:
        SignalMultiplexerValueImpl(
              std::string&& switch_name
            , std::vector<Range>&& value_ranges);
            
        std::unique_ptr<ISignalMultiplexerValue> Clone() const override;

        const std::string& SwitchName() const override;
        const Range& ValueRanges_Get(std::size_t i) const override;
        uint64_t ValueRanges_Size() const override;

        bool operator==(const ISignalMultiplexerValue& rhs) const override;
        bool operator!=(const ISignalMultiplexerValue& rhs) const override;

    private:
        std::string _switch_name;
        std::vector<Range> _value_ranges;
    };
}  // namespace dbcppp
