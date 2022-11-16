/**
* Author: Jorge Raul Tzab Lopez
* Github: https://github.com/SJMA11723
* LinkedIn: https://www.linkedin.com/in/jorgetzab/
*/

#include <fstream>

using namespace std;

int main(){
    ofstream output("response.json");

    output << "{\"nombre\": \"Jorge\"}";

    ifstream input("data_servers.tsv");
    string str;
    input >> str;

    output.close();
    return 0;
}
