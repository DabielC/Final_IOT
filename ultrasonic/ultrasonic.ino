void setup() {
    Serial.begin(9600);
    
    pinMode(7, OUTPUT);  // Trigger pin for ultrasonic sensor
    pinMode(4, INPUT);   // Echo pin for ultrasonic sensor
    pinMode(12, OUTPUT); // LED pin (PWM)
  }
  
  void loop() {
    // ส่งสัญญาณไปที่เซ็นเซอร์อัลตราโซนิก
    digitalWrite(7, HIGH);
    delayMicroseconds(10); 
    digitalWrite(7, LOW);
  
    // รับค่ากลับมาจาก Echo pin
    int pulseWidth = pulseIn(4, HIGH);
  
    // คำนวณระยะทาง (หน่วย: cm)
    long distance = pulseWidth / 29 / 2;
  
    // แสดงค่าบน Serial Monitor
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  
    // แปลงระยะทางเป็นค่าความสว่าง (0 - 255)
    int brightness = map(distance, 5, 100, 255, 0); 
    brightness = constrain(brightness, 0, 255);  // จำกัดค่าให้อยู่ในช่วง 0 - 255
  
    // ปรับความสว่างของ LED
    analogWrite(12, brightness);
  
    delay(100);  // หน่วงเวลาสั้น ๆ ก่อนอ่านค่าใหม่
  }
  