#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "CHONG";
const char* password = "11031023";
const char* scriptUrl = "https://script.google.com/macros/s/AKfycbzGdT_CeJmtBzJ3OkT7oDzosU6aZxRryShMkttr0EjILd-Yx4du0zDg_p2WLByvc5C-/exec";  // Replace with your script URL

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Example barcode data to send
  String barcode = "87654321";
  
  // Send the data
  sendData(barcode);
}

void sendData(String barcode) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    http.begin(scriptUrl);  // Specify the URL
    http.addHeader("Content-Type", "application/json");  // Send data as JSON

    // Create JSON payload
    String jsonData = "{\"barcode\":\"" + barcode + "\"}";

    // Send the POST request
    int httpResponseCode = http.POST(jsonData);

    if (httpResponseCode > 0) {
      String response = http.getString();  // Get the response from the server
      Serial.println(httpResponseCode);  // Print the response code
      Serial.println(response);  // Print the server's response
    } else {
      Serial.println("Error sending POST");
    }
    
    http.end();  // End the HTTP connection
  }
}

void loop() {
  // Put your main code here, to run repeatedly:
}
