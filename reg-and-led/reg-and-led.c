#define MYDDRB   (*(volatile unsigned char *)0x24)
#define MYPORTB  (*(volatile unsigned char *)0x25)
#define MYTTCR0A (*(volatile unsigned char *)0x44)
#define MYTTCR0B (*(volatile unsigned char *)0x45)
#define MYTIMSK0 (*(volatile unsigned char *)0x6E)
#define MYUCSR0A (*(volatile unsigned char *)0xC0)
#define MYUCSR0B (*(volatile unsigned char *)0xC1)
#define MYUCSR0C (*(volatile unsigned char *)0xC2)
#define MYUBRR0L (*(volatile unsigned char *)0xC4)
#define MYUBRR0H (*(volatile unsigned char *)0xC5)
#define MYUDR0   (*(volatile unsigned char *)0xC6)

#define clear_bit(reg, loc) (reg & ~(0x1 << loc))
#define clear_bits(reg, bits, loc) (reg & ~(bits << loc))
#define set_bit(reg, loc) (reg | (0x1 << loc))
#define set_bits(reg, bits, loc) (reg | (bits << loc))
#define check_bit(reg, loc) (reg & (1 << loc))
#define TIMER2_INTERRUPT_HANDLER void __attribute__ ((signal, used, externally_visible)) __vector_16(void)

#define F_CPU  16000000
#define BAUD   115200
#define MYUBRR (F_CPU / 8 / BAUD - 1)

volatile unsigned int timecheck = 0;
void loop();
void led_on();
void led_off();
void mydelay(int times);
void char_transmit(unsigned char character);
TIMER2_INTERRUPT_HANDLER {
	static unsigned int inner_time_check = 0;

	inner_time_check++;
	if(inner_time_check > 10000) {
		timecheck++;
		inner_time_check = 0;
	}
}

int main() {
	//led init
	MYDDRB = set_bit(MYDDRB, 5);
	MYPORTB = clear_bit(MYPORTB, 5);
	
	//timer init
	MYTTCR0A = clear_bits(MYTTCR0A, 0x3, 0);
	MYTTCR0B = clear_bits(MYTTCR0B, 0xE, 0);
	MYTTCR0B = set_bit(MYTTCR0B, 0);
	MYTIMSK0 = set_bit(MYTIMSK0, 0);
	asm("sei");
	
	//usart init
	MYUBRR0H = (unsigned char)(MYUBRR >> 8);
	MYUBRR0L = (unsigned char)MYUBRR;
	MYUCSR0A = set_bit(MYUCSR0A, 1);
	MYUCSR0B = set_bit(MYUCSR0B, 4) | set_bit(MYUCSR0B, 3);
	MYUCSR0B = clear_bit(MYUCSR0B, 2);
	MYUCSR0C = set_bit(MYUCSR0C, 3) | set_bits(MYUCSR0C, 0x3, 1);
	
	while(1) {
		loop();
	}
	
	return 0;
}

void loop() {
	char_transmit('u');
	char_transmit('\n');
	led_on();
	mydelay(10);
	char_transmit('d');
	char_transmit('\n');
	led_off();
	mydelay(10);
}

void led_on() {
	MYPORTB = set_bit(MYPORTB, 5);
}

void led_off() {
	MYPORTB = clear_bit(MYPORTB, 5);
}

void mydelay(int times) {
	unsigned int settime = timecheck+times;
	while(settime != timecheck);
}

void char_transmit(unsigned char character)
{
	while(!check_bit(MYUCSR0A, 5));
	MYUDR0 = character;
}

