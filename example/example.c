#include <vx-sound.h>

int main() {
    VxSndInit();
    AudioSource source;
    VxSndLoadSound("sound.mp3", &source);

    //Loops endlessly
    VxSndPlaySound(&source);
    while (true);
}

