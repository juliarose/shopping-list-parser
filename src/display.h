/**
 * @file display.h
 * @author Julia
 * @brief Methods for displaying shopping list items.
 * @version 0.1
 * @date 2024-06-22
 * 
 * @copyright Copyright (c) 2024 
 */

#ifndef DISPLAY_H
#define DISPLAY_H
#pragma once

#include <iostream>
#include <iomanip>
#include <optional>
#include <cmath>
#include "unit.h"
#include "utils.h"
#include "shopping_list.h"

void printShoppingListItem(ShoppingListItem shoppingListItem, Unit preferredUnit);

#endif
