#include <stm32f4xx.h>
#include <assert.h>

#define ARRAY_SIZE(arr)  (sizeof((arr))/(sizeof(arr[0])))

struct Letter
{
	int post_letter_wait_time;
	int per_line_wait_time;
	char led_status[7][5];
};

#define LINE_WAIT		2
#define LETTER_WAIT		LINE_WAIT * 2

static const struct Letter letters[] =
{
	{
		LETTER_WAIT,
		LINE_WAIT,
		{
		{1,1,1,1,1},
		{0,0,1,0,0},
		{0,0,1,0,0},
		{0,0,1,0,0},
		{0,0,1,0,0},
		{0,0,1,0,0},
		{0,0,1,0,0},
		}
	},
	{
		LETTER_WAIT,
		LINE_WAIT,
		{
		{0,1,1,1,0},
		{1,0,0,0,1},
		{1,0,0,0,1},
		{1,0,0,0,1},
		{1,0,0,0,1},
		{1,0,0,0,1},
		{0,1,1,1,0},
		}
	},
	{
		LETTER_WAIT,
		LINE_WAIT,
		{
		{1,0,0,0,1},
		{1,1,0,1,1},
		{1,0,1,0,1},
		{1,0,0,0,1},
		{1,0,0,0,1},
		{1,0,0,0,1},
		{1,0,0,0,1},
		}
	},
	{
		6 * LETTER_WAIT,
		LINE_WAIT,
		{
		{0,1,1,1,0},
		{1,0,0,0,1},
		{1,0,0,0,1},
		{1,0,0,0,1},
		{1,0,0,0,1},
		{1,0,0,0,1},
		{0,1,1,1,0},
		}
	}
};

struct Pin
{
	uint32_t AHB1_clock_periph;
	GPIO_TypeDef * port;
	uint16_t pin;
};

static struct Pin pins[] =
{
	{RCC_AHB1Periph_GPIOE, GPIOE, GPIO_Pin_13},
	{RCC_AHB1Periph_GPIOE, GPIOE, GPIO_Pin_15},
	{RCC_AHB1Periph_GPIOB, GPIOB, GPIO_Pin_11},
	{RCC_AHB1Periph_GPIOB, GPIOB, GPIO_Pin_13},
	{RCC_AHB1Periph_GPIOB, GPIOB, GPIO_Pin_15},
	{RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_9},
	{RCC_AHB1Periph_GPIOD, GPIOD, GPIO_Pin_11},
};

void SysTick_Handler()
{
	static int wait_count = 0;
	wait_count--;
	if (wait_count > 0)
		return;

	static int current_letter = 0;
	static int current_line = 0;

	if (current_line >= ARRAY_SIZE(letters[current_letter].led_status[0]))
	{
		wait_count = letters[current_letter].post_letter_wait_time;
		current_letter = (current_letter + 1) % ARRAY_SIZE(letters);
		current_line = 0;

		for (int i = 0; i < ARRAY_SIZE(letters[current_letter].led_status); i++)
			GPIO_ResetBits(pins[i].port, pins[i].pin);

		return;
	}

	static_assert(ARRAY_SIZE(pins) == ARRAY_SIZE(letters[current_letter].led_status), "Uncompatible letter definition with pins array");
	for (int i = 0; i < ARRAY_SIZE(letters[current_letter].led_status); i++)
	{
		if (letters[current_letter].led_status[i][current_line])
			GPIO_SetBits(pins[i].port, pins[i].pin);
		else
			GPIO_ResetBits(pins[i].port, pins[i].pin);
	}

	current_line = (current_line + 1) % (ARRAY_SIZE(letters[current_letter].led_status[0]) + 1);
	wait_count = letters[current_letter].per_line_wait_time;
	return;
}

int main(void)
{
	SystemInit();

	// Initiate GPIO pins
	for (int i = 0; i < ARRAY_SIZE(pins); i++)
		RCC_AHB1PeriphClockCmd(pins[i].AHB1_clock_periph, ENABLE);

	// GPIOD init
	GPIO_InitTypeDef gpio_init;
	gpio_init.GPIO_Mode = GPIO_Mode_OUT;
	gpio_init.GPIO_OType = GPIO_OType_PP;
	gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio_init.GPIO_Speed = GPIO_Speed_100MHz;
	for (int i = 0; i < ARRAY_SIZE(pins); i++)
	{
		gpio_init.GPIO_Pin = pins[i].pin;
		GPIO_Init(pins[i].port, &gpio_init);
	}

	// Initiate SysTick
	SysTick_Config(SystemCoreClock / 1000);

	while(1);
	return 0;
}
