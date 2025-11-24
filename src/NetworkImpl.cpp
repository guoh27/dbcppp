#include <algorithm>
#include <fstream>
#include <iomanip>
#include <unordered_map>
#include <optional>
#include "dbcppp/Network.h"
#include "NetworkImpl.h"
#include "Helper.h"

using namespace dbcppp;

namespace
{
    std::optional<std::reference_wrapper<const IAttribute>> find_attribute_by_name(
        const std::vector<AttributeImpl>& attributes,
        const std::string& name)
    {
        auto it = std::find_if(attributes.begin(), attributes.end(),
            [&](const AttributeImpl& attr) { return attr.Name() == name; });
        if (it != attributes.end())
        {
            return std::cref(static_cast<const IAttribute&>(*it));
        }
        return std::nullopt;
    }
}

std::unique_ptr<INetwork> INetwork::Create(
      std::string&& version
    , std::vector<std::string>&& new_symbols
    , std::unique_ptr<IBitTiming>&& bit_timing
    , std::vector<std::unique_ptr<INode>>&& nodes
    , std::vector<std::unique_ptr<IValueTable>>&& value_tables
    , std::vector<std::unique_ptr<IMessage>>&& messages
    , std::vector<std::unique_ptr<IEnvironmentVariable>>&& environment_variables
    , std::vector<std::shared_ptr<IAttributeDefinition>>&& attribute_definitions
    , std::vector<std::unique_ptr<IAttribute>>&& attribute_defaults
    , std::vector<std::unique_ptr<IAttribute>>&& attribute_values
    , std::string&& comment)
{
    BitTimingImpl bt = std::move(dynamic_cast<BitTimingImpl&>(*bit_timing));
    bit_timing.reset(nullptr);
    std::vector<NodeImpl> ns;
    std::vector<ValueTableImpl> vts;
    std::vector<MessageImpl> ms;
    std::vector<EnvironmentVariableImpl> evs;
    std::vector<std::shared_ptr<AttributeDefinitionImpl>> ads;
    std::vector<AttributeImpl> avds;
    std::vector<AttributeImpl> avs;
    for (auto& n : nodes)
    {
        ns.push_back(std::move(dynamic_cast<NodeImpl&>(*n)));
        n.reset(nullptr);
    }
    for (auto& vt : value_tables)
    {
        vts.push_back(std::move(dynamic_cast<ValueTableImpl&>(*vt)));
        vt.reset(nullptr);
    }
    for (auto& m : messages)
    {
        ms.push_back(std::move(dynamic_cast<MessageImpl&>(*m)));
        m.reset(nullptr);
    }
    for (auto& ev : environment_variables)
    {
        evs.push_back(std::move(dynamic_cast<EnvironmentVariableImpl&>(*ev)));
        ev.reset(nullptr);
    }
    for (auto& ad : attribute_definitions)
    {
        auto ptr = std::static_pointer_cast<AttributeDefinitionImpl>(ad);
        ads.push_back(std::move(ptr));
    }
    for (auto& ad : attribute_defaults)
    {
        AttributeImpl attr = std::move(dynamic_cast<AttributeImpl&>(*ad));
        avds.push_back(std::move(attr));
        ad.reset(nullptr);
    }
    for (auto& av : attribute_values)
    {
        AttributeImpl attr = std::move(dynamic_cast<AttributeImpl&>(*av));
        avs.push_back(std::move(attr));
        av.reset(nullptr);
    }
    return std::make_unique<NetworkImpl>(
          std::move(version)
        , std::move(new_symbols)
        , std::move(bt)
        , std::move(ns)
        , std::move(vts)
        , std::move(ms)
        , std::move(evs)
        , std::move(ads)
        , std::move(avds)
        , std::move(avs)
        , std::move(comment));
}

NetworkImpl::NetworkImpl(const NetworkImpl& other)
    : _version(other._version)
    , _new_symbols(other._new_symbols)
    , _bit_timing(other._bit_timing)
    , _nodes(other._nodes)
    , _value_tables(other._value_tables)
    , _messages(other._messages)
    , _environment_variables(other._environment_variables)
    , _attribute_definitions(other._attribute_definitions)
    , _attribute_defaults(other._attribute_defaults)
    , _attribute_values(other._attribute_values)
    , _comment(other._comment)
{
    setNetworkPointers();
}

