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

int compare = 0;

void timer_enable(rcc_periph_clken rcc_tim, unsigned int tim, int presc, int period){
    rcc_periph_clock_enable(rcc_tim); // Активируем тактирование таймера?
    timer_set_prescaler(tim, presc-1); // Установка предделителя
    timer_set_period(tim, period-1); // Установка предела счёта
    timer_enable_counter(tim); // Активация таймера
}

bool flag=true;

int main() {

    timer_enable(RCC_TIM4, TIM4, 16000, 1000);   // Активация и настройка 4 таймера

    rcc_periph_clock_enable(RCC_GPIOD); // Включаем групппу портов ввода вывода D
    gpio_mode_setup(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO15); // Активируем 15 вывод в режиме альтернативной функции
    gpio_set_af(GPIOD, GPIO_AF2, GPIO15); // Канал 4 Таймера 4
    gpio_mode_setup(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO13); // Активируем 13 вывод в режиме альтернативной функции
    gpio_set_af(GPIOD, GPIO_AF2, GPIO13); // Канал 2 Таймера 4
    gpio_mode_setup(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO12); // Активируем 15 вывод в режиме альтернативной функции
    gpio_set_af(GPIOD, GPIO_AF2, GPIO12); // Канал 4 Таймера 4
    gpio_mode_setup(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO14); // Активируем 13 вывод в режиме альтернативной функции
    gpio_set_af(GPIOD, GPIO_AF2, GPIO14); // Канал 2 Таймера 4

    // timer_enable_irq(TIM6, TIM_DIER_UIE); // Разрешает внутри МК отправлять сигнал
    // nvic_enable_irq(NVIC_TIM6_DAC_IRQ);

    // timer_enable_irq(TIM4, TIM_DIER_CC2IE|TIM_DIER_CC3IE|TIM_DIER_CC4IE); // Включаем прерывание канала 2, 3, 4 таймера 4
    // timer_get_flag(TIM4, TIM_SR_CC2IF); // Получаем значение со второго канала. Истина, если флаг установлен (Прерывание сработало).

    timer_enable_irq(TIM4, TIM_DIER_UIE); // Разрешает прерывание по обновлению
    nvic_enable_irq(NVIC_TIM4_IRQ); // Разрешаем прерывание TIM4 в NVIC

    // timer_enable_counter(TIM4);
    // timer_enable_counter(TIM6);

    while (1);
}

void tim4_isr(){
    if (timer_get_flag(TIM4, TIM_SR_UIF)) {
        timer_clear_flag(TIM4, TIM_SR_UIF);
            if (flag){
                    timer_set_oc_mode(TIM4, TIM_OC1, TIM_OCM_PWM2); // Режим ШИМ
                    timer_set_oc_value(TIM4, TIM_OC1, 500); // Значение таймера при котором сработает флаг, но счетчик не обнулится и будет заполняться дальше
                    timer_enable_oc_output(TIM4, TIM_OC1);

                    timer_set_oc_mode(TIM4, TIM_OC2, TIM_OCM_PWM1); // Режим ШИМ
                    timer_set_oc_value(TIM4, TIM_OC2, 500); // Значение таймера при котором сработает флаг, но счетчик не обнулится и будет заполняться дальше
                    timer_enable_oc_output(TIM4, TIM_OC2);

                    timer_set_oc_mode(TIM4, TIM_OC3, TIM_OCM_INACTIVE); // Режим ШИМ
                    timer_set_oc_value(TIM4, TIM_OC3, 0); // Значение таймера при котором сработает флаг, но счетчик не обнулится и будет заполняться дальше
                    timer_enable_oc_output(TIM4, TIM_OC3);

                    timer_set_oc_mode(TIM4, TIM_OC4, TIM_OCM_INACTIVE); // Режим ШИМ
                    timer_set_oc_value(TIM4, TIM_OC4, 0); // Значение таймера при котором сработает флаг, но счетчик не обнулится и будет заполняться дальше
                    timer_enable_oc_output(TIM4, TIM_OC4);

                    flag = !flag;
            }
            else {
                    timer_set_oc_mode(TIM4, TIM_OC4, TIM_OCM_PWM1); // Режим ШИМ
                    timer_set_oc_value(TIM4, TIM_OC4, 500); // Значение таймера при котором сработает флаг, но счетчик не обнулится и будет заполняться дальше
                    timer_enable_oc_output(TIM4, TIM_OC4);

                    timer_set_oc_mode(TIM4, TIM_OC3, TIM_OCM_PWM2); // Режим ШИМ
                    timer_set_oc_value(TIM4, TIM_OC3, 500); // Значение таймера при котором сработает флаг, но счетчик не обнулится и будет заполняться дальше
                    timer_enable_oc_output(TIM4, TIM_OC3);

                    timer_set_oc_mode(TIM4, TIM_OC1, TIM_OCM_INACTIVE); // Режим ШИМ
                    timer_set_oc_value(TIM4, TIM_OC1, 0); // Значение таймера при котором сработает флаг, но счетчик не обнулится и будет заполняться дальше
                    timer_enable_oc_output(TIM4, TIM_OC1);

                    timer_set_oc_mode(TIM4, TIM_OC2, TIM_OCM_INACTIVE); // Режим ШИМ
                    timer_set_oc_value(TIM4, TIM_OC2, 0); // Значение таймера при котором сработает флаг, но счетчик не обнулится и будет заполняться дальше
                    timer_enable_oc_output(TIM4, TIM_OC2);

                    flag = !flag;
            }

    }
}