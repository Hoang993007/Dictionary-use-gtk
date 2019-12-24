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

**************************************************************************

1. Phần code
- Dùng soundex
- Dùng btree

2. Phần giao diện
- Dùng GTK

3. Make file được dùng để kết nối các phần của chương trình 

4. Cách chạy phần mềm
coppy file btree to convert_file_dict_to_btree and run

vào terminal 
gõ: make -f MakefileDictionary 

5. Cấu trúc file dữ liệu 
	bin: lưu file chạy
	btree
	Convert_file_dict_to_btree_file: 
		1) convert từ file từ điển "AnhViet.txt" sang file của btree "DictData.dat"
		2) tạo file btree "Dict_soundex.dat" phân loại các từ theo code có được từ hàm soundex 
	inc: file .h
	obj: file .o
	src: file .c
