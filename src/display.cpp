/**
 * @file display.cpp
 * @author Julia
 * @brief Methods for displaying shopping list items.
 * @version 0.1
 * @date 2024-06-21
 * 
 * @copyright Copyright (c) 2024
 */

#include <iostream>
#include <iomanip>
#include <optional>
#include <cmath>
#include "unit.h"
#include "utils.h"
#include "shopping_list.h"

/**
 * @brief Converts a weight to a displayable weight.
 * 
 * @param weight The weight.
 * @param unit The unit of measurement.
 */
double displayWeight(double weight, Unit unit) {
    if (isWhole(weight)) {
        // This is a whole number
        return weight;
    }
    
    switch (unit) {
        case Unit::Ounce:
            // Convert to 1 decimal place
            return std::round(weight * 10.0) / 10.0;
        case Unit::Pound:
            // Convert to 2 decimal places
            return std::round(weight * 100.0) / 100.0;
        case Unit::Kilogram:    
            // Convert to 2 decimal places
            return std::round(weight * 100.0) / 100.0;
        case Unit::Gram:
            return std::round(weight);
    }
    // Removes compiler warning about unreachable code.
     __builtin_unreachable();
}

/// Converted price per unit result.
struct ConvertedPerUnit {
    // The count of the unit.
    double unitCount;
    /// The unit for the price per unit.
    Unit perUnitUnit;
    /// The price of the item in cents, per unit.
    int64_t priceCentsPerUnit;
};

/**
 * @brief Calculate the conversion for the per unit.
 * 
 * @param unitCount The count of the unit.
 * @param unit The unit.
 * @param priceCentsPerUnit The price of the item in cents, per unit. 
 * @param preferredUnit The preferred unit.
 * @return ConvertedPerUnit 
 */
ConvertedPerUnit calculateConvertedPerUnit(int64_t unitCount, Unit unit, int64_t priceCentsPerUnit, Unit preferredUnit) {
    System system = getUnitSystem(unit);
    System preferredSystem = getUnitSystem(preferredUnit);
    double preferredUnitCount = static_cast<double>(unitCount);
    
    if (system == preferredSystem) {
        // The unit is in the same system as the preferred unit.
        return ConvertedPerUnit {
            .unitCount = preferredUnitCount,
            .perUnitUnit = unit,
            .priceCentsPerUnit = priceCentsPerUnit,
        };
    }
    
    switch (unit) {
        case Unit::Ounce:
        case Unit::Pound:
            preferredUnit = Unit::Kilogram;
            break;
        case Unit::Kilogram:
        case Unit::Gram:
            preferredUnit = Unit::Pound;
            break;
    }
    
    double convertedUnitCount = convertWeight(static_cast<double>(unitCount), unit, preferredUnit);
    int64_t newPriceCentsPerUnit = priceCentsPerUnit;
    
    if (unitCount > 1) {
        // We don't adjust the price but we do adjust the unit count.
        preferredUnitCount = convertedUnitCount;
    } else {
        // Get the ratio between the new unit count and the original unit count.
        double unitCountRatio = preferredUnitCount / static_cast<double>(convertedUnitCount);
        // Calculate the new price per unit.
        newPriceCentsPerUnit = static_cast<int64_t>(priceCentsPerUnit * unitCountRatio);
    }
    
    return ConvertedPerUnit {
        .unitCount = preferredUnitCount,
        .perUnitUnit = preferredUnit,
        .priceCentsPerUnit = newPriceCentsPerUnit,
    };
}

/**
 * @brief Prints an element
 * 
 * @tparam T 
 * @param t The element to print.
 * @param width The width of the column.
 * @param separator The separator to use as spaces.
 */
template<typename T>
void printElement(T t, const int& width, const char& separator = ' ') {
    std::cout << std::left << std::setw(width) << std::setfill(separator) << t;
}

/**
 * @brief Converts a value to a string using a stringstream.
 * 
 * @tparam T 
 * @param t The element to convert.
 * @return The formatted value.
 */
