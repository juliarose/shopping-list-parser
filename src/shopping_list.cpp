/**
 * @file shopping_list.cpp  
 * @author Julia
 * @brief Contains functions for parsing shopping list items.
 * @version 0.1
 * @date 2024-06-21
 * 
 * @copyright Copyright (c) 2024
 */

#include <cstdint>
#include <optional>
#include <string_view>
#include <cmath>
#include <tuple>
#include "unit.h"
#include "utils.h"
#include "shopping_list.h"

/**
 * @brief Extracts the double from the front of the string.
 * 
 * Advances the string view to exclude the number.
 * 
 * @param sView 
 * @return The number.
 */
double detectDoubleFromFront(std::string_view &sView) {
    // The length of the number string.
    size_t numStrLen = 0;
    size_t decimalCount = 0;
    
    // Extract the number string by looping through the beginning of the string until a 
    // non-number is found.
    for (size_t i = 0; i < sView.length();) {
        char c = sView[i];
        size_t charLen = getCharLen(c);
        
        if (charLen != 1) {
            // Not a valid number
            throw std::runtime_error("Expected string to start with a number");
        }
        
        // Check if the character is a digit.
        if (std::isdigit(c)) {
            numStrLen += charLen;
        } else if (c == '.') {
            if (decimalCount > 0) {
                throw std::runtime_error("Too many decimal places in number string");
            } else if (numStrLen == 0) {
                throw std::runtime_error("Expected string to start with a number");
            }
            
            decimalCount++;
            numStrLen += charLen;
        } else if (numStrLen == 0) {
            // Not a digit and no digits have been found yet so we can assume that the string does 
            // not start with a number.
            throw std::runtime_error("Expected string to start with a number");
        } else {
            // The number string has ended.
            break;
        }
        
        i += charLen;
    }
    
    // Get the number string.
    std::string_view numStrView = std::string_view(sView.data(), numStrLen);
    // Convert the number string to an integer.
    std::optional<double> numOpt = stringToDouble(numStrView);
    
    // Check if the number was successfully parsed.
    if (!numOpt.has_value()) {
        throw std::runtime_error("Failed to parse number as double");
    }
    
    // Take the number value.
    double num = std::move(*numOpt);
    // Update the string view to exclude the number string.
    sView = std::string_view(sView.data() + numStrLen, sView.length() - numStrLen);
    
    return num;
}

/**
 * @brief Extracts the decimal from the back of the string.
 * 
 * Advances the string view to exclude the number.
 * 
 * @param sView 
 * @return A tuple containing the whole number and the fractional number, and the precision of the 
 * fractional number.
 */
