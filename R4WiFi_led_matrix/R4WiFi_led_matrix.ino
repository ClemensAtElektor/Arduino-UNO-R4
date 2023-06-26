/*
 * Purpose: Display a text message scrolling from right to left 
 *          on the board's 8x12 LED matrix.
 * Board: Arduino UNO R4 WiFi
 * IDE: 1.8.19 with "Arduino Renesas Boards" version 0.8.5-ea
 *
 * Usage: Upload to the board and enjoy.
 * 
 * Provides many possibilities for doing things better and more efficiently.
 * Parts of this code work because of the way the display works. For instance,
 * there is no need to draw black pixels, only white pixels need drawing.
 * Naively porting this code to another kind of display will probably not work.
 * Buffer overflow protection is probably flawed, so don't count on it.
 *
 * By: Clemens Valens, Elektor
 * Date: 14/6/2023
 */

// Leading spaces ensure starting at the right.
uint8_t banner_text[] = "   Arduino UNO R4 WiFi";

// First value is the width of a character in columns. This allows for
// easy tight spacing on the display (TTF kind of thing).
const uint8_t font_5x8[] = 
{
  3, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // space
  1, 0b01011111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // !
  3, 0b00000011, 0b00000000, 0b00000011, 0b00000000, 0b00000000, // "
  5, 0b00010100, 0b00111110, 0b00010100, 0b00111110, 0b00010100, // #
  4, 0b00100100, 0b01101010, 0b00101011, 0b00010010, 0b00000000, // $
  5, 0b01100011, 0b00010011, 0b00001000, 0b01100100, 0b01100011, // %
  5, 0b00110110, 0b01001001, 0b01010110, 0b00100000, 0b01010000, // &
  1, 0b00000011, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // '
  3, 0b00011100, 0b00100010, 0b01000001, 0b00000000, 0b00000000, // (
  3, 0b01000001, 0b00100010, 0b00011100, 0b00000000, 0b00000000, // )
  5, 0b00101000, 0b00011000, 0b00001110, 0b00011000, 0b00101000, // * 
  5, 0b00001000, 0b00001000, 0b00111110, 0b00001000, 0b00001000, // +
  2, 0b10110000, 0b01110000, 0b00000000, 0b00000000, 0b00000000, // ,
  4, 0b00001000, 0b00001000, 0b00001000, 0b00001000, 0b00000000, // -
  2, 0b01100000, 0b01100000, 0b00000000, 0b00000000, 0b00000000, // .
  4, 0b01100000, 0b00011000, 0b00000110, 0b00000001, 0b00000000, // /
  4, 0b00111110, 0b01000001, 0b01000001, 0b00111110, 0b00000000, // 0
  3, 0b01000010, 0b01111111, 0b01000000, 0b00000000, 0b00000000, // 1
  4, 0b01100010, 0b01010001, 0b01001001, 0b01000110, 0b00000000, // 2
  4, 0b00100010, 0b01000001, 0b01001001, 0b00110110, 0b00000000, // 3
  4, 0b00011000, 0b00010100, 0b00010010, 0b01111111, 0b00000000, // 4
  4, 0b00100111, 0b01000101, 0b01000101, 0b00111001, 0b00000000, // 5
  4, 0b00111110, 0b01001001, 0b01001001, 0b00110000, 0b00000000, // 6
  4, 0b01100001, 0b00010001, 0b00001001, 0b00000111, 0b00000000, // 7
  4, 0b00110110, 0b01001001, 0b01001001, 0b00110110, 0b00000000, // 8
  4, 0b00000110, 0b01001001, 0b01001001, 0b00111110, 0b00000000, // 9
  2, 0b01010000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // :
  2, 0b10000000, 0b01010000, 0b00000000, 0b00000000, 0b00000000, // ;
  3, 0b00010000, 0b00101000, 0b01000100, 0b00000000, 0b00000000, // <
  3, 0b00010100, 0b00010100, 0b00010100, 0b00000000, 0b00000000, // =
  3, 0b01000100, 0b00101000, 0b00010000, 0b00000000, 0b00000000, // >
  4, 0b00000010, 0b01011001, 0b00001001, 0b00000110, 0b00000000, // ?
  5, 0b00111110, 0b01001001, 0b01010101, 0b01011101, 0b00001110, // @
  4, 0b01111110, 0b00010001, 0b00010001, 0b01111110, 0b00000000, // A
  4, 0b01111111, 0b01001001, 0b01001001, 0b00110110, 0b00000000, // B
  4, 0b00111110, 0b01000001, 0b01000001, 0b00100010, 0b00000000, // C
  4, 0b01111111, 0b01000001, 0b01000001, 0b00111110, 0b00000000, // D
  4, 0b01111111, 0b01001001, 0b01001001, 0b01000001, 0b00000000, // E
  4, 0b01111111, 0b00001001, 0b00001001, 0b00000001, 0b00000000, // F
  4, 0b00111110, 0b01000001, 0b01001001, 0b01111010, 0b00000000, // G
  4, 0b01111111, 0b00001000, 0b00001000, 0b01111111, 0b00000000, // H
  3, 0b01000001, 0b01111111, 0b01000001, 0b00000000, 0b00000000, // I
  4, 0b00110000, 0b01000000, 0b01000001, 0b00111111, 0b00000000, // J
  4, 0b01111111, 0b00001000, 0b00010100, 0b01100011, 0b00000000, // K
  4, 0b01111111, 0b01000000, 0b01000000, 0b01000000, 0b00000000, // L
  5, 0b01111111, 0b00000010, 0b00001100, 0b00000010, 0b01111111, // M
  5, 0b01111111, 0b00000100, 0b00001000, 0b00010000, 0b01111111, // N
  4, 0b00111110, 0b01000001, 0b01000001, 0b00111110, 0b00000000, // O
  4, 0b01111111, 0b00001001, 0b00001001, 0b00000110, 0b00000000, // P
  4, 0b00111110, 0b01000001, 0b01000001, 0b10111110, 0b00000000, // Q
  4, 0b01111111, 0b00001001, 0b00001001, 0b01110110, 0b00000000, // R
  4, 0b01000110, 0b01001001, 0b01001001, 0b00110010, 0b00000000, // S
  5, 0b00000001, 0b00000001, 0b01111111, 0b00000001, 0b00000001, // T
  4, 0b00111111, 0b01000000, 0b01000000, 0b00111111, 0b00000000, // U
  5, 0b00001111, 0b00110000, 0b01000000, 0b00110000, 0b00001111, // V
  5, 0b00111111, 0b01000000, 0b00111000, 0b01000000, 0b00111111, // W
  5, 0b01100011, 0b00010100, 0b00001000, 0b00010100, 0b01100011, // X
  5, 0b00000111, 0b00001000, 0b01110000, 0b00001000, 0b00000111, // Y
  4, 0b01100001, 0b01010001, 0b01001001, 0b01000111, 0b00000000, // Z
  2, 0b01111111, 0b01000001, 0b00000000, 0b00000000, 0b00000000, // [
  4, 0b00000001, 0b00000110, 0b00011000, 0b01100000, 0b00000000, // '\'
  2, 0b01000001, 0b01111111, 0b00000000, 0b00000000, 0b00000000, // ]
  3, 0b00000010, 0b00000001, 0b00000010, 0b00000000, 0b00000000, // hat
  4, 0b01000000, 0b01000000, 0b01000000, 0b01000000, 0b00000000, // _
  2, 0b00000001, 0b00000010, 0b00000000, 0b00000000, 0b00000000, // `
  4, 0b00100000, 0b01010100, 0b01010100, 0b01111000, 0b00000000, // a
  4, 0b01111111, 0b01000100, 0b01000100, 0b00111000, 0b00000000, // b
  4, 0b00111000, 0b01000100, 0b01000100, 0b00000000, 0b00000000, // c
  4, 0b00111000, 0b01000100, 0b01000100, 0b01111111, 0b00000000, // d
  4, 0b00111000, 0b01010100, 0b01010100, 0b00011000, 0b00000000, // e
  3, 0b00000100, 0b01111110, 0b00000101, 0b00000000, 0b00000000, // f
  4, 0b10011000, 0b10100100, 0b10100100, 0b01111000, 0b00000000, // g
  4, 0b01111111, 0b00000100, 0b00000100, 0b01111000, 0b00000000, // h
  3, 0b01000100, 0b01111101, 0b01000000, 0b00000000, 0b00000000, // i
  4, 0b01000000, 0b10000000, 0b10000100, 0b01111101, 0b00000000, // j
  4, 0b01111111, 0b00010000, 0b00101000, 0b01000100, 0b00000000, // k
  3, 0b01000001, 0b01111111, 0b01000000, 0b00000000, 0b00000000, // l
  5, 0b01111100, 0b00000100, 0b01111100, 0b00000100, 0b01111000, // m
  4, 0b01111100, 0b00000100, 0b00000100, 0b01111000, 0b00000000, // n
  4, 0b00111000, 0b01000100, 0b01000100, 0b00111000, 0b00000000, // o
  4, 0b11111100, 0b00100100, 0b00100100, 0b00011000, 0b00000000, // p
  4, 0b00011000, 0b00100100, 0b00100100, 0b11111100, 0b00000000, // q
  4, 0b01111100, 0b00001000, 0b00000100, 0b00000100, 0b00000000, // r
  4, 0b01001000, 0b01010100, 0b01010100, 0b00100100, 0b00000000, // s
  3, 0b00000100, 0b00111111, 0b01000100, 0b00000000, 0b00000000, // t
  4, 0b00111100, 0b01000000, 0b01000000, 0b01111100, 0b00000000, // u
  5, 0b00011100, 0b00100000, 0b01000000, 0b00100000, 0b00011100, // v
  5, 0b00111100, 0b01000000, 0b00111100, 0b01000000, 0b00111100, // w
  5, 0b01000100, 0b00101000, 0b00010000, 0b00101000, 0b01000100, // x
  4, 0b10011100, 0b10100000, 0b10100000, 0b01111100, 0b00000000, // y
  3, 0b01100100, 0b01010100, 0b01001100, 0b00000000, 0b00000000, // z
  3, 0b00001000, 0b00110110, 0b01000001, 0b00000000, 0b00000000, // {
  1, 0b01111111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // |
  3, 0b01000001, 0b00110110, 0b00001000, 0b00000000, 0b00000000, // }
  4, 0b00001000, 0b00000100, 0b00001000, 0b00000100, 0b00000000, // ~
};

