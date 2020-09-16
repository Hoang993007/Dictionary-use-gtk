# Dictionary-use-gtk
Create a simple dictionary app use gtk and btree

In this app we use soundex in search function:
Soundex is a phonetic algorithm for indexing names by sound, as pronounced in English. 
Definition of soundex: https://en.wikipedia.org/wiki/Soundex

GTK+ instruction: it contain some knowledge about GTK+ that I had wrote down from website
I have here some website to guide. You can find it in "GTK - DICTIONARY APT SORCE.ods"

bin: contain the .exe file which used to run app. 
But at the last time I ran it, I can not use this file by cliking but use terminal to ran this file.

convert_file_dict_to_btree_file: The teacher give me a file .txt contain english word and definition of it in order to be the data for the dictionary.
The file I recieve is a .txt file and btree doesn't use this file so I have to convert to btree's file which btree use to store and import data to btree.

inc: I have only file dictionary.h, It my own created library

obj: contain file had been complied to .o file or binary file.

src: contain my source code.

I have some file .png and .jpg which I use as some picture and symbol for my app

**************************************************************************

1. Libs
- soundex
- btree

2. GUI
- Use GTK

3. How to run
+) copy file "btree" into file "convert_file_dict_to_btree"
+) In terminal: make -f MakefileDictionary 

4. Files 
- bin: .o file
- btree library
- Convert_file_dict_to_btree_file: 
	1) convert từ file từ điển "AnhViet.txt" sang file của btree "DictData.dat"
	2) tạo file btree "Dict_soundex.dat" phân loại các từ theo code có được từ hàm soundex 
