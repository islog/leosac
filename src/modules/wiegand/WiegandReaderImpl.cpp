#include "WiegandReaderImpl.hpp"
#include "tools/log.hpp"

WiegandReaderImpl::WiegandReaderImpl(zmqpp::context &ctx,
        const std::string &name,
        const std::string &data_high_pin,
        const std::string &data_low_pin) :
bus_sub_(ctx, zmqpp::socket_type::sub),
bus_push_(ctx, zmqpp::socket_type::push),
counter_(0),
name_(name)
{
    bus_sub_.connect("inproc://zmq-bus-pub");
    bus_push_.connect("inproc://zmq-bus-pull");

    topic_high_ = "S_INT:" + data_high_pin;
    topic_low_ = "S_INT:" + data_low_pin;

    bus_sub_.subscribe(topic_high_);
    bus_sub_.subscribe(topic_low_);
}


WiegandReaderImpl::WiegandReaderImpl(WiegandReaderImpl &&o) :
bus_sub_(std::move(o.bus_sub_)),
bus_push_(std::move(o.bus_push_)),
name_(std::move(o.name_))
{
    topic_high_ = o.topic_high_;
    topic_low_ = o.topic_low_;

    buffer_ = o.buffer_;
    counter_ = o.counter_;
}


void WiegandReaderImpl::handle_bus_msg()
{
    std::string msg;
    bus_sub_.receive(msg);

    if (msg == topic_high_)
    {
        buffer_[counter_ / 8] |= (1 << (7 - counter_ % 8));
    }
    else if (msg == topic_low_)
    {
        // well, nothing to do here.
    }
    counter_++;
}

void WiegandReaderImpl::timeout()
{
    if (!counter_)
        return;

    LOG() << "timeout, buffer size = " << counter_;
    std::size_t                 size = ((counter_ - 1) / 8) + 1;

    std::stringstream card_hex;
    // we need to reverse the order
    for (std::size_t i = 0; i < size; ++i)
        {
            card_hex << std::hex << static_cast<int>(buffer_[size - i - 1]);
            if (i + 1 < size)
                card_hex << ":";
        }

    zmqpp::message msg;
    msg << ("S_" + name_) << card_hex.str();
    bus_push_.send(msg);

    std::fill(buffer_.begin(), buffer_.end(), 0);
    counter_ = 0;
}