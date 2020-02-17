#include "menu.hpp"

#include "prompt.hpp"

#include <cstdio>
#include <cstring>

menu::menu(const char* header, const std::vector<char*>& option, const char* footer, const char* prompt)
{
	this->header = 0;
	this->footer = 0;
	this->prompt = 0;

	this->setHeader(header);
	this->setFooter(footer);
	this->setPrompt(prompt);

	if (&option != 0)
	{
		this->option = option;
	}
}

menu::menu(const menu& M)
{
	this->header = 0;
	this->footer = 0;
	this->prompt = 0;

	this->setHeader(M.getHeader());
	this->setFooter(M.getFooter());
	this->setPrompt(M.getPrompt());

	this->option = M.option;
}

menu::~menu()
{
	this->clear();
}

void menu::clear()
{
	size_t i, size;

	if (this->header != 0) free(this->header);
	if (this->footer != 0) free(this->footer);
	if (this->prompt != 0) free(this->prompt);

	this->header = 0;
	this->footer = 0;
	this->prompt = 0;

	i = 0; size = this->option.size();
	while (i < size)
	{
		if (this->option[i] != 0) free(this->option[i]);
		i++;
	}
	this->option.clear();
}

const char* menu::getHeader() const
{
	return this->header;
}

char*& menu::getHeader()
{
	return this->header;
}

bool menu::setHeader(const char* header)
{
	size_t size;

	if (header == this->header) return true;

	free(this->header);
	if (header == 0)
	{
		this->header = 0;
		return true;
	}

	if ((this->header = reinterpret_cast<char*>(malloc(sizeof(char) * (size = strlen(header) + 1)))) == 0) return false;
	memcpy(reinterpret_cast<void*>(this->header), reinterpret_cast<const void*>(header), size);
	return true;
}

const char* menu::getOption(size_t index) const
{
	return this->option[index];
}

char*& menu::getOption(size_t index)
{
	return this->option[index];
}

bool menu::setOption(size_t index, const char* option)
{
	size_t size;

	if (option == this->option[index]) return true;

	free(this->option[index]);
	if (option == 0)
	{
		this->option[index] = 0;
		return true;
	}

	if ((this->option[index] = reinterpret_cast<char*>(malloc(sizeof(char) * (size = strlen(option) + 1)))) == 0) return false;
	memcpy(reinterpret_cast<void*>(this->option[index]), reinterpret_cast<const void*>(option), size);
	return true;
}

bool menu::addOption(const char* option)
{
	size_t i;

	i = this->option.size();
	this->option.push_back(0);
	return this->setOption(i, option);
}

bool menu::insOption(size_t index, const char* option)
{
	/* TODO: bool menu::insOption(size_t index, const char* option) */
	index = 0; option = 0;
	return false;
}

bool menu::delOption(size_t index)
{
	/* TODO: bool menu::delOption(size_t index) */
	index = 0;
	return false;
}

const char* menu::getFooter() const
{
	return this->footer;
}

char*& menu::getFooter()
{
	return this->footer;
}

bool menu::setFooter(const char* footer)
{
	size_t size;

	if (footer == this->footer) return true;

	free(this->footer);
	if (footer == 0)
	{
		this->footer = 0;
		return true;
	}

	if ((this->footer = reinterpret_cast<char*>(malloc(sizeof(char) * (size = strlen(footer) + 1)))) == 0) return false;
	memcpy(reinterpret_cast<void*>(this->footer), reinterpret_cast<const void*>(footer), size);
	return true;
}

const char* menu::getPrompt() const
{
	return this->prompt;
}

char*& menu::getPrompt()
{
	return this->prompt;
}

bool menu::setPrompt(const char* prompt)
{
	size_t size;

	if (prompt == this->prompt) return true;

	free(this->prompt);
	if (prompt == 0)
	{
		this->prompt = 0;
		return true;
	}

	if ((this->prompt = reinterpret_cast<char*>(malloc(sizeof(char) * (size = strlen(prompt) + 1)))) == 0) return false;
	memcpy(reinterpret_cast<void*>(this->prompt), reinterpret_cast<const void*>(prompt), size);
	return true;
}

const char* menu::operator [] (size_t index) const
{
	return this->getOption(index);
}

char*& menu::operator [] (size_t index)
{
	return this->getOption(index);
}

size_t menu::size() const
{
	return this->option.size();
}

size_t menu::askMenu() const
{
	char* buffer;
	size_t i, j, k, size;

	if ((size = this->size()) == 0) return size;
	if ((buffer = reinterpret_cast<char*>(malloc(sizeof(char) * BUFSIZ))) == 0) return size;
	printf("%s\n", this->getHeader());
	
	i = 0;
	while (i < size)
	{
		sprintf(buffer, "%lu", i + 1);
		j = 0; k = strlen(buffer);
		while ((j + k) < 4)
		{
			printf(" ");
			j++;
		}
		printf("%s. %s\n", buffer, this->getOption(i));
		i++;
	}
	printf("%s\n", this->getFooter());
	free(buffer);

	while (true)
	{
		if ((i = prompt_unsignedlong(this->getPrompt()) - 1) < size)
		{
			return i;
		}
		printf("Invalid menu option (%lu)\n", i + 1);
	}
}