NetworkImpl::NetworkImpl(NetworkImpl&& other) noexcept
    : _version(std::move(other._version))
    , _new_symbols(std::move(other._new_symbols))
    , _bit_timing(std::move(other._bit_timing))
    , _nodes(std::move(other._nodes))
    , _value_tables(std::move(other._value_tables))
    , _messages(std::move(other._messages))
    , _environment_variables(std::move(other._environment_variables))
    , _attribute_definitions(std::move(other._attribute_definitions))
    , _attribute_defaults(std::move(other._attribute_defaults))
    , _attribute_values(std::move(other._attribute_values))
    , _comment(std::move(other._comment))
{
    setNetworkPointers();
}

NetworkImpl::NetworkImpl(
      std::string&& version
    , std::vector<std::string>&& new_symbols
    , BitTimingImpl&& bit_timing
    , std::vector<NodeImpl>&& nodes
    , std::vector<ValueTableImpl>&& value_tables
    , std::vector<MessageImpl>&& messages
    , std::vector<EnvironmentVariableImpl>&& environment_variables
    , std::vector<std::shared_ptr<AttributeDefinitionImpl>>&& attribute_definitions
    , std::vector<AttributeImpl>&& attribute_defaults
    , std::vector<AttributeImpl>&& attribute_values
    , std::string&& comment)

    : _version(std::move(version))
    , _new_symbols(std::move(new_symbols))
    , _bit_timing(std::move(bit_timing))
    , _nodes(std::move(nodes))
    , _value_tables(std::move(value_tables))
    , _messages(std::move(messages))
    , _environment_variables(std::move(environment_variables))
    , _attribute_definitions(std::move(attribute_definitions))
    , _attribute_defaults(std::move(attribute_defaults))
    , _attribute_values(std::move(attribute_values))
    , _comment(std::move(comment))
{
    setNetworkPointers();
}
std::unique_ptr<INetwork> NetworkImpl::Clone() const
{
    return std::make_unique<NetworkImpl>(*this);
}
const std::string& NetworkImpl::Version() const
{
    return _version;
}
const std::string& NetworkImpl::NewSymbols_Get(std::size_t i) const
{
    return _new_symbols[i];
}
uint64_t NetworkImpl::NewSymbols_Size() const
{
    return _new_symbols.size();
}
const IBitTiming& NetworkImpl::BitTiming() const
{
    return _bit_timing;
}
const INode& NetworkImpl::Nodes_Get(std::size_t i) const
{
    return _nodes[i];
}
uint64_t NetworkImpl::Nodes_Size() const
{
    return _nodes.size();
}
const IValueTable& NetworkImpl::ValueTables_Get(std::size_t i) const
{
    return _value_tables[i];
}
uint64_t NetworkImpl::ValueTables_Size() const
{
    return _value_tables.size();
}
const IMessage& NetworkImpl::Messages_Get(std::size_t i) const
{
    return _messages[i];
}
uint64_t NetworkImpl::Messages_Size() const
{
    return _messages.size();
}
const IEnvironmentVariable& NetworkImpl::EnvironmentVariables_Get(std::size_t i) const
{
    return _environment_variables[i];
}
uint64_t NetworkImpl::EnvironmentVariables_Size() const
{
    return _environment_variables.size();
}
const IAttributeDefinition& NetworkImpl::AttributeDefinitions_Get(std::size_t i) const
{
    return *(_attribute_definitions[i]);
}
uint64_t NetworkImpl::AttributeDefinitions_Size() const
{
    return _attribute_definitions.size();
}
const IAttribute& NetworkImpl::AttributeDefaults_Get(std::size_t i) const
{
    return _attribute_defaults[i];
}
uint64_t NetworkImpl::AttributeDefaults_Size() const
{
    return _attribute_defaults.size();
}
const IAttribute& NetworkImpl::AttributeValues_Get(std::size_t i) const
{
    return _attribute_values[i];
}
uint64_t NetworkImpl::AttributeValues_Size() const
{
    return _attribute_values.size();
}
std::optional<std::reference_wrapper<const IAttribute>> NetworkImpl::AttributeValue(const std::string& name) const
{
    if (auto attr = find_attribute_by_name(_attribute_values, name))
    {
        return *attr;
    }
    if (auto def = findAttributeDefault(IAttributeDefinition::EObjectType::Network, name))
    {
        return *def;
    }
    return std::nullopt;
}
const std::string& NetworkImpl::Comment() const
{
    return _comment;
}
const IMessage* NetworkImpl::ParentMessage(const ISignal* sig) const
{
    const IMessage* parent = nullptr;
    for (const auto& msg : _messages)
    {
        auto iter = std::find_if(msg.signals().begin(), msg.signals().end(),
            [&](const SignalImpl& other) { return &other == sig; });
        if (iter != msg.signals().end())
        {
            parent = &msg;
            break;
        }
    }
    return parent;
}
std::string& NetworkImpl::version()
{
    return _version;
}
std::vector<std::string>& NetworkImpl::newSymbols()
{
    return _new_symbols;
}
BitTimingImpl& NetworkImpl::bitTiming()
{
    return _bit_timing;
}
std::vector<NodeImpl>& NetworkImpl::nodes()
{
    return _nodes;
}
std::vector<ValueTableImpl>& NetworkImpl::valueTables()
{
    return _value_tables;
}
std::vector<MessageImpl>& NetworkImpl::messages()
{
    return _messages;
}
std::vector<EnvironmentVariableImpl>& NetworkImpl::environmentVariables()
{
    return _environment_variables;
}
std::vector<std::shared_ptr<AttributeDefinitionImpl>>& NetworkImpl::attributeDefinitions()
{
    return _attribute_definitions;
}
std::vector<AttributeImpl>& NetworkImpl::attributeDefaults()
{
    return _attribute_defaults;
}
std::vector<AttributeImpl>& NetworkImpl::attributeValues()
{
    return _attribute_values;
}
std::string& NetworkImpl::comment()
{
    return _comment;
}
void INetwork::Merge(std::unique_ptr<INetwork>&& other)
{
    auto& self = dynamic_cast<NetworkImpl&>(*this);
    auto& o = dynamic_cast<NetworkImpl&>(*other);

    unique_merge(self.newSymbols(), o.newSymbols());
    unique_merge_by_name(self.nodes(), o.nodes());
    unique_merge_by_name(self.valueTables(), o.valueTables());

    // merge message by id
    for (MessageImpl& item2 : o.messages()) {
        auto it = std::find_if(self.messages().begin(), self.messages().end(), [&item2](const MessageImpl& item1) {
            return item1.Id() == item2.Id();
        });
        if (it != self.messages().end()) {
            // merge it
            it->Merge(std::move(item2));
        } else {
            // insert new
            self.messages().push_back(std::move(item2));
        }
    }

    unique_merge_by_name(self.environmentVariables(), o.environmentVariables());
    auto merge_defs = [](auto& v1, auto& v2)
    {
        for (auto& item2 : v2)
        {
            auto it = std::find_if(v1.begin(), v1.end(), [&item2](const auto& item1)
            {
                return item1->Name() == item2->Name();
            });
            if (it != v1.end())
            {
                *it = std::move(item2);
            }
            else
            {
                v1.push_back(std::move(item2));
            }
        }
    };
    merge_defs(self.attributeDefinitions(), o.attributeDefinitions());
    unique_merge_by_name(self.attributeDefaults(), o.attributeDefaults());
    unique_merge_by_name(self.attributeValues(), o.attributeValues());

    self.setNetworkPointers();
    other.reset(nullptr);
}
bool NetworkImpl::operator==(const INetwork& rhs) const
{
    bool equal = true;
    equal &= _version == rhs.Version();
    for (const auto& new_symbol : _new_symbols)
    {
        equal &= std::find(_new_symbols.begin(), _new_symbols.end(), new_symbol) != _new_symbols.end();
    }
    equal &= _bit_timing == rhs.BitTiming();
    for (const auto& node : rhs.Nodes())
    {
        equal &= std::find(_nodes.begin(), _nodes.end(), node) != _nodes.end();
    }
    for (const auto& value_table : rhs.ValueTables())
    {
        equal &= std::find(_value_tables.begin(), _value_tables.end(), value_table) != _value_tables.end();
    }
    for (const auto& message : rhs.Messages())
    {
        equal &= std::find(_messages.begin(), _messages.end(), message) != _messages.end();
    }
    for (const auto& env_var : rhs.EnvironmentVariables())
    {
        equal &= std::find(_environment_variables.begin(), _environment_variables.end(), env_var) != _environment_variables.end();
    }
    for (const auto& attr_def : rhs.AttributeDefinitions())
    {
        auto it = std::find_if(_attribute_definitions.begin(), _attribute_definitions.end(),
            [&](const auto& self_def) { return *self_def == attr_def; });
        equal &= it != _attribute_definitions.end();
    }
    for (const auto& attr : rhs.AttributeDefaults())
    {
        equal &= std::find(_attribute_defaults.begin(), _attribute_defaults.end(), attr) != _attribute_defaults.end();
    }
    for (const auto& attr : rhs.AttributeValues())
    {
        equal &= std::find(_attribute_values.begin(), _attribute_values.end(), attr) != _attribute_values.end();
    }
    equal &= _comment == rhs.Comment();
    return equal;
}
bool NetworkImpl::operator!=(const INetwork& rhs) const
{
    return !(*this == rhs);
}