std::tuple<size_t, size_t, size_t> detectDecimalFromBack(std::string_view &sView) {
    size_t fractionalLength = 0;
    size_t wholeLength = 0;
    size_t decimalCount = 0;
    
    // Extract the price string by looping through the end of the string until a non-number is found.
    for (size_t i = sView.length() - 1; i < sView.length();) {
        char c = sView[i];
        size_t charLen = getCharLen(c);
        
        if (charLen != 1) {
            // Not a valid number
            throw std::runtime_error("Expected string to end with a number");
        }
        
        // Check if the character is a digit.
        if (std::isdigit(c)) {
            if (decimalCount == 0) {
                fractionalLength += charLen;
            } else if (decimalCount == 1) {
                wholeLength += charLen;
            } else {
                throw std::runtime_error("Too many decimal places in price string");
            }
        } else if (c == '.') {
            if (decimalCount > 0) {
                throw std::runtime_error("Too many decimal places in price string");
            } else if (fractionalLength == 0) {
                throw std::runtime_error("Expected string to end with a number");
            }
            
            decimalCount++;
        } else if (fractionalLength == 0) {
            // Not a digit and no digits have been found yet so we can assume that the string does 
            // not end with a number.
            throw std::runtime_error("Expected string to end with a number");
        } else {
            // The price string has ended.
            break;
        }
        
        // Advance backwards.
        i -= charLen;
    }
    
    if (fractionalLength == 0) {
        throw std::runtime_error("Expected string to end with a number");
    }
    
    if (wholeLength == 0) {
        throw std::runtime_error("Expected string to end with a number");
    }
    
    std::string_view fractionalStrView = std::string_view(sView.data() + sView.length() - fractionalLength, fractionalLength);
    
    // Remove the fractional and the period from the string view.
    sView = std::string_view(sView.data(), sView.length() - (fractionalLength + 1));
    
    std::string_view wholeStrView = std::string_view(sView.data() + sView.length() - wholeLength, wholeLength);
    
    // Remove the whole from the string view.
    sView = std::string_view(sView.data(), sView.length() - wholeLength);
    
    std::optional<int64_t> fractionalOpt = stringToInt(fractionalStrView);
    std::optional<int64_t> wholeOpt = stringToInt(wholeStrView);
    
    // Check if the fractional was successfully parsed.
    if (!fractionalOpt.has_value()) {
        throw std::runtime_error("Failed to parse fractional as integer");
    }
    
    // Check if the whole was successfully parsed.
    if (!wholeOpt.has_value()) {
        throw std::runtime_error("Failed to parse whole as integer");
    }
    
    // Take the values.
    int64_t fractional = std::move(*fractionalOpt);
    int64_t whole = std::move(*wholeOpt);
    int64_t precision = std::pow(10, fractionalLength);
    
    return std::make_tuple(whole, fractional, precision);
}

size_t detectIntFromBack(const std::string_view &sView) {
    size_t intLen = 0;
    
    // Extract the price string by looping through the end of the string until a non-number is found.
    for (size_t i = sView.length() - 1; i < sView.length();) {
        char c = sView[i];
        size_t charLen = getCharLen(c);
        
        if (charLen != 1) {
            // Not a valid number
            throw std::runtime_error("Expected string to end with a number");
        }
        
        // Check if the character is a digit.
        if (std::isdigit(c)) {
            intLen += charLen;
        }  else {
            // The unit string has ended.
            break;
        }
        
        i -= charLen;
    }
    
    return intLen;
}

/**
 * @brief Attempts to extract an integer from the back of the string.
 * 
 * Advances the string view to exclude the number.
 * 
 * @param sView 
 * @return An optional containing the integer if it exists.
 */
std::optional<int64_t> tryExtractIntFromBack(std::string_view &sView) {
    size_t numStrLength = 0;
    
    // Extract the count of the price per unit by looping through the end of the string until a 
    // non-number is found. We only expect a whole number.
    for (size_t i = sView.length() - 1; i < sView.length();) {
        char c = sView[i];
        size_t charLen = getCharLen(c);
        
        if (charLen != 1) {
            // Not a valid number
            throw std::runtime_error("Expected string to end with a number");
        }
        
        // Check if the character is a digit.
        if (std::isdigit(c)) {
            numStrLength += charLen;
        }  else {
            // The unit string has ended.
            break;
        }
        
        i -= charLen;
    }
    
    // If this is 0 there is no number.
    if (numStrLength == 0) {
        return std::nullopt;
    }
    
    std::string_view numStrView = std::string_view(sView.data() + sView.length() - numStrLength, numStrLength);
    std::optional<int64_t> numOpt = stringToInt(numStrView);
    
    // Check if the unit count was successfully parsed.
    if (!numOpt.has_value()) {
        throw std::runtime_error("Failed to parse unit count as integer");
    }
    
    // Take the unit count value.
    int64_t num = std::move(*numOpt);
    
    // Update the string view to exclude the unit count string.
    sView = std::string_view(sView.data(), sView.length() - numStrLength);
    
    return num;
}

/**
 * @brief Get the total price of a shopping list item.
 * 
 * @param shoppingListItem 
 * @return The total price of the shopping list item in cents.
 */
