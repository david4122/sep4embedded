/*
 * safeprint.h
 *
 * Created: 25/05/2020 19:46:32
 *  Author: sheep
 */ 
#include<stdbool.h>

bool safeprint_init(void);

bool safeprint(const char* str);

bool safeprintln(const char* str);

bool safeprintln_int(const char* prefix, int i);

bool safeprintln_ints(const char* prefix, int nums, ...);

bool safeprint_callbk(void (*func)(void));

bool safeprint_acquire();

bool safeprint_release();