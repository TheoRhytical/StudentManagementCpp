#ifndef KEY_HPP
#define KEY_HPP
#include <string>

class Key {
public:
	std::string Name, Filename, Content;
	int KeyNum;

	Key(std::string name, std::string content) {
		Name = name;
		Content = content;
		KeyNum = 0;
		for (char c: name) {
			KeyNum += c - 65;
		}
	}
};

#endif