/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#pragma once
#include <stdint.h>
#include <stdbool.h>

#define DEFAULT_POWER  true
#define NVS_PARTITION NVS_DEFAULT_PART_NAME
#define NVS_NAMESPACE "esp32_nmspce"
#define DEVICE_NAME "Kshitij Room"

void app_driver_init(void);
void app_driver_set_state(char* key, bool target, nvs_handle_t nvs_gpio_handle, bool to_flash);
bool app_driver_get_gpio_state(char* key, nvs_handle_t nvs_gpio_handle);
void map_gpio_to_device(esp_rmaker_node_t *node);
char* gpio_name_to_gpio_pin_str(char* gpio_name);
