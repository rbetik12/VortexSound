VX sound library

Sound library built on top of [OpenAL Soft](https://openal-soft.org/).

## Currently Supports
- .mp3 files

## TO-DO
- 3D sound placement
- ogg, wav format support
- pitch, spatial sound configuration

## Example 
Checkout `example` project for real example. API is pretty simple

```c
// First of all initialize audio
VxSndInit();
//Load audio file and create an audio source that is associated with it
void VxSndLoadSound(const char* filename, AudioSource* source);
// Play the sound (you must do it in an endless loop, at least until track finish)
void VxSndPlaySound(AudioSource* source);
```
`AudioSource` is a structure that contains all information for current loaded sound
```c
typedef struct AudioSource {
    uint32_t bufferId; // buffer id (buffer that contains loaded track)
    uint32_t sourceId; // audio source id (defines sound playback object in 3D space)
    bool loaded; // was audio file loaded or not
    float length; // length of the track in seconds
} AudioSource;
```

## Third-party libraries
- [OpenAL Soft](https://github.com/kcat/openal-soft)
- [minimp3](https://github.com/lieff/minimp3)
