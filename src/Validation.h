#ifndef VALIDATION_H
#define VALIDATION_H

// Gets a valid integer from the user within a specified range [min, max].
// Handles wrong data types (letters) and out-of-range numbers.
int getValidInput(int min, int max);

// Gets a valid 'y' (yes) or 'n' (no) character from the user.
// Handles wrong characters and converts input to lowercase.
char getYesNoInput();

#endif // VALIDATION_H