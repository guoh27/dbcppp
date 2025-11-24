#pragma once

#include <vector>
#include <memory>

#include "dbcppp/Node.h"
#include "AttributeImpl.h"

namespace dbcppp
{
    class INetwork;

    class NodeImpl final
        : public INode
    {
    public:
        NodeImpl(
              std::string&& name
            , std::string&& comment
            , std::vector<AttributeImpl>&& attribute_values);
            
        std::unique_ptr<INode> Clone() const override;

        const std::string& Name() const override;
        const IAttribute& AttributeValues_Get(std::size_t i) const override;
        uint64_t AttributeValues_Size() const override;
        std::optional<std::reference_wrapper<const IAttribute>> AttributeValue(const std::string& name) const override;
        const std::string& Comment() const override;
        
        bool operator==(const INode& rhs) const override;
        bool operator!=(const INode& rhs) const override;

        std::vector<AttributeImpl>& attributeValues();
        void setNetwork(const INetwork* network);

    private:
        std::string _name;
        std::string _comment;
        std::vector<AttributeImpl> _attribute_values;
        const INetwork* _network = nullptr;
    };
}  // namespace dbcppp
