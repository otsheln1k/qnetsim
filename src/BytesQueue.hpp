#ifndef _NS_BYTES_QUEUE_HPP_
#define _NS_BYTES_QUEUE_HPP_

#include <deque>
#include <utility>

#include <QObject>

class BytesQueue : public QObject {
private:
    std::deque<unsigned char> _q;

public:
    BytesQueue() :QObject{}, _q{} {}
    explicit BytesQueue(const BytesQueue &q) :QObject{}, _q{q._q} {}
    explicit BytesQueue(BytesQueue &&q) :QObject{}, _q{std::move(q._q)} {}

    BytesQueue &operator=(const BytesQueue &q)
    {
        _q = q._q;
        return *this;
    }

    BytesQueue &operator=(BytesQueue &&q)
    {
        _q = std::move(q._q);
        return *this;
    }

    template<typename Iter>
    void pushIterRange(Iter b, Iter e)
    {
        for (Iter i = b; i != e; ++i) {
            _q.push_back(*i);
        }
    }

    void clear() { _q.clear(); }

    size_t bytesCount() const { return _q.size(); }

    void consume(size_t n)
    {
        _q.erase(_q.begin(), _q.begin() + n);
    }

    auto begin() const -> typeof(_q.begin())
    {
        return _q.begin();
    }

    auto end() const -> typeof(_q.end())
    {
        return _q.end();
    }

    void moveDataTo(BytesQueue &dest) {
        dest.pushIterRange(begin(), end());
        clear();
    }
};


#endif
