#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct AudioSource {
    uint32_t bufferId;
    uint32_t sourceId;
    bool loaded;
    float length;
} AudioSource;

void VxSndInit();

void VxSndLoadSound(const char* filename, AudioSource* source);

void VxSndPlaySound(AudioSource* source);