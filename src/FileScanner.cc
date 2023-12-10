#include <thread>
#include <filesystem>
#include <fstream>
#include <iostream>
#include "FileScanner.h"

void FileScanner::ProcessFolder() {
	for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
		if (entry.is_regular_file()) {
			thread_pool.AddTask([this, entry]() {
				ProcessFile(entry.path().string());
			});
		}
	}
}


void FileScanner::ProcessFile(const std::string& filepath) {
	std::ifstream file(filepath);
	if (!file.is_open()) {
		std::cout << "Failed to open file: " << filepath << std::endl;
		return;
	}

	std::string word;
	while (file >> word) {
		std::cout << word << std::endl;
		// inverted_index.AddWord(word, filepath);
	}

	file.close();
}
