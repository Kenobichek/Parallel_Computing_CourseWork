#include "InvertedIndex.h"
#include <iostream>

void InvertedIndex::AddWords(const std::unordered_set<std::string>& words, const std::filesystem::path& filepath) {
	write_lock _(rw_lock);
	for(auto& word : words) {
		auto it = inverted_index.find(word);
		if (it != inverted_index.end()) {
			it->second.insert(filepath);
		} 
		else {
			inverted_index.insert({word, {filepath}});
		}
	}
}

void InvertedIndex::Merge(InvertedIndex&& source_inverted_index) {
	write_lock _(rw_lock);
	if (inverted_index.empty()) {
		inverted_index = std::move(source_inverted_index.inverted_index);
		return;
	}

	for (auto& [word, files] : source_inverted_index.inverted_index) {
		auto it = inverted_index.find(word);
		if (it != inverted_index.end()) {
			it->second.merge(std::move(files));
		} 
		else {
			inverted_index.insert({ std::move(word), std::move(files) });
		}
	}
}

Files InvertedIndex::GetFiles(const std::string& word) {
	write_lock _(rw_lock);
	return inverted_index[word];
}