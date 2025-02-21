#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// Configuración de Wi-Fi
const char* ssid = "you-wifiSSID";       // Reemplaza con tu red Wi-Fi EDITAR
const char* password = "youPassword"; // Reemplaza con tu contraseña EDITAR

// Configuración de Telegram
const char* botToken = "youApiToken";  // Token del bot de Telegram EDITAR
const String chatID = "youChatID";        // ID del chat de Telegram EDITAR

// Configuración del LED
const int ledPin = 2;  // GPIO donde está conectado el LED

// Cliente seguro y bot de Telegram
WiFiClientSecure client;
UniversalTelegramBot bot(botToken, client);

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);  // Asegurarse de que el LED esté apagado al inicio

  // Conectar a la red Wi-Fi
  Serial.println("Conectando a la red Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nConectado a Wi-Fi.");
  Serial.println("Dirección IP: " + WiFi.localIP().toString());

  // Configurar cliente seguro para Telegram
  client.setInsecure();

  // Mensaje inicial en Telegram
  bot.sendMessage(chatID, "🤖 ESP32 conectado correctamente. Dirección IP: " + WiFi.localIP().toString(), "");
}

void loop() {
  // Verificar si hay nuevos mensajes en Telegram
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

  for (int i = 0; i < numNewMessages; i++) {
    String text = bot.messages[i].text;
    String senderID = bot.messages[i].chat_id;

    // Verificar si el mensaje es del chat autorizado
    if (senderID == chatID) {
      if (text == "/encender") {
        digitalWrite(ledPin, HIGH);
        bot.sendMessage(chatID, "💡 LED encendido.", "");
      } 
      else if (text == "/apagar") {
        digitalWrite(ledPin, LOW);
        bot.sendMessage(chatID, "💡 LED apagado.", "");
      } 
      else {
        bot.sendMessage(chatID, "❓ Comando no reconocido. Usa /encender o /apagar.", "");
      }
    }
  }

  delay(1000);  // Verificar cada segundo
}
