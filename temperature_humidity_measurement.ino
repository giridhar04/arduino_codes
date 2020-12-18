//This code helps us to send the Humidity and Temperature data measured by DHT11 sensor to ThingSpeak website using ESP8266 nodemcu

#include <DHT.h>  // Library for DTH11 Humidity & Temperature Sensor
#include <ESP8266WiFi.h>   //Library For ESP8266 nodeMCU

#define DHTPIN D3  //ESP8266 pin number
#define DHTTYPE DHT11

const char* ssid="ssid_of_your_network";            //replace with your network SSID
const char* password="password_for_your_network";  //raplace with the passwork for your network
const char* host="api.thingspeak.com";            //The website to which we are sending
const char* writeAPIKey="write_API_key";          //This API key is given for your channel in ThingSpeak website

DHT dht(DHTPIN, DHTTYPE,15);

void setup()
{
  
  Serial.begin(115200);                          //Keep same baud on serial monitor also 
  dht.begin();
  delay(1000);
  
  Serial.println("Connecting to ");              
  Serial.print(ssid);

  WiFi.begin(ssid,password);                   //Connecting ESP module to WIFi
  while(WiFi.status() != WL_CONNECTED)             
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" ");
  Serial.println("WiFi connected");

}

void loop()
{
                                             //read temperature and humidity and store them in 2 variables
  float hum=dht.readHumidity();
  float temp=dht.readTemperature();

  if(isnan(hum) || isnan(temp))             //checking whether the data is a number or not
  {
    return;
  }
  
  WiFiClient client;

  const int httpPort=80;
  if(!client.connect(host,httpPort))
  {
    return;
  }

  String url="/update?key=";              //url is to update the fields in the ThingSpeak channel
  url+=writeAPIKey;
  url+="&field1=";
  url+=String(temp);
  url+="&field2=";
  url+=String(hum);
  url+="\r\n";

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +    //Updates fileds in our channel in ThingSpeak
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  Serial.print("Temperature: ");      //printing Temperature on serial monitor to verify the data
  Serial.print(temp);

  Serial.print("   ");
  Serial.print("Humidity: ");        //printing Humidity on serial monitor to verify the data
  Serial.println(hum);

    client.stop();
    Serial.println("Wait for 15 sec to update datapack in ThingSpeak");  //It takes 15 Sec time to show the updated data in ThingSpeak website
    delay(20000);
}
