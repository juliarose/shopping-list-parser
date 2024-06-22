/**
 * @file utils.h
 * @author Julia
 * @brief Contains utility functions.
 * @version 0.1
 * @date 2024-06-21
 * 
 * @copyright Copyright (c) 2024
 */

#ifndef UTILS_H
#define UTILS_H
#pragma once

#include <cstdint>
#include <optional>
#include <string_view>
#include <charconv>
#include <cmath>

bool isWhole(double num);
bool startsWith(const std::string_view& fullString, const std::string_view& start);
bool endsWith(const std::string_view& fullString, const std::string_view& ending);
size_t getCharLen(char c);
double centsToDollars(int64_t cents);
double toPrecision(double num, int precision);
std::optional<double> stringToDouble(const std::string_view& s);
std::optional<int64_t> stringToInt(const std::string_view& s);

#endif