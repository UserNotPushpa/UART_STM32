/*
 * Copyright (c) 2021 Jonathan Hahn
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/drivers/gpio.h>

#define UART_NODE1 DT_ALIAS(ardusart)

const struct device *const sl_uart1 = DEVICE_DT_GET(UART_NODE1);


#define LED0_NODE DT_ALIAS(led0)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

#define LED1_NODE DT_ALIAS(led2)
static const struct gpio_dt_spec led_1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);


int main(void)
{
	unsigned char recv = 0x00;

	if (!device_is_ready(sl_uart1)) {
		printk("uart devices not ready\n");
		return 0;
	}

	gpio_pin_configure_dt(&led, GPIO_OUTPUT_LOW);
	gpio_pin_configure_dt(&led_1, GPIO_OUTPUT_LOW);

	while (true) {

		uart_poll_out(sl_uart1, recv);

		/* give the uarts some time to get the data through */
		k_sleep(K_MSEC(50));

		int ret = uart_poll_in(sl_uart1, &recv);		

		if (ret < 0) {
			printk("Receiving failed. Error: %d", ret);
			gpio_pin_toggle_dt(&led_1);
		} else {			
			//printk("Received %d\n", recv);
			gpio_pin_toggle_dt(&led);
		}
		
		k_sleep(K_MSEC(10));
	}
	return 0;
}
