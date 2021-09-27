#include <liblpc40xx/output_pin.hpp>
#include <liblpc40xx/uart.hpp>
#include <liblpc40xx/startup.hpp>
#include <libembeddedhal/context.hpp>
#include <libarmcortex/dwt_counter.hpp>
#include <libembeddedhal/clock.hpp>

cortex_m::dwt_counter counter;

int main()
{
  embed::lpc40xx::initialize_platform();
  auto &led = embed::lpc40xx::get_output_pin<1, 18>();
  auto &terminal = embed::lpc40xx::get_uart<0>();

  [[maybe_unused]] bool success;
  success = led.initialize();
  success = terminal.initialize();
  counter.start();

  auto global_sleep = [](std::chrono::nanoseconds delay)
  {
    using namespace std::chrono_literals;

    auto timeout_time = counter.count64() + delay / 83ns;
    while (timeout_time > counter.count64())
    {
      continue;
    }
  };

  embed::this_thread::set_global_sleep(global_sleep);

  while (true)
  {
    using namespace std::chrono_literals;
    led.level(false);
    embed::this_thread::sleep_for(250ms);
    led.level(true);
    embed::this_thread::sleep_for(250ms);
    terminal.write(std::as_bytes(std::span<const char>("\nHello, World> ")));
    if (terminal.bytes_available() > 0)
    {
      std::array<std::byte, 128> buffer{};
      terminal.write(terminal.read(buffer));
    }
  }

  return 0;
}

extern "C"
{
  inline void __cxa_pure_virtual() {}
  inline void __cxa_atexit() {}
}

namespace __cxxabiv1
{
  /* Override default terminate handler to just abort. */
  std::terminate_handler __terminate_handler = std::abort;
} /* namespace __cxxabiv1 */