// LED matrix info.
const uint8_t led_matrix_pin_first = 28;
const uint8_t led_matrix_pin_last = 38;
const uint8_t led_matrix_pin_count = led_matrix_pin_last - led_matrix_pin_first + 1;
const uint8_t led_matrix_rows = 8;
const uint8_t led_matrix_cols = 12;

// Pixel-to-pin translation table.
// A HEX value encodes two pin numbers. The MSB is to be driven LOW,
// the LSB is to be driven HIGH.
// Example: pixel (4,2) contains the value 0x60, meaning that pin 6 must
// be driven low and pin 0 must be driven high to activate the pixel.
// The pin number is an offset to the constant led_matrix_pin_first
// Note that they all appear in pairs, so you could make the table 50%
// smaller at the cost of doing some swapping for odd or even columns.
// (0,0) is upper left corner when the board's USB connector points to the left.
const uint8_t led_matrix_pins[led_matrix_rows][led_matrix_cols] =
{
  //  0     1     2     3     4     5     6     7     8     9    10     11
  { 0x37, 0x73, 0x47, 0x74, 0x43, 0x34, 0x87, 0x78, 0x83, 0x38, 0x84, 0x48 }, // 0
  { 0x07, 0x70, 0x03, 0x30, 0x04, 0x40, 0x08, 0x80, 0x67, 0x76, 0x63, 0x36 }, // 1
  { 0x64, 0x46, 0x68, 0x86, 0x60, 0x06, 0x57, 0x75, 0x53, 0x35, 0x54, 0x45 }, // 2
  { 0x58, 0x85, 0x50, 0x05, 0x56, 0x65, 0x17, 0x71, 0x13, 0x31, 0x14, 0x41 }, // 3
  { 0x18, 0x81, 0x10, 0x01, 0x16, 0x61, 0x15, 0x51, 0x27, 0x72, 0x23, 0x32 }, // 4
  { 0x24, 0x42, 0x28, 0x82, 0x20, 0x02, 0x26, 0x62, 0x25, 0x52, 0x21, 0x12 }, // 5
  { 0xa7, 0x7a, 0xa3, 0x3a, 0xa4, 0x4a, 0xa8, 0x8a, 0xa0, 0x0a, 0xa6, 0x6a }, // 6
  { 0xa5, 0x5a, 0xa1, 0x1a, 0xa2, 0x2a, 0x97, 0x79, 0x93, 0x39, 0x94, 0x49 }, // 7
};

