void setup() {
  Serial.begin(115200); // use the same baud-rate as the python side
}
void loop() {
  Serial.println("Hello world from Arduino!"); // write a string
  delay(1000);
}
