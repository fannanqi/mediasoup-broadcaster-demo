/*
 * @Author: fannanqi 1773252187@qq.com
 * @Date: 2024-10-29 20:46:13
 * @LastEditors: fannanqi 1773252187@qq.com
 * @LastEditTime: 2024-10-30 01:03:34
 * @FilePath: /mediasoup-broadcaster-demo/deps/libwebrtc/test/frame_utils.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/*
 *  Copyright (c) 2013 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */
#ifndef TEST_FRAME_UTILS_H_
#define TEST_FRAME_UTILS_H_

#include <stdint.h>

#include "api/scoped_refptr.h"
#include "api/video/nv12_buffer.h"

namespace webrtc {
class I420Buffer;
class VideoFrame;
class VideoFrameBuffer;
namespace test {

bool EqualPlane(const uint8_t* data1,
                const uint8_t* data2,
                int stride1,
                int stride2,
                int width,
                int height);

static inline bool EqualPlane(const uint8_t* data1,
                              const uint8_t* data2,
                              int stride,
                              int width,
                              int height) {
  return EqualPlane(data1, data2, stride, stride, width, height);
}

bool FramesEqual(const webrtc::VideoFrame& f1, const webrtc::VideoFrame& f2);

bool FrameBufsEqual(const rtc::scoped_refptr<webrtc::VideoFrameBuffer>& f1,
                    const rtc::scoped_refptr<webrtc::VideoFrameBuffer>& f2);

rtc::scoped_refptr<I420Buffer> ReadI420Buffer(int width, int height, FILE*);

rtc::scoped_refptr<NV12Buffer> ReadNV12Buffer(int width, int height, FILE*);

}  // namespace test
}  // namespace webrtc

#endif  // TEST_FRAME_UTILS_H_
