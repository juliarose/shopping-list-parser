/**
 * @file shopping_list.cpp  
 * @author Julia
 * @brief Contains functions for parsing shopping list items.
 * @version 0.1
 * @date 2024-06-21
 * 
 * @copyright Copyright (c) 2024
 */

#ifndef SHOPPING_LIST_H
#define SHOPPING_LIST_H
#pragma once

#include <cstdint>
#include <optional>
#include <string_view>
#include <cmath>
#include <tuple>
#include "unit.h"
#include "utils.h"

/// A shopping list item.
struct ShoppingListItem {
    /// The name of the item.
    std::string name;
    /// The price of the item in cents, per unit.
    int64_t priceCentsPerUnit;
    /// The count of the item.
    double count;
    // The count of the unit.
    int64_t unitCount;
    /// The type of count for the item.
    CountType countType;
    /// The type of count for the price per unit.
    CountType countTypePerUnit;
};

ShoppingListItem parseShoppingListItemStr(const std::string &s);
int64_t getShoppingListItemTotalPrice(const ShoppingListItem &shoppingListItem);

#endif