#include <thread>
#include <filesystem>
#include <fstream>
#include <iostream>
#include "FileScanner.h"

void FileScanner::ProcessFolder() {
	int tasks_count = 0;
	for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
		if (entry.is_regular_file()) {
			thread_pool.AddTask([this, entry, &tasks_count]() {
				ProcessFile(entry.path());
				{
					write_lock _(rw_lock);
					--tasks_count;
				}
				termination_cv.notify_one();
			});
			{
				write_lock _(rw_lock);
				++tasks_count;
			}
		}
	}

	write_lock lock(rw_lock);

	termination_cv.wait(rw_lock, [&tasks_count] {
		return !tasks_count; 
	});
}


void FileScanner::ProcessFile(const std::filesystem::path& filepath) {
	std::ifstream file(filepath);
	if (!file.is_open()) {
		std::cout << "Failed to open file: " << filepath << std::endl;
		return;
	}

	std::string word;
	while (file >> word) {
		inverted_index.AddWord(word, filepath);
	}

	file.close();
}
