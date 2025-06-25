
#pragma once

#include <string>
#include <dbcppp/ValueEncodingDescription.h>

namespace dbcppp
{
    class ValueEncodingDescriptionImpl
        : public IValueEncodingDescription
    {
    public:
        std::unique_ptr<IValueEncodingDescription> Clone() const override;

        ValueEncodingDescriptionImpl(int64_t value, std::string&& description);
        int64_t Value() const override;
        const std::string& Description() const override;
        
        bool operator==(const IValueEncodingDescription& rhs) const override;
        bool operator!=(const IValueEncodingDescription& rhs) const override;

    private:
        int64_t _value;
        std::string _description;
    };
}  // namespace dbcppp
