
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