#include "InputClass.h"
#include <cassert>

///¼üµÄ¸öÊý
#define NUM_KEY		256

InputClass::InputClass()
	:m_keys(NUM_KEY,0)
{
	
}


InputClass::~InputClass()
{
}

void InputClass::Initialize()
{
	for(auto& e : m_keys)
		e = false;
}


bool InputClass::IsKeyDown(unsigned int key)
{
	assert(key < m_keys.size());
	return m_keys[key];
}

void InputClass::KeyDown(unsigned int key)
{
	assert(key < m_keys.size());
	m_keys[key] = true;
}

void InputClass::KeyRelease(unsigned int key)
{
	assert(key < m_keys.size());
	m_keys[key] = false;
}