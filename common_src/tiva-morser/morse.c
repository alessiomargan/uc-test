#include "morse.h"

// state machinery to morse strings

char *ALPHABET[26] = {
 ".-", // A
 "-...", // B
 "-.-.", // C
 "-..", // D
 ".", // E
 "..-.", // F
 "--.", // G
 "....", // H
 "..", // I
 ".---", // J
 "-.-", // K
 ".-..", // L
 "--", // M
 "-.", // N
 "---", // O
 ".--.", // P
 "--.-", // Q
 "-.-", // R
 "...", // S
 "-", // T
 "..-", // U
 "...-", // V 
 ".--", // W 
 "-..-", // X
 "-.--", // Y 
 "--.." // Z
};

// godawful..
uint8_t lengths[26] = { 
  2,
  4,
  4,
  3,
  1,
  4,
  3,
  4,
  2,
  4,
  3,
  4,
  2,
  2,
  3,
  4,
  4,
  3,
  3,
  1,
  3,
  4,
  3,
  4,
  4,
  4
};

static uint8_t char_to_index(char c) {
  return (c > 96 ? c-97 : c-65);
}

void next_symbol(struct morser *morser) {
  uint8_t cch = char_to_index(morser->string[morser->string_pos]);
  uint8_t cpos = morser->char_pos; 
  switch(ALPHABET[cch][cpos]) {
    case '.':
      morser->tick_count = 1;
      return;
    case '-':
      morser->tick_count = 3;
      return;
  };
}

enum NEXT {
  CHAR,
  SPACE,
  EOS
}; 

enum NEXT next_char(struct morser *morser) { 
  morser->string_pos++;
  char cur = morser->string[morser->string_pos];
  if (cur == '\0') {
    return EOS;
  } else if (cur == ' ') {
    return SPACE;
  } else {
    morser->char_pos = 0;
    uint8_t cch = char_to_index(morser->string[morser->string_pos]);
    morser->char_length = lengths[cch]; 
    //morser->char_length = strlen(ALPHABET[cch]);
    next_symbol(morser);
    return CHAR;
  };
}

enum OUTPUT tick(struct morser *morser) { 
  morser->tick_count = morser->tick_count-1;
  switch(morser->tick_count) {
    case -1:
      morser->char_pos++; 
      if(morser->char_pos >= morser->char_length) {
        switch(morser->state) {
          case SPACING: // inter-character spacing done;
            switch(next_char(morser)) {
              case CHAR:
                morser->state = TICKING;
                morser->tick_count--;
                return HIGH;
              case SPACE:
                morser->tick_count = 6; // 7 dot blank after each word
                morser->state = SPACING;
                return LOW;
              case EOS:
                morser->tick_count = 6;
                morser->state = ENDING;
                return LOW;
            }
          case TICKING:  // character done, insert blanks
            morser->state = SPACING;
            morser->tick_count = 2; // 3 dots to a space, minus the one on the next line
            return LOW;
          case ENDING:
            return END;
        };
      } else {
        next_symbol(morser);
        return LOW; // insert single blank dot between symbols
      };
    default:
      switch(morser->state) {
        case ENDING:
          return LOW;
        case SPACING:
          return LOW;
        case TICKING:
          return HIGH;
      };
  };
  return LOW;
}

void init_morser(struct morser *m, char *str) { 
  m->state = TICKING;
  m->string = str;
  m->string_pos = 255;
  next_char(m); 
}
