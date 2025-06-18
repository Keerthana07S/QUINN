#include <Adafruit_NeoPixel.h>

#define LED_PIN    6       
#define NUM_LEDS   9     

#define H_GATE_PIN 2
#define X_GATE_PIN 3
#define S_GATE_PIN 4
#define I_GATE_PIN 5

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

//identity matrix (usually when there isn't a gate in the quantum circuit)
int identity[3][3] = {
  {1, 0, 0},
  {0, 1, 0},
  {0, 0, 1}
};

//hadamard matrix
int hadamard[3][3] = {
  {1, 1, 0},
  {1, -1, 0},
  {0, 0, 1}
};

//x gate
int xgate[3][3] = {
  {0, 1, 0},
  {1, 0, 0},
  {0, 0, 1}
};

//s gate
int sgate[3][3] = {
  {1, 0, 0},
  {0, 0, -1},
  {0, 1, 0}
};

int result[3][3];

void setup() {
  strip.begin();
  strip.show(); //all pixels start as "off"

  pinMode(H_GATE_PIN, INPUT);
  pinMode(X_GATE_PIN, INPUT);
  pinMode(S_GATE_PIN, INPUT);
  pinMode(I_GATE_PIN, INPUT);

  Serial.begin(9600);
}

void loop() {
  bool useH = digitalRead(H_GATE_PIN);
  bool useX = digitalRead(X_GATE_PIN);
  bool useS = digitalRead(S_GATE_PIN);
  bool useI = digitalRead(I_GATE_PIN);

  //start with identity
  copyMatrix(identity, result);

  if (useH) multiplyMatrix(hadamard, result);
  if (useX) multiplyMatrix(xgate, result);
  if (useS) multiplyMatrix(sgate, result);
  if (useI) multiplyMatrix(identity, result);  

  //map matrix values to LEDs
  int ledIndex = 0;
  for (int row = 0; row < 3; row++) {
    for (int col = 0; col < 3; col++) {
      int val = result[row][col];
      if (val == 1) {
        strip.setPixelColor(ledIndex, strip.Color(0, 255, 0)); //green
      } else if (val == -1) {
        strip.setPixelColor(ledIndex, strip.Color(255, 0, 0)); //red
      } else {
        strip.setPixelColor(ledIndex, strip.Color(0, 0, 255)); //blue
      }
      ledIndex++;
    }
  }

  strip.show();
  delay(250)
}

void copyMatrix(int source[3][3], int dest[3][3]) {
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      dest[i][j] = source[i][j];
}

void multiplyMatrix(int gate[3][3], int mat[3][3]) {
  int temp[3][3] = {0};
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      for (int k = 0; k < 3; k++) {
        temp[i][j] += gate[i][k] * mat[k][j];
      }
    }
  }

  //
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      mat[i][j] = temp[i][j];
}
