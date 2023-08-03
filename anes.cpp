#include &lt;LiquidCrystal.h&gt;
#define PUL 5
#define DIR 6
#define ENA 7
#define SWT 4
#define POT A0
#define S_TIME 200
#define SAMPLE 25
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);
const int respPin = 3;
const int pulsePin = 2;
const int tempPin = A2;
const int buzPin = 17; // A3
unsigned int x, c;
int sec = 0, pcnt = 0, pulse = 0, rcnt = 0, resp = 0, temp = 0;
char str[32], p_f, r_f, t_f, t_f1, sw_f, sw_v, sw_s, k, n;
long ms;
int i, j, pos;
unsigned int adc_v, adc_t;
unsigned char doa;
unsigned int q;
void setup()
{
Serial.begin(9600);
pinMode(SWT, INPUT);
pinMode(respPin, INPUT);
pinMode(pulsePin, INPUT);
pinMode(buzPin, OUTPUT);
pinMode(DIR, OUTPUT);
pinMode(PUL, OUTPUT);
pinMode(ENA, OUTPUT);
digitalWrite(DIR, HIGH);
digitalWrite(PUL, HIGH);
digitalWrite(ENA, HIGH);
lcd.begin(16, 2);
lcd.setCursor(0, 0);
lcd.print(&quot;Patient Monitor&quot;);
lcd.setCursor(0, 1);
lcd.print(&quot;Start press switch&quot;);

58

p_f = 0; r_f = 0; t_f = 0; t_f1 = ; x = 0; sw_f = 0; sw_v = 0; sw_s = 0; doa = 3;
n = 0;
digitalWrite(buzPin, HIGH);
delay(2000); }
void Buzzer (long dly)
{
digitalWrite(buzPin, LOW);
delay(dly);
digitalWrite(buzPin, HIGH);
delay(dly);
}
void loop()
{
while (sw_s == 0)
{
sw_v = digitalRead(SWT);
if (sw_f == 0 &amp;&amp; sw_v == 1 &amp;&amp; sw_s == 0)
{
sw_f = 1;
}
else if (sw_f == 1 &amp;&amp; sw_v == 0 &amp;&amp; sw_s == 0)
{
sw_f = 2;
}
else if (sw_f == 2 &amp;&amp; sw_v == 1 &amp;&amp; sw_s == 0)
{
sw_f = 3;
sw_s = 1;
lcd.clear();
lcd.setCursor(0, 0);
lcd.print(&quot;SEC PUL RES TEM&quot;);
}
}
adc_t = 0;
for (k = 0; k &lt; SAMPLE; k++)
{
adc_t = adc_t + analogRead(POT);
delay(2);

59

}
adc_v = adc_t / SAMPLE;
adc_v = map(adc_v, 0, 1023, 1, 120);
Serial.print(&quot;weight is &quot;);
Serial.println(adc_v);
lcd.setCursor(0, 0);
lcd.print(&quot; &quot;);
lcd.setCursor(0, 0);
lcd.print(&quot;Weight: &quot;);
lcd.print(adc_v);
if (sw_s == 1)
{
Check_Pulse();
Check_Respiration();
Check_Temperature();
if (millis() - ms &gt; 1000)
{
ms = millis();
lcd.setCursor(0, 0);
lcd.print(&quot;SEC PUL RES TEM&quot;);
lcd.setCursor(0, 1);
sprintf(str, &quot;%3u %3u %3u %3u &quot;, sec, pcnt, rcnt, temp);
lcd.print(str);
if (++sec &gt;= 60)
{
if (temp &lt; 99.0)
{
lcd.setCursor(12, 1);
lcd.print(&quot;NRM&quot;); t_f = 0;
}
else if (temp &gt;= 99.0)
{
lcd.setCursor(12, 1);
lcd.print(&quot;FEV&quot;);
t_f = 1;
}
if (pcnt &lt; 60)
{
lcd.setCursor(4, 1);
lcd.print(&quot;LOW&quot;);

60

p_f = 1;
}
else if (pcnt &gt; 60 &amp;&amp; pcnt &lt; 100)
{
lcd.setCursor(4, 1);
lcd.print(&quot;NRM&quot;);
p_f = 0;
}
else if (pcnt &gt; 100)
{
lcd.setCursor(4, 1);
lcd.print(&quot;HIG&quot;);
p_f = 1;
}
if (resp &lt; 12)
{
lcd.setCursor(8, 1);
lcd.print(&quot;Hyx&quot;);
r_f = 1;
}
else if (rcnt &gt; 12 &amp;&amp; rcnt &lt; 18)
{
lcd.setCursor(8, 1);
lcd.print(&quot;NRM&quot;);
r_f = 0;
}
else if (resp &gt; 18)
{
lcd.setCursor(8, 1);
lcd.print(&quot;HIG&quot;);
r_f = 1;
}
pcnt = pcnt + 1;
q = (3 * adc_v) * (10 * pcnt);
Serial.println(q);
delay(5000);

61

lcd.clear();
lcd.setCursor(0, 0);
lcd.print(&quot;Anthesia Requ:&quot;);
lcd.setCursor(0, 1);
lcd.print(q / 1000);
lcd.print(&#39;.&#39;);
lcd.print((q % 1000) / 100);
lcd.print(&quot; ml&quot;);
sec = pcnt = rcnt = 0;
sw_f = 0;
sw_s = 0;
}
}
if (p_f == 1 || r_f == 1 || t_f == 1)
{
Buzzer(3000);
p_f = r_f = t_f = n = 0;
lcd.clear();
lcd.setCursor(0, 0);
lcd.print(&quot;Patient Monitor&quot;);
lcd.setCursor(0, 1);
lcd.print(&quot;Start press switch&quot;);
}
}
}
void Check_Pulse()
{
static long flag = 0, t = millis();
if (!digitalRead(pulsePin))
{
if ((flag != 1) &amp;&amp; (millis() - t &gt; 500))
{
flag = 1;
t = millis();
pcnt++;
}
}

62

else
{
flag = 0;
}
}
void Check_Respiration()
{
static long flag = 0, t = millis();
if (!digitalRead(respPin))
{
if ((flag != 1) &amp;&amp; (millis() - t &gt; 500))
{
flag = 1;
t = millis();
rcnt++;
}
}
else
{
flag = 0;
}
}
void Check_Temperature()
{
long tt = 0;
for (int i = 0; i &lt; 20; i++)
{
tt += (long)analogRead(tempPin);
}
tt /= 20;
temp = (((float)tt / 2.048) * 1.8) + 32.0;
}

63

void stepper_motor_fw()
{
unsigned int qt;
digitalWrite(DIR, LOW);
for (qt = 0; qt &lt; (q / 100); qt++)
{
digitalWrite(PUL, LOW);
delay(5);
digitalWrite(PUL, HIGH);
delay(5);
}
t_f1 = 1;
}
void stepper_motor_bw()
{
unsigned int qt;
digitalWrite(DIR, HIGH);
for (qt = 0; qt &lt; (q / 100); qt++)
{
digitalWrite(PUL, LOW);
delay(5);
digitalWrite(PUL, HIGH);
delay(5);
}
}