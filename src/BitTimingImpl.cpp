#include <ostream>
#include "BitTimingImpl.h"

using namespace dbcppp;

std::unique_ptr<IBitTiming> IBitTiming::Create(uint64_t baudrate, uint64_t BTR1, uint64_t BTR2)
{
    return std::make_unique<BitTimingImpl>(baudrate, BTR1, BTR2);
}
BitTimingImpl::BitTimingImpl()
    : _baudrate(0)
    , BTR1_(0)
    , BTR2_(0)
{}
BitTimingImpl::BitTimingImpl(uint64_t baudrate, uint64_t BTR1, uint64_t BTR2)
    : _baudrate(baudrate)
    , BTR1_(BTR1)
    , BTR2_(BTR2)
{}
std::unique_ptr<IBitTiming> BitTimingImpl::Clone() const
{
    return std::make_unique<BitTimingImpl>(*this);
}
uint64_t BitTimingImpl::Baudrate() const
{
    return _baudrate;
}
uint64_t BitTimingImpl::BTR1() const
{
    return BTR1_;
}
uint64_t BitTimingImpl::BTR2() const
{
    return BTR2_;
}
bool BitTimingImpl::operator==(const IBitTiming& rhs) const
{
    bool result = true;
    result &= _baudrate == rhs.Baudrate();
    result &= BTR1_ == rhs.BTR1();
    result &= BTR2_ == rhs.BTR2();
    return result;
}
bool BitTimingImpl::operator!=(const IBitTiming& rhs) const
{
    return !(*this == rhs);
}
