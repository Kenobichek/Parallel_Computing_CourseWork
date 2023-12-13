#include "InvertedIndex.h"
#include <iostream>

void InvertedIndex::AddWord(const std::string& word, const std::filesystem::path& filepath) {
	write_lock _(rw_lock);
	auto it = inverted_index.find(word);
	if (it != inverted_index.end()) {
		it->second.push_back(filepath);
	} 
	else {
		inverted_index.insert({word, {filepath}});
	}
}

Files InvertedIndex::GetFiles(const std::string& word) {
	write_lock _(rw_lock);
	return inverted_index[word];
}