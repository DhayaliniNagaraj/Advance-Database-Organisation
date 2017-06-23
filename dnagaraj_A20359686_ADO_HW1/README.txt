To Run the code:

The code is written in java.
Import the whole source code into eclipse and run the program.
The Cartesian product,Natural join and Left Outer Join of the two files are displayed in the console.


Algorithm  Used:

Cartesian Product:

Two Files are considered as two tables.
Cartesian product method returns cartesian product of the files.
Two files are read and each line of the first file is combined with each line of the second file.
Array list is used to add the first line of the first file and the first line of the second file using addAll method to produce the required result. (Concatenation of string is not used.)

Natural Join

Each row in a file is considered as a tuple and column as attribute.

Assumptions: 
First column in each file is the primary key or a common attribute on which the join is done.

The first element of the first tuple in first file is checked with the first element of all the tuples in the second file.
If the first element matches with the first element of the tuple in second file then both the tuple are added to a array list and printed.
This condition is checked for all the tuples in first file with all the tuples in the second file.
ArrayList is used to combine the tuples from first and second file.
The Natural join of  the two files is displayed.


Left Outer Join:

Each row in a file is considered as a tuple and column as attribute.

Assumptions: 
First column in each file is the primary key or a common attribute on which the join is done.

The first element of the first tuple in first file is checked with the first element of all the tuples in the second file.
If the first element matches with the first element of the tuple in second file then both the tuple are added to a array list and printed.
This condition is checked for all the tuples in first file with all the tuples in the second file.
If the tuple in first file does not have a match in the second file then the tuple is printed with null values for the other attributes.
ArrayList is used to combine the tuples from first and second file.
The Left Outer join for the two files is displayed.



