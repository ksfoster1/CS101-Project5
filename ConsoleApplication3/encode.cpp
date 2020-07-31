/*Kenya Foster
File: main.cpp
CS 101 Project 5
encode.cpp
*/

#include <iostream>
#include <fstream>
#include <vector> //holds 0s or 1s for code tree
#include <string>
using namespace std;

ofstream file3; //for the traversals tree.txt
struct Node
{
	char name = NULL;
	Node* left = NULL;
	Node* right = NULL;
	string data; //holds the encoding
	int freq = 0; //number of occurrences
};
class Heap
{
public:
	Node** heap; //array where the nodes are kept
	int heapsize = 0; //size of node array
	static const int count = 128;

	vector<Node*> encodings; //holds the leaf nodes

	Heap()
	{
		heap = new Node * [count];
		for (int i = 0; i < count; i++)
			heap[i] = NULL;
	}

	~Heap()
	{
		delete heap;
	}

	bool contains(char c)
	{
		bool contain = false;
		for (int i = 0; i < heapsize; i++)
		{
			if (heap[i]->name == c)
			{
				contain = true;
			}
		}
		return contain;
	}

	void buildHeap(int root, int bottom) //if adding to min heap
	{
		int parent;
		Node* temp = new Node;

		parent = (bottom - 1) / 2;
		if (heap[parent]->freq > heap[bottom]->freq)
		{
			temp = heap[parent];
			heap[parent] = heap[bottom];
			heap[bottom] = temp;
			buildHeap(root, parent);
		}
	}

	void insert(char c, int freq) //to insert into min heap
	{
		if (contains(c))
		{
			for (int i = 0; i < heapsize; i++)
			{
				if (heap[i]->name == c)
				{
					heap[i]->freq++;
					buildHeap(0, heapsize - 1);
					break;
				}
			}

		}
		else
		{
			Node* temp = new Node();
			temp->name = c;
			temp->freq = freq;

			if (heapsize > 0)
			{
				heap[heapsize] = temp;
				buildHeap(0, heapsize - 1);
				heapsize++;
			}
			else
			{
				heap[0] = temp;
				heapsize++;
			}
		}
	}

	void printCodes(string a, Node* root, int top)
	{
		string b = a;
		unsigned int n = a.size();
		// Assign 0 to left edge and recur
		if (root->left != NULL)
		{
			a.resize(n + 1);
			a[top] = '0';
			printCodes(a, root->left, top + 1);
		}

		// Assign 1 to right edge and recur
		if (root->right != NULL)
		{
			a.resize(n + 1);
			a[top] = '1';
			printCodes(a, root->right, top + 1);
		}

		if (isLeaf(root))
		{
			root->data = b;
			encodings.push_back(root);
			a = "";
		}
	}

	void preorder(Node* temp, string str, ofstream& file)
	{
		if (temp != NULL)
		{
			if (isLeaf(temp))
			{
				str += 'L';
				str += temp->name;
				str += ' ';
				file3 << "L:" << temp->name << " ";
			}
			else
			{
				str += 'I';
				str += (int)temp->name;
				str += ' ';
				file3 << "I:" << (int)temp->name << " ";
				preorder(temp->left, str, file);
				preorder(temp->right, str, file);
			}
		}
	}

	void postorder(Node* temp, string str, ofstream& file3)
	{
		if (temp != NULL)
		{
			if (isLeaf(temp))
			{
				str += 'L';
				str += temp->name;
				str += ' ';
				file3 << "L:" << temp->name << " ";
			}
			else
			{
				postorder(temp->left, str, file3);
				postorder(temp->right, str, file3);
				str += 'I';
				str += (int)temp->name;
				str += ' ';
				file3 << "I:" << (int)temp->name << " ";
			}
		}
	}

	bool isLeaf(Node* root)
	{
		bool leaf = false;
		if (root->left == NULL && root->right == NULL)
			leaf = true;
		return leaf;
	}
	void codeTree() //builds huffman code tree
	{
		int bottom = heapsize - 1;
		int parent;

		Node* temp = new Node;
		Node* temp2 = new Node;
		Node* temp3 = new Node;

		if (heapsize != 1)
		{
			parent = (bottom - 1) / 2;

			if (heap[parent]->freq > heap[bottom]->freq)
			{
				temp2->left = heap[parent]->left;
				temp2->right = heap[parent]->right;
				temp3->left = heap[bottom]->left;
				temp3->right = heap[bottom]->right;

				temp = heap[parent];
				heap[parent] = heap[bottom];
				heap[bottom] = temp;

				heap[bottom]->left = temp2->left;
				heap[bottom]->right = temp2->right;
				heap[parent]->left = temp3->left;
				heap[parent]->right = temp3->right;

				codeTree();
			}
		}
	}

	void print() //makes sure # of occurences is correct
	{
		for (int i = 0; i < heapsize; i++)
		{
			cout << heap[i]->name << heap[i]->freq << " " << endl;
		}
	}

	void buildheap2(int root, int bottom) //if extracting from min heap
	{
		int maxChild;
		int rightChild;
		int leftChild;
		Node* temp;

		leftChild = root * 2 + 1;
		rightChild = root * 2 + 2;

		if (leftChild <= bottom)
		{
			if (leftChild == bottom)
			{
				maxChild = leftChild;
			}
			else
			{
				if (heap[leftChild]->freq >= heap[rightChild]->freq)
					maxChild = rightChild;
				else
					maxChild = leftChild;
			}
			if (heap[root]->freq > heap[maxChild]->freq)
			{
				temp = heap[root];
				heap[root] = heap[maxChild];
				heap[maxChild] = temp;
				buildheap2(maxChild, bottom);
			}
		}
	}


	Node* extractMin()
	{
		Node* temp = new Node;
		temp = heap[0];
		heap[0] = heap[heapsize - 1];
		--heapsize;
		buildheap2(0, heapsize - 1);
		return temp;
	}
};

string look(char c, vector<Node*> b) //looks up and returns encoding
{
	string a;
	for (unsigned int i = 0; i < b.size(); ++i)
	{
		if (c == b[i]->name)
		{
			a = b[i]->data;
		}
	}
	return a;
}

int main(int argc, char* argv[])
{
	ifstream infile(argv[1]);
	char c;
	Heap min = Heap(); //regular min heap
	vector<char> code; //stores the letters to be coded to later

	while (infile >> noskipws >> c)
	{
		min.insert(c, 1);
		code.push_back(c);
	}

	int num3 = 1;
	while (min.heapsize > 1)
	{
		Node* name = min.extractMin();
		Node* name2 = min.extractMin();

		int num = name->freq;
		int num2 = name2->freq;

		int total = num + num2;

		Node* temp = new Node();
		temp->name = (unsigned char)num3;
		temp->freq = total;
		temp->left = name;
		temp->right = name2;
		min.heap[min.heapsize] = temp;
		min.heapsize++;

		min.codeTree();
		num3++;
	}
	Node* temp = min.heap[0];
	string a;
	int top = 0;
	min.printCodes(a, temp, top);
	vector<Node*> b = min.encodings;

	ofstream file;
	file.open("length.txt");
	for (unsigned int i = 0; i < b.size(); ++i)
		file << int(b[i]->name) << " " << b[i]->data.size() << endl;
	file.close();

	ofstream file2;
	file2.open("code.txt");
	for (unsigned int i = 0; i < code.size(); ++i)
	{
		file2 << look(code[i], b);
	}
	file2.close();

	string str;
	string str2;
	file3.open("tree.txt");
	min.preorder(temp, str, file3);
	file3 << endl;
	min.postorder(temp, str2, file3);
	file3.close();

	return 0;
}