NetworkImpl& NetworkImpl::operator=(const NetworkImpl& other)
{
    if (this != &other)
    {
        _version = other._version;
        _new_symbols = other._new_symbols;
        _bit_timing = other._bit_timing;
        _nodes = other._nodes;
        _value_tables = other._value_tables;
        _messages = other._messages;
        _environment_variables = other._environment_variables;
        _attribute_definitions = other._attribute_definitions;
        _attribute_defaults = other._attribute_defaults;
        _attribute_values = other._attribute_values;
        _comment = other._comment;
        setNetworkPointers();
    }
    return *this;
}

NetworkImpl& NetworkImpl::operator=(NetworkImpl&& other) noexcept
{
    if (this != &other)
    {
        _version = std::move(other._version);
        _new_symbols = std::move(other._new_symbols);
        _bit_timing = std::move(other._bit_timing);
        _nodes = std::move(other._nodes);
        _value_tables = std::move(other._value_tables);
        _messages = std::move(other._messages);
        _environment_variables = std::move(other._environment_variables);
        _attribute_definitions = std::move(other._attribute_definitions);
        _attribute_defaults = std::move(other._attribute_defaults);
        _attribute_values = std::move(other._attribute_values);
        _comment = std::move(other._comment);
        setNetworkPointers();
    }
    return *this;
}

