#include <memory>
#include <iostream>
#include <string>
#include <exception>
#include <ctime>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h> 

//#define MY_POINTER
//#define MY_AUTO
#define MY_INT


template<class I>
class list_node {
public:
	I val;
	list_node(list_node<I>* p, list_node<I>* n, I value) :prev(p), next(n), val(value) {};
	list_node(I value) :list_node(NULL, NULL, value) {};
	list_node<I>* next;
	list_node<I>* prev;
	~list_node() {  }
};

template<class T/*,class U*/>
class moja_lista
{
	long size;
	list_node<T>* tail;
	list_node<T>* head;
public:
	moja_lista() :tail(NULL), head(NULL), size(0) {};

	void add_tail(const T& value)
	{
		if (this->tail == NULL)
		{
			try{ 
				this->tail = new list_node<T>(value); 
			}
			catch (std::bad_alloc& ba) { throw; }
			this->head = this->tail;
			this->tail->val = value;

		}
		else
		{
			list_node<T>* tmp = this->tail;
			try {
				this->tail = new list_node<T>(value);
			}
			catch (std::bad_alloc& ba) { throw; }
			tmp->next = this->tail;
			this->tail->prev = tmp;
		}
		this->size++;
	};

	void add_head(const T& value)
	{
		if (this->head == NULL)
		{
			try { 
				this->head = new list_node<T>(value); 
			}
			catch (std::bad_alloc& ba) { throw; }
			this->tail = this->head;
			this->head->val = value;
		}
		else
		{
			list_node<T>* tmp = this->head;
			try {
				this->head = new list_node<T>(value);
			}
			catch (std::bad_alloc& ba) { throw; }
			tmp->prev = this->head;
			this->head->next = tmp;
		}
		this->size++;
	};

	bool add(const T& value, bool (*funkcja) (const T  a, const T  b))
	{
		bool flag = false;
		if (this->head == NULL)
		{
			try {
				 this->head = new list_node<T>(value);
			}
			catch (std::bad_alloc& ba) { throw; }
			this->tail = this->head;
			this->head->val = value;
			flag = true;
		}
		else
		{
			list_node<T>* scroll = this->head;
			while (scroll)
			{
				if (funkcja (value,scroll->val))
				{
					list_node<T>* tmp = scroll->prev;
					if (tmp == NULL)
					{
						try {
							this->head = scroll->prev = new list_node<T>(NULL, scroll, value);
						}
						catch (std::bad_alloc & ba) { throw; }
					}
					else
					{
						try {
							tmp->next = scroll->prev = new list_node<T>(scroll->prev, tmp->next, value);
						}
						catch (std::bad_alloc & ba) { throw; }	
					}
					
					flag = true; break;
				}
				scroll = scroll->next;
			}
		}
		if (!flag)
		{
			try{
				this->tail = this->tail->next = new list_node<T>(this->tail, NULL, value);
			}
			catch (std::bad_alloc & ba) { throw; }
			flag = true;
		}
		size++;
		return flag;
	};

	T get_tail() { return this->tail->val; };
	T get_head() { return this->tail->val; };
	long get_size() { return this->size; };

	void print(void (*funkcja) (const T  a))
	{
		list_node<T>* scroll = this->head;;
		while (scroll)
		{
			funkcja(scroll->val);
			scroll = scroll->next;
		}
	};

	bool find_value_bool(const T& value, bool (*funkcja) (const T  a, const T  b))
	{
		list_node<T>* scroll = this->head;
		while (scroll)
		{
			if (funkcja(scroll->val, value)) return true;
			scroll = scroll->next;
		}
		return false;
	}


	T find_value(const T& value, bool (*funkcja) (const T  a, const T  b)) // throw std::invalid_argument
	{
		list_node<T>* scroll = this->head;
		while (scroll)
		{
			if (funkcja(scroll->val, value))
			{
				T tmp = scroll->val;
				return tmp;
			}
			scroll = scroll->next;
		}
		throw std::invalid_argument("Don't find this value");
	}

	long find_position(const T& value, bool (*funkcja) (const T  a, const T  b))// throw std::invalid_argument
	{
		long position = 0;
		list_node<T>* scroll = this->head;
		while (scroll)
		{
			if (funkcja(value,scroll->val)) return position;
			position++;
			scroll = scroll->next;
		}
		throw std::invalid_argument("Don't find this value");
	}

	T& operator[] (const long& pos)// throw std::out_of_range
	{
		if (pos < 0 || pos > this->size) throw std::out_of_range ("Don't find this value");
		if (pos < (this->size/2))
		{
			list_node<T>* scroll = this->head;
			for (long i = 0; i < pos; i++)  scroll = scroll->next;
			return scroll->val;
		}
		else
		{
			list_node<T>* scroll = this->tail;
			for (long i = 0; i < (this->size-pos); i++)  scroll = scroll->prev;
			return scroll->val;
		}
	}

