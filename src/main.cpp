/**
 * @file main.cpp
 * @author Julia
 * @brief Parses a shopping list from a file and prints it.
 * @version 0.1
 * @date 2024-06-21
 * 
 * @copyright Copyright (c) 2024
 */

#include <iostream>
#include <iomanip>
#include <vector>
#include <optional>
#include <chrono>
#include <fstream>
#include <cmath>
#include "unit.h"
#include "utils.h"
#include "shopping_list.h"
#include "display.h"

/**
 * @brief Runs a benchmark to test the performance of the parser.
 */
void runBenchmark() {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration;
    
    std::string name = "1 lb. Chicken Breasts, $4.99";
    
    /// Number of iterations to run.
    size_t iterCount = 100000;
    
    // Warm up the CPU before running the benchmark.
    for (size_t i = 0; i < 1000000; ++i) {
        ShoppingListItem shoppingListItem = parseShoppingListItemStr(name);
    }
    
    auto t1 = high_resolution_clock::now();
    
    for (size_t i = 0; i < iterCount; ++i) {
        ShoppingListItem shoppingListItem = parseShoppingListItemStr(name);
    }
    
    auto t2 = high_resolution_clock::now();
    /// The duration in nanoseconds.
    duration<double, std::nano> ns_double = t2 - t1;
    
    std::cout << "parseShoppingListItemStr: " << printf("%.2f", ns_double.count() / iterCount) << "ns\n";
}

/**
 * @brief Picks a unit of measurement based on a string.
 * 
 * @param unitStr The unit string.
 * @return The unit of measurement.
 */
Unit pickUnit(const std::string &unitStr) {
    std::optional<Unit> preferredUnitOpt = convertStringToUnit(unitStr);
    
    Unit preferredUnit = Unit::Pound;
    
    if (preferredUnitOpt.has_value()) {
        preferredUnit = std::move(*preferredUnitOpt);
    } else {
        std::cout << "Invalid unit \"" << unitStr << "\"; using pounds" << std::endl;
    }
    
    return preferredUnit;
}

/**
 * @brief Reads a shopping list from a file.
 * 
 * @param filePath The path to the shopping list file.
 * @return The shopping list items.
 */
std::vector<ShoppingListItem> readShoppingListFromFile(std::string &filePath) {
    std::ifstream file(filePath);
    
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file.");
    }
    
    std::vector<ShoppingListItem> shoppingListItems;
    
    for (std::string line; getline(file, line);) {
        // Skip empty lines.
        if (line.empty()) {
            continue;
        }
        
        if (startsWith(line, "//")) {
            // Skip comments.
            continue;
        }
        
        try {
            ShoppingListItem shoppingListItem = parseShoppingListItemStr(line);
            shoppingListItems.push_back(shoppingListItem);
        } catch (std::runtime_error& e) {
            std::cerr << "Failed to parse line \"" << line << "\": " << e.what() << "; ignoring" << std::endl;
            // Ignore errors and continue to the next line.
        }
    }
    
    return shoppingListItems;
}

int main(int argc, char* argv[]) {
    // Test the performance of the parser.
    // runBenchmark();
    
    // Get the file path from the command line arguments.
    std::string filePath;
    
    if (argc > 1) {
        filePath = argv[1];
    } else {
        std::cerr << "No file name provided" << std::endl;
        return 1;
    }
    
    // Get the preferred unit of measurement from the command line arguments.
    std::string preferredUnitStr = "lb";
    
    if (argc > 2) {
        preferredUnitStr = argv[2];
    }
    
    Unit preferredUnit = pickUnit(preferredUnitStr);
    
    // Read the shopping list from the file.
    auto shoppingListItems = readShoppingListFromFile(filePath);
    
    int64_t totalPriceCents = 0;
    
    // Print the shopping list.
    for (ShoppingListItem shoppingListItem : shoppingListItems) {
        printShoppingListItem(shoppingListItem, preferredUnit);
        
        int64_t itemTotalPriceCents = getShoppingListItemTotalPrice(shoppingListItem);
        
        // Add to the total price.
        totalPriceCents += itemTotalPriceCents;
    }
    
    std::cout << "\nTotal: $" << centsToDollars(totalPriceCents) << std::endl;
    
    return 0;
}
