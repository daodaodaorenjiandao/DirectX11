#pragma once
#include <vector>

class InputClass
{
public:
	InputClass();
	~InputClass();

	void Initialize();

	void KeyDown(unsigned int key);
	void KeyRelease(unsigned int key);
	bool IsKeyDown(unsigned int key);

private:
	std::vector<bool> m_keys;
};
