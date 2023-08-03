#include <SoftwareSerial.h> 
#include <LiquidCrystal.h> 
#define L_RS 8 
#define L_EN 9 
#define L_D4 10 
#define L_D5 11 
#define L_D6 12 
#define L_D7 13 
#define M1_P 4 
#define M1_N 5 
#define M2_P 6 
#define M2_N 7 
#define BT_TX 2 
#define BT_RX 3 
#define MEM_X A0 
#define MEM_Y A1 
#define MEM_Z A2 
#define HMN_P 17
  signed long i, x, y, z, d1, d2, d3, diff = 10; 
  char str[80], n, smpl, rx_byte, bt_rx_f; 
unsigned long x_v, y_v, z_v; 
unsigned char rx_dat[64], index, j[4], k[4]; 
const char gesture_name[4][16] = 
{ 
 "up", //1 
 "down", //2 
 "left", //3 
 "right", //4 
}; 
const int action[4][3] = 
{ 
 /* X Y Z*/ 
 285, 310, 345, //1 
 340, 345, 285, //2 
 328, 278, 372, //3 
 328, 405, 335, //4 
}; 
LiquidCrystal lcd(L_RS, L_EN, L_D4, L_D5, L_D6, L_D7); SoftwareSerial UART_SW(BT_RX, BT_TX); // RX, TX void setup()
{  
(M2_P, LOW);  digitalWrite(M2_N, LOW);  smpl = 100; 
 index = 0; 
 bt_rx_f = 0;

 for (i = 0; i < 4; i++) 
 { 
 j[i] = 0; 
 k[i] = 0; 
 } 
 i = 0; 
 rx_dat[index] = '\0'; 
 delay(500); 
 Serial.println("X Y Z");  UART_SW.println("X Y Z");  lcd.clear(); 
 lcd.setCursor(0, 0); 
 lcd.print("X"); 
 lcd.setCursor(5, 0); 
 lcd.print("Y"); 
 lcd.setCursor(10, 0); 
 lcd.print("Z"); 
 // test 
 /*digitalWrite(M1_P, HIGH);  digitalWrite(M1_N, LOW);  digitalWrite(M2_P, HIGH);  digitalWrite(M2_N, LOW);

 delay(500); 
 digitalWrite(M1_P, LOW);  digitalWrite(M1_N, HIGH);  digitalWrite(M2_P, LOW);  digitalWrite(M2_N, HIGH);  delay(500); 
 digitalWrite(M1_P, LOW);  digitalWrite(M1_N, LOW);  digitalWrite(M2_P, LOW);  digitalWrite(M2_N, LOW);*/ 
 // test 
 delay(500); 
} 
void mem_read() 
{ 
 x_v = 0; 
 y_v = 0; 
 z_v = 0; 
 for (n = 0; n < smpl; n++)  { 
 x = analogRead(MEM_X);  y = analogRead(MEM_Y);
 z = analogRead(MEM_Z);  x_v = x_v + x; 
 y_v = y_v + y; 
 z_v = z_v + z; 
 delay(2); 
 } 
 x = x_v / smpl; 
 y = y_v / smpl; 
 z = z_v / smpl; 
 Serial.print(x); 
 Serial.print(" "); 
 Serial.print(y); 
 Serial.print(" "); 
 Serial.print(z); 
 Serial.println(); 
 lcd.setCursor(0, 1); 
 lcd.print(x); 
 lcd.setCursor(5, 1); 
 lcd.print(y); 
 lcd.setCursor(10, 1); 
 lcd.print(z); 
 for (i = 0; i < 4; i++)
{ 
 d1 = action[i][0] - x; 
 d2 = action[i][1] - y; 
 d3 = action[i][2] - z; 
 if ((abs(d1) < diff) && (abs(d2) < diff) && (abs(d3) < diff))  { 
 sprintf(str, "%s", gesture_name[i]); 
 Serial.println(str); 
 switch (i) 
 { 
 case 0: 
 j[0]++; 
 j[1] = 0; 
 j[2] = 0; 
 j[3] = 0; 
 if (j[0] >= 10) 
 { 
 Serial.println("\n\ru"); 
 move_up(); 
 j[0] = 0;
 
 } 
 break; 
 case 1: 
 j[0] = 0; 
 j[1]++; 
 j[2] = 0; 
 j[3] = 0; 
 if (j[1] >= 10) 
 { 
 if (digitalRead(HMN_P) == 1)  { 
 Serial.println("\n\rd"); 
 move_down(); 
 } 
 else 
 { 
 Serial.println("\n\rdown reached.");  } 
 j[1] = 0; 
 } 
 break; 
 case 2:

 j[0] = 0; 
 j[1] = 0; 
 j[2]++; 
 j[3] = 0; 
 if (j[2] >= 10) 
 { 
 Serial.println("\n\rr");  move_right(); 
 j[2] = 0; 
 } 
 break; 
 case 3: 
 j[0] = 0; 
 j[1] = 0; 
 j[2] = 0; 
 j[3]++; 
 if (j[3] >= 10) 
 { 
 Serial.println("\n\rl");  move_left();

 j[3] = 0; 
 } 
 break; 
 } 
 } 
 } 
} 
void bt_receive() 
{ 
 if (UART_SW.available() > 0)  { 
 rx_byte = UART_SW.read(); 
 // test 
 /*UART_SW.print(rx_byte);  Serial.println(rx_byte); 
 lcd.setCursor(15, 0); 
 lcd.print(rx_byte);*/ 
 // test 
 if (rx_byte == '$' && index == 0)
 
 { 
 rx_dat[index++] = rx_byte;  rx_dat[index] = '\0'; 
 } 
 else if (rx_byte != '#') 
 { 
 if (index > 0 && index < 64)  { 
 rx_dat[index++] = rx_byte; 
 rx_dat[index] = '\0';  } 
 else 
 { 
 index = 0; 
 bt_rx_f = 0; 
 rx_dat[index] = '\0';  } 
 } 
 else if (rx_byte == '#')  {

 if (index > 0 && index < 64)  { 
 rx_dat[index++] = rx_byte;  rx_dat[index] = '\0';  bt_rx_f = 1; 
 } 
 else 
 { 
 index = 0; 
 bt_rx_f = 0; 
 rx_dat[index] = '\0';  } 
 } 
 else 
 { 
 index = 0; 
 bt_rx_f = 0; 
 rx_dat[index] = '\0'; 
 } 
 rx_byte = ' '; 
 }
 
 // test 
 /*if (bt_rx_f == 1) 
 { 
 sprintf(str, "%s", rx_dat); 
 Serial.println(str); 
 bt_rx_f = 0; 
 index = 0; 
 }*/ 
 // test 
 if (bt_rx_f == 1) 
 { 
 if (rx_dat[0] == '$' && rx_dat[1] == ',' && rx_dat[2] == 'U' && rx_dat[3] == ',' &&  rx_dat[4] == '#') 
 { 
 Serial.println("\n\rU"); 
 move_up(); 
 } 
 else if (rx_dat[0] == '$' && rx_dat[1] == ',' && rx_dat[2] == 'D' && rx_dat[3] == ',' &&  rx_dat[4] == '#') 
 {
 
 if (digitalRead(HMN_P) == 1) 
 { 
 Serial.println("\n\rD"); 
 move_down(); 
 } 
 else 
 { 
 Serial.println("\n\rDown reached."); 
 } 
 } 
 else if (rx_dat[0] == '$' && rx_dat[1] == ',' && rx_dat[2] == 'L' && rx_dat[3] == ',' &&  rx_dat[4] == '#') 
 { 
 Serial.println("\n\rL"); 
 move_right(); 
 } 
 else if (rx_dat[0] == '$' && rx_dat[1] == ',' && rx_dat[2] == 'R' && rx_dat[3] == ',' &&  rx_dat[4] == '#') 
 { 
 Serial.println("\n\rR"); 
 move_left(); 
 } 
 bt_rx_f = 0;

 index = 0; 
 rx_dat[index] = '\0';  } 
} 
void move_up() 
{ 
 digitalWrite(M1_P, HIGH);  digitalWrite(M1_N, LOW);  delay(200); 
 digitalWrite(M1_P, LOW);  digitalWrite(M1_N, LOW); } 
void move_down() 
{ 
 digitalWrite(M1_P, LOW);  digitalWrite(M1_N, HIGH);  delay(200); 
 digitalWrite(M1_P, LOW);  digitalWrite(M1_N, LOW); } 
void move_right()

{ 
 digitalWrite(M2_P, HIGH);  digitalWrite(M2_N, LOW); delay(200); 
 digitalWrite(M2_P, LOW);  digitalWrite(M2_N, LOW); } 
void move_left() 
{ 
 digitalWrite(M2_P, LOW);  digitalWrite(M2_N, HIGH);  delay(200); 
 digitalWrite(M2_P, LOW);  digitalWrite(M2_N, LOW); } 
void loop() 
{ 
 mem_read(); 
 bt_receive(); 
 delay(100); 
}
