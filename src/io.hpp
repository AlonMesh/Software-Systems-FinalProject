#ifndef IO_HPP
#define IO_HPP

#include <iostream>

/**
 * @brief Print the usage of the program.
 *
 * This function prints the usage of the program to the standard output.
 *
 * @param programName The name of the program.
 */
void printUsage(const char *programName);

/**
 * @brief Print an error message.
 *
 * This function prints an error message to the standard output.
 *
 * @param message The error message to print.
 */
void printErrorMessage(const char *message);

/**
 * @brief Convert a string to a long long.
 *
 * This function converts a string to a long long.
 *
 * @param string The string to convert.
 * @return The long long representation of the string.
 */
long long convertToLongLong(const char *string);

#endif // IO_HPP