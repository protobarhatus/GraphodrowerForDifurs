#pragma once
#include<vector>
#include<map>
template<typename T>
void DeleteElement(std::vector<T> & container, int index)
{
	auto iterator = container.begin();
	
	std::advance(iterator, index);
	container.erase(iterator);
}
template<typename T>
void DeleteElement(std::vector<T> & container, T & element)
{
	for (int i = 0; i < container.size(); ++i)
	{
		if (container[i] == element)
		{
			DeleteElement(container, i);
		}
	}
}
template<typename T>
void DeleteElement(std::vector<T> & container, T && element)
{
	for (int i = 0; i < container.size(); ++i)
	{
		if (container[i] == element)
		{
			DeleteElement(container, i);
		}
	}
}
template<typename T, typename U>
void AddToMap(std::map<T, U> & container, std::map<T, U> & added_map)
{
	for (auto i = added_map.begin(); i != added_map.end(); ++i)
	{
		
		container.insert(std::pair<T, U>(i->first, i->second));
	}
}