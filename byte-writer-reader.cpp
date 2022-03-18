#include <iostream>

template <typename T, size_t N>
constexpr size_t arraySize(T(&)[N]) noexcept { return N; }

template <typename Type, typename Buffer, typename Data>
auto write_to_heap_buffer(Buffer& buffer_, Data const& data_) {
  auto writer_ptr = buffer_;
  for (size_t i = 0; i < arraySize(data_); ++i) {
    if (static_cast<size_t>(buffer_ + arraySize(buffer_) - writer_ptr) >= sizeof(Type)) {
      new (writer_ptr) Type(data_[i]);
      writer_ptr += sizeof(Type);
    }
  }
}
template <typename Type, typename Buffer>
auto read_from_heap_buffer(Buffer& buffer_) {
  auto reader_ptr = buffer_;
  for (size_t i = 0; i < arraySize(buffer_); i += sizeof(Type)) {
    auto data = *reinterpret_cast<Type*>(reader_ptr);
    reader_ptr += sizeof(Type);
    if (data != decltype(data){})
      std::cout << data << std::endl;
  }
}

template <typename Buffer>
auto clear_heap_buffer(Buffer& buffer_) {
  delete[] buffer_;
}

template <typename Type, typename Buffer, typename Data>
auto write_to_stack_buffer(Buffer& buffer_, Data const& data_) {
  for (size_t i = 0; i < arraySize(data_); ++i)
    buffer_[i] = data_[i];
}

template <typename Type, typename Buffer>
auto read_from_stack_buffer(Buffer& buffer_) {
  for (auto const elem : buffer_) {
    if (elem != decltype(elem) {})
      std::cout << elem << std::endl;
  }
}

static char string_buffer[256]{};
static char int_buffer[256]{};
static float float_buffer[256]{};
int main() {
  std::string const string_data_to_write[4]{ "LAX", "SVO", "DME", "LED" };
  write_to_heap_buffer<std::string>(string_buffer, string_data_to_write);
  read_from_heap_buffer<std::string>(string_buffer);

  size_t const int_data_to_write[4]{ 1, 2, 15, 101 };
  write_to_heap_buffer<size_t>(int_buffer, int_data_to_write);
  read_from_heap_buffer<size_t>(int_buffer);

  float const float_data_to_write[4]{ 1.1f, 2.2f, 15.3f, 101.1f };
  write_to_stack_buffer<float>(float_buffer, float_data_to_write);
  read_from_stack_buffer<size_t>(float_buffer);

  size_t exit_code{};
  while (exit_code != 1)
    std::cin >> exit_code;
}