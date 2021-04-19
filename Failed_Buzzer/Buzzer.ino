/*
TaskHandle_t Buzzer1;
TaskHandle_t Buzzer2;

int buzzer1_pin = 14;
int buzzer2_pin = 4;

int tempo = 130;
int tempo2 = 70;

void setup() {
  Serial.begin(9600);
  
  pinMode(buzzer1_pin, OUTPUT);
  pinMode(buzzer2_pin, OUTPUT);

  xTaskCreatePinnedToCore(
    Buzzer1_code, 
    "Buzzer1", 
    10000,  
    NULL, 
    1,  
    &Buzzer1,  
    0); 
    
  xTaskCreatePinnedToCore(
    Buzzer2_code, 
    "Buzzer2", 
    10000, 
    NULL, 
    1,  
    &Buzzer2,  
    1); 

}

void loop() {
  //fuck
}
*/