	bool remove_value(const T value, bool (*funkcja) (const T  a, const T  b))
	{
		list_node<T>* scroll = this->head,*tmp;
		if (scroll == NULL) return false;
		bool flag = false,flag2 = true;
		

		if (funkcja(scroll->val, value))
		{
			if (scroll->next != NULL)
			{
				scroll = scroll->next;
				this->head = scroll;
				scroll->prev = NULL;
				delete scroll->prev;
				this->size--;
			}
			else
			{
				delete scroll;
				this->head = this->tail = NULL;
				this->size--;
				return true;
			}
		}
		
		while (scroll->next)
		{
			flag2 = true;
			if (funkcja(scroll->val, value))
			{
				if (!scroll->prev)
				{
					scroll = scroll->next;
					delete scroll->prev;
					scroll->prev = NULL;
					this->head = scroll;
					this->size--;
					flag2 = false;
				}
				else
				{
					tmp = scroll;
					scroll->next->prev = scroll->prev;
					scroll->prev->next = scroll->next;
					scroll = scroll->next;
					delete tmp;
					this->size--;
					flag2 = false;
				}
			}
			if (flag2) scroll = scroll->next;
		}

		if (funkcja(scroll->val, value))
		{
			scroll->prev != NULL ? this->tail = scroll->prev : this->tail = this->head = NULL;
			delete scroll;
			this->size--;
		}

		return flag;
	}

	bool remove_position(const long& pos)// throw std::out_of_range
	{
		if (pos < 0 || pos > this->size) return false;
		list_node<T>* scroll = this->head,*tmp;
		for (long i = 0; i < pos; i++) scroll = scroll->next;
		if (!scroll->prev && !scroll->next)
		{
			delete scroll;
			this->tail = this->head = NULL;
		}
		else if (!scroll->prev)
		{
			scroll = scroll->next;
			delete scroll->prev;
			scroll->prev = NULL;
			this->head = scroll;
			this->size--;
			return true;
		}
		else if (!scroll->next)
		{
			tmp = scroll;
			scroll = scroll->prev;
			this->tail = scroll;
			scroll->next = NULL;
			delete tmp;
			this->size--;
			return true;
		}
		else
		{
			tmp = scroll;
			scroll->next->prev = scroll->prev;
			scroll->prev->next = scroll->next;
			scroll = scroll->next;
			delete tmp;
			this->size--;
			return true;
		}

		return false;
	}

	void erase_ptr(void)
	{
		list_node<T>* scroll = this->head;
		
			while (scroll->next)
			{
				scroll = scroll->next;
				delete scroll->prev->val;
				delete scroll->prev;
			}

		delete scroll->val;
		delete scroll;
		this->head = this->tail = NULL;
		this->size = 0;
	}

	void erase(void)
	{
		list_node<T>* scroll = this->head;
		
		if (scroll == NULL) return;

			while (scroll->next)
			{
				scroll = scroll->next;
				delete scroll->prev;
			}
		
		
		delete scroll;
		this->head = this->tail = NULL;
		this->size = 0;
	}


	~moja_lista() { };
};



class my
{
	int a;
	char b;
	std::string c;
public:
	friend bool prrr(const my, const my);
	friend bool prrr( my* , my*);
	friend void ret(my*);
	friend void ret(my);

	my(int aa, char bb, std::string cc):a(aa),b(bb),c(cc) {};
	std::string ret() 
	{
		std::string ciag;
		ciag += std::to_string(this->a);
		ciag += "   ";
		ciag += std::to_string(this->b);
		ciag += "   ";
		ciag += this->c;
		return ciag;
	}
	bool operator < (my& dr)
	{
		return this->a < dr.a ? true : false;
	}
};
#ifdef MY_AUTO
void ret(my dr)
{
	std::string ciag;
	ciag += std::to_string(dr.a);
	ciag += "   ";
	ciag += std::to_string(dr.b);
	ciag += "   ";
	ciag += dr.c;
	std::cout << ciag << std::endl;
}
bool prrr(const my a, const my b)
{
	return (a.a == b.a && a.b == b.b && a.c == b.c) ? true : false;
}

bool prrr_c(my a, my b)
{
	return (a < b) ? true : false;
}
#endif
#ifdef MY_POINTER
void ret(my* dr)
{
	std::string ciag;
	ciag += std::to_string(dr->a);
	ciag += "   ";
	ciag += std::to_string(dr->b);
	ciag += "   ";
	ciag += dr->c;
	std::cout << ciag << std::endl;
}
bool prrr(my* a, my* b)
{
	return (a->a == b->a && a->b == b->b && a->c == b->c) ? true : false;
}
bool prrr_c(my* a, my* b)
{
	return (*a < *b) ? true : false;
}
#endif


