
#pragma once

#include <iostream>
#include "dbcppp/Attribute.h"

namespace dbcppp
{
    class AttributeImpl final
        : public IAttribute
    {
    public:
        AttributeImpl(std::string&& name,
            IAttributeDefinition::EObjectType object_type,
            IAttribute::value_t value,
            std::shared_ptr<const IAttributeDefinition> definition);

        std::unique_ptr<IAttribute> Clone() const override;

        const std::string& Name() const override;
        IAttributeDefinition::EObjectType ObjectType() const override;
        const value_t& Value() const override;
        std::shared_ptr<const IAttributeDefinition> Definition() const override;
        
        bool operator==(const IAttribute& rhs) const override;
        bool operator!=(const IAttribute& rhs) const override;

    private:
        std::string _name;
        IAttributeDefinition::EObjectType _object_type;
        IAttribute::value_t _value;
        std::shared_ptr<const IAttributeDefinition> _definition;
    };
}  // namespace dbcppp
