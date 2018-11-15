#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>

//Function Template
template <typename T, int a = 1> 
void fExample1(T a)
{
	
};

//Class Template
template <typename T1, typename T2>
class cExample2
{
	public:
		std::vector<T1> aV;
		T2 x; 
};

//Struct Template
template <typename T>
struct Node
{
	T info;
	Node *next;
};

//Function Template (EX2)
template <typename T>
T fExample2(int size, const T *data)
{
	T result = data[0];
	for (int i = 0; i < size; i++)
	{
		if (result > data[i])
		{
			result = data[i];
		}
	}
	return result;
}

//Class Template (EX2)
template <typename T> 
class SuperQueue
{
	public:
		Node<T> *first;
		Node<T> *last;
		int tam;

		//CONSTRUCTORES
		SuperQueue<T>() {
			last = nullptr;
			first = nullptr;
			tam = 0;
		}

		SuperQueue<T>(SuperQueue &e) 
		{
			this->last = e.last;
			this->first = e.first;
			this->size = e.tam;
		}
		
		//METODOS
		int size()
		{
			return tam;
		}

		void push(T a)
		{
			Node<T> *node = new Node<T>;
			node->info = a;
			node->next = nullptr;
			if (last != nullptr)
			{
				last->next = node;
			}
			else
			{
				first = node;
			}

			last = node;
			tam++;
		}

		void pop()
		{
			if (tam > 0)
			{
				Node<T> *aux = first->next;
				delete first;
				first = aux;
				tam--;
			}
		}
};

void main()
{
	//FUNCTION EXAMPLE
	fExample1<int>(2);
	fExample1<int, 20>(2);

	//CLASS EXAMPLE
	cExample2<int, char> myExample2;

	//STRUCT EXAMPLE
	Node<int> n1;
	Node<char> n2;

	//ARRAY EXAMPLE
	int arrInt[5] = { 1, 2, 3, 4, 5 };
	std::string arrString[7] = { "Gula", "Soberbia", "Pereza", "Ira", "Envidia", "Lujuria", "Avaricia" };

	//FUNCTION EXAMPLE
	int resultInt = fExample2<int>(4, arrInt);
	std::string resultString = fExample2<std::string>(4, arrString);

	//MYDEQUE EXAMPLE
	SuperQueue<int> q1;
	q1.push(5);
	q1.push(6);
	q1.pop();

	system("pause");
}