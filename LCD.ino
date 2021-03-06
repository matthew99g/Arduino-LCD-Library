// Custom LCD Library Test - 4Bit Mode
// 16 x 2 LCD Based On HD44780

// UNRELEASED PRE_ALPHA TEST

// Matthew Geiger

// Define Pins
#define LCD_RS 2
#define LCD_RW 3
#define LCD_E  4
#define LCD_D0 5
#define LCD_D1 6
#define LCD_D2 7
#define LCD_D3 8
#define LCD_D4 9
#define LCD_D5 10
#define LCD_D6 11
#define LCD_D7 12

int LCD_POS = 0;

// Setup LCD
void SetupLCD() {
  // Set Pins for Output
  pinMode(LCD_RS, OUTPUT);
  pinMode(LCD_RW, OUTPUT);
  pinMode(LCD_E,  OUTPUT);
  pinMode(LCD_D0, OUTPUT);
  pinMode(LCD_D1, OUTPUT);
  pinMode(LCD_D2, OUTPUT);
  pinMode(LCD_D3, OUTPUT);
  pinMode(LCD_D4, OUTPUT);
  pinMode(LCD_D5, OUTPUT);
  pinMode(LCD_D6, OUTPUT);
  pinMode(LCD_D7, OUTPUT);
  
  // Run INIT
  InitLCD();
  
  // Give LCD Time To Catch Up
  delay(100);
}

void PullDownBus() {
  digitalWrite(LCD_D0, LOW);
  digitalWrite(LCD_D1, LOW);
  digitalWrite(LCD_D2, LOW);
  digitalWrite(LCD_D3, LOW);
  digitalWrite(LCD_D4, LOW);
  digitalWrite(LCD_D5, LOW);
  digitalWrite(LCD_D6, LOW);
  digitalWrite(LCD_D7, LOW);
}

// Send Character
void SendCharacter(char cCharacter) {
  LCD_POS++;
 SendCommand(byte(cCharacter), 0x1);
}

// Init LCD
void InitLCD() {
  // Start Communication Timing
  digitalWrite(LCD_E, LOW);
  digitalWrite(LCD_RW, LOW);
  
  // Run LCD 4Bit Init
  InitSequence();
} 

// LCD 4Bit Init
void InitSequence() {
 // LCD Init Commands for 4Bit Mode
 SendCommand(0b00110011, 0x0);
 SendCommand(0b00110010, 0x0);
 
 // Reset Screen and Cursor
 // Configure Dimensions
 SendCommand(0b00101100, 0x0);
 // Turn on Screen, Enable Cursor, and Blink Cursor
 SendCommand(0b00001111, 0x0);
 // Clear Display
 SendCommand(0b00000001, 0x0);
 //Set Print from Left To Write
 SendCommand(0b00000110, 0x0);
}

// Sends Write Command
void SendCommand(byte cCharacter, byte bBool) {
  digitalWrite(LCD_RW, LOW);
  
  // Select Register
  if(bBool) {
   digitalWrite(LCD_RS, HIGH);
  } else {
   digitalWrite(LCD_RS, LOW);
  }
  
  // Communication Timing
  delayMicroseconds(1);
 
  // Send First Section of ASCII
  if((cCharacter >> 4) & 1) {
   digitalWrite(LCD_D4, HIGH); 
  }else { 
   digitalWrite(LCD_D4, LOW);
  }
  
  if((cCharacter >> 5) & 1) {
   digitalWrite(LCD_D5, HIGH); 
  } else { 
   digitalWrite(LCD_D5, LOW);
  }
  
  if((cCharacter >> 6) & 1) {
   digitalWrite(LCD_D6, HIGH); 
  } else { 
   digitalWrite(LCD_D6, LOW);
  }
  
  if((cCharacter >> 7) & 1) {
   digitalWrite(LCD_D7, HIGH); 
  } else { 
   digitalWrite(LCD_D7, LOW);
  }
  
  // Pulse Enable
  ClockPulse();
  
  // Send Second Section of ASCII
  if((cCharacter >> 0) & 1) {
   digitalWrite(LCD_D4, HIGH); 
  } else { 
   digitalWrite(LCD_D4, LOW);
  }
  
  if((cCharacter >> 1) & 1) {
   digitalWrite(LCD_D5, HIGH); 
  } else { 
   digitalWrite(LCD_D5, LOW);
  }
  
  if((cCharacter >> 2) & 1) {
   digitalWrite(LCD_D6, HIGH); 
  } else { 
   digitalWrite(LCD_D6, LOW);
  }
  
  if((cCharacter >> 3) & 1) {
   digitalWrite(LCD_D7, HIGH); 
  } else { 
   digitalWrite(LCD_D7, LOW);
  }
  
  // Pulse Enable
  ClockPulse();
  
  // Pull Bus LOW
  PullDownBus();
}

