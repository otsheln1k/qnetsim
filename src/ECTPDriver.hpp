#ifndef _NS_ECTP_DRIVER_HPP_
#define _NS_ECTP_DRIVER_HPP_

#include <QObject>

#include "MACAddr.hpp"
#include "EthernetFrame.hpp"

class ECTPDriver : public QObject {
    Q_OBJECT;

    static bool ourFrame(const EthernetFrame *frame);

public:
    // Notes:
    // - Use back inserter iterators if you don’t want to precompute the
    //   sequence size. However, there’s a helper for size calculations:
    //   messageSequenceSize. For simple loopbacks, n_forwards=1.
    // - Append payload after the reply message.
    // - The simplest usecase is a loopback message sequence. There’s a special
    //   helper function for these: makeLoopback. It’ll even std::copy the
    //   payload for you.

    template<typename OutIter>
    static OutIter beginMessage(OutIter iter, uint16_t skipOffset = 0)
    {
        *iter++ = skipOffset & 0xFF;
        *iter++ = (skipOffset >> 8) & 0xFF;
        return iter;
    }

    template<typename OutIter>
    static OutIter writeForwardMessage(MACAddr dest, OutIter iter)
    {
        *iter++ = 0x02;
        *iter++ = 0x00;

        uint8_t buf[6];
        dest.write(buf);
        return std::copy(buf, &buf[6], iter);
    }

    template<typename OutIter>
    static OutIter writeReplyMessage(uint16_t seq, OutIter iter)
    {
        *iter++ = 0x01;
        *iter++ = 0x00;
        *iter++ = seq & 0xFF;
        *iter++ = (seq >> 8) & 0xFF;
        return iter;
    }

    static size_t messageSequenceSize(size_t n_forwards, size_t payload_size)
    {
        return 2 + n_forwards * 8 + 4 + payload_size;
    }

    template<typename InIter,
             typename OutIter>
    static OutIter makeLoopback(MACAddr through,
                                uint16_t seq,
                                InIter bpayload,
                                InIter epayload,
                                OutIter iter)
    {
        iter = beginMessage(iter);
        iter = writeForwardMessage(through, iter);
        iter = writeReplyMessage(seq, iter);
        iter = std::copy(bpayload, epayload, iter);
        return iter;
    }

public slots:
    void handleFrame(const EthernetFrame *frame);
    void handleData(const uint8_t *data, size_t size);

signals:
    void reply(uint16_t seq, const uint8_t *payload, size_t payloadSize);
    void forward(MACAddr dest, const uint8_t *data, size_t size);
};

#endif
