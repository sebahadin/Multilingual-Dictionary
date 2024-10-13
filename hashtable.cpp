#include <iostream>
#include <sstream>
#include <cctype> 
#include <fstream>
#include "hashtable.h"
using namespace std;

Translation ::Translation(string meanings, string language)
{
    this->language = language;// Set the language
    string meaning;
    stringstream stream(meanings);// Create a string stream

    while (getline(stream,meaning,';'))// Loop to extract meanings
    {
        bool  found = false;
        for(int i = 0; i<this->meanings.size();i++)//   Check if the meaning already exists
        {
            if (this->meanings[i] == meaning)// If the meaning already exists
            {
                found = true; 
                break;
            } 
        }
        if (!found)
        {
            this->meanings.push_back(meaning);// Add the meaning to the vector
        }
    }

}
// Add a new meaning to the translation
void Translation :: addMeaning(string newMeanings)// Add a new meaning to the translation
{
    string meaning;
    stringstream stream(newMeanings);
    while (getline(stream,meaning,';'))
    {// Loop to extract meanings
        bool found = false;
        for(int i = 0; i< this->meanings.size();i++)// Check if the meaning already exists
        {
            if (this->meanings[i] == meaning)// If the meaning already exists
            {
                found = true; // Set found to true
                break;
            } 
        }
        if (!found && !meaning.empty())// If the meaning is not found and not empty
        {
            this->meanings.push_back(meaning);// Add the meaning to the vector
        }
    }

}

void Translation :: display()// Display the translation
{
    if (!(this->meanings.empty()))// Check if the meanings vector is not empty
    {
        cout<<this->language<<" : ";
        for (int i = 0; i < this->meanings.size()-1; i++)// Loop to display meanings
        {
            cout<<this->meanings[i]<<";";
        }
        cout<<this->meanings[meanings.size()-1]<<endl;// Display the last meaning
    }
}


//======================================================================================
Entry :: Entry(string word, string meanings,string language)
{
    this->word = word;
    this->translations.push_back(Translation(meanings,language));// Add the translation
    this->deleted = false;

}


void Entry:: addTranslation(string newMeanings, string language)// Add a new translation
{
    for (int i = 0; i < this->translations.size(); i++)
    {
        if (this->translations[i].language == language)// Check if the language already exists
        {
            this->translations[i].addMeaning(newMeanings);// Add the meaning to the translation
            return;
        } 
    }
    this->translations.push_back(Translation(newMeanings, language));// Add the translation
}

void Entry:: print()
{
    if (!(this->translations.empty()))// Check if the translations vector is not empty
    {
        for (int i = 0; i < this->translations.size(); i++)// Loop to display translations
        {

            this->translations[i].display();// Display the translation
        }
    }
}


HashTable :: HashTable(int capacity)
{
    buckets = new Entry*[capacity];        // Array of Pointers to Entries
    for(int i=0; i<capacity; i++)
        buckets[i] = nullptr;

    this->size =0;					   	//Current Size of HashTable
    this->capacity = capacity;			// Total Capacity of HashTable
    this->collisions = 0; 	

}


//
// //compute the hash code of a word and return the index
// unsigned long HashTable ::hashCode(string word) 
// {
//     unsigned long h = 0; // assumed to be 4 bytes = 32 bits
//     for (int i = 0; i < word.length(); i++) {
//         h = (h << 5) | (h >> 27); // 5-bit cyclic shift
//         h += (unsigned long) word[i]; // add in next character
//     }
//     return  h % this->capacity; // return index in the range 0..capacity-1
// }


//compute the hash code of a word and return the index
// unsigned long HashTable :: hashCode(string word)
// {
//     const unsigned long x = 33; // Use a prime number as base to minimize hash collisions
//     const unsigned long p = 1e9 + 9; // Use a large prime number as the modulo value
//     unsigned long h = 0; // Initialize the hash value

//     for (int i = 0; i < word.length(); i++) // Loop through the characters of the word
//      { 
//         h = (h * x + word[i]) % p; // Update the hash value
//     }
//     return h % this->capacity;// Return the computed index
// }



