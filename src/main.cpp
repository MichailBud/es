#include <libopencm3/stm32/rcc.h> // reset and clock control
#include <libopencm3/stm32/gpio.h>  // general purpose input output (общего назначения)
#include  <libopencm3/stm32/timer.h>


// class Rcc{
// public:
//     Rcc(){
//         rcc_clock_setup_pll(&rcc_hse_16mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
//     }
// };

// Rcc clock_system;

int main() {
    rcc_periph_clock_enable(RCC_GPIOD); // Включаем групппу портов ввода вывода D
    gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,GPIO15); // Активируем 15 вывод

    rcc_periph_clock_enable(RCC_TIM6); // Активируем 6 таймер
    timer_set_prescaller(TIM6, 8000-1); // Установка предделителя
    timer_set_period(TIM6, 1000-1); // Установка предела счёта
    timer_enable_counter(TIM6); // Активацтя таймера
    
    while (true) {
        if (timer_get_counter(TIM6) < 500) {
            gpio_set(GPIOD, GPIO15);
        } else {
            gpio_clear(GPIOD, GPIO15);
        }
    }
}
