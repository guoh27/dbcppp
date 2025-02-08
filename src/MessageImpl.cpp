#include <algorithm>
#include <unordered_set>
#include "MessageImpl.h"
#include "Helper.h"

using namespace dbcppp;


std::unique_ptr<IMessage> IMessage::Create(
      uint64_t id
    , std::string&& name
    , uint64_t message_size
    , std::string&& transmitter
    , std::vector<std::string>&& message_transmitters
    , std::vector<std::unique_ptr<ISignal>>&& signals_
    , std::vector<std::unique_ptr<IAttribute>>&& attribute_values
    , std::string&& comment
    , std::vector<std::unique_ptr<ISignalGroup>>&& signal_groups)
{
    std::vector<SignalImpl> ss;
    std::vector<AttributeImpl> avs;
    std::vector<SignalGroupImpl> sgs;
    for (auto& s : signals_)
    {
        ss.push_back(std::move(static_cast<SignalImpl&>(*s)));
        s.reset(nullptr);
    }
    for (auto& av : attribute_values)
    {
        if (av->ObjectType() != IAttributeDefinition::EObjectType::Message)
        {
            throw std::runtime_error("Create IMessage with non message AttributeDefination: " + av->Name());
        }
        avs.push_back(std::move(static_cast<AttributeImpl&>(*av)));
        av.reset(nullptr);
    }
    for (auto& sg : signal_groups)
    {
        if (sg->MessageId() != id)
        {
            throw std::runtime_error("Create IMessage with non message SignalGroup: " + sg->Name());
        }
        sgs.push_back(std::move(static_cast<SignalGroupImpl&>(*sg)));
        sg.reset(nullptr);
    }
    return std::make_unique<MessageImpl>(
          id
        , std::move(name)
        , message_size
        , std::move(transmitter)
        , std::move(message_transmitters)
        , std::move(ss)
        , std::move(avs)
        , std::move(comment)
        , std::move(sgs));
}
MessageImpl::MessageImpl(
      uint64_t id
    , std::string&& name
    , uint64_t message_size
    , std::string&& transmitter
    , std::vector<std::string>&& message_transmitters
    , std::vector<SignalImpl>&& signals_
    , std::vector<AttributeImpl>&& attribute_values
    , std::string&& comment
    , std::vector<SignalGroupImpl>&& signal_groups)
    
    : _id(std::move(id))
    , _name(std::move(name))
    , _message_size(std::move(message_size))
    , _transmitter(std::move(transmitter))
    , _message_transmitters(std::move(message_transmitters))
    , _signals(std::move(signals_))
    , _attribute_values(std::move(attribute_values))
    , _comment(std::move(comment))
    , _signal_groups(std::move(signal_groups))
    , _mux_signal(nullptr)
    , _error(EErrorCode::NoError)
{
    bool have_mux_value = false;
    for (const auto& sig : _signals)
    {
        switch (sig.MultiplexerIndicator())
        {
        case ISignal::EMultiplexer::MuxValue:
            have_mux_value = true;
            break;
        case ISignal::EMultiplexer::MuxSwitch:
            _mux_signal = &sig;
            break;
        }
    }
    if (have_mux_value && _mux_signal == nullptr)
    {
        SetError(EErrorCode::MuxValeWithoutMuxSignal);
    }

    auto hasDuplicateNames = [](const std::vector<SignalImpl>& signals) {
        std::unordered_set<std::string> names;
        for (const auto& signal : signals) {
            const std::string& name = signal.Name();
            if (names.find(name) != names.end()) {
                return true;
            }
            names.insert(name);
        }
        return false; 
    };

    if (hasDuplicateNames(_signals)) {
        SetError(EErrorCode::SignalNameDuplicated);
    }

}
MessageImpl::MessageImpl(const MessageImpl& other)
{
    _id = other._id;
    _name = other._name;
    _message_size = other._message_size;
    _transmitter = other._transmitter;
    _message_transmitters = other._message_transmitters;
    _signals = other._signals;
    _attribute_values = other._attribute_values;
    _comment = other._comment;
    _mux_signal = nullptr;
    for (const auto& sig : _signals)
    {
        switch (sig.MultiplexerIndicator())
        {
        case ISignal::EMultiplexer::MuxSwitch:
            _mux_signal = &sig;
            break;
        }
    }
    _error = other._error;
}
MessageImpl& MessageImpl::operator=(const MessageImpl& other)
{
    _id = other._id;
    _name = other._name;
    _message_size = other._message_size;
    _transmitter = other._transmitter;
    _message_transmitters = other._message_transmitters;
    _signals = other._signals;
    _attribute_values = other._attribute_values;
    _comment = other._comment;
    _mux_signal = nullptr;
    for (const auto& sig : _signals)
    {
        switch (sig.MultiplexerIndicator())
        {
        case ISignal::EMultiplexer::MuxSwitch:
            _mux_signal = &sig;
            break;
        }
    }
    _error = other._error;
    return *this;
}
std::unique_ptr<IMessage> MessageImpl::Clone() const
{
    return std::make_unique<MessageImpl>(*this);
}
uint64_t MessageImpl::Id() const
{
    return _id;
}
const std::string& MessageImpl::Name() const
{
    return _name;
}
uint64_t MessageImpl::MessageSize() const
{
    return _message_size;
}
const std::string& MessageImpl::Transmitter() const
{
    return _transmitter;
}
const std::string& MessageImpl::MessageTransmitters_Get(std::size_t i) const
{
    return _message_transmitters[i];
}
uint64_t MessageImpl::MessageTransmitters_Size() const
{
    return _message_transmitters.size();
}
const ISignal& MessageImpl::Signals_Get(std::size_t i) const
{
    return _signals[i];
}
uint64_t MessageImpl::Signals_Size() const
{
    return _signals.size();
}
const IAttribute& MessageImpl::AttributeValues_Get(std::size_t i) const
{
    return _attribute_values[i];
}
uint64_t MessageImpl::AttributeValues_Size() const
{
    return _attribute_values.size();
}
const std::string& MessageImpl::Comment() const
{
    return _comment;
}
const ISignalGroup& MessageImpl::SignalGroups_Get(std::size_t i) const
{
    return _signal_groups[i];
}
uint64_t MessageImpl::SignalGroups_Size() const
{
    return _signal_groups.size();
}
const ISignal* MessageImpl::MuxSignal() const 
{
    return _mux_signal;
}
bool MessageImpl::Error(EErrorCode code) const
{
    return code == _error || (uint64_t(_error) & uint64_t(code));
}
void MessageImpl::SetError(EErrorCode code)
{
    _error = EErrorCode(uint64_t(_error) | uint64_t(code));
}

