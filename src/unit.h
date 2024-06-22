/**
 * @file unit.h
 * @author Julia
 * @brief Declares enums and functions for converting between units of measurement.
 * @version 0.1
 * @date 2024-06-21
 * 
 * @copyright Copyright (c) 2024
 */

#ifndef UNIT_H
#define UNIT_H
#pragma once

#include <cstdint>
#include <optional>
#include <string_view>

/// Units of measurement.
enum class Unit {
    /// Ounces.
    Ounce,
    /// Pounds.
    Pound,
    /// Kilograms.
    Kilogram,
    /// Grams.
    Gram
};

/// System of measurement.
enum class System {
    /// The metric system.
    Metric,
    /// The imperial system.
    Imperial
};

/// The type of count for an item.
enum class CountType {
    /// The item is measured by ounces.
    Ounce, 
    /// The item is measured by pounds.
    Pound, 
    /// The item is measured by kilograms.
    Kilogram,
    /// The item is measured by grams.
    Gram,
    /// The item is counted by its quantity.
    Quantity
};

System getUnitSystem(Unit unit);
std::optional<Unit> convertCountTypeToUnit(CountType countType);
CountType convertUnitToCountType(Unit unit);
std::string convertUnitToString(Unit unit);
std::string convertCountTypeToString(CountType countType);
std::optional<Unit> convertStringToUnit(const std::string_view& s);
double convertWeight(double weight, Unit from, Unit to);

#endif