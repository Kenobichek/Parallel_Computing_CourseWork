#pragma once
#include <string>
#include "InvertedIndex.h"
#include "ThreadPool.h"

class FileScanner {
	public:
		FileScanner(const std::string& folder_path, InvertedIndex& inverted_index)
        : folder_path(folder_path), inverted_index(inverted_index) {}

		void ProcessFolder();
		void ProcessFile(const std::string& filepath);

	private:
		std::string folder_path;
		InvertedIndex& inverted_index;
		ThreadPool thread_pool;
};