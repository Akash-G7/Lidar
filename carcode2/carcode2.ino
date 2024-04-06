#define BUFFER_SIZE 512
// #include <cvzone.h>

// SerialData serialData(1,1);
int valsRec[1];
int left = 3;
int right = 4;
int forward = 5;
int backward = 6;
int analogPin = A7;
int steerAngle = 0;

// Create a buffer to hold the incoming communication
// Note: Most commands should be less than 20 characters long so picking a
// larger number just to be safe.
char buffer[BUFFER_SIZE];
int current_index = 0;

// Setup a couple of booleans to show what state we are in
bool start_character_detected = false;
bool end_character_detected = false;

void setup() {
//  serialData.begin();
  Serial.begin(9600);
  pinMode(left, OUTPUT); 
  pinMode(right, OUTPUT); 
  pinMode(forward, OUTPUT); 
  pinMode(backward, OUTPUT); 
}

void update_buffer() {
  // If we have a character ready to be read the process it.
  if (Serial.available()) {
    char c = Serial.read();

    // If we haven't started reading a command yet
    if (start_character_detected == false) {
      // If we have received a start character then we record the fact
      if (c == '<') {
        start_character_detected = true;
      }
    }

    // If we have started the command but not detected a end character yet
    // this means that we are in the middle of reading a command
    if (start_character_detected == true && end_character_detected == false) {
      // If we have received an end character then we can record the fact
      if (c == '>') {
        end_character_detected = true;
      }
    }

    // If we have received a start character then we need to keep track of
    // all the characters we have received so far 
    if (start_character_detected) {
      buffer[current_index] = c;
      current_index++;
      // If we have gone over 500 characters then we reset the index back to the start
      // to stop any chance of a buffer override error.
      if (current_index > 500) {
        current_index = 0;
      }
      // Mark the end of the string with a NUL character
      buffer[current_index] = '\0';
    }
  }
}

void process_command() {
  // Send back a identifier string when requested 
  if (strcmp("<w>", buffer) == 0) {
    digitalWrite(backward, LOW);
    delay(500);
    digitalWrite(forward, HIGH);
    Serial.println("<forward>");
  }  
  else if (strcmp("<w->", buffer) == 0) {
    digitalWrite(forward, LOW);
  }
  else if (strcmp("<s>", buffer) == 0) {
    digitalWrite(forward, LOW);
    delay(500);
    digitalWrite(backward, HIGH);
    Serial.println("<backwards>");
  }
  else if (strcmp("<s->", buffer) == 0) {
    digitalWrite(backward, LOW);
  }
  else if (strcmp("<a>", buffer) == 0) {
    digitalWrite(left, HIGH);
    delay(5000);
    digitalWrite(left, LOW);
    Serial.println("<left>");
  }
  else if (strcmp("<d>", buffer) == 0) {
    digitalWrite(right, HIGH);
    delay(5000);
    digitalWrite(right, LOW);
    Serial.println("<right>");
  }
  else if (strcmp("<r>", buffer) == 0) {
    straighten(true);
    delay(9500);
    straighten(false);
    Serial.println("<straight>");
    }
}

void straighten(bool x){
  while (x == true){
    steerAngle = analogRead(A7);  // read the input pin
    Serial.println(steerAngle);
    if (steerAngle < 460){
     // Serial.println("Right");  
      digitalWrite(left, HIGH); //Turn Left
      delay(150);
      digitalWrite(left, LOW); //Stop Turn Left
      delay(100);
    }else if(steerAngle > 500){
    //  Serial.println("Left");
      digitalWrite(right, HIGH); //Turn Right
      delay(150);
      digitalWrite(right, LOW); //Stop Turn Right
      delay(100);
    }else{
     // Serial.println("Straight");
      x = false;
    }
  }
}

void loop() {
  // Process the incoming buffer 
  update_buffer();
	// serialData.Get(valsRec);
  // Serial.println(valsRec[0]);

  
 
  // If we have received a valid command string structure, that is
  // we have received a start character, zero of more other characters and then
  // an end character.
  if (start_character_detected == true && end_character_detected == true) {
    // Process the received command
    process_command();

    // Reset the buffer and start/end flags.
    start_character_detected = false;
    end_character_detected = false;
    current_index = 0;
  }
}
