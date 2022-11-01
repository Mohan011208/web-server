void setup() {
  // put your setup code here, to run once:

}
#include <WiFi.h> // Load Wi-Fi library

// Replace with your network credentials
const char ssid[] = "ARYAN"; 
const char password[] = "********"; 

// Set web server port number to 80 default HTTP port
WiFiServer server(80);

// Variable to store the HTTP request
String header;

//Variables to store the current output state of LED
String whiteLEDState = "off";
String greenLEDState = "off";

// Assign output variables to GPIO pins
const int whiteLED = 15;
const int greenLED = 5;

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(whiteLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  // Set outputs to LOW
  digitalWrite(whiteLED, LOW);
  digitalWrite(greenLED, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /white/on") >= 0) {
              Serial.println("White LED is on");
              whiteLEDState = "on";
              digitalWrite(whiteLED, HIGH);
            } else if (header.indexOf("GET /white/off") >= 0) {
              Serial.println("White LED is off");
              whiteLEDState = "off";
              digitalWrite(whiteLED, LOW);
            } else if (header.indexOf("GET /green/on") >= 0) {
              Serial.println("Green LED is on");
              greenLEDState = "on";
              digitalWrite(greenLED, HIGH);
            } else if (header.indexOf("GET /green/off") >= 0) {
              Serial.println("Green LED is off");
              greenLEDState = "off";
              digitalWrite(greenLED, LOW);
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            // CSS to style the on/off buttons 
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #e62936; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #2a09e6;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP32: Controlling External LED using Wi-Fi</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 15 
            client.println("<p>White LED State " + whiteLEDState + "</p>");
            // If the whiteLEDState is off, it displays the ON button       
            if (whiteLEDState=="off") {
              client.println("<p><a href=\"/white/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/white/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for GPIO 5 
            client.println("<p>Green LED State " + greenLEDState + "</p>");
            // If the greenLEDState is off, it displays the ON button       
            if (greenLEDState=="off") {
              client.println("<p><a href=\"/green/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/green/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
void loop() {
  // put your main code here, to run repeatedly:

}
