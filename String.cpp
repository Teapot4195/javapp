
#include "String.h"

String::String(const std::string &str) : backing(str) {
}

String::String(const char *str) : backing(str) {
}
