# InformationSystems
Name: Patrick Di Salvo
Student ID: 0629465

There are two main programs for this submission. A main for the B+ tree implementation and a main for the hash table implementation
To run the tree implementation type "make tree" in the terminal.
To run the hash table implementation type "make hash" in the terminal.
The default input file is testTwo.txt
You can change the input file by setting the environment variable called INPUT. e.g make tree INPUT=newFile.txt 
                                                                                    make hash INPUT=newFile.txt

When running the tree program, the data is written to Tree.txt
When running the hash program, the data is writtien to Hash.txt

press "make" or "make clean" to remove the contents of the bin folder
press "make cleanFiles" to remove TreeData.txt and hashData.txt



Discussion Question:
    The benefits of the Hash Table implementation is that it allows for O(1) insertion and O(1) retrieval for the hash table as opposed to
    O(log(n)) insertion and retrieval with the B+ tree implementation.

    The benefits of the B+ tree implementation is that it allows for range search using keys whereas the Hash Table implementation does
    not have this feature. This is beacause the B+ tree implementation stores the data to disk in a sorted order whereas the hash table 
    implementation does not. 