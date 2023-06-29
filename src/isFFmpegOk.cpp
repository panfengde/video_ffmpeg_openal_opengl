//
// Created by 潘峰 on 2023/6/27.
//

#if defined(__cplusplus)
extern "C"
{
#endif

#include <libavcodec/avcodec.h>

#if defined(__cplusplus)
}
#endif

#include <iostream>

using namespace std;

void isFFmpeg() {
    cout << "avcodec_configuration : " << avcodec_configuration() << endl;
}