// Pulse Enable
void ClockPulse() {
 delayMicroseconds(1);
 digitalWrite(LCD_E, HIGH);
 delayMicroseconds(1);
 digitalWrite(LCD_E, LOW);
 delayMicroseconds(1);
 
}

// Sends String
void SendPhrase(char *szPhrase) {
 // Send Characters
 for(int i = 0; i < strlen(szPhrase); i++) {
   SendCharacter(byte(szPhrase[i]));
 }
}

void SetCursor(byte choice) {
  if(choice == 0x0) {
    SendCommand(0b00001100, 0x0);
  } else if(choice == 0x1) {
    SendCommand(0b00001110, 0x0);
  } else {
    SendCommand(0b00001111, 0x0); 
  }
}

void ClearScreen() {
  SendCommand(0b00000001, 0x0);
  LCD_POS = 0;
}

void ReturnHome() {
 SendCommand(0b00000010, 0x0);

 delay(1); 
}

void ShiftDisplayDown() {
 for(int i = 0; i < LCD_POS; i++) {
  SendCommand(0b00010000, 0x0);
 } 
 
 for(int i = 0; i < 40; i++) {
  SendCommand(0b00010100, 0x0);
 }
 
 LCD_POS = 0;
}

void ShiftDisplayUp() {
  
 for(int i = 0; i < LCD_POS + 40; i++) {
  SendCommand(0b00010000, 0x0);
 } 
 
 LCD_POS = 0;
}

void ClearLine() {
 for(int i = 0; i < 16; i++) {
   SendCharacter(' ');
   delay(25);
 } 
}

void WipeScreen() {
 SetCursor(1);
  
 LCD_POS = 0;
 ReturnHome();
 
 ClearLine();
 
 ShiftDisplayDown();
 
 ClearLine();
 
 SetCursor(0);
 
 ClearScreen(); 
 
 delay(10);
}

void setup() {
  // Initialize LCD
  SetupLCD();
  
  // Stop Cursor
  SetCursor(0);
}
  
void loop() {
  for(int i = 0; i < 4; i++) {
   SendPhrase("Driver Test V0.2");
   delay(500);
   if(i < 3) {
    ClearScreen();
   }
   delay(100);
  }
  
  ShiftDisplayDown();
  SetCursor(2);
  delay(1500);
  
  for(int i = 0; i < 3; i++) {
   SendCharacter('T');
   delay(500);
   SendCharacter('E');
   delay(500);
   SendCharacter('S');
   delay(500);
   SendCharacter('T');
   delay(500);
   SendCharacter(' ');
   delay(500);
  }
  
  ShiftDisplayUp();
  
  for(int i = 0; i < 3; i++) {
   SendCharacter('T');
   delay(250);
   SendCharacter('E');
   delay(250);
   SendCharacter('S');
   delay(250);
   SendCharacter('T');
   delay(250);
   SendCharacter(' ');
   delay(250);
  }
  
  SendPhrase("  ");
  
  ShiftDisplayDown(); 
  
  for(int i = 0; i < 3; i++) {
   SendCharacter('C');
   delay(100);
   SendCharacter('O');
   delay(100);
   SendCharacter('O');
   delay(100);
   SendCharacter('L');
   delay(100);
   SendCharacter(' ');
   delay(100);
  }
  
  ShiftDisplayUp();
  
  for(int i = 0; i < 3; i++) {
   SendCharacter('C');
   delay(50);
   SendCharacter('O');
   delay(50);
   SendCharacter('O');
   delay(50);
   SendCharacter('L');
   delay(50);
   SendCharacter(' ');
   delay(50);
  }
  
  WipeScreen();
  delay(500);
  
  for(int i = 0; i < 4; i++) {
   SendPhrase("LCD Initialized!");
   ShiftDisplayDown();
  
   for(int i = 0; i < 4; i++) {
    // Move Cursor to Center Text
    SendCommand(0b00010100, 0x0);
   }
  
   SendPhrase("Have Fun");
  
   delay(750);
   if(i < 3) {
    ClearScreen();
   }
   delay(250);
  }
  
  WipeScreen();
}