template<typename T>
inline static const std::string toStringStreamStr(T t) {
    std::stringstream ss;
    ss << t;
    return ss.str();
}

/**
 * @brief Prints a price.
 * 
 * @param priceCents The price in cents.
 * @param width The width of the column.
 * @param separator The separator to use as spaces.
 */
void printPriceElemet(int64_t priceCents, const int& width, const char& separator = ' ') {
    std::stringstream ss;
    
    ss.imbue(std::locale("en_US.UTF-8"));
    ss << "$" << std::put_money(priceCents);
    printElement(ss.str(), 10, separator);
}

/**
 * @brief Prints a shopping list item.
 * 
 * @param shoppingListItem The shopping list item.
 * @param preferredUnit The preferred unit of measurement.
 */
void printShoppingListItem(ShoppingListItem shoppingListItem, Unit preferredUnit) {
    const char separator = ' ';
    
    std::optional<Unit> unitOpt = convertCountTypeToUnit(shoppingListItem.countType);
    std::optional<Unit> perUnitUnitOpt = convertCountTypeToUnit(shoppingListItem.countTypePerUnit);
    
    int64_t itemTotalPriceCents = getShoppingListItemTotalPrice(shoppingListItem);
    int64_t priceCentsPerUnit = shoppingListItem.priceCentsPerUnit;
    int64_t unitCount = shoppingListItem.unitCount;
    CountType countTypePerUnit = shoppingListItem.countTypePerUnit;
    
    printElement(shoppingListItem.name, 20, separator);
    
    std::string countColumn = "";
    
    if (unitOpt.has_value()) {
        // Convert the weight to preferred unit.
        Unit unit = std::move(*unitOpt);
        double weight = convertWeight(shoppingListItem.count, unit, preferredUnit);
        std::string unitStr = convertUnitToString(preferredUnit);
        
        // Convert to a sensible precision for display.
        countColumn = toStringStreamStr(displayWeight(weight, preferredUnit)) + " " + unitStr + ".";
    } else {
        // This is a quantity
        countColumn = toStringStreamStr(shoppingListItem.count);
    }
    
    printElement(countColumn, 10, separator);
    
    std::stringstream ssPerUnitColumn;
    
    ssPerUnitColumn.imbue(std::locale("en_US.UTF-8"));
    
    if (perUnitUnitOpt.has_value()) {
        Unit perUnitUnit = std::move(*perUnitUnitOpt);
        ConvertedPerUnit convertedPerUnit = calculateConvertedPerUnit(
            unitCount,
            perUnitUnit,
            priceCentsPerUnit,
            preferredUnit
        );
        
        countTypePerUnit = convertUnitToCountType(convertedPerUnit.perUnitUnit);
        priceCentsPerUnit = convertedPerUnit.priceCentsPerUnit;
        double unitCountDouble = convertedPerUnit.unitCount;
        
        ssPerUnitColumn << "@ $" << std::put_money(priceCentsPerUnit);
        ssPerUnitColumn << " / ";
        
        if (isWhole(unitCountDouble)) {
            if (unitCountDouble > 1) {
                ssPerUnitColumn << static_cast<int64_t>(unitCountDouble) << " ";
            }
        } else {
            ssPerUnitColumn << toPrecision(unitCountDouble, 2) << " ";
        }
        
        ssPerUnitColumn << convertCountTypeToString(countTypePerUnit);
        ssPerUnitColumn << ".";
    } else {
        // This is a quantity
        if (unitCount != 1) {
            ssPerUnitColumn << "@ " << unitCount;
            ssPerUnitColumn << " / ";
            ssPerUnitColumn << "$" << std::put_money(priceCentsPerUnit);
        } else {
            ssPerUnitColumn << "@ $" << std::put_money(priceCentsPerUnit);
            ssPerUnitColumn << " / ";
            ssPerUnitColumn << convertCountTypeToString(countTypePerUnit);
            ssPerUnitColumn << ".";
        }
    }
    
    printPriceElemet(itemTotalPriceCents, 8, separator);
    printElement(ssPerUnitColumn.str(), 24, separator);
    
    std::cout << std::endl;
}