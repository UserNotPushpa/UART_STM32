#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/drivers/gpio.h>
#include <string.h>

#define UART_NODE1 DT_ALIAS(ardusart)

const struct device *const sl_uart1 = DEVICE_DT_GET(UART_NODE1);


#define LED1_NODE DT_ALIAS(led0)
static const struct gpio_dt_spec myled1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);

#define LED2_NODE DT_ALIAS(led2)
static const struct gpio_dt_spec myled2 = GPIO_DT_SPEC_GET(LED2_NODE, gpios);


void send_str(const struct device *uart, char *str)
{
	int msg_len = strlen(str);

	for (int i = 0; i < msg_len; i++) {
		uart_poll_out(uart, str[i]);
	}

	printk("Device %s sent: \"%s\"\n", uart->name, str);
}

void recv_str(const struct device *uart, char *str)
{
	char *head = str;
	char c;

	while (!uart_poll_in(uart, &c)) {
		*head++ = c;
		//uart_poll_out(uart, c);
	}
	*head = '\0';

	printk("Device %s received: \"%s\"\n", uart->name, str);
}


const int MSG_SIZE = 4;

int main(void)
{
	unsigned char recv[MSG_SIZE];
	unsigned char txmsg[1] = {0xcc};

	if (!device_is_ready(sl_uart1)) {
		printk("uart devices not ready\n");
		return 0;
	}

	gpio_pin_configure_dt(&myled1, GPIO_OUTPUT_LOW);
	gpio_pin_configure_dt(&myled2, GPIO_OUTPUT_LOW);

	while (true) {

		// tx 0xFF to arduino
		// arduino will send 4 bytes -- receive them
		uart_poll_out(sl_uart1, txmsg[0]);

		k_sleep(K_MSEC(200));

		//recv_str(sl_uart1, recv);
		//k_sleep(K_MSEC(10));
	}
	return 0;
}
