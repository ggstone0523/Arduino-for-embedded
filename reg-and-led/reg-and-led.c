#define MYPORTB (*(volatile unsigned char *)0x25)
#define MYDDRB (*(volatile unsigned char *)0x24)
#define MYTTCR0A (*(volatile unsigned char *)0x44)
#define MYTTCR0B (*(volatile unsigned char *)0x45)
#define MYTIMSK0 (*(volatile unsigned char *)0x6E)
#define clear_bit(reg, loc) (reg & ~(0x1 << loc))
#define clear_bits(reg, bits, loc) (reg & ~(bits << loc))
#define set_bit(reg, loc) (reg | (0x1 << loc))
#define set_bits(reg, bits, loc) (reg | (bits << loc))
#define TIMER2_INTERRUPT_HANDLER void __attribute__ ((signal, used, externally_visible)) __vector_16(void)

volatile unsigned int timecheck = 0;
void loop();
void led_on();
void led_off();
void mydelay(int times);
TIMER2_INTERRUPT_HANDLER {
	static unsigned int inner_time_check = 0;

	inner_time_check++;
	if(inner_time_check > 10000) {
		timecheck++;
		inner_time_check = 0;
	}
}

int main() {
	// put your setup code here, to run once:
	MYDDRB = clear_bit(MYDDRB, 5);
	MYDDRB = set_bit(MYDDRB, 5);
	MYPORTB = clear_bit(MYPORTB, 5);
	MYTTCR0A = clear_bits(MYTTCR0A, 0x3, 0);
	MYTTCR0B = clear_bits(MYTTCR0B, 0xE, 0);
	MYTTCR0B = set_bit(MYTTCR0B, 0);
	MYTIMSK0 = set_bit(MYTIMSK0, 0);
	asm("sei");
	
	while(1) {
		loop();
	}
	
	return 0;
}

void loop() {
	// put your main code here, to run repeatedly:
	led_on();
	mydelay(10);
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

