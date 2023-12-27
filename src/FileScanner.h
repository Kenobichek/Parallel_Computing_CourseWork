#pragma once
#include <string>
#include "InvertedIndex.h"
#include "ThreadPool.h"

using read_write_lock = std::shared_mutex;
using read_lock = std::shared_lock<read_write_lock>;
using write_lock = std::unique_lock<read_write_lock>;
using Paths = std::vector<std::filesystem::path>;

class FileScanner {
	public:
		FileScanner(const std::string& folder_path, InvertedIndex& inverted_index)
        : folder_path(folder_path), inverted_index(inverted_index) {}

		void ParallelFileProcessing();

	private:
		void ProcessFile(const std::filesystem::path& filepath, InvertedIndex& idx);
		void ProcessFolder(const std::filesystem::path& current_path, Paths& paths);

		mutable read_write_lock rw_lock;
		mutable std::condition_variable_any termination_cv;

		std::string folder_path;
		InvertedIndex& inverted_index;
		ThreadPool thread_pool;
};