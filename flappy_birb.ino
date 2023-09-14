#include <SparkFunColorLCDShield.h>

LCDShield lcd;

// Define the positions and dimensions of three obstacles as rectangles
int obs[3][4] = {{0, 40, 50, 60}, {80, 80, 140, 100}, {0, 120, 20, 140}};

// Bird's initial position
int bird[2] = {50, 20};
int button = 5;

void setup() {
  pinMode(button, INPUT);  // Set the button as an input
  digitalWrite(button, HIGH); 
  randomSeed(analogRead(0));  // Seed the random number generator with an analog value
  lcd.init(PHILIPS);  // Initialize the LCD with the PHILIPS driver, try PHILIPS,1 if your display is inverted or EPSON for the EPSON driver
  lcd.contrast(-63);  // Set the LCD contrast
  lcd.clear(BLACK);  // Clear the LCD screen with a black background
}

int i = 0;
int j = 0;
int speed = 20;
int birdjump = -2;
int gameover = 0;
int start = 0;
int score = 0;
char scoreChar[10];

void loop() {
  while (!start) {
    lcd.setStr("Flappy", 40, 40, WHITE, GREEN);
    lcd.setStr("Birb", 60, 50, WHITE, GREEN);
    if (!digitalRead(button)) {
      start = 1;
    }
  }
  if (gameover) {
    lcd.setStr("Gameover", 40, 30, WHITE, GREEN);
    lcd.setStr("Score", 60, 30, WHITE, GREEN);
    itoa(score, scoreChar, 10);  // Convert the score to a string
    lcd.setStr(scoreChar, 80, 50, WHITE, GREEN);  // Display the score as a string
    if (!digitalRead(button)) {  // If the button is pressed, reset the game
      start = 0;
      gameover = 0;
      score = 0;
    }
  } else {
    runGame();
    lcd.setCircle(bird[0], bird[1], 5, WHITE, 5);  // Draw the bird as a white circle
  }
  lcd.clear(BLACK);  // Clear the screen with a black background
}

void runGame() {
  for (i = 0; i < 3; i++) {
    obstacles(obs[i][0], obs[i][1], obs[i][2], obs[i][3]);  // Draw obstacles
    collision(obs[i][0], obs[i][1], obs[i][2], obs[i][3]);  // Check for collision with obstacles
    obs[i][1] -= speed;  // Move obstacles 
    obs[i][3] -= speed;
    if (obs[i][3] < 0) {
      obs[i][1] = 110;  // Reset the obstacle's position if it goes off-screen
      obs[i][3] = 130;
      random(i);  // Generate a random obstacle
      score += 1;  // Increment the score
    }
  }

  bird[0] -= birdjump;  // Move the bird downwards
  if (birdjump > -4) {
    birdjump -= 1;  // Apply gravity to the bird
  }

  if (!digitalRead(button)) {
    birdjump += 7;  // Jump when the button is pressed
  }

  if (bird[0] < 10) {
    bird[0] = 10;  // Keep the bird within the screen boundaries
    birdjump = 0;
  }
  if (bird[0] > 100) {
    bird[0] = 100;
    birdjump = 0;
  }
}

void obstacles(int x1, int y1, int x2, int y2) {
  lcd.setRect(x1, y1, x2, y2, 1, WHITE);  // Draw a rectangular obstacle
}

int pos = 0;

void random(int a) {
  pos = random(0, 2);
  if (pos == 1) {
    obs[a][0] = 0;
    obs[a][2] = random(40, 60);
  } else {
    obs[a][0] = random(80, 100);
    obs[a][2] = 140;
  }
}

void collision(int x1, int y1, int x2, int y2) {
  if (bird[0] > x1 && bird[0] < x2) {
    if (bird[1] > y1 && bird[1] < y2) {
      gameover = 1;  // Set the gameover flag if there's a collision
    }
  }
}
