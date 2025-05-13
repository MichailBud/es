#include <libopencm3/stm32/rcc.h> // reset and clock control
#include <libopencm3/stm32/gpio.h>  // general purpose input output (общего назначения)
#include  <libopencm3/stm32/timer.h>

#include  <libopencm3/cm3/nvic.h>


void timer_enable(rcc_periph_clken rcc_tim, uint32_t tim, uint32_t presc, uint32_t period){
    rcc_periph_clock_enable(rcc_tim); // Активируем тактирование таймера?
    timer_set_prescaler(tim, presc-1); // Установка предделителя
    timer_set_period(tim, period-1); // Установка предела счёта
    timer_enable_counter(tim); // Активация таймера
}

int main() {

    rcc_periph_clock_enable(RCC_GPIOA); // Включаем группу портов ввода вывода D
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO0); // Активируем 0 вывод в режиме альтернативной функции
    gpio_set_af(GPIOA, GPIO_AF2, GPIO0); // Включаем альтернативную функцию порта (таймер)



    while (true){
    }
}

