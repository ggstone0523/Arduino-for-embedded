#define MYPORTB (*(volatile unsigned char *)0x25)
#define MYDDRB (*(volatile unsigned char *)0x24)
#define clear_bit(reg, loc) (reg & ~(0x1 << loc))
#define set_bit(reg, loc) (reg | (0x1 << loc))

void setup() {
  // put your setup code here, to run once:
  MYDDRB = clear_bit(MYDDRB, 5); 
  MYPORTB = clear_bit(MYPORTB, 5);

  MYDDRB = set_bit(DDRB, 5);
}

void loop() {
  // put your main code here, to run repeatedly:
  led_on();
  delay(1000);
  led_off();
  delay(1000);
}

void led_on() {
  MYPORTB = set_bit(MYPORTB, 5);
}

void led_off() {
  MYPORTB = clear_bit(MYPORTB, 5);
}
