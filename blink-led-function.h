/* 
 * Alauddin Ansari
 * 2018-11-20
 * Functions:
 * Blink LED
 * Blink Number on LED
 * 
 * Use: blinkNumber(ledPin, number)
 */


void blinkLed(int ledPin, int times)
{
  if (times > 0) {
    for (int i = 0; i < times; i++) {
      digitalWrite(ledPin, HIGH);
      delay(300);
      digitalWrite(ledPin, LOW);
      delay(500);
    }
  } else {
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(500);
  }
}

void blinkNumber(int ledPin, int num) {
  if (num > 0) {
    while(num > 0) //do till num greater than  0
    {
        int mod = num % 10;  //split last digit from number
        
        blinkLed(ledPin, mod); //print the digit. 
    
        num = num / 10;    //divide num by 10. num /= 10 also a valid one 
        
        delay(1000);
    }
  } else {
    blinkLed(ledPin, 0);
  }
}
