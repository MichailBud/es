#include <libopencm3/stm32/rcc.h> // reset and clock control
#include <libopencm3/stm32/gpio.h>  // general purpose input output (общего назначения)
#include  <libopencm3/stm32/timer.h>

#include  <libopencm3/cm3/nvic.h>


// class Rcc{
// public:
//     Rcc(){
//         rcc_clock_setup_pll(&rcc_hse_16mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
//     }
// };

// Rcc clock_system;

int compare = 0;

void timer_enable(rcc_periph_clken rcc_tim, unsigned int tim, int presc, int period){
    rcc_periph_clock_enable(rcc_tim); // Активируем тактирование таймера?
    timer_set_prescaler(tim, presc-1); // Установка предделителя
    timer_set_period(tim, period-1); // Установка предела счёта
    timer_enable_counter(tim); // Активация таймера
}



int main() {

    timer_enable(RCC_TIM4, TIM4, 80, 1000);   // Активация и настройка 4 таймера
    timer_enable(RCC_TIM6, TIM6, 16000, 1000);   // Активация и настройка 6 таймера

    rcc_periph_clock_enable(RCC_GPIOD); // Включаем групппу портов ввода вывода D
    gpio_mode_setup(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO15); // Активируем 15 вывод
    gpio_set_af(GPIOD, GPIO_AF2, GPIO15);



    timer_enable_irq(TIM6, TIM_DIER_UIE); // Разрешает внутри МК отправлять сигнал
    nvic_enable_irq(NVIC_TIM6_DAC_IRQ);

    timer_set_oc_mode(TIM4, TIM_OC4, TIM_OCM_PWM1); // Режим ШИМ
    timer_set_oc_value(TIM4, TIM_OC4, 100); // Значение таймера при котором сработает флаг, но счетчик не обнулится и будет заполняться дальше
    timer_enable_oc_output(TIM4, TIM_OC4);

    // timer_enable_counter(TIM4);
    // timer_enable_counter(TIM6);

    while (1);
}

void tim6_dac_isr (void){
    timer_clear_flag(TIM6, TIM_SR_UIF);
    if (compare >= 1000) {
        compare = 0;
    }
    else {
        compare += 200;
    }
    timer_set_oc_value(TIM4, TIM_OC4, compare);
}