int64_t getShoppingListItemTotalPrice(const ShoppingListItem &shoppingListItem) {
    int64_t priceCents = shoppingListItem.priceCentsPerUnit;
    int64_t perUnitCount = shoppingListItem.perUnitCount;
    int64_t count = shoppingListItem.count;
    
    // If the item is measured by quantity, multiply the price by the quantity.
    if (
        shoppingListItem.perUnitCountType == CountType::Quantity ||
        shoppingListItem.countType == CountType::Quantity
    ) {
        // Do not do any arithmetic on the unit count type is not a quantity.
        if (shoppingListItem.perUnitCountType == CountType::Quantity && perUnitCount != 1) {
            double ratio = static_cast<double>(count) / static_cast<double>(perUnitCount);
            priceCents *= ratio;
        } else {
            priceCents *= count;
        }
        
        return priceCents;
    }
    
    // Get the unit for the item.
    std::optional<Unit> unitOpt = convertCountTypeToUnit(shoppingListItem.countType).value();
    // Get the unit for the price per unit.
    std::optional<Unit> perUnitUnitOpt = convertCountTypeToUnit(shoppingListItem.perUnitCountType);
    
    if (!unitOpt.has_value()) {
        // This should never happen.
        throw std::runtime_error("Invalid unit");
    }
    
    if (!perUnitUnitOpt.has_value()) {
        // This should never happen.
        throw std::runtime_error("Invalid unit");
    }
    
    // Convert the weight to the same unit that the price is in.
    double weight = convertWeight(shoppingListItem.count, *unitOpt, *perUnitUnitOpt);
    // Convert the per unit weight to the preferred unit.
    double perUnitWeight = shoppingListItem.perUnitCount;
    
    // If the item is measured by weight, multiply the price by the weight.
    priceCents *= (weight / perUnitWeight);
    
    return priceCents;
}

/**
 * @brief Parse a shopping list item string.
 * 
 * @param s 
 * @return The shopping list item.
 */
