#include <thread>
#include <filesystem>
#include <fstream>
#include <iostream>
#include "FileScanner.h"


void FileScanner::ParallelFileProcessing() {
	Paths paths;
	ProcessFolder(folder_path, paths);
	int thread_number = thread_pool.GetThreadCount();
	std::vector<InvertedIndex> inverted_indexes(thread_number);
	int start_i = 0;
	int chunk_size = paths.size() / thread_number;
	int remaining_size = paths.size() % thread_number;

	for(auto& idx : inverted_indexes) {
		int end_i = start_i == 0 ? chunk_size + remaining_size : chunk_size;
		end_i += start_i;
		thread_pool.AddTask([this, &paths, &idx, start_i, end_i]() {
			for (int i = start_i; i < end_i; i++) {
				ProcessFile(paths[i], idx);
			}
		});
		start_i = end_i;
	}

	thread_pool.Terminate(true);

	for (auto& idx : inverted_indexes) {
		inverted_index.Merge(std::move(idx));
	}
}

void FileScanner::ProcessFolder(const std::filesystem::path& current_path, Paths& paths) {
	for (const auto& entry : std::filesystem::directory_iterator(current_path)) {
		if (entry.is_regular_file()) {
			paths.push_back(entry.path());
		}
		else if (entry.is_directory()) {
			ProcessFolder(entry.path(), paths);
		}
	}
}

void FileScanner::ProcessFile(const std::filesystem::path& filepath, InvertedIndex& idx) {
	std::ifstream file(filepath);
	if (!file.is_open()) {
		std::cout << "Failed to open file: " << filepath << std::endl;
		return;
	}

	std::unordered_set<std::string> words;
	std::string word;
	while (file >> word) {
		words.emplace(word);
	}
	idx.AddWords(words, filepath);
	file.close();
}
