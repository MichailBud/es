#include <libopencm3/stm32/rcc.h> // reset and clock control
#include <libopencm3/stm32/gpio.h>  // general purpose input output (общего назначения)
#include  <libopencm3/stm32/usart.h>

#include  <libopencm3/cm3/nvic.h>



int main() {

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

    usart_enable(USART2);



    while (true){

        usart_send_blocking(USART2, 'A');
        usart_send_blocking(USART2, 10);
        for (volatile uint32_t i = 0; i < 50000; ++i);
    }
}

