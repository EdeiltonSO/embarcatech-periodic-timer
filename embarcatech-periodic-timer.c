#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

#define RED_GPIO_PIN   13
#define BLUE_GPIO_PIN  12
#define GREEN_GPIO_PIN 11

#define RED_STATUS    0
#define YELLOW_STATUS 1
#define GREEN_STATUS  2

#define STATUS_TIME_MS 3000

bool repeating_timer_callback(struct repeating_timer *t) {
    // inicia na segunda cor, pois a primeira já foi acesa no começo
    static uint8_t light_status = YELLOW_STATUS;

    switch (light_status)
    {
        case RED_STATUS:
            // acende vermelho e prepara o amarelo pra próxima chamada
            gpio_put(RED_GPIO_PIN, true);
            gpio_put(BLUE_GPIO_PIN, false);
            gpio_put(GREEN_GPIO_PIN, false);
            light_status = YELLOW_STATUS;
            break;
        case YELLOW_STATUS:
            // acende amarelo e prepara o verde pra próxima chamada
            gpio_put(RED_GPIO_PIN, true);
            gpio_put(BLUE_GPIO_PIN, false);
            gpio_put(GREEN_GPIO_PIN, true);
            light_status = GREEN_STATUS;
            break;
        case GREEN_STATUS:
            // acende verde e prepara o vermelho pra próxima chamada
            gpio_put(RED_GPIO_PIN, false);
            gpio_put(BLUE_GPIO_PIN, false);
            gpio_put(GREEN_GPIO_PIN, true);
            light_status = RED_STATUS;
            break;
        default:
            // se sair da condição, desliga o semáforo até o sistema ser reiniciado
            gpio_put(RED_GPIO_PIN, false);
            gpio_put(BLUE_GPIO_PIN, false);
            gpio_put(GREEN_GPIO_PIN, false);
            light_status = -1;
            break;
    }
    return true;
}

int main() {
    stdio_init_all();
    
    // inicializa pinos
    gpio_init(RED_GPIO_PIN);
    gpio_init(BLUE_GPIO_PIN);
    gpio_init(GREEN_GPIO_PIN);
    gpio_set_dir(RED_GPIO_PIN, GPIO_OUT);
    gpio_set_dir(BLUE_GPIO_PIN, GPIO_OUT);
    gpio_set_dir(GREEN_GPIO_PIN, GPIO_OUT);

    // na primeira vez, acende o vermelho
    gpio_put(RED_GPIO_PIN, true);
    gpio_put(BLUE_GPIO_PIN, false);
    gpio_put(GREEN_GPIO_PIN, false);

    struct repeating_timer timer;

    // depois de STATUS_TIME_MS milissgundos, chama a próxima cor
    add_repeating_timer_ms(STATUS_TIME_MS, repeating_timer_callback, NULL, &timer);

    while (true) {
    }

    return 0;
}