//compute the hash code of a word and return the index using Horner's rule
unsigned long HashTable::hashCode(string word)
{
    unsigned long h = 0; // Initialize the hash value

    for (int i = 0; i < word.length(); i++) // Loop through the characters of the word
    {
        h = (h * 131) + word[i]; // Update the hash value using a prime number as multiplier
    }

    return h % this->capacity; // Return the computed index
}



unsigned int HashTable :: getSize()// Return the size of the hash table
{
    return this->size;
    
}
unsigned int HashTable :: getCollisions()// Return the number of collisions
{
    return this->collisions;
}
void HashTable :: import(string path) {
    ifstream infile(path);
    if (infile.fail()) {
        cout << "Couldn't import data!" << endl;
        return;
    }

    string language, line;
    getline(infile, language);  // Read the language
    int count = 0;
   

    while (getline(infile, line)) {  // Loop to read each subsequent line
        stringstream stream(line);
        string word, translations, singleTranslation;
        getline(stream, word, ':');  // Extract the word before ':'
        getline(stream, translations);  // Extract all translations after ':'

        stringstream translationStream(translations);
        string allTranslations;  // This will collect all translations separated by ';'
        
        while (getline(translationStream, singleTranslation, ';')) {
            if (!allTranslations.empty()) {
                allTranslations += ";";  // Add semicolon between translations
            }
            allTranslations += singleTranslation;
        }

        insert(word, allTranslations, language);  // Insert the word with all translations
        count++;  // Increment the count
    }

    infile.close();
    cout << count << " " << language << " words have been imported successfully" << endl;
}



void HashTable :: addWord(string word, string meanings, string language)// Add a new word to the dictionary
{   
    if (meanings.empty() || word.empty() || language.empty())// Check if the input is empty
    {
        cerr<<"Invalid input"<<endl;
        return;
    }

    string result = word; // Copy the input string to avoid modifying the original
    for (size_t i = 0; i < result.length(); i++) {
        result[i] = tolower(static_cast<unsigned char>(result[i])); // Use tolower on each character
    }
    word = result;  // Update the word to the lowercase version

    int index = hashCode(word);  // Compute the bucket index
    int i = 0;
    int start_index = index;  // Save the start index
    bool exists = false;
    while (buckets[index] != nullptr)  // Loop to find the key
    {
        if (!buckets[index]->deleted && buckets[index]->word == word)  // Check for non-deleted entries
        {
            exists = true;// Set the exists flag to true
        }
        i++;
        index = (start_index + (i*i)) % this->capacity;  // Move to the next bucket
        
        if (i >=this->size)// Check if the index is greater than the size
        {
            break;
        }
    }

    insert(word, meanings, language);// Insert the word into the hash table
    if (!exists)
    {
        cout<<"new word has been successfully added to the dictionary."<<endl;// Display the success message
    }
}

// Deletes a word from the hash table
void HashTable :: delWord(string word)// Delete a word from the dictionary
{
    if (word.empty())// Check if the input is empty
    {
        cerr<<"Invalid input"<<endl;// Display the error message
        return;// Exit the function
    }

    string result = word; // Copy the input string to avoid modifying the original
    for (size_t i = 0; i < result.length(); i++) {
        result[i] = tolower(static_cast<unsigned char>(result[i])); // Use tolower on each character
    }
    word = result;  // Update the word to the lowercase version

    int index = hashCode(word);  // Compute the bucket index
    int i = -1;
    int start_index = index;  // Save the start index
    while(buckets[index] != nullptr)// Loop to find the word
    {
        i++;
        index = (start_index + (i*i)) % this->capacity ;  
        
        if (buckets[index] == nullptr || buckets[index]->deleted == true)// Check if the bucket is empty or previously deleted
        {
            cout<<word<< " not found in the dictionary."<<endl;

            return;
        }
        else if (buckets[index]->word == word)// If the word is found
        {
            buckets[index]->deleted = true;  // Mark the entry as deleted
            this->size--;  // Decrement the size
            cout<<buckets[index]->word <<" has been successfully deleted from the dictionary."<<endl;
            break;
        }
    }
}





