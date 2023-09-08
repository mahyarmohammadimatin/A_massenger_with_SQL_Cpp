## Overview
This project focuses on the development of a CPP application that integrates SQL queries and file-based database management. The application, which functions
as a Twitter-like platform, allows users to interact with a client-side interface, view menus, access and post tweets, and perform various other actions.
Users can create tweets, search for tweets, like tweets, comment on tweets, and engage in private messaging with friends.

## Features
Client-Server Communication: The application employs a client-server architecture, where the client-side application handles user requests and displays menus and tweets.

SQL Queries for Database Interaction: To manage data stored in the file-based database on server, the client application generates SQL queries and sends them to the server
for processing.

File Database Management: The server-side application is responsible for storing and managing the database files. Upon receiving valid requests from clients,
the server retrieves or updates data in the database accordingly by processing each SQL query.

## Architecture
### Overview
By client.cpp and server.cpp you can run code and no need for other files. 

application.cpp is for when you want to run the hole code in one system and ofline.

mydatabase.cpp is just contains kind of compiler for SQL queries. Feel free to use them for any other application if it needs SQL database managing

### Server (server.cpp)
The server.cpp file contains the implementation of the server component. Here's what it does:

Creates a socket to listen for incoming connections.

Listens for client connections on a specified port (e.g., port 8080).

Receives SQL-like commands from connected clients.

Parses and processes the received commands, including INSERT, SELECT, DELETE, and UPDATE operations.

Communicates with the database component to execute database operations.

Sends back the results or error messages to the clients.

### Client (client.cpp)
The client.cpp file implements the client component. Its main functions are:

Connects to the server running on a specified host and port.

Convert user inputs into SQL commands.

Sends SQL query to the server for processing.

Receives and displays the results or error messages from the server.

### Database (database.cpp)
The database.cpp file contains the implementation of the database component. It handles:

Data storage and retrieval in text files (e.g., users.txt).

Enforces data integrity by checking schema definitions.

Supports basic SQL operations like INSERT, SELECT, DELETE, and UPDATE.

Manages schema definitions in a separate file (e.g., schema.txt).

Performs data type validation and error handling.

### Application (application.cpp)
The application.cpp file is just using for local application test with no client and server.


