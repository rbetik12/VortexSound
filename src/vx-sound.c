#include "vx-sound.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <AL/al.h>
#include <AL/alc.h>

#define MINIMP3_IMPLEMENTATION

#include <minimp3.h>
#include <minimp3_ex.h>

static ALCdevice* device = NULL;
static mp3dec_t mp3Dec;

#define VX_LOG(x) printf("[VX Sound] %s\n", x)

void ALCheckForErrors() {
    ALCenum error = alGetError();
    if (error != ALC_NO_ERROR) {
        fprintf(stderr, "An error occurred!\n");
        switch (error) {
            case ALC_INVALID_VALUE:
                fprintf(stderr, "ALC_INVALID_VALUE: an invalid value was passed to an OpenAL function");
                break;
            case ALC_INVALID_DEVICE:
                fprintf(stderr, "ALC_INVALID_DEVICE: a bad device was passed to an OpenAL function");
                break;
            case ALC_INVALID_CONTEXT:
                fprintf(stderr, "ALC_INVALID_CONTEXT: a bad context was passed to an OpenAL function");
                break;
            case ALC_INVALID_ENUM:
                fprintf(stderr, "ALC_INVALID_ENUM: an unknown enum value was passed to an OpenAL function");
                break;
            case ALC_OUT_OF_MEMORY:
                fprintf(stderr, "ALC_OUT_OF_MEMORY: an unknown enum value was passed to an OpenAL function");
                break;
            default:
                fprintf(stderr, "UNKNOWN ALC ERROR");
        }
        fprintf(stderr, "\n");
    }
}

ALenum GetOpenALFormat(uint8_t channels) {
    switch (channels) {
        case 1:
            return AL_FORMAT_MONO16;
        case 2:
            return AL_FORMAT_STEREO16;
        default:
            return 0;
    }
}

void ALnssSleep(unsigned long nsec) {
    struct timespec ts, rem;
    ts.tv_sec = (time_t) (nsec / 1000000000ul);
    ts.tv_nsec = (long) (nsec % 1000000000ul);
    while (nanosleep(&ts, &rem) == -1 && errno == EINTR)
        ts = rem;
}

void VxSndInit() {
    ALCcontext* context;
    device = alcOpenDevice(NULL);

    if (!device) {
        fprintf(stderr, "Can't open audio device!\n");
        exit(EXIT_FAILURE);
    }

    context = alcCreateContext(device, NULL);
    if (context == NULL || alcMakeContextCurrent(context) == ALC_FALSE) {
        if (context != NULL)
            alcDestroyContext(context);
        alcCloseDevice(device);
        fprintf(stderr, "Could not set a context!\n");
        exit(EXIT_FAILURE);
    }

    const char* name = NULL;
    if (alcIsExtensionPresent(device, "ALC_ENUMERATE_ALL_EXT"))
        name = alcGetString(device, ALC_ALL_DEVICES_SPECIFIER);
    if (!name || alcGetError(device) != AL_NO_ERROR)
        name = alcGetString(device, ALC_DEVICE_SPECIFIER);

    printf("Successfully initialized sound! Device name: %s\n", name);
}

void VxSndLoadSound(const char* filename, AudioSource* const source) {
    mp3dec_file_info_t info;
    if (mp3dec_load(&mp3Dec, filename, &info, NULL, NULL)) {
        fprintf(stderr, "Can't load %s!\n", filename);
        source->loaded = false;
        return;
    }
    uint32_t size = info.samples * sizeof(mp3d_sample_t);

    int sampleRate = info.hz;
    int channels = info.channels;
    int alFormat = GetOpenALFormat(channels);
    float lengthSeconds = size / (info.avg_bitrate_kbps * 1024.0f);

    ALuint buffer;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, alFormat, info.buffer, size, sampleRate);

    source->bufferId = buffer;
    source->loaded = true;
    source->length = lengthSeconds;

    alGenSources(1, &source->sourceId);
    alSourcei(source->sourceId, AL_BUFFER, buffer);

    printf("Loaded mp3 info:\n");
    printf("    Filename: %s\n", filename);
    printf("    Channels: %d\n", channels);
    printf("    Sample Rate: %d\n", sampleRate);
    printf("    Size: %d bytes\n", size);
    printf("    Length: %f sec\n", lengthSeconds);

    if (alGetError() != AL_NO_ERROR) {
        fprintf(stderr, "Failed to setup audio source!\n");
    }
}

void VxSndPlaySound(AudioSource* const source) {
    alSourcePlay(source->sourceId);
}