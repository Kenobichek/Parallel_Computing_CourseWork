#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <filesystem>
#include <mutex>
#include <shared_mutex>

using read_write_lock = std::shared_mutex;
using read_lock = std::shared_lock<read_write_lock>;
using write_lock = std::unique_lock<read_write_lock>;
using Files = std::vector<std::filesystem::path>;

class InvertedIndex {
	public:

		void AddWord(const std::string& word, const std::filesystem::path& filepath);
		Files GetFiles(const std::string& word);

	private:
		mutable read_write_lock rw_lock;
		std::unordered_map<std::string, Files> inverted_index;
};