// Every byte represents a column of the LED matrix.
// Can hold 32 5x8-font characters.
// Buffer can be smaller at the price of more code.
uint8_t led_matrix_buffer[5*32];

// Activate the pixel at (x,y) for ontime microseconds.
void put_pixel(uint8_t x, uint8_t y, uint32_t ontime)
{
  uint8_t pins = led_matrix_pins[y][x];
  uint8_t l = (pins>>4) + led_matrix_pin_first;
  uint8_t h = (pins&0xf) + led_matrix_pin_first;
  pinMode(l,OUTPUT);
  digitalWrite(l,LOW);
  pinMode(h,OUTPUT);
  digitalWrite(h,HIGH);
  // If ontime = 0, pixel remains active until it is deactivated
  // by another put_pixel that happens to use the same pin(s).
  if (ontime!=0)
  {
    delayMicroseconds(ontime);
    pinMode(l,INPUT);
    pinMode(h,INPUT);
  }
}

// Call periodically at desired fps rate.
// ontime specifies how long a pixel remains on.
void led_matrix_buffer_show(uint32_t x_offset, uint32_t ontime)
{
  for (uint8_t i=0; i<led_matrix_cols; i++)
  {
    if (i+x_offset>=sizeof(led_matrix_buffer)) return;
    uint8_t col = led_matrix_buffer[i+x_offset];
    for (uint8_t row=0; row<led_matrix_rows; row++)
    {
      if ((col&0x01)!=0)
      {
        put_pixel(i,row,ontime);
      }
      col >>= 1;
    }
  }
}

