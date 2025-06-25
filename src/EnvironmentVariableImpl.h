#pragma once

#include "dbcppp/EnvironmentVariable.h"
#include "NodeImpl.h"
#include "AttributeImpl.h"
#include "ValueEncodingDescriptionImpl.h"

namespace dbcppp
{
    class EnvironmentVariableImpl final
        : public IEnvironmentVariable
    {
    public:
        EnvironmentVariableImpl(
              std::string&& name
            , EVarType var_type
            , double minimum
            , double maximum
            , std::string&& unit
            , double initial_value
            , uint64_t ev_id
            , EAccessType access_type
            , std::vector<std::string>&& access_nodes
            , std::vector<ValueEncodingDescriptionImpl>&& value_encoding_descriptions
            , uint64_t data_size
            , std::vector<AttributeImpl>&& attribute_values
            , std::string&& comment);
            
        std::unique_ptr<IEnvironmentVariable> Clone() const override;
        
        const std::string& Name() const override;
        EVarType VarType() const override;
        double Minimum() const override;
        double Maximum() const override;
        const std::string& Unit() const override;
        double InitialValue() const override;
        uint64_t EvId() const override;
        EAccessType AccessType() const override;
        const std::string& AccessNodes_Get(std::size_t i) const override;
        uint64_t AccessNodes_Size() const override;
        const IValueEncodingDescription& ValueEncodingDescriptions_Get(std::size_t i) const override;
        uint64_t ValueEncodingDescriptions_Size() const override;
        uint64_t DataSize() const override;
        const IAttribute& AttributeValues_Get(std::size_t i) const override;
        uint64_t AttributeValues_Size() const override;
        const std::string& Comment() const override;
        
        bool operator==(const IEnvironmentVariable& rhs) const override;
        bool operator!=(const IEnvironmentVariable& rhs) const override;

        std::vector<AttributeImpl>& attributeValues();

    private:
        std::string _name;
        EVarType _var_type;
        double _minimum;
        double _maximum;
        std::string _unit;
        double _initial_value;
        uint64_t _ev_id;
        EAccessType _access_type;
        std::vector<std::string> _access_nodes;
        std::vector<ValueEncodingDescriptionImpl> _value_encoding_descriptions;
        uint64_t _data_size;
        std::vector<AttributeImpl> _attribute_values;
        std::string _comment;
    };
}  // namespace dbcppp

