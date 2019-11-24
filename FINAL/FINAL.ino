#define Trig 6
#define Echo 5

long duration;
int distance;

const int A1A = 9;
const int A1B = 8;

const int B1A = 13;
const int B1B = 12;

const int maxRange = 150;
const float maxEdge = 1.0;

const int QRD1114_PIN = A0;

void setup() {
  
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);

//  digitalWrite(B1A, LOW);
//  digitalWrite(B1B, LOW);
//  
//  digitalWrite(A1A, LOW);
//  digitalWrite(A1B, LOW);
  
  pinMode(B1A, OUTPUT);
  pinMode(B1B, OUTPUT);
  
  pinMode(A1A, OUTPUT);
  pinMode(A1B, OUTPUT);

  pinMode(QRD1114_PIN, INPUT);
  
  Serial.begin(9600);

  delay(5000);
}

void loop() {

  int range = trueRange();
  float edge = 5.0;
  forwards();
  
  while (edge > maxEdge){       // if ring is not detected
  
    while (range <= maxRange || range > 2000){  // if other sumobot is detected
      
      Serial.println("in range");
      Serial.println(range);
      range = trueRange();
      edge = trueEdge();
      Serial.println("No edge detected");
      Serial.println(edge);
      if (edge < maxEdge) break;
      delay(500);
    }

    if (edge < maxEdge) break;
    spin();
  
    while (range > maxRange){   // if other sumobot isnt detected
      Serial.println("out of range");
      Serial.println(range);
      range = trueRange();
      edge = trueEdge();
      Serial.println("No edge detected");
      Serial.println(edge);
      if (edge < maxEdge) break;
      delay(500);
    }

    if (edge < maxEdge) break;
    forwards();
    
  }

  if (edge < maxEdge) {
    Serial.println("Edge detected");
    Serial.println(edge);
    reverse();
    delay(1200);
    spin();
    delay(2800);
    no();
  }
  
}

int trueRange() { // finds mode range from sensor
  
  int range = 0;
  int goodRange = 0; // number of ranges that match the found range
  
  while (goodRange < 15){

    range = findRange();
    goodRange = 0;
    int upper = range + 5;
    int lower = range - 5;
  
    for (int i = 0; i < 30; i++){
      int compRange = findRange();
      if (lower <= compRange & compRange <= upper) goodRange++;
    }
  }
  return range;
}

int trueEdge() { // finds mode range from sensor
  
  float edge;
  int goodEdge = 0; // number of ranges that match the found range
  
  while (goodEdge < 8){

    edge = findEdge();
    goodEdge = 0;
    float upper = edge + 5.0;
    float lower = edge - 5.0;
  
    for (int i = 0; i < 15; i++){
      int compEdge = findEdge();
      if (lower <= compEdge & compEdge <= upper) goodEdge++;
    }
  }
  return edge;
}

int findRange() {
  
  digitalWrite(Trig, LOW);
  delayMicroseconds(20);

  digitalWrite (Trig, HIGH);
  delayMicroseconds(100);
  
  digitalWrite (Trig, LOW);
  duration = pulseIn(Echo, HIGH);
  distance = duration * 0.034 / 2;
  
  delayMicroseconds(20);
  return distance;
}

int findEdge() {
  
  int proximityADC = analogRead(QRD1114_PIN);
  float proximityV = (float)proximityADC * 5.0 / 1023.0;
  return proximityV;
}

void no(){

  digitalWrite(B1A, LOW);
  digitalWrite(B1B, LOW);
  digitalWrite(A1A, LOW);
  digitalWrite(A1B, LOW);
  
}

void forwards() {

  no();
  
  delay(500);
  
  digitalWrite(B1A, LOW);
  digitalWrite(B1B, HIGH);

  digitalWrite(A1A, HIGH);
  digitalWrite(A1B, LOW);
  
  Serial.println("GOGOGOGOGO");
}

void spin() {
  
  no();

  delay(500);
  
  digitalWrite(B1A, HIGH);
  digitalWrite(B1B, LOW);
  
  digitalWrite(A1A, HIGH);
  digitalWrite(A1B, LOW);

  Serial.println("SPIN SPIN SPIN");
}

void reverse() {

  no();

  delay(500);
  
  digitalWrite(B1A, HIGH);
  digitalWrite(B1B, LOW);

  digitalWrite(A1A, LOW);
  digitalWrite(A1B, HIGH);

  Serial.println("bakcbakcbakck");
}