std::optional<std::reference_wrapper<const IAttribute>> NetworkImpl::findAttributeDefault(
    IAttributeDefinition::EObjectType object_type,
    const std::string& name) const
{
    auto it = std::find_if(_attribute_defaults.begin(), _attribute_defaults.end(),
        [&](const AttributeImpl& attr)
        {
            return attr.ObjectType() == object_type && attr.Name() == name;
        });
    if (it != _attribute_defaults.end())
    {
        return std::cref(static_cast<const IAttribute&>(*it));
    }
    return std::nullopt;
}

void NetworkImpl::setNetworkPointers()
{
    for (auto& node : _nodes)
    {
        node.setNetwork(this);
    }
    for (auto& msg : _messages)
    {
        msg.setNetwork(this);
    }
    for (auto& env_var : _environment_variables)
    {
        env_var.setNetwork(this);
    }
}

std::unique_ptr<INetwork> INetwork::LoadNetworkFromFile(const std::filesystem::path& filename)
{
    auto is = std::ifstream(filename);
    if (!is.is_open())
    {
        std::cout << "Error: Could not open file " << filename << "\n";
    }
    else if (filename.extension() == ".dbc")
    {
        return LoadDBCFromIs(is);
    }
    return nullptr;
}

std::unique_ptr<INetwork> INetwork::LoadNetworkFromFile(const std::filesystem::path& filename, std::string& error_message)
{
    auto is = std::ifstream(filename);
    if (!is.is_open())
    {
        error_message = "Error: Could not open file " + filename.string() + "\n";
    }
    else if (filename.extension() == ".dbc")
    {
        return LoadDBCFromIs(is, error_message);
    }
    return nullptr;
}
