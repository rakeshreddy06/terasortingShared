# terasortingShared



In-Memory Sorting:

In-memory sorting is a technique in which the whole dataset to be sorted fits inside the main memory (RAM) of the computer. Because the data is wholly stored in RAM, the sorting algorithms can immediately access it, resulting in speedier sort processes. Quick Sort, Merge Sort, Bubble Sort, and Insertion Sort are examples of in-memory sorting algorithms. These algorithms make full use of RAM's random-access capabilities, allowing them to swap and compare items in real time. In-memory sorting is often appropriate for relatively small datasets when performance is crucial.

External Sorting: 

When the dataset is too vast to fit into the computer's main memory, external sorting is used. In this case, the data is often stored on an external storage device, such as a hard drive. Multiple rounds of reading and writing between main memory and external storage are required throughout the sorting process. One popular method for external sorting is to separate a huge dataset into smaller chunks that fit into main memory, sort each chunk in memory, and then fuse the sorted chunks back together. External sorting is often slower than in-memory sorting since disk I/O operations are substantially slower. External Merge Sort algorithms are particularly intended for this type of operation.

Detailed explanation of how to run code


No need for any manual installations everything is provided in the directory
Use the make command to generate the first files and  compile the c code
Then after compilation, you can execute the datasets using
./mysort  64Gb.txt
  it will generate logs  and all sorted data into it
 no need for deletion of any temp files as the code takes off after code completion
 smaller than 8GB files will be using in-memory sorting and larger files will use external sorting




OS requirements

No LSB modules are available.
Distributor ID:    Ubuntu
Description:    Ubuntu 22.04.3 LTS
Release:    22.04
Codename:    jammy

Minimum System requirements

Linux rakesh 6.2.0-35-generic #35~22.04.1-Ubuntu SMP PREEMPT_DYNAMIC Fri Oct  6 10:23:26 UTC 2 x86_64 x86_64 x86_64 GNU/Linux

Architecture:        	x86_64
  CPU op-mode(s):    	32-bit, 64-bit
  Address sizes:     	39 bits physical, 48 bits virtual
  Byte Order:        	Little Endian
CPU(s):              	4
  On-line CPU(s) list:   0-3
Vendor ID:           	GenuineIntel
  Model name:        	Intel(R) Core(TM) i5-10500H CPU @ 2.50GHz
	CPU family:      	6
	Model:           	165
	Thread(s) per core:  1
	Core(s) per socket:  4
	Socket(s):       	1
	Stepping:        	2











Note

Log files generate after the code execution on the unsorted files
Valsort and gensort are in the directory
Linsort logs can be generated using the mentioned commands in the report

Git repo link : https://github.com/rakeshreddy06/terasortingShared/edit/main/README.md
