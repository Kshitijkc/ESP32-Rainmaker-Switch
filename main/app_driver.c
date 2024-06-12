/* Switch demo implementation using button and RGB LED
   
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <sdkconfig.h>
#include <inttypes.h>
#include <nvs.h>

#include <iot_button.h>
#include <esp_rmaker_core.h>
#include <esp_rmaker_standard_params.h> 

#include <app_reset.h>
#include <ws2812_led.h>
#include "app_priv.h"

/* These values correspoind to H,S,V = 120,100,10 */
#define DEFAULT_RED     0
#define DEFAULT_GREEN   25
#define DEFAULT_BLUE    0


static void app_indicator_set(bool state)
{
    if (state) {
        ws2812_led_set_rgb(DEFAULT_RED, DEFAULT_GREEN, DEFAULT_BLUE);
    } else {
        ws2812_led_clear();
    }
}

static void app_indicator_init(void)
{
    ws2812_led_init();
    app_indicator_set(DEFAULT_POWER);
}

void flash_gpio_state(char* gpio_pin_str, bool state, nvs_handle_t nvs_gpio_handle) {
    esp_err_t err = true;
    while(err || err != ESP_OK) {
        err = nvs_set_i32(nvs_gpio_handle, gpio_pin_str, state);
        printf("Flashing GPIO %s state %s\n", gpio_pin_str, state ? "true" : "false");
    }
    err = true;
    while(err || err != ESP_OK) {
        err = nvs_commit(nvs_gpio_handle);
        printf("Committing GPIO %s state %s\n", gpio_pin_str, state ? "true" : "false");
    }
}

void app_driver_set_state(char* key, bool state, nvs_handle_t nvs_gpio_handle, bool to_flash)
{
    int gpio_pin = atoi(key);
    gpio_set_level(gpio_pin, state);
    printf("Completed setting GPIO %s state %s\n", key, state ? "true" : "false");
    if(to_flash) {
        flash_gpio_state(key, state, nvs_gpio_handle);
        printf("Completed flashing GPIO %s state %s\n", key, state ? "true" : "false");
    }
    app_indicator_set(state);
}

void app_driver_init()
{
    /* Configure power */
    gpio_config_t io_conf = {
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = 1,
    };
    io_conf.pin_bit_mask = ((uint64_t)1 << 19);
    /* Configure the GPIO */
    gpio_config(&io_conf);
    gpio_set_direction(18, GPIO_MODE_OUTPUT);
    gpio_set_direction(25, GPIO_MODE_OUTPUT);
    gpio_set_direction(26, GPIO_MODE_OUTPUT);
    app_indicator_init();
}

bool app_driver_get_gpio_state(char* key, nvs_handle_t nvs_gpio_handle)
{
    printf("Reading GPIO %s state ... ", key);
    int32_t value = 0; // value will default to 0, if not set yet in NVS
    esp_err_t err = nvs_get_i32(nvs_gpio_handle, key, &value);
    switch (err) {
        case ESP_OK:
            printf("Done\n");
            bool state = (bool) value;
            printf("GPIO pin: %s, state: %s\n", key, state ? "true" : "false");
            return (bool) state;
        case ESP_ERR_NVS_NOT_FOUND:
            printf("The value is not initialized yet!\n");
            break;
        default :
            printf("Error (%s) reading!\n", esp_err_to_name(err));
    }

    return NULL;
}
