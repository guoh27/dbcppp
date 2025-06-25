#pragma once

#include "dbcppp/AttributeDefinition.h"

namespace dbcppp
{
    class AttributeDefinitionImpl final
        : public IAttributeDefinition
    {
    public:
        AttributeDefinitionImpl(std::string&& name, EObjectType object_type, value_type_t value_type);

        std::unique_ptr<IAttributeDefinition> Clone() const override;

        EObjectType ObjectType() const override;
        const std::string& Name() const override;
        const value_type_t& ValueType() const override;
        EDefinitionType DefinitionType() const override;

        bool operator==(const IAttributeDefinition& rhs) const override;
        bool operator!=(const IAttributeDefinition& rhs) const override;

    private:
        std::string _name;
        EObjectType _object_type;
        value_type_t _value_type;
        EDefinitionType _definition_type;
    };
}  // namespace dbcppp
