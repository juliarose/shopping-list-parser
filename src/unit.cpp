/**
 * @file unit.cpp
 * @author Julia
 * @brief Declares functions for converting between units of measurement.
 * @version 0.1
 * @date 2024-06-21
 * 
 * @copyright Copyright (c) 2024
 */

#include <cstdint>
#include <optional>
#include <string_view>
#include "unit.h"

/// Number of ounces in a pound.
int64_t const OZ_PER_LB = 16;
/// Number of grams in a kilogram.
int64_t const GRAM_PER_KG = 1000;

/**
 * @brief Gets the system of measurement for a unit.
 * 
 * @param unit The unit.
 * @return The system of measurement for the unit.
 */
System getUnitSystem(Unit unit) {
    switch (unit) {
        case Unit::Ounce:
        case Unit::Pound:
            return System::Imperial;
        case Unit::Kilogram:
        case Unit::Gram:
            return System::Metric;
    }
    // Removes compiler warning about unreachable code.
     __builtin_unreachable();
}

/**
 * @brief Converts a count type to a unit.
 * 
 * @param countType The count type.
 * @return The unit that corresponds to the count type.
 */
std::optional<Unit> convertCountTypeToUnit(CountType countType) {
    switch (countType) {
        case CountType::Ounce:
            return Unit::Ounce;
        case CountType::Pound:
            return Unit::Pound;
        case CountType::Kilogram:
            return Unit::Kilogram;
        case CountType::Gram:
            return Unit::Gram;
        default:
            return std::nullopt;
    }
    // Removes compiler warning about unreachable code.
     __builtin_unreachable();
}

/**
 * @brief Converts a unit to a count type.
 * 
 * @param unit The unit.
 * @return The count type that corresponds to the unit.
 */
CountType convertUnitToCountType(Unit unit) {
    switch (unit) {
        case Unit::Ounce:
            return CountType::Ounce;
        case Unit::Pound:
            return CountType::Pound;
        case Unit::Kilogram:
            return CountType::Kilogram;
        case Unit::Gram:
            return CountType::Gram;
    }
    // Removes compiler warning about unreachable code.
     __builtin_unreachable();
}

/**
 * @brief Converts a unit to a string.
 * 
 * @param unit The unit.
 * @return The string representation of the unit.
 */
std::string convertUnitToString(Unit unit) {
    switch (unit) {
        case Unit::Ounce:
            return "oz";
        case Unit::Pound:
            return "lb";
        case Unit::Kilogram:
            return "kg";
        case Unit::Gram:
            return "g";
    }
    // Removes compiler warning about unreachable code.
     __builtin_unreachable();
}

/**
 * @brief Converts a count type to a string.
 * 
 * @param countType The count type.
 * @return The string representation of the count type.
 */
std::string convertCountTypeToString(CountType countType) {
    switch (countType) {
        case CountType::Ounce:
            return "oz";
        case CountType::Pound:
            return "lb";
        case CountType::Kilogram:
            return "kg";
        case CountType::Gram:
            return "g";
        case CountType::Quantity:
            return "ea";
    }
    // Removes compiler warning about unreachable code.
     __builtin_unreachable();
}

/**
 * @brief Converts a count type to a string.
 * 
 * @param unit The count type.
 * @return The string representation of the count type.
 */
std::optional<Unit> convertStringToUnit(const std::string_view& s) {
    if (s == "oz") {
        return Unit::Ounce;
    } else if (s == "lb") {
        return Unit::Pound;
    } else if (s == "kg") {
        return Unit::Kilogram;
    } else if (s == "g") {
        return Unit::Gram;
    }
    
    return std::nullopt;
}

/**
 * @brief Converts ounces to pounds.
 * 
 * @param ounces The weight in ounces.
 * @return The equivalent weight in pounds.
 */
double ouncesToPounds(double ounces) {
    return ounces / static_cast<double>(OZ_PER_LB);
}

/**
 * @brief Converts pounds to kilograms.
 * 
 * @param pounds The weight in pounds.
 * @return The equivalent weight in kilograms.
 */
double poundsToKilograms(double pounds) {
    return pounds * 0.45359237;
}

/**
 * @brief Converts kilograms to pounds.
 * 
 * @param kilograms The weight in kilograms.
 * @return The equivalent weight in pounds.
 */
double kilogramsToPounds(double kilograms) {
    return kilograms / 0.45359237;
}

/**
 * @brief Converts a weight from one unit to another.
 * 
 * @param weight The weight to convert.
 * @param from The unit of the weight.
 * @param to The unit to convert the weight to.
 * @return The converted weight.
 */
double convertWeight(double weight, Unit from, Unit to) {
    // There might be a better way to do this but hopefully these are all correct.
    switch (from) {
        case Unit::Ounce:
            switch (to) {
                case Unit::Ounce:
                    return weight;
                case Unit::Pound:
                    return ouncesToPounds(weight);
                case Unit::Kilogram:
                    return poundsToKilograms(ouncesToPounds(weight));
                case Unit::Gram:
                    return poundsToKilograms(ouncesToPounds(weight)) * GRAM_PER_KG;
            }
        case Unit::Pound:
            switch (to) {
                case Unit::Ounce:
                    return weight * OZ_PER_LB;
                case Unit::Pound:
                    return weight;
                case Unit::Kilogram:
                    return poundsToKilograms(weight);
                case Unit::Gram:
                    return poundsToKilograms(weight) * GRAM_PER_KG;
            }
        case Unit::Kilogram:
            switch (to) {
                case Unit::Ounce:
                    return kilogramsToPounds(weight) * OZ_PER_LB;
                case Unit::Pound:
                    return kilogramsToPounds(weight);
                case Unit::Kilogram:
                    return weight;
                case Unit::Gram:
                    return weight * GRAM_PER_KG;
            }
        case Unit::Gram:
            switch (to) {
                case Unit::Ounce:
                    return kilogramsToPounds(weight / GRAM_PER_KG) * OZ_PER_LB;
                case Unit::Pound:
                    return kilogramsToPounds(weight / GRAM_PER_KG);
                case Unit::Kilogram:
                    return weight / GRAM_PER_KG;
                case Unit::Gram:
                    return weight;
            }
    }
    // Removes compiler warning about unreachable code.
     __builtin_unreachable();
}
