#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

const char *ssid = "DeneyapKart1Av2RGBLed";
const char *password = "";

WiFiServer server(80);

// Renklerin RGB kodları
struct Color {
  int r, g, b;
};

Color colors[] = {
    {255, 0, 0},    // Kırmızı
    {0, 255, 0},    // Yeşil
    {0, 0, 255},    // Mavi
    {255, 255, 0},  // Sarı
    {255, 165, 0},  // Turuncu
    {255, 255, 255}, // Beyaz
    {0, 0, 0} // Kapalı
};

void setup() {
  pinMode(RGBLED, OUTPUT);
  neopixelWrite(RGBLED, 0, 0, 0);  // LED'i kapat

  Serial.begin(115200);
  Serial.println();
  Serial.println("Erişim Noktası (AP) konfigure ediliyor...");

  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("Erişim noktası IP adresi: ");
  Serial.println(myIP);
  server.begin();
  Serial.println("Server başlatıldı");
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("Yeni istemci talebi geldi.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.print("<html><head><style>");
            client.print("body { font-family: Arial, sans-serif; text-align: center; background-color: #f3f3f3; }");
            client.print("h1 { color: #333; }");
            client.print("button { width: 200px; height: 60px; font-size: 20px; margin: 10px; border-radius: 10px; border: none; }");
            client.print(".on { background-color: #4CAF50; color: white; }");
            client.print(".off { background-color: #f44336; color: white; }");
            client.print("</style></head><body>");
            client.print("<h1 style='color:blue;'>Deneyap Kart 1Av2 RGB LED Control</h1>");
            client.print("<button style='background-color:red;width:200px;height:100px;' onclick=\"location.href='/color?red'\">Red</button>");
            client.print("<button style='background-color:green;width:200px;height:100px;' onclick=\"location.href='/color?green'\">Green</button><br>");
            client.print("<button style='background-color:blue;width:200px;height:100px;' onclick=\"location.href='/color?blue'\">Blue</button>");
            client.print("<button style='background-color:yellow;width:200px;height:100px;' onclick=\"location.href='/color?yellow'\">Yellow</button><br>");
            client.print("<button style='background-color:orange;width:200px;height:100px;' onclick=\"location.href='/color?orange'\">Orange</button>");
            client.print("<button style='background-color:white;width:200px;height:100px;' onclick=\"location.href='/color?white'\">White</button><br>");
            client.print("<button style='background-color:white;width:200px;height:100px;' onclick=\"location.href='/color?black'\">Off</button>");
            client.print("</body></html>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }

        if (currentLine.endsWith("GET /color?red")) {
          setLEDColor(colors[0]);
        } else if (currentLine.endsWith("GET /color?green")) {
          setLEDColor(colors[1]);
        } else if (currentLine.endsWith("GET /color?blue")) {
          setLEDColor(colors[2]);
        } else if (currentLine.endsWith("GET /color?yellow")) {
          setLEDColor(colors[3]);
        } else if (currentLine.endsWith("GET /color?orange")) {
          setLEDColor(colors[4]);
        } else if (currentLine.endsWith("GET /color?white")) {
          setLEDColor(colors[5]);
        } else if (currentLine.endsWith("GET /color?black")) {
          setLEDColor(colors[6]);
        }
      }
    }
    client.stop();
    Serial.println("Client Disconnected.");
  }
}

void setLEDColor(Color color) {
  neopixelWrite(RGBLED, color.r, color.g, color.b);
  Serial.print("LED Renk Ayarlandı: R=");
  Serial.print(color.r);
  Serial.print(", G=");
  Serial.print(color.g);
  Serial.print(", B=");
  Serial.println(color.b);
}
