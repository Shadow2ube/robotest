#include <array>

#ifndef BUFFER_H
#define BUFFER_H

template <typename TYPE, std::size_t SIZE> class Buffer {
  std::array<TYPE, SIZE> _data;

public:
  Buffer(TYPE fill) {
    for (auto &i : _data) {
      i = fill;
    }
  }

  std::array<TYPE, SIZE> data() { return _data; }

  std::size_t size() { return _data.size(); }

  TYPE operator[](int i) { return _data[i]; }

  Buffer operator+(TYPE in) {
    Buffer data = *this;

    for (int i = 1; i < data.size(); ++i) {
      data[i] = data[i - 1];
    }
    data[0] = in;

    return data;
  }

  Buffer &operator+=(TYPE in) {
    // this->_data = operator+(in).data();
    for (int i = 0; i < _data.size(); ++i) {
      _data[i] = _data[i + 1];
    }
    _data[SIZE-1] = in;
    return *this;
  }
};

#endif // BUFFER_H