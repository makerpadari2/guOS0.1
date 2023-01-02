#include "print.h"
#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>


const static size_t NUM_COLS = 80;
const static size_t NUM_ROWS = 25;



// Character struct to hold memory characters
struct Char {
    uint8_t character; // Each character is represented with the ascii character
    uint8_t color; // 8 bit color code
};

struct Char* buffer = (struct Char*) 0xb8000; // Buffer variable, 0xb8000 is the start of the memory location
size_t col = 0; // Keeps track of row and column numbers for printing
size_t row = 0;
uint8_t color = PRINT_COLOR_WHITE | PRINT_COLOR_BLACK << 4; // Keeps track of current color

void clear_row(size_t row) {
    struct Char empty = (struct Char) { // Print empty space character
        character: ' ',
        color: color,
    };

    for (size_t col = 0; col < NUM_COLS; col++) {
        buffer[col + NUM_COLS * row] = empty; // Update current character in the buffer to empty
    }
}

void print_clear() {
    for (size_t i = 0; i < NUM_ROWS; i++) { // Go through all rows
        clear_row(i); // For each row call function
    }
}

void print_newline() {
    col = 0;

    if (row < NUM_ROWS - 1) { //Check last row
        row++;
        return;
    }

    for (size_t row = 1; row < NUM_ROWS; row++) {
        for (size_t col = 0; col < NUM_COLS; col++) {
            struct Char character = buffer[col + NUM_COLS * row];
            buffer[col + NUM_COLS * (row - 1)] = character;
        }
    }

    clear_row(NUM_ROWS - 1);

}

void print_char(char character) { // Function for printing characters
    if (character == '\n') { // Check for new line character
        print_newline();
        return;
    }

    if (col > NUM_COLS) { 
        print_newline();
    }

    buffer[col + NUM_COLS * row] = (struct Char) { //Update character in buffer
        character: (uint8_t) character, // Set character to character passed to the function
        color: color,
    };
    col++;
}

void print_str(char* str) { // Function for printing strings
    for (size_t i = 0; 1; i++) {
        char character = (uint8_t) str[i]; // Grab the character at positon i
        
        if (character == '\0') { // Return to this function if comes across a \0
            return;
        }
        print_char(character);
    }
}

void print_set_color(uint8_t foreground, uint8_t background) { // Function for changing color
    color = foreground + (background << 4);
}

/*
static bool print(const char* data, size_t length) {
    const unsigned char* bytes = (const unsigned char*) data;
    for (size_t i = 0; i < length; i++) {
        if (putchar(bytes[i]) == EOF)
            return false;
        return true;
    }
}



int printf(const char* restrict format, ...) {
	va_list parameters;
	va_start(parameters, format);
 
	int written = 0;
 
	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;
 
		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}
 
		const char* format_begun_at = format++;
 
		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int );
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(&c, sizeof(c)))
				return -1;
			written++;
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
		} else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, len))
				return -1;
			written += len;
			format += len;
		}
	}
 
	va_end(parameters);
	return written;
}

int puts (const char* string) {
    return printf("%s\n", string);
}

int putchar (int ic) {

    char c = (char) ic;
    terminal_write(&c, sizeof(c));

    return ic;
}
*/