# miniDB (C++)

- This project implements a lightweight relational database engine in C++ using B-Trees for efficient data storage and retrieval. It supports essential CRUD operations (CREATE, READ, UPDATE, DELETE) and integrates a custom SQL-like interface.

# Key functionalities include:

  - Implemented a lexer and parser to process and validate SQL-like commands, converting them into an Abstract Syntax Tree (AST).
Extended B-Tree functionality to store rows with multiple columns and enforce primary key constraints.
Designed a modular backend to handle row-level updates and deletions efficiently.
Learned:
  - Backend development and system design principles for relational database systems.
  -Advanced data structure implementation, particularly B-Trees.
  -Tokenization, parsing, and AST construction for SQL-like syntax.

# Learned
- Backend development and system design principles for relational database systems.
- Advanced data structure implementation, particularly B-Trees.
- Tokenization, parsing, and AST construction for SQL-like syntax.

# Inspiration:
- The design and implementation of miniDB drew inspiration from the following seminal works in distributed systems and storage architecture:
- [MapReduce: Simplified Data Processing on Large Clusters by Jeffrey Dean and Sanjay Ghemawat]([url](https://static.googleusercontent.com/media/research.google.com/en//archive/mapreduce-osdi04.pdf))
This paper influenced the conceptualization of data processing workflows and the efficient handling of CRUD operations within miniDB.

-[Bigtable: A Distributed Storage System for Structured Data by Fay Chang, Jeffrey Dean, Sanjay Ghemawat.]([url](https://static.googleusercontent.com/media/research.google.com/en//archive/bigtable-osdi06.pdf))
The Bigtable model's simplicity and scalability informed the approach to schema design and data management in miniDB.

# What's next?
- Implement multi-threading to improve query performance.
Add indexing for faster search and retrieval of rows.
Build a frontend interface for interactive database operations.
