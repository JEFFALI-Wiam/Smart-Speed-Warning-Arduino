#define trigPin 9
#define echoPin 10

#define ledVert 5
#define ledJaune 6
#define ledRouge 7
#define buzzer 8

float lastDistance = 0;
unsigned long lastTime = 0;

float getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 30000);
  if (duration == 0) return -1;
  return duration * 0.034 / 2;
}

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(ledVert, OUTPUT);
  pinMode(ledJaune, OUTPUT);
  pinMode(ledRouge, OUTPUT);
  pinMode(buzzer, OUTPUT);

  Serial.begin(9600);  // TX/RX

  lastDistance = getDistance();
  lastTime = millis();
}

void loop() {
  delay(100); 

  float distance = getDistance();
  if (distance < 0) return; // no echo

  unsigned long now = millis();
  float dt = (now - lastTime) / 1000.0;
  float dd = lastDistance - distance;

  
  if (abs(dd) < 1) {
    lastDistance = distance;
    lastTime = now;
    return;
  }

  float speed = abs(dd / dt) * 0.036; // km/h
  String etat;

  // Reset LEDs & buzzer
  digitalWrite(ledVert, LOW);
  digitalWrite(ledJaune, LOW);
  digitalWrite(ledRouge, LOW);
  digitalWrite(buzzer, LOW);

  // Affichage LED et buzzer selon vitesse
  if (speed < 30) {
    digitalWrite(ledVert, HIGH);
    digitalWrite(buzzer, LOW);
    etat = "VERT";
  } else if (speed < 40) {
    digitalWrite(ledJaune, HIGH);
    digitalWrite(buzzer, LOW);
    etat = "JAUNE";
  } else {
    digitalWrite(ledRouge, HIGH);
    digitalWrite(buzzer, HIGH); // buzzer ON
    etat = "ROUGE";
  }

  // Debug Serial
  Serial.print("Distance=");
  Serial.print(distance);
  Serial.print(" cm | Vitesse=");
  Serial.print(speed);
  Serial.print(" km/h | Etat=");
  Serial.println(etat);

  // Envoyer au Raspberry
  Serial.print(speed);
  Serial.print(",");
  Serial.println(etat);

  lastDistance = distance;
  lastTime = now;
}

