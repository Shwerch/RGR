#pragma once

#include <vector>
#include <string>
#include <cstdint>

std::vector<uint8_t> readFile(const std::string& path);
void writeFile(const std::string& path, const std::vector<uint8_t>& data);

std::vector<uint8_t> readFromStdin();
void writeToStdout(const std::vector<uint8_t>& data);
