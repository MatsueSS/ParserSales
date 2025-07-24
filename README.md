This application is a program for tracking prices on the Pyaterochka website and an approximate forecast of the next discounts

----- The idea of ​​the application -----

I wanted to implement a convenient application that frees me from the daily routine of tracking prices. I very often have to think about what I should eat and buy. It has become quite an obsessive and boring task. I wanted to make it so that I would stop thinking about it. The application currently performs the functions of indicating the necessary product cards and subsequent notifications, as well as a forecast based on geometric distribution.

----- Tools used -----

I used C++, Python to write the program. I chose the open source library for parsing json-files nlohmann::json and jq.I also analyzed geometric distribution, which requires knowledge of statistics and probability theory.In Python I used libraries such as requests, selenium, sys, json. As a database I chose PosgtresDB

----- exceptions -----

I wrote a pretty handy module for working with the database and wrote tests for it. This module generates 2 exceptions. One when the connection is lost, and the other when the parameters are incorrect. In fact, this is a very handy module. I used parameterized queries. Now I want to discuss other modules. In other modules, I did not describe exceptions, because they look unobvious and strange there. For example, the Python script launcher module simply issues error codes, and other modules signal errors from their codes.

----- Decisions taken -----

Now I want to touch on the decisions I made. I decided to use a third-party library for reading json files because it is well written and perfectly suits my task. But I still wrote a small module for writing jq queries. It seems to me that this allows you to save on creating objects. I also tried to fit all the queries into one query in order to pass through the file as little as possible linearly.
In the BotTelegram class, I used a hash associative container. I assumed a large number of users and frequent searches for user ids. User cards are a hash container set. I decided to give preference to this container, since a large number of searches occur.
You can see that I have a similar structure to the Observer pattern. I hesitated for a long time about giving preference to this pattern, it seemed to me that I was writing a too narrowly focused user class and that it would not be suitable for future extensions. I simply do not assume them and they are not obvious to me at this stage, so I do not give preference to this

----- PyScripts -----

I wrote two python scripts. One of them makes a regular request to the site, and the other makes a request using selenium. I do a search on the resource where the captcha bypass is needed

----- how to install -----

To start using the application, you need to write the path to the directory in good_functions.hpp. Next, you need to create a .env file in the project folder. This is the main file, which should have the following location - line 1 - telegram bot token, line 2 - any data (in my case, my telegram id), line 3 - connection to the database, example - dbname=remstocks user=matsuess password=731177889232 host=localhost port=5432 client_encoding=UTF8, line 5 - message offset

That is, you must have a database created in advance, then you need to run scripts from the load folder to create tables. You also need to configure the bot. You need to go to Telegram and create a bot, and then describe the commands to it - /add_card, /forecast, /status, /del_card, /my_cards. Also, if you want to parse another site, you need to replace the Python scripts with another site

Next you need to compile the file
mkdir build
cd build
cmake ..
make

----- used instruments -----

C++/C, STL, git, cmake, libpq, libcurl, py-selenium, py-requests, jq