void HashTable :: insert(string word, string meanings,string language)
{

   if (this->size == this->capacity)
   {
       cout << "Could not add " << word << ", max capacity is reached!" << endl;  // Check for full hash table
       return;
   }

    string result = word; // Copy the input string to avoid modifying the original
    for (size_t i = 0; i < result.length(); i++) {
        result[i] = tolower(static_cast<unsigned char>(result[i])); // Use tolower on each character
    }
    word = result;  // Update the word to the lowercase version


   unsigned int index = hashCode(word);  // Compute the bucket index
   bool placed = false;  // Flag to check if insertion was successful
   int i = 0;
   int start = index; // Save the start index
   while (buckets[index] != nullptr && !placed)  // Loop to find an empty or deletable bucket
   {
       i++;
       if (this->buckets[index]->deleted || this->buckets[index]->word == word) 
       {

        if (!buckets[index]->deleted && buckets[index]->word == word)
        {
            buckets[index]->addTranslation(meanings, language);// Add the translation  if the word already exists

        }
        else if(buckets[index]->deleted)
        {
            delete this->buckets[index];  // Delete the entry if it was previously deleted
            buckets[index] = new Entry(word, meanings, language); // Create a new entry
            this->size++;
        }
           placed = true;
       }
       else
       {
           this->collisions++;  // Increment collisions if we skip an occupied bucket
           index = (start + (i * i) ) % this->capacity;  // Move to the next bucket
          
           if (index == start) break; // Check to avoid infinite loop
       }
   }
   if (!placed && buckets[index] == nullptr) 
   {
        buckets[index] = new Entry(word, meanings, language);  // Create a new entry if no suitable bucket was found
        this->size++;
    }
}


// Add a new translation to the dictionary
void HashTable :: delTranslation(string word, string language)
{
    if (word.empty() || language.empty())// Check if the input is empty
    {
        cerr<<"Invalid input"<<endl;// Display the error message
        return;// Exit the function
    }
    
    string result = word; // Copy the input string to avoid modifying the original
    for (size_t i = 0; i < result.length(); i++) {
        result[i] = tolower(static_cast<unsigned char>(result[i])); // Use tolower on each character
    }
    word = result;  // Update the word to the lowercase version


    int index = hashCode(word);  // Compute the bucket index
    int i = -1;
    int start_index = index;  // Save the start index
    while(buckets[index] != nullptr)// Loop to find the word
    {
        i++;
        index = (start_index +(i*i)) % this->capacity ;  // Compute the index with linear probing
        
        if (buckets[index] == nullptr || buckets[index]->deleted == true)
        {
            cout<<word<< " does not exist"<<endl;// Check if the bucket is empty or previously deleted
            return;
        }
        else if (buckets[index]->word == word)// If the word is found
        {
            for (int j = 0; j < buckets[index]->translations.size(); j++)// Loop through the translations
            {
                if (buckets[index]->translations[j].language == language)// Check if the language exists
                {
                    buckets[index]->translations.erase(buckets[index]->translations.begin()+j); // Remove the translation

                    if (buckets[index]->translations.empty()) 
                    {
                        buckets[index]->deleted = true; // Mark the bucket as deleted if the translation vector is empty
                    }
                    cout<<"Translation has been successfully removed from the dictionary!"<<endl;// Remove the translation
                    return; // Exit after deletion
                }
            }
            break;// Exit if the language is not found
        }
    }
    cout<<language<<" not found"<<endl;// Display the language not found message
}


// Delete a meaning from the dictionary
void HashTable :: delMeaning(string word, string meaning, string language)
{
    if (word.empty() || meaning.empty() || language.empty())// Check if the input is empty
    {
        cerr<<"Invalid input"<<endl;// Display the error message
        return;// Exit the function
    }


    string result = word; // Copy the input string to avoid modifying the original
    for (size_t i = 0; i < result.length(); i++) {
        result[i] = tolower(static_cast<unsigned char>(result[i])); // Use tolower on each character
    }
    word = result;  // Update the word to the lowercase version

    int index = hashCode(word);  // Compute the bucket index
    int i = -1;// Initialize the probe index
    int start_index = index;  // Save the start index
    
    while(buckets[index] != nullptr)// Loop to find the word
    {
        i++;
        index = (start_index + (i*i)) % this->capacity ;  // Compute the index with linear probing
        
        if (buckets[index] == nullptr || buckets[index]->deleted == true)
        {
            cout<<word<< " does not exist"<<endl;// Check if the bucket is empty or previously deleted
            return;// Exit if the word is not found
        }
        else if (buckets[index]->word == word)// If the word is found
        {
            for (int j = 0; j < buckets[index]->translations.size(); j++)// Loop through the translations
            {
                if (buckets[index]->translations[j].language == language)// Check if the language exists
                {
                    for (int k = 0; k < buckets[index]->translations[j].meanings.size(); k++)// Loop through the meanings
                    {
                        if (buckets[index]->translations[j].meanings[k]==meaning)// Check if the meaning exists
                        {
                            buckets[index]->translations[j].meanings.erase(buckets[index]->translations[j].meanings.begin()+k);// Remove the meaning
                            cout<<"Meaning has been successfully removed from the dictionary!"<<endl;// Display the success message

                            if (buckets[index]->translations[j].meanings.empty()) 
                            {
                                buckets[index]->translations.erase(buckets[index]->translations.begin()+j);// Remove the translation if the meanings vector is empty
                            }
                            return;
                        }
                    }
                    cout<<"Meaning not found."<<endl;// Display the meaning not found message
                    return; // Exit if the meaning is not found
                }
            }
            cout<<language<< " not found in the dictionary"<<endl;//    Display the language not found message
            break;
        }
    }

}

