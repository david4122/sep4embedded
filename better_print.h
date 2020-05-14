/*
 * better_print.h
 *
 * Created: 5/14/2020 10:16:54 PM
 *  Author: cichy
 */
#pragma once

void bprintf_init();

void bprintf(const char* str);

void bprintf_int(const char* str, uint8_t n);