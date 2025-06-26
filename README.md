This application is a program for tracking prices on the Pyaterochka website and an approximate forecast of the next discounts

----- The idea of ​​the application -----

I wanted to implement a convenient application that frees me from the daily routine of tracking prices. I very often have to think about what I should eat and buy. It has become quite an obsessive and boring task. I wanted to make it so that I would stop thinking about it. The application currently performs the functions of indicating the necessary product cards and subsequent notifications, as well as a forecast based on geometric distribution.

----- Tools used -----

I used C++, Python to write the program. I chose the open source library for parsing json-files nlohmann::json and jq.I also analyzed geometric distribution, which requires knowledge of statistics and probability theory.In Python I used libraries such as requests, selenium, sys, json. As a database I chose PosgtresDB

----- instructions -----

I will describe in full what is needed to launch the application and start using it. First of all, you need to install libpq, libcurl to be able to build the projecе. 

for ubuntu
sudo apt install libpq-dev libcurl4-openssl-dev


After that you need to create a database in your psql profile and 2 tables for users and product cards. 

CREATE TABLE cards (name TEXT, price INT, dicsount INT, date DATE);

CREATE TABLE users (id TEXT, cards TEXT[]);

After that, you need to create a telegram bot and get a token, and then configure this bot according to the instructions below

need create 6 commands