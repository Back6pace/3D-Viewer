#ifndef SRC_SETTINGS_H_
#define SRC_SETTINGS_H_

#include <fstream>
#include <iostream>

namespace settings {
const bool kDebugMode = false;
const bool kDebugModel = false;
const bool kDebugController = false;
const bool kDebugView = false;
const bool kDebugParser = false;
const bool kDebugAffine = false;

// Указать свой путь до лога
const std::string kDebugFile =
    "/Users/ricardac/Documents/CPP4_3DViewer_v2.0-2/src/debug.log";
}  // namespace settings

#endif  // SRC_SETTINGS_H_