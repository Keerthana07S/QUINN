//author: keerthana srinivasan
//date: June 17, 2025
//description: this code allows for classic quantum gates and identity matrices to be mapped as colors using NeoPixels after multiplication


#include <Adafruit_NeoPixel.h>
#include <Complex.h>

#define LED_PIN    12       
#define NUM_LEDS   16     

#define H_GATE_PIN 7
#define X_GATE_PIN 4
#define S_GATE_PIN 2
#define I_GATE_PIN 8

const Complex i(0, 1);

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

//note: initially, these matrices would be 2X2 matrices, but I used the Kronecker product to make them 4X4
//now, the activity is more fun! :)

//identity matrix (usually when there isn't a gate in the quantum circuit)
//we need to use Complex, because sometimes we deal with i which is sqrt(-1). 
Complex identity[4][4] = {
  {1, 0, 0, 0},
  {0, 1, 0, 0},
  {0, 0, 1, 0},
  {0, 0, 0, 1},
};

//hadamard matrix
Complex hadamard[4][4] = {
  {1, 1, 1, 1},
  {1, -1, 1, -1},
  {1, 1, -1, -1},
  {1, -1, -1, 1},
};

//pauli-x gate
Complex xgate[4][4] = {
  {0, 0, 0, 1},
  {0, 0, 1, 0},
  {0, 1, 0, 0},
  {1, 0, 0, 0},
};

//s gate
Complex sgate[4][4] = {
  {1, 0, 0, 0},
  {0, i, 0, 0},
  {0, 0, i, 0},
  {0, 0, 0, i**2},
};

int result[4][4];

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
        strip.setPixelColor(ledIndex, strip.Color(230, 150, 222)); //pink
      } else if (val == -1) {
        strip.setPixelColor(ledIndex, strip.Color(177, 150, 230)); //purple
      } else {
        strip.setPixelColor(ledIndex, strip.Color(150, 182, 230)); //blue
      }
      ledIndex++;
    }
  }

  strip.show();
  delay(250)
}

void copyMatrix(int source[4][4], int dest[4][4]) {
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      dest[i][j] = source[i][j];
}

void multiplyMatrix(int gate[4][4], int mat[4][4]) {
  int temp[4][4] = {0};
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      for (int k = 0; k < 4; k++) {
        temp[i][j] += gate[i][k] * mat[k][j];
      }
    }
  }

  //
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      mat[i][j] = temp[i][j];
}
