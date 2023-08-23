// trigger
// EOL reading

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


// assuming burst resc -- interupts other processes
// later buffer read with endstring "\n"
void recv_str(const struct device *uart, char *str)
{   
	char *head = str;
	char c;

	while (!uart_poll_in(uart, &c)){
		*head++ = c;
	}

	*head = '\0';
	printk("Device %s received: \"%s\"\n", uart->name, str);
}


const int RX_BUFFER_SIZE = 5;

int main(void)
{
	unsigned char recv[RX_BUFFER_SIZE];
	unsigned char txmsg[1] = {0xcc};

	if (!device_is_ready(sl_uart1)) {
		printk("uart devices not ready\n");
		return 0;
	}

	gpio_pin_configure_dt(&myled1, GPIO_OUTPUT_LOW);
	gpio_pin_configure_dt(&myled2, GPIO_OUTPUT_LOW);

	int ret;

	while (true) {

		uart_poll_out(sl_uart1, txmsg[0]);

		k_sleep(K_MSEC(2));

		for (int i=0; i<RX_BUFFER_SIZE; i++){
			
			while (uart_poll_in(sl_uart1, recv[i])!=-1)
			{
				uart_poll_out(sl_uart1, recv[i]);
			}
				
		}

        k_sleep(K_MSEC(1000));
	}



	return 0;
}
