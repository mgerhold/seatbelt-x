#pragma once

#include <filesystem>

void interpret(std::filesystem::path const& path, std::ostream& std_out);
void compile(std::filesystem::path const& path);
