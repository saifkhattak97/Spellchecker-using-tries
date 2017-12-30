#include <iostream>
#include <string>
#include <fstream>

using namespace std;

#define ALPHASIZE 26 //The number of alphabets - Size of Nodal Array
#define index ((int)word[i]-(int)'a') // The index of nodal array where the alphabet is to be added.



/*
Create Trie
Insert Word to Dictionary Trie
Read a txt file
Compare word by word with dictionary 
Flags errors
Provides Suggestions
Shows Mistake Density
*/

string UppertoLower(string word){   //converts upper case letters to lowercase letters
	for (int i = 0; i < word.length(); i++){
		if (word[i]>64 && word[i] < 91){
			word[i] += 32;
		}
	}
	return word;
}

class node{  //Node class for trie
public:
	node* parent; //Allows child to parent access
	bool isEnd; // marks the last node for a given word
	node * child[ALPHASIZE]; // 26 alphabet nodes as 26 children
	char letter; // letter stored in a given note
};

class trie{
public:
	node *root; //root node of trie

	trie(){  //initializes rootnode
		root = new node();
		root->parent = NULL;
		root->isEnd = false;
		root->letter = NULL;
	}

	node *NewNode(void) //creation of new node for trie - Instatiation to NULL.
	{
		node *NNode = new node;

		NNode->isEnd = false;
		for (int i = 0; i < ALPHASIZE; i++){
			NNode->child[i] = NULL;
		}
		return NNode;
	}

	/*Inserting node function. Checks if a given child is empty. 
	If yes, creates a new node, assigns letter of a given string to node and makes parent child connection
	Takes word as input. Puts it into nodes letterwise. Traverses	*/
	void insert(string word){
		node* current = root;
		for (int i = 0; i < word.length(); i++){
			if (current->child[index] == NULL){
				current->child[index] = NewNode();  
				current->child[index]->letter = word[i];
				current->child[index]->parent = current;
			}
			current = current->child[index];
		}
		current->isEnd = true;
	}

	//Shows whether a given word exists in the trie or not. Compares the children and the ending.
	bool search(string word){
		node *current = root;
		for (int i = 0; i < word.length(); i++){
			if (current->child[index] == NULL){ return false; }
			current = current->child[index];
		}
		return current->isEnd;
	}

	
	//Inserts a given word into the dictionary file as well as the trie.
	void ActualInsert(string word){
		fstream file;
		file.open("words_alpha.txt",ios::app);
		file <<"\n" <<word;
		insert(UppertoLower(word));
		cout << word << " added to Dictionary";
	}

	//Finds the node where the mistake in word arises. 
	//It is the node where the deviation of paths occur but the forward path doesn't coincide with the provided paths
	node *mistake(string word) { 
		node *current = root;
		for (int i = 0; i < word.length(); i++) {
			if (current->child[index] == NULL) { return current; }
			current = current->child[index];
		}
		if (current->isEnd) return NULL;
		else return current;
	}
	
	//Checks if the word entered is a solo number
	bool isnumber(string word) {
		for (int i = 0; i < word.size(); ++i) {
			if (!isdigit(word[i])) return false;
		}
		return true;
	}

	//Checks if spelling of word is correct or incorrect
	bool WordSpellingChecker(string word) {
		if (isnumber(word)) {
			cout << "Correct";
			return 0;
		}
		for (int i = 0; i < word.length(); i++) {
			if (!isalpha(word[i])) {
				cout << "Incorrect";
				return 1;
			}
		}
		if (this->search(word)) {
			cout << "Correct";
			return 0;
		}
		else {
			cout << "Incorrect";
			return 1;
		}
	}

	//Checks if all the children are empty
	bool AllClear(node* current){
		int count = 0;
		for (int i = 0; i < 26; i++){
			if (current->child != NULL){
				count++;
			}
		}
		if (count>0){ return false; }
		else{ return true; }
	}

	//Prints trie
	void print(class node * root){
		node* current = root;

		for (int i = 0; i < 26; i++) {
			if (current->child[i] != NULL){
				print(current->child[i]);
				if (current->child[i]->isEnd){
					cout << getWord(current->child[i]) << endl;
				}
			}
		}
	}

	//recursively traverses from base (ending child) to parent and provides the word 
	//Concatenates letters
	string getWord(node *base) {
		if (base->parent == NULL) return "";
		return getWord(base->parent) + base->letter;
	}


