#include <liblpc40xx/output_pin.hpp>

int main()
{
  auto &led = embed::lpc40xx::get_output_pin<0, 0>();

  [[maybe_unused]] bool success = led.initialize();
  led.level(true);
  return 0;
}

extern "C"
{
  // void *_impure_ptr = nullptr;
  inline void __cxa_pure_virtual() {}
  inline void __cxa_atexit() {}
}

namespace __cxxabiv1
{
  /* Override default terminate handler to just abort. */
  std::terminate_handler __terminate_handler = std::abort;
} /* namespace __cxxabiv1 */