const std::vector<SignalImpl>& MessageImpl::signals() const
{
    return _signals;
}
bool MessageImpl::operator==(const IMessage& rhs) const
{
    bool equal = true;
    equal &= _id == rhs.Id();
    equal &= _name == rhs.Name();
    equal &= _transmitter == rhs.Transmitter();
    for (const auto& msg_trans : rhs.MessageTransmitters())
    {
        auto beg = _message_transmitters.begin();
        auto end = _message_transmitters.end();
        equal &= std::find(beg, end, msg_trans) != _message_transmitters.end();
    }
    for (const auto& sig : rhs.Signals())
    {
        auto beg = _signals.begin();
        auto end = _signals.end();
        equal &= std::find(beg, end, sig) != _signals.end();
    }
    for (const auto& attr : rhs.AttributeValues())
    {
        auto beg = _attribute_values.begin();
        auto end = _attribute_values.end();
        equal &= std::find(beg, end, attr) != _attribute_values.end();
    }
    equal &= _comment == rhs.Comment();
    for (const auto& sg : rhs.SignalGroups())
    {
        auto beg = _signal_groups.begin();
        auto end = _signal_groups.end();
        equal &= std::find(beg, end, sg) != end;
    }
    return equal;
}
bool MessageImpl::operator!=(const IMessage& rhs) const
{
    return !(*this == rhs);
}

void MessageImpl::Merge(MessageImpl &&o) {
    // refuse to merge if id not same
    if (_id != o._id) {
        return;
    }
    compare_set(_name, o._name);
    compare_set(_message_size, o._message_size);
    compare_set(_transmitter, o._transmitter);
    
    unique_merge(_message_transmitters, o._message_transmitters);
    // merge signal by name
    for (SignalImpl& item2 : o._signals) {
        auto it = std::find_if(_signals.begin(), _signals.end(), [&item2](const SignalImpl& item1) {
            return item1.Name() == item2.Name();
        });
        if (it != _signals.end()) {
            // merge it
            it->Merge(std::move(item2));
        } else {
            // insert new
            _signals.push_back(std::move(item2));
        }
    }
    // same type attr name is unique in one message
    unique_merge_by_name(_attribute_values, o._attribute_values);
    // name is unique in one message(id same)
    unique_merge_by_name(_signal_groups, o._signal_groups);
    compare_set(_comment, o._comment);

    // need to update the _mux_signal and error flag
    _mux_signal = nullptr;
    bool have_mux_value = false;
    for (const auto& sig : _signals)
    {
        switch (sig.MultiplexerIndicator())
        {
        case ISignal::EMultiplexer::MuxValue:
            have_mux_value = true;
            break;
        case ISignal::EMultiplexer::MuxSwitch:
            _mux_signal = &sig;
            break;
        }
    }
    if (have_mux_value && _mux_signal == nullptr)
    {
        SetError(EErrorCode::MuxValeWithoutMuxSignal);
    }
}

void IMessage::Merge(std::unique_ptr<IMessage>&& other) {
    auto& self = static_cast<MessageImpl&>(*this);
    auto& o = static_cast<MessageImpl&>(*other);
    self.Merge(std::move(o));
}
