#pragma once

#include "dbcppp/Message.h"
#include "SignalImpl.h"
#include "NodeImpl.h"
#include "AttributeImpl.h"
#include "SignalGroupImpl.h"

namespace dbcppp
{
    class INetwork;

    class MessageImpl final
        : public IMessage
    {
    public:
        MessageImpl(
              uint64_t id
            , std::string&& name
            , uint64_t message_size
            , std::string&& transmitter
            , std::vector<std::string>&& message_transmitters
            , std::vector<SignalImpl>&& signals_
            , std::vector<AttributeImpl>&& attribute_values
            , std::string&& comment
            , std::vector<SignalGroupImpl>&& signal_groups);
        MessageImpl(const MessageImpl& other);
        MessageImpl(MessageImpl&& other) = default;
        MessageImpl& operator=(const MessageImpl& other);
        MessageImpl& operator=(MessageImpl&&) = default;
            
        std::unique_ptr<IMessage> Clone() const override;
        
        uint64_t Id() const override;
        const std::string& Name() const override;
        uint64_t MessageSize() const override;
        const std::string& Transmitter() const override;
        const std::string& MessageTransmitters_Get(std::size_t i) const override;
        uint64_t MessageTransmitters_Size() const override;
        const ISignal& Signals_Get(std::size_t i) const override;
        uint64_t Signals_Size() const override;
        const IAttribute& AttributeValues_Get(std::size_t i) const override;
        uint64_t AttributeValues_Size() const override;
        std::optional<std::reference_wrapper<const IAttribute>> AttributeValue(const std::string& name) const override;
        const std::string& Comment() const override;
        const ISignalGroup& SignalGroups_Get(std::size_t i) const override;
        uint64_t SignalGroups_Size() const override;
        const ISignal* MuxSignal() const override;
        
        bool Error(EErrorCode code) const override;
        
        const std::vector<SignalImpl>& signals() const;
        std::vector<SignalImpl>& signals();
        std::vector<AttributeImpl>& attributeValues();
        
        bool operator==(const IMessage& rhs) const override;
        bool operator!=(const IMessage& rhs) const override;

        void Merge(MessageImpl &&other);
        void setNetwork(const INetwork* network);
        
    private:

        void SetError(EErrorCode code);

        uint64_t _id;
        std::string _name;
        uint64_t _message_size;
        std::string _transmitter;
        std::vector<std::string> _message_transmitters;
        std::vector<SignalImpl> _signals;
        std::vector<AttributeImpl> _attribute_values;
        std::string _comment;
        std::vector<SignalGroupImpl> _signal_groups;

        const ISignal* _mux_signal;
        const INetwork* _network = nullptr;

        EErrorCode _error;
    };
}  // namespace dbcppp