// Write a character to the buffer.
uint8_t led_matrix_putch(uint8_t *p_buffer, uint16_t buffer_size, uint8_t ch)
{
  uint8_t i;
  
  if (ch<' ') return 0;
  ch -= ' ';
  uint16_t offset = 6*ch;
  uint8_t width = font_5x8[offset];
  for (i=0; i<width; i++) 
  { 
    offset += 1;
    // This is supposed to prevent buffer overflow.
    if (i>=buffer_size) break;
    p_buffer[i] = font_5x8[offset];
  }
  return i+1;
}

// Write a string to the buffer.
uint16_t led_matrix_puts(uint8_t *p_buffer, uint16_t buffer_size, uint8_t *p_str)
{
  uint8_t *p = p_buffer;
  while (*p_str!=0)
  {
    p += led_matrix_putch(p,buffer_size-(p-p_buffer),*p_str);
    p_str++;
  }
  return p - p_buffer;
}

uint32_t t_prev = 0;

void setup(void)
{
  // Initialize LED matrix pins.
  for (uint8_t i=0; i<led_matrix_pin_count; i++)
  {
    pinMode(led_matrix_pin_first+i,INPUT); // all off
  }
  // Load text message.
  led_matrix_puts(led_matrix_buffer,sizeof(led_matrix_buffer),banner_text);
  // Ready...
  t_prev = millis();
  // Go!
}

void loop(void)
{
  // Scroll speed is determined by both scroll_speed and ontime.
  const uint8_t scroll_speed = 50; // milliseconds
  const uint16_t ontime = 521; // microseconds. 521 (us) * 96 (pixels) = 50 ms frame rate if all the pixels are on.
  static uint8_t scroll = 0; // scroll position.

  // Refresh display.
  led_matrix_buffer_show(scroll,ontime);

  // Update scroll position.
  if (millis()>=t_prev+scroll_speed)
  {
    t_prev = millis();
    scroll += 1; // Scroll to the left.
    if (scroll>5*strlen((char*)banner_text)) scroll = 0; // restart
  }
}
