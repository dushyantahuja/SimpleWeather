
// Replace with your wifi credentials
const char * ssid = "DUSHYANT";
const char * password = "ahuja987";

// Get and API Key by registering on
// https://openweathermap.org/api



String Key = "15121d64b58b9086439fed7f00050c04";

weatherData w;
SimpleWeather weather(Key, "London,uk");


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("WiFi Failed");
      while(1) {
          delay(1000);
      }
  }
}

void loop() {
  weather.updateStatus(&w);
  Serial.println()
  delay(600000);       // Wait for 600 seconds
  // put your main code here, to run repeatedly:
}