#include <fstream>
#include <string>

std::string get_conn(){
    std::ifstream file("../.env");
    std::string temp, conn;
    getline(file, temp);
    getline(file, temp);
    getline(file, conn);
    return conn;
}