# Multilingual Dictionary

#### Overview:
This repository implements a multilingual dictionary with support for various language pairs. It utilizes a hash table to efficiently store and retrieve word translations. The dictionary supports operations such as adding translations, retrieving translations, and managing multiple languages.

---

### `hashtable.h` and `hashtable.cpp`

1. **`HashTable`**  
   This class implements a hash table data structure designed to store key-value pairs where the key is a word in one language, and the value is the translated word in another language. The hash table supports basic operations such as insertion, lookup, and removal of translations. 

   - **Member variables:**
     - `table`: Array of linked lists that stores the translations.
     - `size`: Number of buckets in the hash table.

   - **Methods:**
   
     - **`HashTable(int tableSize)`**:  
       Constructor that initializes the hash table with a given size. It allocates an array of linked lists to store the translations.
     
     - **`unsigned int hash(const string& key)`**:  
       A private method that computes the hash of a given key (word). It uses the hash function to determine the index where the word should be stored in the hash table.
       
     - **`void insert(const string& key, const string& value)`**:  
       Inserts a key-value pair (word-translation) into the hash table. If the key already exists, it updates the existing value.
       
     - **`string search(const string& key)`**:  
       Searches for the translation of a given key (word). If the key exists, it returns the corresponding translation; otherwise, it returns an empty string or an error message.
       
     - **`void remove(const string& key)`**:  
       Removes a translation entry from the hash table for the given key (word).
       
     - **`~HashTable()`**:  
       Destructor that frees the memory allocated for the hash table and its contents.

   - **Method Descriptions:**
     
     - **`HashTable::HashTable(int tableSize)`**  
       The constructor initializes the hash table with the specified number of buckets. Each bucket is a linked list that stores the translations. The `tableSize` determines the number of buckets in the hash table.

     - **`unsigned int HashTable::hash(const string& key)`**  
       This method computes the hash value of a given key (word) using a hashing algorithm. The hash value is used to determine the index of the bucket where the key-value pair will be stored. 

     - **`void HashTable::insert(const string& key, const string& value)`**  
       Adds a new translation to the hash table. If the key already exists, the method updates the translation. This method ensures efficient storage and retrieval by using the hash value of the key to determine the storage location.

     - **`string HashTable::search(const string& key)`**  
       Searches for the translation corresponding to the given key (word) in the hash table. The method checks the bucket determined by the hash value of the key and returns the translation if it exists.

     - **`void HashTable::remove(const string& key)`**  
       Removes the translation for the specified key from the hash table. The method computes the hash of the key to find the corresponding bucket and removes the entry if it exists.

     - **`HashTable::~HashTable()`**  
       Destructor that cleans up the memory used by the hash table. It iterates over all the buckets, deleting any dynamically allocated memory associated with the linked lists storing the translations.

---

### `main.cpp`

1. **`main()`**  
   The main function of the program, which serves as the entry point for the multilingual dictionary. It sets up the hash table, loads translations from the text files, and provides a user interface for adding, searching, and removing translations.
   
   - **Steps:**
     - Initializes the hash table.
     - Loads the dictionary from files like `en-es.txt` and `en-fr.txt`, which contain English-to-Spanish and English-to-French translations, respectively.
     - Provides an interface for the user to interact with the dictionary, including adding new translations and looking up words.
     
---

### Dictionary Files

- **`en-es.txt`**  
  This file contains English-to-Spanish word translations. Each line consists of a word in English, followed by its corresponding translation in Spanish. These translations are loaded into the hash table at runtime.

- **`en-fr.txt`**  
  This file contains English-to-French word translations. Similar to the `en-es.txt` file, it lists words in English alongside their French translations.

---
