#include "io.hpp"
#include <string.h>

void printUsage(const char *programName) {
    std::cerr << "Usage: " << programName << " <limit>" << std::endl;
}

void printErrorMessage(const char *message) {
    std::cerr << "Error: " << message << std::endl;
}

long long convertToLongLong(const char *string) {
    // Handle the case where the string is empty
    if (string[0] == '\0') {
        printErrorMessage("Input is empty. Please enter a number.");
        return 0;
    }

    // Handle the case where the string is not a number
    for (int i = 0; string[i] != '\0'; i++) {
        if (string[i] < '0' || string[i] > '9') {
            printErrorMessage("Input is not a number. Please enter a valid number.");
            return 0;
        }
    }

    // Handle the case where the string is too large
    if (strlen(string) > 19) {
        printErrorMessage("Input is too large. Please enter a smaller number.");
        return 0;
    }

    // Convert the string to a long long
    return std::stoll(string);
}