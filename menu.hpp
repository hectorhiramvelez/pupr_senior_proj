#pragma once

#ifndef MENU_HPP
#define MENU_HPP

#include <vector>

class menu
{
	private:
		char* header;
		std::vector<char*> option;
		char* footer;
		char* prompt;

	public:
		menu(const char* header = 0, const std::vector<char*>& option = *(reinterpret_cast<std::vector<char*>*>(0)), const char* footer = 0, const char* prompt = 0);
		menu(const menu& M);
		~menu();

		void clear();

		const char* getHeader() const;
		char*& getHeader();
		bool setHeader(const char* header);

		const char* getOption(size_t index) const;
		char*& getOption(size_t index);
		bool setOption(size_t index, const char* option);

		bool addOption(const char* option);
		bool insOption(size_t index, const char* option);
		bool delOption(size_t index);

		const char* getFooter() const;
		char*& getFooter();
		bool setFooter(const char* footer);

		const char* getPrompt() const;
		char*& getPrompt();
		bool setPrompt(const char* prompt);

		const char* operator [] (size_t index) const;
		char*& operator [] (size_t index);

		size_t size() const;

		size_t askMenu() const;
};

#endif /* MENU_HPP */
