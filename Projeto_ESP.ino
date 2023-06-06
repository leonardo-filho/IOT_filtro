#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "la net de la muerte again";
const char* password = "draven123";
const char* mqtt_server = "broker.hivmq.com"; //BROKER MQTT
const int MSG_BUFFER_SIZE = 128; // Tamanho máximo do buffer de mensagem
char msg[MSG_BUFFER_SIZE]; // Declaração do buffer de mensagem
int LED = 5;

WiFiClient espClient;
PubSubClient client(espClient);



void setup_wifi() {
  delay(10);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");

  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP Adress: ");
  Serial.println(WiFi.localIP());

}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
	  Serial.print((char)payload[i]);
  }
  Serial.println();

  if ((char)payload[0] == 'L') {
	  digitalWrite(LED, HIGH);
	  sprintf (msg, "A Bomba esta ligada");
	  Serial.print("Publica mensagem: ");
	  Serial.println(msg);
	  client.publish("engeasier/led", msg);
  }

  if((char)payload[0] == 'l') {
    digitalWrite(LED, LOW);
    sprintf (msg, "A Bomba esta desligada");
    Serial.print("Publica mensagem: ");
    Serial.println(msg);
    client.publish("engeasier/led", msg);
    }
}


void reconnect(){
  while (!client.connected()) {
 Serial.print("Tentando conectar com o MQTT...");
  String clientID = "Projeto_MQTT";
  clientID += String(random(0xffff), HEX);
  // Attempt to connect
  if (client.connect(clientID.c_str())) {
   Serial.println("Conectado");

   client.subscribe("engeasier/publisher");
  } else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println(" try again in 5 seconds");
    // Wait 5 seconds before retrying
    delay(5000);
  }
 }
}


void setup() {
   LED = 5;

  pinMode(LED, OUTPUT);

  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);  
  client.setCallback(callback);

}

void loop() {
  if (!client.connected()) {
	  reconnect();
  }
 client.loop();

}
