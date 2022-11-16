#include <fstream>
#include <string>
#include <map>
#include <iomanip>
#include <iostream>

using namespace std;

struct config{
    string region;
    string vCPU;
    int shards;
    int CN;
    int storage;
    int use_periodH = 0;
    int use_periodM = 0;
    long double cost = 0;
};

const string region = "user_region", approach = "user_approach";
map<string, long double> props;

const int segs = 30 * 24 * 60 * 60;
const long double vcpu_cost[2][3] = {
    {3.36, 1640, 16400},
    {3.205128, 1538.461538, 15384.615385}
};

const long double storage_cost[3][3] = {
    {0.00084, 0.6, 6},
    {0.0015, 0.642, 6.42},
    {0.000646, 0.553846, 5.538462}
};

/// coenxions per second per CNs
const int CPS[4] = {2000, 4000, 9000, 18000};

string vCPU(int i){
    if(i == 0) return "8 vCPUs";
    if(i == 1) return "16 vCPUs";
    if(i == 2) return "32 vCPUs";
    return "64 vCPUs";
}

int main(){
    ifstream input("data.txt");
    ofstream output("response.json");
    ofstream outputJS("public/respuesta.js");

    string user_region, user_approach;

    /// read input
    for(int i = 0; i < 6; ++i){
        string prop;
        input >> prop;
        if(prop == region) input >> user_region;
        else if(prop == approach) input >> user_approach;
        else {
            double value;
            input >> value;
            props[prop] = value;
        }
    }

    /// determina si es mensual, anual o pago por uso
    int period = 0;
    if(props.find("hours") == props.end()){
        if(props["months"] == 12) period = 2;
        else period = 1;
    }

    /// best match
    config nearest;
    nearest.region = user_region;
    nearest.storage = props["user_diskSpace"] + 1;
    nearest.storage = (nearest.storage + 159) / 160 * 160;
    cout << "Redondeado: " << nearest.storage << '\n';
    nearest.storage = max(160, nearest.storage);
    nearest.storage = min(144000, nearest.storage);

    for(nearest.shards = 1; nearest.shards < 9 && 16000 * nearest.shards < nearest.storage; ++(nearest.shards)){
        //
    }

    /// compute shards cost
    int reg_cost = 0;
    if(nearest.region == "CN-Southwest-Guiyang1") reg_cost = 2;
    else if(nearest.region == "AP-Jakarta") reg_cost = 1;

    nearest.cost += nearest.shards * storage_cost[reg_cost][period] * nearest.storage;

    cout << "Despues de shards: " << nearest.storage << ' ' << nearest.cost << '\n';

    /// compute vCPU cost
    reg_cost = 0;
    if(nearest.region == "CN-Southwest-Guiyang1") reg_cost = 1;

    int QPS = props["user_monthlyPetitions"] / segs + 1;
    long double pot2 = 1, bestCost = 1e30;
    for(int i = 0; i < 4; ++i){
        for(int cn = 1; cn < 10; ++cn){
            if(CPS[i] * cn < QPS) continue;

            long double new_cost = cn * vcpu_cost[reg_cost][period] * pot2 + vcpu_cost[reg_cost][period] * pot2 * 3;
            if(new_cost < bestCost){
                bestCost = new_cost;
                nearest.CN = cn;
                nearest.vCPU = vCPU(i);
            }
        }
        pot2 *= 2;
    }
    cout << "Servidor: " << reg_cost << '\n';
    cout << "Despues de vCpu: " << nearest.cost << ' ' << bestCost << '\n';
    nearest.cost += bestCost;


    /// mas barato
    config cheaper;
    cheaper.region = "CN-Southwest-Guiyang1";
    cheaper.region = user_region;
    cheaper.storage = props["user_diskSpace"] + 1;
    cheaper.storage = (cheaper.storage + 159) / 160;
    cheaper.storage = max(160, cheaper.storage);
    cheaper.storage = min(144000, cheaper.storage);

    for(cheaper.shards = 1; cheaper.shards < 9 && 16000 * cheaper.shards < cheaper.storage; ++(cheaper.shards)){
        //
    }


    cout << "Costo total: " << nearest.cost << '\n';

    output << "{";

    output << "\"region\": " << "\"" + nearest.region + "\"" << ",\n";
    output << "\"vCPU\": " << "\"" + nearest.vCPU + "\"" << ",\n";
    output << "\"shards\": " << nearest.shards << ",\n";
    output << "\"CN\": " << nearest.CN << ",\n";
    output << "\"storage\": " << nearest.storage << ",\n";
    if(period == 0){
        output << "\"hours\" : " << props["hours"] << ",\n";
        output << "\"cost\" : " << fixed << setprecision(2) << nearest.cost * props["hours"];
    } else if(period == 1){
        output << "\"months\" : " << props["months"] << ",\n";
        output << "\"cost\" : " << fixed << setprecision(2) << nearest.cost * props["months"];
        cout << nearest.cost << ' ' << "M: " << props["months"] << '\n';
    } else {
        output << "\"yearly\" : " << 1 << ",\n";
        output << "\"cost\" : " << fixed << setprecision(2) << nearest.cost * 10;
    }

    output << "}";

    /// escribo las variables en el javascript
    outputJS << "const regionSR = " << "\"" + nearest.region + "\"" << "\n";
    outputJS << "const vCPUSR = " << "\"" + nearest.vCPU + "\"" << "\n";
    outputJS << "const shardsSR = " << nearest.shards << "\n";
    outputJS << "const CNSR = " << nearest.CN << "\n";
    outputJS << "const storageSR = " << nearest.storage << "\n";
    if(period == 0){
        outputJS << "const hoursSR = " << props["hours"] << "\n";
        outputJS << "const costSR = " << fixed << setprecision(2) << nearest.cost * props["hours"] << "\n";
    } else if(period == 1){
        outputJS << "const monthsSR = " << props["months"] << "\n";
        outputJS << "const costSR = " << fixed << setprecision(2) << nearest.cost * props["months"] << "\n";
        cout << nearest.cost << ' ' << "M: " << props["months"] << '\n';
    } else {
        outputJS << "const yearlySR = " << 1 << "\n";
        outputJS << "const costSR = " << fixed << setprecision(2) << nearest.cost * 10 << "\n";
    }

    ifstream codeJS("editHTML.js");

    {
        string str;
        while(getline(codeJS, str)){
            outputJS << str << '\n';
        }

    }

    codeJS.close();
    outputJS.close();
    input.close();
    output.close();
    return 0;
}
