#include <reg52.h>

// --- ????? ??? Pins ?????? ---
#define LCD P2
sbit RS = P3^5;
sbit EN_LCD = P3^7;

sbit GREEN = P3^0;
sbit RED   = P3^6; // ????? ??????? ???? ?????? ??????
// P3.1 ????? ??? TX (????????) ????????

sbit IN1 = P3^2;
sbit IN2 = P3^3;
sbit EN1 = P3^4; // PWM

// --- ??????? ?????? ---
unsigned char speed_val = '1'; 
unsigned int runtime = 0;
bit fan_running = 0;
bit is_timer_mode = 0; // 0 = ????? ?????? 1 = ?????[cite: 8]

// --- Prototypes ---
void delay(unsigned int ms);
void lcd_cmd(unsigned char cmd);
void lcd_data(unsigned char dat);
void lcd_string(char *str);
void lcd_init();
void uart_init();
void uart_tx(char ch);
void uart_msg(char *str);
char keypad_scan();
void motor_control();

void main() {
    char key;
    uart_init(); // ????? ????????
    lcd_init();
    
    IN1 = 1; IN2 = 0; EN1 = 0;
    GREEN = 0; RED = 1;
    
    uart_msg("System Ready...\r\n"); // ????? ????? ??? ??????????

    while(1) {
        key = keypad_scan();

        if(key >= '1' && key <= '6') { // 6 ????? ????? ?????
            speed_val = key;
            is_timer_mode = 0; // ????? ??????? ??????? ???????
            lcd_cmd(0x80); lcd_string("Speed Set: "); lcd_data(speed_val);
            lcd_cmd(0xC0); lcd_string("Mode: Continuous");
            uart_msg("Speed changed to: "); uart_tx(speed_val); uart_msg("\r\n");
        }
        else if(key == 'A' || key == 'B' || key == 'C') { // 3 ????? ?????
            is_timer_mode = 1;
            if(key == 'A') { runtime = 500;  lcd_cmd(0xC0); lcd_string("Timer: 5 sec  ");  uart_msg("Timer: 5s\r\n"); }
            if(key == 'B') { runtime = 1000; lcd_cmd(0xC0); lcd_string("Timer: 10 sec "); uart_msg("Timer: 10s\r\n"); }
            if(key == 'C') { runtime = 1500; lcd_cmd(0xC0); lcd_string("Timer: 15 sec "); uart_msg("Timer: 15s\r\n"); }
        }
        else if(key == '*') { // ?? ?????? ??? START (???? ??? C ??????)
            fan_running = 1;
            GREEN = 1; RED = 0;
            lcd_cmd(0x01); lcd_string("Fan ON...");
            uart_msg("Fan Started\r\n");
        }
        else if(key == 'D') { // STOP
            fan_running = 0;
            EN1 = 0; GREEN = 0; RED = 1;
            lcd_cmd(0x01); lcd_string("Fan Stopped");
            uart_msg("Fan Stopped Manually\r\n");
            delay(500);
        }

        if(fan_running) {
            motor_control(); 
            if(is_timer_mode) { // ?? ?? ??? ???????? ???? ???? ????????[cite: 6]
                if(runtime > 0) runtime--;
                if(runtime == 0) {
                    fan_running = 0; EN1 = 0; GREEN = 0; RED = 1;
                    lcd_cmd(0x01); lcd_string("Time Up! Stop");
                    uart_msg("Fan Stopped: Time Expired\r\n");
                    delay(1000);
                }
            }
        }
    }
}

// --- ????? ???????? (UART) ---
void uart_init() {
    TMOD = 0x20; // Timer 1, Mode 2 (8-bit auto-reload)
    TH1 = 0xFD;  // 9600 Baud rate @ 11.0592MHz
    SCON = 0x50; // 8-bit UART, enable receiver
    TR1 = 1;     // Start Timer
}
void uart_tx(char ch) {
    SBUF = ch;
    while(TI == 0);
    TI = 0;
}
void uart_msg(char *str) {
    while(*str) uart_tx(*str++);
}

// --- ????? ??? PWM ??? 6 ????? ---
void motor_control() {
    unsigned char duty;
    // ????? ??????? ?? 6 ??????? (1-6)
    if(speed_val == '1') duty = 2;
    else if(speed_val == '2') duty = 4;
    else if(speed_val == '3') duty = 6;
    else if(speed_val == '4') duty = 7;
    else if(speed_val == '5') duty = 8;
    else duty = 9; // ???? 6

    EN1 = 1; delay(duty);
    EN1 = 0; delay(10 - duty);
}

// --- ????? ??? LCD ???? Keypad ???? Delay (??? ??) ---
void lcd_cmd(unsigned char cmd) {
    LCD = cmd; RS = 0; EN_LCD = 1; delay(1); EN_LCD = 0;
    if(cmd == 0x01) delay(20);
}
void lcd_data(unsigned char dat) { LCD = dat; RS = 1; EN_LCD = 1; delay(1); EN_LCD = 0; }
void lcd_init() { lcd_cmd(0x38); lcd_cmd(0x0C); lcd_cmd(0x01); lcd_cmd(0x06); }
void lcd_string(char *str) { while(*str) lcd_data(*str++); }
void delay(unsigned int ms) { unsigned int i, j; for(i=0; i<ms; i++) for(j=0; j<120; j++); }
char keypad_scan() {
    unsigned char row, col;
    char key_map[4][4] = {{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}};
    P1 = 0xF0;
    if(P1 != 0xF0) {
        for(row=0; row<4; row++) {
            P1 = ~(1 << row);
            for(col=0; col<4; col++) {
                if(!(P1 & (0x10 << col))) {
                    while(!(P1 & (0x10 << col)));
                    return key_map[row][col];
                }
            }
        }
    }
    return 0;
}