	//Provides all possible suggestions for words with mistakes by printing the subtree after the mistake node
	void suggest(string word) {
		node *current = mistake(word);
		if (current) {
			cout << "\n--------SUGGESTIONS!---------" << endl;
			print(current);
		}
		else {
			cout << "\nNo suggestions." << endl;
		}

	}

};

// Fills trie with words from a dictionary file
void FillTrie(trie Trie){
	cout << "\t\t\t Filling the Trie with words.." << endl<<endl;
	fstream file;
	file.open("words_alpha.txt");
	if (!file.is_open()){ cout << "File can't be opened"; return; }
	string word;
	while (file >> word)
	{
		Trie.insert(word);
	}
}

//Checks spellings of the data in file word by word
void SpellChecker(fstream& file, trie Dictionary) { 
	float mistakes = 0; //counter for no of mistakes
	float density; // Shows density of mistakes
	cout << endl; 
	string word;
	float count = 0; //counts no of words 
	cout << "Sentence Entered: ";
	file.open("dummy.txt"); 
	while (file >> word) {
		cout << word << " "; //Prints the data in file
	}
	cout << endl << endl;
	file.clear(); //Overwrites the current value of flag
	file.seekg(0, ios::beg); // set ptr to beginning
	cout << "-----ANALYSIS-----" << endl << endl;
	file >> word;
	do
	{	//ignores the punctuations
		int beg = word.find_first_not_of(",.!-:;\'\"?*()&/#"); 
		if (beg == string::npos) { continue; }
		word = word.substr(beg);
		int end = word.find_first_of(",.!-:;\'\"?*()&/#");
		word = word.substr(0, end);
		count++;
		cout << word << ": ";
		if (Dictionary.WordSpellingChecker(UppertoLower(word)) == 1) { //If mistake, increment counter. Asks for addition of words to dictionary
			mistakes++;
			char c;
			cout << endl << "Do you want to insert " << word << " to your dictionary? [y if yes]: ";
			cin >> c;
			if (c == 'y'){
				Dictionary.ActualInsert(UppertoLower(word));
				mistakes--;
			}
			cout << endl;
			Dictionary.suggest(UppertoLower(word));
		}
		cout << endl << endl;
	} while (file >> word);
	file.close();
	density = (mistakes / count) * 100;
	cout << endl << "Total mistakes:" << mistakes << endl;
	cout << "Total Words:" << count << endl;
	cout << "Mistake Density: " << density << "%" << endl;
}

void menu(){ //Menu function
	cout << "We have the following options for you:" << endl << endl;
	cout << "1. Check Word Spelling." << endl;
	cout << "2. Read a File." << endl;
	cout << "3. Insert Word To Dictionary." << endl;
	cout << "4. Print Dictionary." << endl;
	cout << "5. Exit." << endl;
}

void WordSpelling(trie Dictionary){ //Checks spelling and provides suggestions for a single word.
	string word;
	cout << endl << "Enter a word: ";
	cin >> word;
	Dictionary.WordSpellingChecker(UppertoLower(word));
	cout << endl;
	cout << endl << endl;
	Dictionary.suggest(UppertoLower(word));
}


int main(){
	cout << "\t\t -------WELCOME TO SAIF's SPELLCHECKER PROJECT!-------" << endl << endl << endl;
	fstream file; //declare file
	trie Dictionary; // Declare dictionary Trie
	FillTrie(Dictionary); //Fill Dictionary Trie
	int choice;
	
	//Menu with back and forth movement

	do{
		menu();
		cout << "Make a choice: ";
		cin >> choice;

		switch (choice){
		case(1) : {		system("cls");
					  WordSpelling(Dictionary);
					  break;
		}
		case(2) : {		system("cls");
					  SpellChecker(file, Dictionary);
					  break;
		}
		case(3) : {	   system("cls");
			string word;
			cout << "Enter Word to be Inserted: ";
			cin >> word;
			Dictionary.ActualInsert(UppertoLower(word));
					  break;
		}
		case(4) : {
					  Dictionary.print(Dictionary.root); //Takes 2:59:82 m:s:ms on my PC. 
					  break;
		}
		case(5) : {
					  break;
		}
		default:{	system("cls");
					cout << "Incorrect Choice" << endl;
					break;
		}
		}
		if (choice != 5){
			char bf;
			cout << endl << "Press < to go back or any other key to exit: ";
			cin >> bf;
			if (bf != '<'){ break; }
		}
		system("cls");
	} while (choice != 5);

	cout << "What is dead may never die!" << endl; // Project Slogan
	system("pause");
}
