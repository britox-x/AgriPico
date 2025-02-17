#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

// Configurações de Wi-Fi
#define SECRET_SSID "Matheus_2G"    
#define SECRET_PASS "q3biG267$"  
#define SECRET_CH_ID  "2842417" 
#define SECRET_WRITE_APIKEY "4MS7X6RI4WZS4U0P"   

// Configurações do I2C e LEDs
#define I2C_PORT i2c1
#define I2C_SDA 15
#define I2C_SCL 14
#define LED_B 12
#define LED_R 13
#define LED_G 11
#define SW 22  // Pino do Botão do Joystick
#define VRY 27 // Porta ADC de variação do Y do Joystick
#define VRX 26 // Porta ADC de variação do X do joystick
#define BUZZER_PIN 21 // Pino do buzzer
#define TEMP_SENSOR_PIN 0 // Pino ADC para sensor de temperatura
#define HUMIDITY_SENSOR_PIN 1 // Pino ADC para sensor de umidade

ssd1306_t disp; // Variável para o display OLED

void setup() {
    // Inicialização do OLED
    stdio_init_all();
    i2c_init(I2C_PORT, 400*1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    disp.external_vcc = false;
    ssd1306_init(&disp, 128, 64, 0x3C, I2C_PORT);
    ssd1306_clear(&disp);

    // Inicializa LEDs
    gpio_init(LED_B);
    gpio_init(LED_R);
    gpio_init(LED_G);
    gpio_set_dir(LED_B, GPIO_OUT);
    gpio_set_dir(LED_R, GPIO_OUT);
    gpio_set_dir(LED_G, GPIO_OUT);

    // Inicialização do botão do joystick
    gpio_init(SW);
    gpio_set_dir(SW, GPIO_IN);
    gpio_pull_up(SW);

    // Inicializar ADC do joystick e sensores
    adc_init();
    adc_gpio_init(VRY);
    adc_gpio_init(VRX);
    adc_gpio_init(TEMP_SENSOR_PIN);
    adc_gpio_init(HUMIDITY_SENSOR_PIN);

    // Configuração do buzzer
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
}

void print_texto(int x, int y, int tam, char *msg) {
    ssd1306_draw_string(&disp, x, y, tam, msg);
    ssd1306_show(&disp);
}

void print_retangulo(int x1, int y1, int x2, int y2) {
    ssd1306_draw_empty_square(&disp, x1, y1, x2, y2);
    ssd1306_show(&disp);
}

void set_leds(int R, int G, int B) {
    gpio_put(LED_R, R);
    gpio_put(LED_G, G);
    gpio_put(LED_B, B);
}

void beep(uint duration_ms) {
    gpio_put(BUZZER_PIN, 1); // Liga o buzzer
    sleep_ms(duration_ms);
    gpio_put(BUZZER_PIN, 0); // Desliga o buzzer
}

int read_temperature() {
    adc_select_input(TEMP_SENSOR_PIN);
    return adc_read() * (3.3 / (1 << 12)) * 100; // Conversão para Celsius
}

int read_humidity() {
    adc_select_input(HUMIDITY_SENSOR_PIN);
    return adc_read() * (3.3 / (1 << 12)) * 100; // Conversão para porcentagem
}

void loop() {
    while (true) {
        // Lê temperatura e umidade
        int temperature = read_temperature();
        int humidity = read_humidity();

        // Atualiza display OLED
        ssd1306_clear(&disp);
        print_texto(0, 0, 1, "Temp:");
        print_texto(40, 0, 1, itoa(temperature, buf, 10)); // Convertendo para string
        print_texto(0, 10, 1, "Umid:");
        print_texto(40, 10, 1, itoa(humidity, buf, 10)); // Convertendo para string
        ssd1306_show(&disp);

        // Verifica o botão
        if (gpio_get(SW) == 0) {
            beep(500); // Bipe de 500ms
            // Acionar LEDs ou outras funções conforme necessário
        }

        sleep_ms(500); // Delay para evitar leituras muito rápidas
    }
}

int main() {
    setup();
    loop();
    return 0;
}