// Export data to a file
void HashTable :: exportData(string language, string filePath)// Export data to a file
{
    if(language.empty() || filePath.empty())// Check if the input is empty
    {
        cerr<<"Invalid input"<<endl;// Display the error message
        return;// Exit the function
    }

    ofstream outfile(filePath);// Create an output file stream
    if (outfile.fail())
    {
        cout<<"couldn't export file"<<endl;// Check if the file could not be created
    }
    else
    {
        int count = 0;
        outfile << language << endl;// Write the language to the file

        for (int i = 0; i < this->capacity; i++) // Loop through the hash table
        {
            if (buckets[i] != nullptr && !buckets[i]->deleted) 
            {  // Check for non-deleted entries
                count++;
                outfile << buckets[i]->word << ":";

                for (int j = 0; j < buckets[i]->translations.size(); j++) // Loop through the translations
                {
                    if (buckets[i]->translations[j].language == language ) 
                    {  // Check language match and non-empty meanings
                        for (int k = 0; k < buckets[i]->translations[j].meanings.size(); k++) // Loop through the meanings
                        {
                            outfile << buckets[i]->translations[j].meanings[k];// Write the meanings to the file
                            if (k < buckets[i]->translations[j].meanings.size() - 1) 
                            {
                                outfile << ";";
                            }


                        }
                        outfile << endl;  // Use back() to avoid index out of range
                    }
                }
            }
        }

        outfile.close();    // Close the file
        cout << count << " Records have been successfully exported to " << filePath << endl;// Display the success message
    }
    

}





// Find a word in the hash table
void HashTable :: find(string word)// Find a word in the dictionary
{
    if (word.empty())// Check if the input is empty
    {
        cerr<<"Invalid input"<<endl;// Display the error message
        return;// Exit the function
    }

    string result = word; // Copy the input string to avoid modifying the original
    for (size_t i = 0; i < result.length(); i++) {
        result[i] = tolower(static_cast<unsigned char>(result[i])); // Use tolower on each character
    }
    word = result;  // Update the word to the lowercase version

    int index = hashCode(word);  // Compute the bucket index
    int i = 0;
    int start_index = index;  // Save the start index
    while (buckets[index] != nullptr)  // Loop to find the key
    {
        if (!buckets[index]->deleted && buckets[index]->word == word)  // Check for non-deleted entries
        {
            cout<<word<<+ " found in the dictionary after "<< to_string(i + 1)<<" Comparisons."<< endl;;  // Return the value and comparison count
            buckets[index]->print();// Display the translations
            return;
        }
        i++;
        index = (start_index + (i*i)) % this->capacity;  // Move to the next bucket
        
        if (i >=this->size)
        {
            break;
        }
    }
    cout<<word<< " not found...!"<<endl;  // Return not found message if the key is not present
}



HashTable :: ~HashTable()// Destructor
{
    for (int i = 0; i < this->capacity; i++)// Loop to delete the buckets
    {
        if (this->buckets[i] != nullptr)// Check if the bucket is not null
        {
            delete this->buckets[i];// Delete the bucket
        }
    }
    delete [] this->buckets;// Delete the array of buckets
}