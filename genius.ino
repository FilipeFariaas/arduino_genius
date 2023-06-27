// ----- LEDS -----

#define LED_BLUE 2
#define LED_RED 3
#define LED_YELLOW 4
#define LED_GREEN 5

// ----- BUTTONS -----

#define BUTTON_LED_BLUE 8
#define BUTTON_LED_YELLOW 9
#define BUTTON_LED_RED 10
#define BUTTON_LED_GREEN 11

#define BUTTON_RESTART 12

// ----- SYSTEM -----

#define UNDEFINED -1
#define DELAY 300

// ----- GAME LENGTH -----

#define SEQUENCE 20

// ----- VARIABLES -----

bool running = true;
int ledsSequence[SEQUENCE];
int gameTurn = 0;
int correctAnswers = 0;
int lastTurn = SEQUENCE + 1;

// ----- GAME STATES -----

enum States {
  NEXT_ROUND_READY,
  USER_TURN,
  GAME_COMPLETED,
  GAME_OVER
};

// ----- FUNCTIONS -----

void startPorts() {
  
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  pinMode(BUTTON_LED_BLUE, INPUT_PULLUP);
  pinMode(BUTTON_LED_YELLOW, INPUT_PULLUP);
  pinMode(BUTTON_LED_RED, INPUT_PULLUP);
  pinMode(BUTTON_LED_GREEN, INPUT_PULLUP);
  
  pinMode(BUTTON_RESTART, INPUT_PULLUP);
  
}

int pickColor() {
  
  return random(2, 6);
  
}

void startGame() {
  
  int game = analogRead(0);
  randomSeed(game);

  for (int i = 0; i < SEQUENCE; i++) {
    
    ledsSequence[i] = pickColor();
    
  }
  
}

void prepareNextTurn() {
  
  gameTurn++;
  correctAnswers = 0;
  
  if (gameTurn <= SEQUENCE) {
    
    lightTurnLeds();
    
  }
  
}

void getUserAnswer() {
  
  int answer = verifyUserAnswer();

  if(answer == UNDEFINED) {
    
    return;
    
  } else if(answer == ledsSequence[correctAnswers]) {
    
    correctAnswers++;
    
  } else {
    
    running = false;
    
  }
  
}

int setState() {
  
  if(gameTurn <= SEQUENCE) {
    
    if(correctAnswers == gameTurn) {
      
      return NEXT_ROUND_READY; 
      
    } else {
      
      return USER_TURN;
      
    }
    
  } else if (gameTurn == lastTurn) {
    
    return GAME_COMPLETED;
    
  } else {
    
    return GAME_OVER;
    
  }
  
}

int ledBlinker(int ledPort) {

  digitalWrite(ledPort,HIGH);
  delay(DELAY);
  digitalWrite(ledPort,LOW);
  delay(DELAY);

  return ledPort;
  
}

void lightTurnLeds() {
  
  for (int i = 0; i < gameTurn; i++) {
    
    ledBlinker(ledsSequence[i]);
    
  }
  
}

int verifyUserAnswer() {
  
  if (digitalRead(BUTTON_LED_BLUE) == LOW) {
    
    return ledBlinker(LED_BLUE);
    
  } else if (digitalRead(BUTTON_LED_YELLOW) == LOW) {
    
    return ledBlinker(LED_RED);
    
  } else if (digitalRead(BUTTON_LED_RED) == LOW) {
    
    return ledBlinker(LED_YELLOW);
    
  } else if (digitalRead(BUTTON_LED_GREEN) == LOW) {
    
    return ledBlinker(LED_GREEN);
    
  } else {
   
    return UNDEFINED;
    
  }
  
}

void restart() {
  
  turnLedsOff();
  gameTurn = 0;
  running = true;
  
  delay(2000);
  
  startGame();
  prepareNextTurn();
  
}

void turnLedsOff() {
  
  digitalWrite(LED_BLUE,LOW);
  digitalWrite(LED_RED,LOW);
  digitalWrite(LED_YELLOW,LOW);
  digitalWrite(LED_GREEN,LOW);
  
}

void gameCompleted() {
  
  ledBlinker(LED_BLUE);
  ledBlinker(LED_RED);
  ledBlinker(LED_YELLOW);
  ledBlinker(LED_GREEN);
  ledBlinker(LED_YELLOW);
  ledBlinker(LED_RED);
  
  if(digitalRead(BUTTON_RESTART) == LOW) {
    
    Serial.println("game restarted from success");
	restart();
    
  }

}

void gameOver() {
  
  digitalWrite(LED_BLUE,HIGH);
  digitalWrite(LED_RED,HIGH);
  digitalWrite(LED_YELLOW,HIGH);
  digitalWrite(LED_GREEN,HIGH);
  delay(DELAY);
  digitalWrite(LED_BLUE,LOW);
  digitalWrite(LED_RED,LOW);
  digitalWrite(LED_YELLOW,LOW);
  digitalWrite(LED_GREEN,LOW);
  delay(DELAY);
  
  if(digitalRead(BUTTON_RESTART) == LOW) {
    
    Serial.println("game restarted from fail");
	restart();
    
  }
  
}

// ----- GAME -----

void setup() {
  
  Serial.begin(9600);
  
  startPorts();
  startGame();
  
}

void loop() {
  
  if(running == true){
    
    switch (setState()) {
      
      case NEXT_ROUND_READY:
        Serial.println("round ready");
        prepareNextTurn();
        break;
      
      case USER_TURN:
        Serial.println("user turn");
        getUserAnswer();
        break;
      
      case GAME_COMPLETED:
        Serial.println("game completed");
        gameCompleted();
        break;
      
      case GAME_OVER:
        Serial.println("gameover");
        gameOver();
        break;
    }
    
    delay(DELAY);
    
  } else {
    
  	gameOver();
    
  }
  
}
