#include <iostream>
#include <fstream>
#include <ios>
#include <cassert>

#include "PostgresDB.h"
#include "good_function.hpp"

std::ofstream file(path+"res/test_PostgresDB.txt", std::ios_base::app);

void error_connection(PostgresDB& db, const std::string& conn, const std::string& num_test){
    try{
        db.connect(conn);
    } catch(BadConnectionDBexception& e) {
        file << "Error connection " << num_test << '\n';
    }
}

//Create a test table
void test1_create_test_table()
{
    file << "----------\n";
    std::string conn = get_conn(), str = "test1_create_table_1";
    PostgresDB db;
    error_connection(db, conn, str);
    bool result = false;
    try{
        result = db.execute(std::string("CREATE TABLE IF NOT EXISTS test (id TEXT);"), std::vector<std::string>{});
    } catch(BadConnectionDBexception& e){
        db.connect(conn);
        result = db.execute(std::string("CREATE TABLE IF NOT EXISTS test (id TEXT);"), std::vector<std::string>{});
    } catch(ErrorQueryResultDBexception& e){
        result = false;
    }
    assert(result == true);
    file << "Good query 1 " << str << '\n';
    assert(db.is_connect());
    db.close();
    assert(!db.is_connect());
    file << "----------\n";
}

//Delete a test table
void test2_drop_test_table()
{
    file << "----------\n";
    std::string conn = get_conn(), str = "test2_drop_table_1";
    PostgresDB db;
    error_connection(db, conn, str);
    bool result = false;
    try{
        result = db.execute(std::string("DROP TABLE test;"), std::vector<std::string>{});
    } catch(BadConnectionDBexception& e) {
        db.connect(conn);
        result = db.execute(std::string("DROP TABLE test;"), std::vector<std::string>{});
    } catch(ErrorQueryResultDBexception& e){
        result = false;
    }
    assert(result == true);
    file << "Good query 2 " << str << '\n';
    assert(db.is_connect());
    db.close();
    assert(!db.is_connect());
    file << "----------\n";
}

//insert a test value
void test3_append_data_table()
{
    file << "----------\n";
    std::string data = "test", conn = get_conn(), str = "test3_append_data_1";
    PostgresDB db;
    error_connection(db, conn, str);
    bool result = false;
    try{
        result = db.execute(std::string("INSERT INTO test (id) VALUES ($1);"), std::vector<std::string>{data});
    } catch(BadConnectionDBexception& e){
        db.connect(conn);
        result = db.execute(std::string("INSERT INTO test (id) VALUES ($1);"), std::vector<std::string>{data});
    } catch(ErrorQueryResultDBexception& e){
        result = false;
    }
    assert(result == true);
    file << "Good query 3 " << str << '\n';
    assert(db.is_connect());
    db.close();
    assert(!db.is_connect());
    file << "----------\n";
}

//take a test value
void test4_take_test_data()
{
    file << "----------\n";
    PostgresDB db;
    std::string conn = get_conn(), str = "test4_take_data_1";
    error_connection(db, conn, str);
    std::vector<std::vector<std::string>> res;
    try{
        res = db.fetch(std::string("SELECT * FROM test;"), std::vector<std::string>{});
    } catch(BadConnectionDBexception& e) {
        db.connect(conn);
        res = db.fetch(std::string("SELECT * FROM test;"), std::vector<std::string>{});
    } catch(ErrorQueryResultDBexception& e){
        std::cout << e.what() << '\n';
        res = db.fetch(std::string("SELECT * FROM test;"), std::vector<std::string>{});
    }
    assert(res[0][0] == "test");
    file << "Good query 4 " << str << '\n';
    file << "----------\n";
}

//delete a test value
void test5_delete_from_table()
{
    file << "----------\n";
    PostgresDB db;
    std::string conn = get_conn(), test = "test", str = "test5_delete_1";
    error_connection(db, conn, str);
    bool result = false;
    try{
        result = db.execute(std::string("DELETE FROM test WHERE id = $1;"), std::vector<std::string>{test});
    } catch(BadConnectionDBexception& e){
        db.connect(conn);
        result = db.execute(std::string("DELETE FROM test WHERE id = $1;"), std::vector<std::string>{test});
    } catch(ErrorQueryResultDBexception& e){
        result = false;
    }
    assert(result == true);
    file << "Good query 5 " << str << '\n';
    file << "----------\n";
}

