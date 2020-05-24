#pragma once

#include <stdio.h>
#include <stdint.h>

#include<lora_driver.h>

#include "config.h"

//Updates console with the package data
void sent_upload_messages(lora_payload_t* lora_payload);