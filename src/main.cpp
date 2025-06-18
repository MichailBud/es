#include "circular_buffer.hpp"

#include <libopencm3/stm32/rcc.h> // reset and clock control
#include <libopencm3/stm32/gpio.h>  // general purpose input output (общего назначения)
#include  <libopencm3/stm32/usart.h>

#include  <libopencm3/cm3/nvic.h>

Circular_buffer b;


int main() {
    

    rcc_periph_clock_enable(RCC_GPIOD); // Включаем 15 вывод (светодиод)
    gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO15); 


    rcc_periph_clock_enable(RCC_GPIOA); // Включаем группу портов ввода вывода A
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2 | GPIO3); // Активируем 2 и 3 вывод в режиме альтернативной функции
    gpio_set_af(GPIOA, GPIO_AF7, GPIO2 | GPIO3); // Включаем альтернативную функцию порта (UART) 2(TX), 3(RX)

    // Настройка UART
    rcc_periph_clock_enable(RCC_USART2);  // Подаём тактовый сигнал

    usart_set_baudrate(USART2, 115200); // Скорость передачи
    usart_set_databits(USART2, 8); // Размер посылки
    usart_set_parity(USART2, USART_PARITY_NONE);  // Проверка на чётность имеется или нет
    usart_set_stopbits(USART2, USART_STOPBITS_1); // Кол-во стопбитов

    usart_set_mode(USART2, USART_MODE_TX_RX); // Режим приёмопередатчика
    usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

    usart_enable_rx_interrupt(USART2); // Включаем прерывание по приёму
    nvic_enable_irq(NVIC_USART2_IRQ); // Вкл. прерывание

    usart_enable(USART2);

    while (true){
        // if (usart_get_flag(USART2, USART_SR_RXNE) and !b.full()){ // Условие Read data register not empty
           
        //     // send blocking --- если предыдущий байт ещё не передан - не отправлять
        //     // recv(receive) без blocking, потому что мы знаем заранее что данные пришли. blocking ждёт данные
        // }
        if (!b.empty()){
            usart_send_blocking(USART2, b.get());
        }
    }
}

void usart2_isr (void){
    gpio_toggle(GPIOD, GPIO15); // Переключение светодиода по прерыванию
     b.put(static_cast<uint8_t>(usart_recv(USART2)));
}