#ifdef MY_INT
bool comp1(int a, int b)
{
	return (a < b) ? true : false;
}
bool comp2(int a, int b)
{
	return (a == b) ? true : false;
}

void printer(int a)
{
	std::cout << a << std::endl;
}
#endif

using std::cout;
using std::endl;
using std::string;


int main()
{
	const int MAX_ORDER = 6;
	//pomiar czasu
	


#ifdef MY_POINTER
	moja_lista<my*> lista;
	lista.add_tail(new my(7, 'a', "aa"));
	lista.add_tail(new my(8, 'b', "bb"));
	lista.add_head(new my(3, 'c', "cc"));
	lista.add_head(new my(4, 'd', "dd"));
	lista.add_head(new my(7, 'e', "ee"));
	lista.add_head(new my(9, 'f', "ff"));

	lista.print(ret);

	try {
		cout << lista.find_position(new my(8, 'b', "bb"), prrr) << endl;
	}
	catch (const std::invalid_argument be) {
		cout << "Not found element with this value!" << endl;
	}
	try {
		cout << lista.find_value(my(8, 'b', "bb"), prrr).ret() << endl;
	}
	catch (const std::invalid_argument be) {
		cout << "Not found element with this value!" << endl;
	}
	lista.add(new my(20, 'j', "jj"), prrr_c);

	lista.print(ret);

	lista.remove_position(3);
	lista.remove_value(new my(9, 'f', "ff"), prrr);

	cout << endl;
	cout << lista[4]->ret();
	lista[4] = new my(300, 'j', "jj");

	lista.erase_ptr();
#endif

#ifdef MY_AUTO
	moja_lista<my> lista;
	lista.add_tail(my(7, 'a', "aa"));
	lista.add_tail(my(8, 'b', "bb"));
	lista.add_head(my(3, 'c', "cc"));
	lista.add_head(my(4, 'd', "dd"));
	lista.add_head(my(2, 'e', "ee"));
	lista.add_head(my(9, 'f', "ff"));

	lista.print(ret);

	try {
		cout << lista.find_position(my(8, 'b', "bb"), prrr) << endl;
	}
	catch (const std::invalid_argument be) {
		cout << "Not found element with this value!" << endl;
	}
	try {
		cout << lista.find_value(my(8, 'b', "bb"), prrr).ret() << endl;
	}
	catch (const std::invalid_argument be) {
		cout << "Not found element with this value!" << endl;
	}
	lista.add(my(20, 'j', "jj"), prrr_c);

	lista.print(ret);

	lista.remove_position(3);
	lista.remove_value(my(9, 'f', "ff"), prrr);

	cout << endl;
	cout << lista[4].ret();
	lista[4] = my(300, 'j', "jj");

	lista.erase();
#endif

//#ifdef MY_INT
//	moja_lista<int> lista;
//	lista.add_tail(5);
//	lista.add_tail(12);
//	lista.add_head(3);
//	lista.add_head(7);
//	lista.add_tail(30);
//	lista.add(8, comp1);
//
//	
//	lista.print(printer);
//	cout << std::boolalpha << endl<< lista.find_position(12, comp2)<<endl<<endl;
//	lista[3] = 430;
//	lista.print(printer);
//	try {
//		cout << endl << lista.find_value(500, comp2);
//	}
//	catch (const std::invalid_argument be) {
//		cout << "Not found element with this value!" << endl;
//	}
//	lista.remove_position(3);
//	lista.remove_value(5, comp2);
//	lista.print(printer);
//	
//	lista.erase();
//#endif
	moja_lista<int> lista;
	for (int o = 0; o < MAX_ORDER; o++)
	{
		const int n = pow(10, o);
		clock_t  t1 = clock();
		srand(time(NULL));
		for (int i = 0; i < n; i++)
		{
			lista.add_head(rand() % 100);
		}
		clock_t  t2 = clock();

		double elapsed_secs = (double(t2) - double(t1)) / CLOCKS_PER_SEC;
		cout << endl << elapsed_secs << endl;

		const int m = pow(10, 4);

		t1 = clock();
		srand(time(NULL));
		for (int i = 0; i < m; i++)
		{
			int kk = (rand() % 100);
			lista.remove_value(2,comp2);
		}

		t2 = clock();

		elapsed_secs = (double(t2) - double(t1)) / CLOCKS_PER_SEC;
		cout << endl << elapsed_secs << endl;

		lista.erase();
	}


	//pomiar czasu
	

	

	//My klawa =  My(4, 'd', "dd");
	//cout << std::boolalpha << lista.find__element_bool(4,comp) << endl;


}