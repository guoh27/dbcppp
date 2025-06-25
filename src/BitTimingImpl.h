#pragma once

#include "dbcppp/BitTiming.h"

namespace dbcppp
{
    class BitTimingImpl final
        : public IBitTiming
    {
    public:
        BitTimingImpl();
        BitTimingImpl(uint64_t baudrate, uint64_t BTR1, uint64_t BTR2);
        
        std::unique_ptr<IBitTiming> Clone() const override;

        uint64_t Baudrate() const override;
        uint64_t BTR1() const override;
        uint64_t BTR2() const override;
        
        bool operator==(const IBitTiming& rhs) const override;
        bool operator!=(const IBitTiming& rhs) const override;

    private:
        uint64_t _baudrate;
        uint64_t BTR1_;
        uint64_t BTR2_;
    };
}  // namespace dbcppp