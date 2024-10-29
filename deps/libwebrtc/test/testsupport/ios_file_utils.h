/*
 * @Author: fannanqi 1773252187@qq.com
 * @Date: 2024-10-29 20:46:13
 * @LastEditors: fannanqi 1773252187@qq.com
 * @LastEditTime: 2024-10-30 01:09:27
 * @FilePath: /mediasoup-broadcaster-demo/deps/libwebrtc/test/testsupport/ios_file_utils.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/*
 *  Copyright (c) 2018 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef TEST_TESTSUPPORT_IOS_FILE_UTILS_H_
#define TEST_TESTSUPPORT_IOS_FILE_UTILS_H_

#include <string>

#include "absl/strings/string_view.h"

namespace webrtc
{
namespace test
{

	std::string IOSOutputPath();
	std::string IOSRootPath();
	std::string IOSResourcePath(absl::string_view name, absl::string_view extension);

} // namespace test
} // namespace webrtc

#endif // TEST_TESTSUPPORT_IOS_FILE_UTILS_H_
