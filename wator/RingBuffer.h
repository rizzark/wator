#pragma once

template<class _T>
class RingBuffer
{
private:
    const size_t _size;
    _T* _buffer;
    size_t _start;
    size_t _length;


public:
    RingBuffer(const size_t size) : _size(size)
    {
        _buffer = new _T[size];
        _start = _length = 0;
    } // end - RingBuffer

    virtual ~RingBuffer()
    {
        delete[] _buffer;
        _buffer = NULL;
    } // end - ~RingBuffer


    virtual inline const size_t length() const { return _length; }

    virtual void push(const _T& value)
    {
        size_t pos = (_start + _length) % _size;

        _buffer[pos] = value;
        if (_length < _size)
            _length++;
        else
            _start = (_start + 1) % _size;
    } // end - push

    virtual inline _T& operator[](const unsigned pos) const
    {
        if (pos >= _length)
            throw std::out_of_range("pos out of range!");

        size_t realPos = (_start + pos) % _size;
        return _buffer[realPos];
    }
};


