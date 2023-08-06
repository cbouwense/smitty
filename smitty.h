#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define expect(condition) return condition;
typedef bool test;

//--------------------------------------------------------------------------------------------------
// Time functions
//--------------------------------------------------------------------------------------------------

const double time_in_seconds(double time);

const double time_in_milliseconds(double time);

const double time_in_microseconds(double time);

const double time_in_nanoseconds(double time);

void print_most_readable_time(double time);

//--------------------------------------------------------------------------------------------------
// Output style functions
//--------------------------------------------------------------------------------------------------

void set_output_color_to_green();

void set_output_color_to_red();

void set_output_style_to_bold();

void reset_output_color();
