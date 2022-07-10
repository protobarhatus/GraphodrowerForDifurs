#pragma once
#include<queue>


template<class T>
struct St
{
	T object;
	int priority;
public:
	
	St(T t, int ipriority)
	{
		object = t;
		priority = ipriority;
	}
};
//чем больше приоритет, тем глубже в списке
template<class T>
bool operator<(const St<T> a, const St<T> b)
{
	if (b.priority < a.priority)
		return true;
	else return false;
}

template<class T>
struct PriorityQueueDes
{
private:
	std::priority_queue<St<T>> queue;
public:
	PriorityQueueDes() {}
	void push(T t, int priority)
	{
		queue.push(St<T>(t, priority));
	}
	T top()
	{
		return queue.top().object;
	}
	void pop()
	{
		queue.pop();
	}
	bool empty()
	{
		return queue.empty();
	}
	int size()
	{
		return queue.size();
	}
	
};

template<class T>
struct PriorityQueueAs
{
private:
	std::priority_queue<std::pair<T, int>> queue;
public:
	PriorityQueueAs() {}
	void push(T t, int priority)
	{
		queue.push(std::pair<T, int>(t, priority));
	}
	T top()
	{
		return queue.top().first;
	}
	void pop()
	{
		queue.pop();
	}
	bool empty()
	{
		return queue.empty();
	}
	int size()
	{
		return queue.size();
	}

};
/*
template<class T>
struct PriorityQueueDes
{
private:
	std::priority_queue<std::pair<int, T>> queue;
public:
	PriorityQueueDes() {}
	void push(T t, int priority)
	{
		queue.push(std::pair<int, T>(priority, t));
	}
	T top()
	{
		return queue.top().second;
	}
	void pop()
	{
		queue.pop();
	}
	bool empty()
	{
		return queue.empty();
	}
	int size()
	{
		return queue.size();
	}

};*/