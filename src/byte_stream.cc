#include "byte_stream.hh"

#include <iostream>

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ) {}

void ByteStream::debug() const {
  std::cout << "ByteStream Debug Information:\n";
  std::cout << "  Capacity: " << capacity_ << "\n";
  std::cout << "  Closed: " << closed_ << "\n";
  std::cout << "  Error: " << error_ << "\n";
  std::cout << "  Pushed: " << bytes_pushed_ << "\n";
  std::cout << "  Popped: " << bytes_popped_ << "\n";
  std::cout << "  Buffer (first 16 bytes): ";
  for (size_t i = 0; i < std::min((size_t)16, buffer_.size()); ++i) {
      std::cout << std::hex << static_cast<int>(buffer_[i]) << " ";
  }
  std::cout << "\n" << endl;
}

bool Writer::is_closed() const
{
  return closed_;
}

void Writer::push( string data )
{
  // cout << "push data:\n" << data << endl;

  if (error_ || closed_ || data.empty()) return;
  uint64_t spaceAvailable = available_capacity();
  if (spaceAvailable == 0) return; // Buffer is full, do not write

  uint64_t bytesToWrite = std::min(spaceAvailable, data.length());
  buffer_.append(data.substr(0, bytesToWrite));
  bytes_pushed_ += bytesToWrite;

  // debug();
}

void Writer::close()
{
  closed_ = true;
}

uint64_t Writer::available_capacity() const
{
  return capacity_ - buffer_.size();
}

uint64_t Writer::bytes_pushed() const
{
  return bytes_pushed_;
}

bool Reader::is_finished() const
{
  return closed_ && (buffer_.size() == 0);
}

uint64_t Reader::bytes_popped() const
{
  return bytes_popped_;
}

string_view Reader::peek() const
{
  return buffer_;
}

void Reader::pop( uint64_t len )
{
  // cout << "pop len:\n" << len << endl;

  if (error_ || buffer_.size() == 0 || len == 0) return;
  uint64_t pop_len = std::min(len, buffer_.size());
  buffer_.erase(buffer_.begin(), buffer_.begin() + pop_len);
  bytes_popped_ += pop_len;

  // debug();
}

uint64_t Reader::bytes_buffered() const
{
  return buffer_.size();
}