ShoppingListItem parseShoppingListItemStr(const std::string &s) {
    std::string_view sView = std::string_view(s.data(), s.length());
    
    // Get the unit count type.
    CountType perUnitCountType;
    bool hasPerUnitCountType = true;
    
    // Remove the period if it exists.
    if (endsWith(sView, ".")) {
        sView = std::string_view(sView.data(), sView.length() - 1);
    }
    
    if (endsWith(sView, "lbs")) {
        sView = std::string_view(sView.data(), sView.length() - 3);
        perUnitCountType = CountType::Pound;
    } else if (endsWith(sView, "lb")) {
        sView = std::string_view(sView.data(), sView.length() - 2);
        perUnitCountType = CountType::Pound;
    } else if (endsWith(sView, "oz")) {
        sView = std::string_view(sView.data(), sView.length() - 2);
        perUnitCountType = CountType::Ounce;
    } else if (endsWith(sView, "g")) {
        sView = std::string_view(sView.data(), sView.length() - 1);
        perUnitCountType = CountType::Gram;
    } else if (endsWith(sView, "kg")) {
        sView = std::string_view(sView.data(), sView.length() - 2);
        perUnitCountType = CountType::Kilogram;
    }  else if (endsWith(sView, "ea")) {
        sView = std::string_view(sView.data(), sView.length() - 2);
        perUnitCountType = CountType::Quantity;
    } else {
        hasPerUnitCountType = false;
        // Assume it's a quantity.
        perUnitCountType = CountType::Quantity;
    }
    
    // Space is optional.
    if (endsWith(sView, " ")) {
        sView = std::string_view(sView.data(), sView.length() - 1);
    }
    
    int64_t perUnitCount = 1;
    
    if (hasPerUnitCountType) {
        std::optional<int64_t> perUnitCountOpt = tryExtractIntFromBack(sView);
        
        if (perUnitCountOpt.has_value()) {
            perUnitCount = std::move(*perUnitCountOpt);
        }
        
        // We expect a slash before the price if there is a count type per unit.
        if (endsWith(sView, "/")) {
            sView = std::string_view(sView.data(), sView.length() - 1);
        } else {
            throw std::runtime_error("Expected slash before price");
        }
    }
    
    size_t dollars;
    size_t cents;
    // Unused
    size_t _precision;
    
    // Extract the price string.
    std::tie(dollars, cents, _precision) = detectDecimalFromBack(sView);
    
    // Combine the dollars and cents to get the price.
    int64_t priceCentsPerUnit = (dollars * 100) + cents;
    
    if (endsWith(sView, "$")) {
        sView = std::string_view(sView.data(), sView.length() - 1);
    } else {
        throw std::runtime_error("Expected dollar sign before price");
    }
    
    // Look for a slash ahead of the price, this means it's a quantity e.g. 5/$1.00
    if (!hasPerUnitCountType && endsWith(sView, "/")) {
        sView = std::string_view(sView.data(), sView.length() - 1);
        
        std::optional<int64_t> perUnitCountOpt = tryExtractIntFromBack(sView);
        
        if (perUnitCountOpt.has_value()) {
            perUnitCount = std::move(*perUnitCountOpt);
            // Make sure this is quantity.
            perUnitCountType = CountType::Quantity;
        } else {
            throw std::runtime_error("Expected unit count before price");
        }
    }
    
    // Space is optional.
    if (endsWith(sView, " ")) {
        sView = std::string_view(sView.data(), sView.length() - 1);
    }
    
    if (endsWith(sView, ",")) {
        sView = std::string_view(sView.data(), sView.length() - 1);
    } else {
        throw std::runtime_error("Expected comma before price");
    }
    
    // Expects a quantity at the front.
    bool expectsQuantity = perUnitCountType == CountType::Quantity ||
        perUnitCount != 1;
    double count;
    
    // Take the count value.
    if (expectsQuantity) {
        try {
            count = detectDoubleFromFront(sView);
        } catch(const std::runtime_error &e) {
            // Default to 1 if there is no quantity.
            count = 1;
        }
    } else {
        count = detectDoubleFromFront(sView);
    }
    
    // Look for a space after the count.
    if (startsWith(sView, " ")) {
        sView = std::string_view(sView.data() + 1, sView.length() - 1);
    }
    
    CountType countType;
    
    if (startsWith(sView, "lbs")) {
        sView = std::string_view(sView.data() + 3, sView.length() - 3);
        countType = CountType::Pound;
    } else if (startsWith(sView, "lb")) {
        sView = std::string_view(sView.data() + 2, sView.length() - 2);
        countType = CountType::Pound;
    } else if (startsWith(sView, "oz")) {
        sView = std::string_view(sView.data() + 2, sView.length() - 2);
        countType = CountType::Ounce;
    } else if (startsWith(sView, "g")) {
        sView = std::string_view(sView.data() + 1, sView.length() - 1);
        countType = CountType::Gram;
    } else if (startsWith(sView, "kg")) {
        sView = std::string_view(sView.data() + 2, sView.length() - 2);
        countType = CountType::Kilogram;
    } else if (isWhole(count)) {
        // Assume it's a quantity
        countType = CountType::Quantity;
    } else {
        throw std::runtime_error("Expected a unit of measurement after the quantity");
    }
    
    // Not necessary to check if this is a count.
    if (countType != CountType::Quantity) {
        // Period usually occurs after the unit of measurement and is optional.
        if (startsWith(sView, ".")) {
            sView = std::string_view(sView.data() + 1, sView.length() - 1);
        }
        
        // Check if there is a space after the unit of measurement.
        if (startsWith(sView, " ")) {
            sView = std::string_view(sView.data() + 1, sView.length() - 1);
        } else {
            throw std::runtime_error("Expected space after the unit of measurement");
        }
    }
    
    std::string name = std::string(sView.data(), sView.length());
    
    return ShoppingListItem {
        .name = name,
        .priceCentsPerUnit = priceCentsPerUnit,
        .count = count,
        .countType = countType,
        .perUnitCount = perUnitCount,
        .perUnitCountType = perUnitCountType,
    };
}
