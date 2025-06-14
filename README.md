This application is a program for tracking prices on the Pyaterochka website and an approximate forecast of the next discounts

----- The idea of ​​the application -----

I wanted to implement a convenient application that frees me from the daily routine of tracking prices. I very often have to think about what I should eat and buy. It has become quite an obsessive and boring task. I wanted to make it so that I would stop thinking about it. The application currently performs the functions of indicating the necessary product cards and subsequent notifications, as well as a forecast based on geometric distribution.

----- Tools used -----

I used C++, Python to write the program. I chose the open source library for parsing json-files nlohmann::json and jq.I also analyzed geometric distribution, which requires knowledge of statistics and probability theory.In Python I used libraries such as requests, selenium, sys, json. As a database I chose PosgtresDB

----- Selected ideas -----

I recently read Scott Myers' book and learned about universal links and I wanted to spread this idea as my own project. I thought it was a good idea, so I gave a lot of weight to universal links.

In the TelegramSender, I implemented sending and reading telegram messages as a single function.

In the TelegramUser, I implemented a sample data type for individual users. They store cards of preferred products. At the beginning of the design, it seemed to me a good idea to use the Observer pattern there, so I stuck to this idea. Perhaps in the future, something will change and this will be used more actively.

In the PyLoader, I realize load py-scripts.

In the PostgresDB, i realize work with postgresdb.

In the JsonReader, I realize read json.

In the good_function, the functions used are stored.