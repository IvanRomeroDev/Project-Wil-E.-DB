#include <fstream>
#include <string>
#include <map>
#include <iomanip>

using namespace std;

struct config{
    string region;
    string vCPU;
    int shards;
    int CN;
    int storage;
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

/// string que identifica al vCPU
string vCPU(int i);

/// reajusta el almacenamiento a multiplos de 160
void fix_storage(config &conf);

/// genera shards, almacemamiento y actualiza el precio
void update_shards(config &conf, int period);

/// genera vCPU y actualiza el precio
void update_vCPU(config &conf, int period);

/// escribe la configuracion como variables javaScript en el archivo indicado
void printConfig(const config &conf, int period, string varSuffix, ofstream &output);

int main(){
    ifstream input("data.txt");

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

    fix_storage(nearest);
    update_shards(nearest, period);
    update_vCPU(nearest, period);


    /// mas barato
    config cheaper;
    cheaper.region = "CN-Southwest-Guiyang1";

    fix_storage(cheaper);
    update_shards(cheaper, period);
    update_vCPU(cheaper, period);

    /// balance
    config balance;
    long double budget = props["user_budget"];

    fix_storage(balance);



    for(int i = 0; i < 3; ++i){ /// grupos de regiones
        config conf;
        conf = balance;
        conf.cost = 0;

        int reg_cost = i;
        for(conf.shards = 1; conf.shards < 9 && 16000 * conf.shards < conf.storage; ++(conf.shards)){
            long double c = conf.shards * storage_cost[reg_cost][period] * conf.storage;
            if(budget < conf.cost + c){
                break;
            }
        }
        conf.cost += conf.shards * storage_cost[reg_cost][period] * conf.storage;

        reg_cost = max(0, reg_cost - 1);

        int QPS = props["user_monthlyPetitions"] / segs + 1;
        long double pot2 = 1, bestCost = 0;

        bestCost = vcpu_cost[reg_cost][period] + vcpu_cost[reg_cost][period] * 3;
        for(int i = 0; i < 4; ++i){
            for(int cn = 1; cn < 10; ++cn){
                if(CPS[i] * cn < QPS) continue;

                long double new_cost = cn * vcpu_cost[reg_cost][period] * pot2 + vcpu_cost[reg_cost][period] * pot2 * 3;

                long double time = props["months"];
                if(!time) time = props["hours"];

                if( time * (conf.cost + new_cost) <= budget && bestCost < new_cost){
                    bestCost = new_cost;
                    conf.CN = cn;
                    conf.vCPU = vCPU(i);
                    //cout << "Selected: " << new_cost << ' ' << conf.cost + new_cost << '\n';
                }
            }
            pot2 *= 2;
        }

        //cout << "Conf: " << conf.cost << ' ' << bestCost << '\n';
        conf.cost += bestCost;

        if(balance.cost < conf.cost){
            balance = conf;
            if(i == 0) balance.region = "LA-Mexico-City2";
            else if(i == 2) balance.region = "AP-Jakarta";
            else balance.region = "CN-Southwest-Guiyang1";
            //cout << "New: " << balance.cost << '\n';
        }
    }



    ofstream outputJS("public/respuesta.js");

    printConfig(nearest, period, "SR", outputJS);

    printConfig(cheaper, period, "BC", outputJS);

    printConfig(balance, period, "B", outputJS);

    ifstream codeJS("editHTML.js");

    {
        string str;
        while(getline(codeJS, str)){
            outputJS << str << '\n';
        }

    }

    outputJS.close();
    codeJS.close();
    input.close();
    return 0;
}


string vCPU(int i){
    if(i == 0) return "8 vCPUs";
    if(i == 1) return "16 vCPUs";
    if(i == 2) return "32 vCPUs";
    return "64 vCPUs";
}

void fix_storage(config &conf){
    conf.storage = props["user_diskSpace"] + 1;
    conf.storage = (conf.storage + 159) / 160 * 160;
    conf.storage = max(160, conf.storage);
    conf.storage = min(144000, conf.storage);
}

void update_shards(config &conf, int period){
    for(conf.shards = 1; conf.shards < 9 && 16000 * conf.shards < conf.storage; ++(conf.shards)){
        //
    }

    /// compute shards cost
    int reg_cost = 0;
    if(conf.region == "CN-Southwest-Guiyang1") reg_cost = 2;
    else if(conf.region == "AP-Jakarta") reg_cost = 1;

    conf.cost += conf.shards * storage_cost[reg_cost][period] * conf.storage;
}

void update_vCPU(config &conf, int period){
    /// compute vCPU cost
    int reg_cost = 0;
    if(conf.region == "CN-Southwest-Guiyang1") reg_cost = 1;

    int QPS = props["user_monthlyPetitions"] / segs + 1;
    long double pot2 = 1, bestCost = 1e30;
    for(int i = 0; i < 4; ++i){
        for(int cn = 1; cn < 10; ++cn){
            if(CPS[i] * cn < QPS) continue;

            long double new_cost = cn * vcpu_cost[reg_cost][period] * pot2 + vcpu_cost[reg_cost][period] * pot2 * 3;
            if(new_cost < bestCost){
                bestCost = new_cost;
                conf.CN = cn;
                conf.vCPU = vCPU(i);
            }
        }
        pot2 *= 2;
    }
    conf.cost += bestCost;
}

void printConfig(const config &conf, int period, string varSuffix, ofstream &output){
    /// escribo las variables en el javascript
    /// mejor coincidencia
    output << "const region" + varSuffix + " = " << "\"" + conf.region + "\"" << "\n";
    output << "const vCPU" + varSuffix + " = " << "\"" + conf.vCPU + "\"" << "\n";
    output << "const shards" + varSuffix + " = " << conf.shards << "\n";
    output << "const CN" + varSuffix + " = " << conf.CN << "\n";
    output << "const storage" + varSuffix + " = " << conf.storage << "\n";
    if(period == 0){
        output << "const months" + varSuffix + " = 0\n";
        output << "const hours" + varSuffix + " = " << props["hours"] << "\n";
        output << "const cost" + varSuffix + " = " << fixed << setprecision(2) << conf.cost * props["hours"] << "\n";
    } else {
        output << "const hours" + varSuffix + " = 0\n";
        output << "const months" + varSuffix + " = " << props["months"] << "\n";

        long double c;
        if(period == 1) c = conf.cost * props["months"];
        else c = conf.cost;

        output << "const cost" + varSuffix + " = " << fixed << setprecision(2) << c << "\n";
        //cout << conf.cost << ' ' << "M: " << props["months"] << '\n';
    }

    output << "\n";
}
