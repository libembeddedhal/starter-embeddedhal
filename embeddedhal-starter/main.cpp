#include <liblpc40xx/output_pin.hpp>
#include <libembeddedhal/context.hpp>
#include <cstdio>

void loop_for(int count)
{
  volatile int i = 0;
  while (i < count)
  {
    i = i + 1;
    continue;
  }
}

int main()
{
  auto &led = embed::lpc40xx::get_output_pin<1, 18>();

  [[maybe_unused]] bool success = led.initialize();

  while (true)
  {
    led.level(false);
    loop_for(100'000);
    led.level(true);
    loop_for(100'000);
  }

  // printf("Hello from Embedded Land!\n");
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
