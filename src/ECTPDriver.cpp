#include "ECTPDriver.hpp"

bool ECTPDriver::ourFrame(const EthernetFrame *frame)
{
    return frame->hasEtherType()
        && frame->etherType() == ETHERTYPE_ECTP;
}

void ECTPDriver::handleFrame(const EthernetFrame *frame)
{
    if (!ourFrame(frame)) {
        return;
    }

    const auto &p = frame->payload();
    handleData(p.data(), p.size());
}

void ECTPDriver::handleData(const uint8_t *data, size_t size)
{
    if (size < 4) {
        return;                 // invalid payload
    }

    uint16_t skip = data[0] | (data[1] << 8);
    uint16_t off = skip + 4;

    if (size < off) {
        return;                 // skipCount too large
    }

    uint16_t func = data[off] | (data[off+1] << 8);

    switch (func) {
    case 1: {
        if (size < (size_t)(off+4)) {
            return;             // reply message won’t fit
        }

        uint16_t seq = data[off+2] | (data[off+3] << 8);
        const uint8_t *payload = &data[off+4];
        size_t payloadSize = size - (off+4);

        emit reply(seq, payload, payloadSize);

        break;
    }
    case 2: {
        // may forward known-invalid messages, e.g. if size < (off+8)
        if (size < (size_t)(off+6)) {
            return;             // dest MAC address won’t fit
        }

        MACAddr dest;
        dest.read(&data[off+2]);

        std::vector<uint8_t> ndata (size);
        uint16_t nskip = skip + 8;
        ndata[0] = nskip & 0xFF;
        ndata[1] = (nskip >> 8) & 0xFF;
        std::copy(data, data + size, ndata.begin()+2);

        emit forward(dest, ndata.data(), size);

        break;
    }

    default:
        break;                  // invalid function
    }
}
