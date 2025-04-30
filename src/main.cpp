#include <libopencm3/stm32/rcc.h> // reset and clock control
#include <libopencm3/stm32/gpio.h>  // general purpose input output (общего назначения)
#include  <libopencm3/stm32/timer.h>

#include  <libopencm3/cm3/nvic.h>


// class Rcc{
// public:
//     Rcc(){
//         rcc_clock_setup_pll(&rcc_hse_16mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
//     }у
// };

// Rcc clock_system;

constexpr uint16_t step_interval{2000}; // Шаг по времени с которым происходит событие update
constexpr uint16_t timer5_clk_freq_hz{1000}; // Частота тактирования таймера (Гц)

uint32_t duty{};
uint32_t period{};


void timer_enable(rcc_periph_clken rcc_tim, uint32_t tim, uint32_t presc, uint32_t period){
    rcc_periph_clock_enable(rcc_tim); // Активируем тактирование таймера?
    timer_set_prescaler(tim, presc-1); // Установка предделителя
    timer_set_period(tim, period-1); // Установка предела счёта
    timer_enable_counter(tim); // Активация таймера
}

int main() {

    timer_enable(RCC_TIM5, TIM5, rcc_get_timer_clk_freq(TIM5)/timer5_clk_freq_hz - 1, step_interval);   // Активация и настройка 4 таймера

    rcc_periph_clock_enable(RCC_GPIOA); // Включаем группу портов ввода вывода D
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO0); // Активируем 0 вывод в режиме альтернативной функции
    gpio_set_af(GPIOA, GPIO_AF2, GPIO0); // Включаем альтернативную функцию порта (таймер)

    timer_ic_set_input(TIM5, TIM_IC1, TIM_IC_IN_TI1); // Функция устанавливает с какого входа таймера будет считываться сигнал для захвата счётчика.
    // timer_ic_set_filter();
    timer_ic_set_polarity(TIM5, TIM_IC1, TIM_IC_RISING);  // Настраиваем захват по фронту сигнала
 

    timer_ic_set_input(TIM5, TIM_IC2, TIM_IC_IN_TI1); // Функция устанавливает с какого входа таймера будет считываться сигнал для захвата счётчика.
    // timer_ic_set_filter();
    timer_ic_set_polarity(TIM5, TIM_IC2, TIM_IC_FALLING);  // Настраиваем захват по фронту сигнала
    

    timer_slave_set_trigger(TIM5, TIM_SMCR_TS_TI1FP1); // режим раба
    timer_slave_set_mode(TIM5, TIM_SMCR_SMS_RM); // reset mode 



    timer_enable_irq(TIM5, TIM_DIER_CC1IE); // Разрешает прерывание по обновлению
    nvic_enable_irq(NVIC_TIM5_IRQ); // Разрешаем прерывание TIM4 в NVIC

    timer_ic_enable(TIM5, TIM_IC1); // Активация каналов таймера
    timer_ic_enable(TIM5, TIM_IC2);

    while (true){
        volatile uint32_t d = duty;
        volatile uint32_t p = period;
    }
}

void tim5_isr(void){
    timer_clear_flag(TIM5, TIM_SR_CC1IF);
    duty = TIM_CCR2(TIM5);
    period = TIM_CCR1(TIM5);
}