//bad sql value-type
void test6_no_valid_query_1()
{
    file << "----------\n";
    PostgresDB db;
    std::string conn = get_conn(), str = "test6_no_valid_1";
    error_connection(db, conn, str);
    bool result = false;
    try{
        result = db.execute(std::string("CREATE TABLE test (id TNT);"), std::vector<std::string>{});
    } catch(BadConnectionDBexception& e){
        db.connect(conn);
        result = db.execute(std::string("CREATE TABLE test (id TNT);"), std::vector<std::string>{});
    } catch(ErrorQueryResultDBexception& e){
        file << "exception1 was caught\n";
        result = false;
    }

    assert(result == false);
    file << "Good query 6 " << str << '\n';
    file << "----------\n";
}

//lose brackets
void test7_no_valid_query_2()
{
    file << "----------\n";
    PostgresDB db;
    std::string conn = get_conn(), str = "test7_no_valid_2";
    error_connection(db, conn, str);
    bool result = false;
    try{
        result = db.execute(std::string("CREATE TABLE test id INT;"), std::vector<std::string>{});
    } catch(BadConnectionDBexception& e){
        db.connect(conn);
        result = db.execute(std::string("CREATE TABLE test id INT;"), std::vector<std::string>{});
    } catch(ErrorQueryResultDBexception& e){
        file << "exception2 was caught\n";
        result = false;
    }
    assert(result == false);
    file << "Good query 7 " << str << '\n';
    file << "----------\n";
}

//wrong value
void test8_no_valid_query_3()
{
    file << "----------\n";
    PostgresDB db;
    std::string conn = get_conn(), str = "test8_no_valid_3";
    error_connection(db, conn, str);
    std::vector<std::vector<std::string>> result;
    try{
        result = db.fetch(std::string("SELECT name WHERE id = 1;"), std::vector<std::string>{});
    } catch(BadConnectionDBexception& e){
        db.connect(conn);
        result = db.fetch(std::string("SELECT name WHERE id = 1;"), std::vector<std::string>{});
    } catch(ErrorQueryResultDBexception& e){
        file << "exception3 was caught\n";
    }
    assert(result.empty());
    file << "Good query 8 " << str << '\n';
    file << "----------\n";
}

//extra value
void test9_no_valid_query_4()
{
    file << "----------\n";
    PostgresDB db;
    std::string conn = get_conn(), str = "test9_no_valid_4";
    error_connection(db, conn, str);
    std::vector<std::vector<std::string>> result;
    try{
        result = db.fetch(std::string("SELECT id, name, FROM test;"), std::vector<std::string>{});
    } catch(BadConnectionDBexception& e){
        db.connect(conn);
        result = db.fetch(std::string("SELECT id, name, FROM test;"), std::vector<std::string>{});
    } catch(ErrorQueryResultDBexception& e){
        file << "exception4 was caught\n";
    }
    assert(result.empty());
    file << "Good query 9 " << str << '\n';
    file << "----------\n";
}

//wrong sql query
void test10_no_valid_5()
{
    file << "----------\n";
    PostgresDB db;
    std::string conn = get_conn(), str = "test10_no_valid_5";
    error_connection(db, conn, str);
    bool result = false;
    try{
        result = db.execute(std::string("GIVEME ALL FROM test;"), std::vector<std::string>{});
    } catch(BadConnectionDBexception& e){
        db.connect(conn);
        result = db.execute(std::string("GIVEME ALL FROM test;"), std::vector<std::string>{});
    } catch(ErrorQueryResultDBexception& e){
        file << "exception5 was caught\n";
        result = false;
    }
    assert(result == false);
    file << "Good  query 10 " << str << '\n';
    file << "----------\n";
}

