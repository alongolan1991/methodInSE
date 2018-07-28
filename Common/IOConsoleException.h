#pragma once
#include <iostream>
class IOConsoleException : public std::exception {
	std::string msg;
public:
	IOConsoleException() : std::exception() {};
	IOConsoleException(const char* message) : msg(message) {}
	IOConsoleException(const std::string& message) : msg(message) {}

	const char* what() const throw() override {
		return msg.c_str();
	}
};

class DimentionException	: public IOConsoleException {};
class OverlapException     : public IOConsoleException {};
class HeightNotOverrideable : public IOConsoleException {};