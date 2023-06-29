//
// Created by William.Hua on 2020/9/2.
//
// #include "mp3ToPcm.cpp"
#include "iostream"
using namespace std;

//
// Created by JSL on 2021/9/1.
//
#include <AL/al.h>
#include <AL/alext.h>
#include <al/alc.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
    ALCdevice *device = NULL;
    ALCcontext *context = NULL;
    device = alcOpenDevice(NULL);
    if (!device) {
        fprintf(stderr, "fail to open device\n");
        return -1;
    }
    context = alcCreateContext(device, NULL);
    if (!context) {
        fprintf(stderr, "fail to create context.\n");
        return -1;
    }
    alcMakeContextCurrent(context);
    if (alGetError() != AL_NO_ERROR) {
        return -1;
    }
    //音频播放源
    ALuint source;
    //音频数据
    ALuint buffer;
    //音频格式
    ALenum audioFormat = AL_FORMAT_STEREO_FLOAT32;
    //声道数目
    ALshort channels = 2;
    //音频采样率
    ALsizei sample_rate = 44100;
    //是否循环播放
    ALboolean loop = 1;
    //播放源的位置
    ALfloat position[] = {0.0f, 0.0f, 0.0f};
    //播放的速度
    ALfloat velocity[] = {0.0f, 0.0f, 0.0f};
    alGenBuffers(1, &buffer);
    alGenSources(1, &source);

    FILE *f = fopen("/Users/panfeng/coder/mediaLean/FFmpegLearn/FFmpegCmake/xxx.wav", "rb");
    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    rewind(f);
    char *data = (char *) malloc(length);
    fread(data, sizeof(char), length, f);
    fclose(f);

    alBufferData(buffer, audioFormat, data, length, sample_rate);
    if (alGetError() != AL_NO_ERROR) {
        return -1;
    }
    //为source绑定数据
    alSourcei(source, AL_BUFFER, buffer);
    //音高倍数
    alSourcef(source, AL_PITCH, 0.2f);
    //声音的增益
    alSourcef(source, AL_GAIN, 0.2f);
    //设置位置
    alSourcefv(source, AL_POSITION, position);
    //设置声音移动速度
    alSourcefv(source, AL_VELOCITY, velocity);
    //设置是否循环播放
    alSourcei(source, AL_LOOPING, loop);
    //播放音乐
    alSourcePlay(source);
    getchar();
    //释放资源
    free(data);
    alcMakeContextCurrent(NULL);
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
    alcDestroyContext(context);
    alcCloseDevice(device);
    return 0;
}