//lose connect
void test11_lose_connect()
{
    file << "----------\n";
    PostgresDB db;
    std::string conn = get_conn(), str = "test11_lose_connect";
    // error_connection(db, conn, str);
    std::vector<std::vector<std::string>> result;
    try{
        result = db.fetch(std::string("SELECT * FROM test;"), std::vector<std::string>{});
    } catch(BadConnectionDBexception& e){
        file << "exception6 was caught\n";
    } catch(ErrorQueryResultDBexception& e){
        file << "Bad error\n";
    }

    assert(result.empty());
    file << "Good query 11 " << str << '\n';
    file << "----------\n";
}

//not exist table
void test12_no_valid_query_6()
{
    file << "----------\n";
    PostgresDB db;
    std::string conn = get_conn(), str = "test12_no_valid_6";
    error_connection(db, conn, str);
    std::vector<std::vector<std::string>> result;
    try{
        result = db.fetch(std::string("SELECT * FROM not_exist;"), std::vector<std::string>{});
    } catch(BadConnectionDBexception& e){
        db.connect(conn);
        result = db.fetch(std::string("SELECT * FROM not_exist;"), std::vector<std::string>{});
    } catch(ErrorQueryResultDBexception& e){
        file << "exceprion7 was caught\n";
    }

    assert(result.empty());
    file << "Good query 12 " << str << '\n';
    file << "----------\n";
}

//wrong value
void test13_no_valid_query_7()
{
    file << "----------\n";
    PostgresDB db;
    std::string conn = get_conn(), str = "test13_no_valid_7";
    error_connection(db, conn, str);
    std::vector<std::vector<std::string>> result;
    try{
        result = db.fetch(std::string("SELECT name FROM test;"), std::vector<std::string>{});
    } catch(BadConnectionDBexception& e){
        db.connect(conn);
        result = db.fetch(std::string("SELECT name FROM test;"), std::vector<std::string>{});
    } catch(ErrorQueryResultDBexception& e){
        file << "exception8 was caught\n";
    }
    assert(result.empty());
    file << "Good query 13 " << str << '\n';
    file << "----------\n";
}

//not a hand over params
void test14_no_valid_query_8()
{
    file << "----------\n";
    PostgresDB db;
    std::string conn = get_conn(), str = "test14_no_valid_8";
    error_connection(db, conn, str);
    bool result = false;
    try{
        result = db.execute(std::string("INSERT INTO test (id, name) VALUES ($1, $2)"), std::vector<std::string>{});
    } catch(BadConnectionDBexception& e){
        db.connect(conn);
        result = db.execute(std::string("INSERT INTO test (id, name) VALUES ($1, $2)"), std::vector<std::string>{});
    } catch(ErrorQueryResultDBexception& e){
        file << "exception9 was caught\n";
        result = false;
    }
    assert(result == false);
    file << "Good query 14 " << str << '\n';
    file << "----------\n";
}

//wrong count args
void test15_no_valid_query_9()
{
    file << "----------\n";
    PostgresDB db;
    std::string conn = get_conn(), str = "test15_no_valid_9";
    error_connection(db, conn, str);
    bool result = false;
    try{
        result = db.execute(std::string("DELETE FROM test WHERE id = $1 AND name = $2;"), std::vector<std::string>{});
    } catch(BadConnectionDBexception& e){
        db.connect(conn);
        result = db.execute(std::string("DELETE FROM test WHERE id = $1 AND name = $2;"), std::vector<std::string>{});
    } catch(ErrorQueryResultDBexception& e){
        file << "exception10 was caught\n";
        result = false;
    }
    assert(result == false);
    file << "Good query 15 " << str << '\n';
    file << "----------\n";
}

void run_all_tests()
{
    //-----
    test1_create_test_table();

    //-----
    test3_append_data_table();

    //-----
    test4_take_test_data();

    //-----
    test5_delete_from_table();

    //-----
    test6_no_valid_query_1();

    //-----
    test7_no_valid_query_2();

    //-----
    test8_no_valid_query_3();

    //-----
    test9_no_valid_query_4();

    //-----
    test10_no_valid_5();

    //-----
    test11_lose_connect();

    //-----
    test12_no_valid_query_6();

    //-----
    test13_no_valid_query_7();

    //-----
    test14_no_valid_query_8();

    //-----
    test15_no_valid_query_9();

    //-----
    test2_drop_test_table();
}

int main(void)
{
    run_all_tests();
}