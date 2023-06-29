//
// Created by William.Hua on 2020/9/2.
//
#include "mp3ToPcm.cpp"
#include "play.cpp"

using namespace std;

int main(int argc, char *argv[]) {
    // FFmpeg 解码
//    mp3ToPcm(
//            "/Users/panfeng/coder/mediaLean/FFmpegLearn/FFmpegCmake/run.mp3",
//            "/Users/panfeng/coder/mediaLean/FFmpegLearn/FFmpegCmake/run.pcm"
//            );

    // openal 播放裸流
    play("/Users/panfeng/coder/mediaLean/FFmpegLearn/FFmpegCmake/run.mp3");
}

