/**
 * @file utils.cpp
 * @author Julia
 * @brief Contains utility functions.
 * @version 0.1
 * @date 2024-06-21
 * 
 * @copyright Copyright (c) 2024
 */

#include <cstdint>
#include <optional>
#include <string_view>
#include <charconv>
#include <cmath>

/**
 * @brief Checks if a number is a whole number.
 * 
 * @param num The number.
 * @return True if the number is a whole number, false otherwise.
 */
bool isWhole(double num) {
    return num == static_cast<int>(num);
}

/**
 * @brief Checks if a string starts with a given prefix.
 * 
 * @param fullString The string to check.
 * @param start The prefix to check for.
 * @return True if the string starts with the prefix, false otherwise.
 */
bool startsWith(const std::string_view& fullString, const std::string_view& start) {
    return fullString.rfind(start, 0) == 0;
}

/**
 * @brief Checks if a string ends with a given suffix.
 * 
 * @param fullString The string to check.
 * @param ending The suffix to check for.
 * @return True if the string ends with the suffix, false otherwise.
 */
bool endsWith(const std::string_view& fullString, const std::string_view& ending) {
    // Sourced from https://stackoverflow.com/questions/874134/find-out-if-string-ends-with-another-string-in-c
    size_t fullStringLen = fullString.length();
    size_t endingLen = ending.length();
    
    // If the ending is longer than the full string, it can't be a suffix
    if (endingLen > fullStringLen) {
        return false;
    }
    
    size_t offset = fullString.compare(fullStringLen - endingLen, endingLen, ending);
    
    return 0 == offset;
}

/**
 * @brief Gets the length of a UTF-8 character by checking the first byte.
 * 
 * @param c The character.
 * @return The length of the character.
 */
size_t getCharLen(char c) {
    if ((c & 0xf8) == 0xf0) {
        return 4;
    } else if ((c & 0xf0) == 0xe0) {
        return 3;
    } else if ((c & 0xe0) == 0xc0) {
        return 2;
    }
    
    return 1;
}

/**
 * @brief Converts cents to dollars.
 * 
 * @param cents The amount in cents.
 * @return The amount in dollars.
 */
double centsToDollars(int64_t cents) {
    return static_cast<double>(cents) / 100.0;
}

/**
 * @brief Rounds a number to a specific precision.
 * 
 * @param num The number.
 * @param precision The number of decimal places to round to.
 * @return The rounded number.
 */
double toPrecision(double num, int precision) {
    return std::round(num * std::pow(10, precision)) / std::pow(10, precision);
}

/**
 * @brief Converts a string to a double.
 * 
 * @param s The string.
 * @return The double value of the string.
 */
std::optional<double> stringToDouble(const std::string_view& s) {
    return std::atof(s.data());
}

/**
 * @brief Converts a string to an integer.
 * 
 * @param s The string.
 * @return The integer value of the string.
 */
std::optional<int64_t> stringToInt(const std::string_view& s) {
    return std::atoi(s.data());
}
