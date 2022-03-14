#include <bits/stdc++.h>
#include <stdlib.h>
using namespace std;
fstream schemafile;

//Check schema of database
void checkSchema(string table, vector<string>& schema){
    schemafile.open("Schema.txt", ios::in);
    string line;
    while(getline(schemafile, line)){
        string l1 = line.substr(0, line.find('#'));
        if(table == l1){
            string tab = line;
            int i=0;
            string curr = "";
            while(i < tab.length()){
                if(tab[i] == '#'){
                    schema.push_back(curr);
                    curr = "";
                }else
                    curr += tab[i];
                i++;
            }
            schema.push_back(curr);
            break;
        }
    }
    schemafile.close();
}

//Checking the given string is number or not
bool is_number(const std::string& s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) 
        it++;
    return !s.empty() && it == s.end();
}

//Finding datatype 
void getDatatype(string tableName,vector<string>& data, vector<string> schema){
    for(int i=2;i<schema.size();i+=2){
        data.push_back(schema[i]);
    }
}

//Insert function
void insert_Into(vector<string> cmd){
    string table_name = cmd[2];
    vector<string> schema;
    checkSchema(table_name, schema);
    if(schema.empty()){
        cout << "Table is not already exist" << endl;
        cout << "\n-----------------------------------------------------------------" << endl;
        return;
    }

    fstream table;
    table_name += ".txt";
    const char *c = table_name.c_str();
    table.open(c, ios::app);

    int start = -1, end = -1;
    for (int i=4; i<cmd.size(); i++) {
        if (cmd[i] == "(") {
            start = i;
        }
        if (cmd[i] == ")") {
            end = i;
        }
    }

    if (start == -1 || end == -1) {
        cout << "Error" << endl;
        return;
    }

    int k=0; 
    bool invalidType=false;
    int st=start;
    vector<string> data;
    getDatatype(table_name, data, schema);

    while (st < end - 1) {
        st++;
        if (cmd[st] != ",") {
            bool flag_int = is_number(cmd[st]);
            if( ( data[k]!="int" && flag_int ) || 
                ( data[k]=="int" && !flag_int )){
                cout<<"Invalid Data Type "<<cmd[st]<<endl;
                cout << "\n-----------------------------------------------------------------" << endl;
                return;
            }
            k++;
        }
    }

    while (start < end - 1) {
        start++;
        if (cmd[start] != ",") {
            table << cmd[start];
            if (start != end - 1) {
                table << "#";
            } else {
                table << "\n";
            }
        }
    }
    schemafile << endl;
    cout << "Tuple inserted successfully" << endl;
    schemafile.close();
    cout << "\n-----------------------------------------------------------------" << endl;
}
//Describe table

void describe(vector<string> cmd){
    schemafile.open("Schema.txt", ios::in);
    int flag =0;
    if (cmd.size() > 1) {
        string table_name = cmd[2];
        string line;

        while (getline(schemafile, line)) {
            string l1 = line.substr(0, line.find('#'));

            if (table_name == l1) {
                flag =1;
                cout<<"Table Name\t ---> \t"<<table_name<<endl;
                cout<<"Column\t\t"<<"Data type"<<endl; 
                string l2 = line.substr(line.find('#') + 1, line.size());
                int cnt = 1;

                for (int i=0; i<l2.length(); i++) {
                    if (l2[i] == '#') {
                        if (cnt % 2 != 0) {
                            cout << "\t-->\t";
                        } else {
                            cout << endl;
                        }
                        cnt++;
                    } else {
                        cout << l2[i];
                    }
                }
            }
        }
        if(flag == 0){
            cout << "Table is not already exist" << endl;
        cout << "\n-----------------------------------------------------------------" << endl;
        return;
        }
    } 
    else{
        cout<<"Object to be described could not be found."<<endl;
    }
    cout << "\n-----------------------------------------------------------------" << endl;
}
//Drop table

void dropTable(vector<string> cmd){
    if(cmd.size() == 2) {
        cout<<"Table name is not mentioned"<<endl;
        cout << "\n-----------------------------------------------------------------" << endl;
        return;
    }

    string table_name = cmd[2];
    vector<string> schema;
    checkSchema(table_name, schema);
    if(schema.empty()){
        cout << "Table is not already exist" << endl;
        cout << "\n-----------------------------------------------------------------" << endl;
        return;
    }

    schemafile.open("Schema.txt", ios::in);
    fstream temp;
    temp.open("temp.txt", ios::out);
    string line;

    while(getline(schemafile, line)) {
        string l1 = line.substr(0, line.find('#'));
        if (table_name != l1) {
            temp << line << endl;
        }
    }

    schemafile.close();
    temp.close();

    remove("Schema.txt");
    rename("temp.txt", "Schema.txt");
    fstream table;
    table_name += ".txt";
    const char *c = table_name.c_str();
    remove(c);
    cout << "Table dropped successfully" << endl;
    cout << "\n-----------------------------------------------------------------" << endl;
}


//Create table
void createTable(vector<string> cmd) {
    if(cmd.size() == 3){
        cout<<"Missing left parenthesis"<<endl;
        cout << "\n-----------------------------------------------------------------" << endl;
        return;
    }
    if(cmd.size() < 7){
        cout<<"Invalid indentifier"<<endl;
        cout << "\n-----------------------------------------------------------------" << endl;
        return;
    }
    string table_name = cmd[2];
    cout << table_name;
    vector<string> schema;
    checkSchema(table_name, schema);

    if(!schema.empty()){
        cout<<"Table Already Exists"<<endl;
        cout << "\n-----------------------------------------------------------------" << endl;
        return;
    }

    schemafile.open("Schema.txt", ios::app);
    schemafile << table_name;

    int start = -1, end = -1;
    for (int i = 3; i < cmd.size(); i++) {
        if (cmd[i] == "(") {
            start = i;
        }
        if (cmd[i] == ")") {
            end = i;
        }
    }

    if (start == -1 || end == -1) {
        cout << "Error" << endl;
        return;
    }

    while (start < end - 1) {
        start++;
        if (cmd[start] != ",") {
            schemafile << "#" << cmd[start];
        }
    }
    schemafile << endl;
    cout << "Table created successfully" << endl;
    fstream table;
    table_name = table_name + ".txt";
    const char *c = table_name.c_str();
    table.open(c, ios::app);
    schemafile.close();
    cout << "\n-----------------------------------------------------------------" << endl;
}







void handleCmd(vector<string> cmd){
    //Create table
    if (cmd[0] == "create" && cmd[1] == "table") {
        createTable(cmd);
    }
    else if (cmd[0] == "drop" && cmd[1] == "table") {
        dropTable(cmd);
    }
    else if (cmd[0] == "describe" && cmd[1] == "table") {
        describe(cmd);
    }
   else if (cmd[0] == "insert" && cmd[1] == "into") {
        insert_Into(cmd);
    }
}

void convertToVector(string input, vector<string> &v){
    string word = "";
    int i=0;
    while(i<input.length()){
        if(input[i] == ' '){
            v.push_back(word);
            word = "";
        }else{
            word = word + input[i];
        }
        i++;
    }
    v.push_back(word);
}

int main() {
    cout<<endl;
    cout << "\n-----------------------------------------------------------------\n" << endl;
    cout<<"\t     Group No. :- 45\t\n\tDatabase Management System\t\n"<<endl;

    cout << "\n-----------------------------------------------------------------\n" << endl;
    vector<string> cmd;
    string input;
    cout<<"Enter your SQL command: "<<endl;
    cout<<"\t\t1. create table\n\t\t2. Drop table\n\t\t3. Describe table\n\t\t4. Insert Into\n"<<endl;
    getline(cin, input);
    while (input != "Quit") {
        convertToVector(input, cmd);
        handleCmd(cmd);
        cmd.clear();
        cout<<endl;
        cout<<"Enter your SQL command: "<<endl;
        cout<<"\t\t1. create table\n\t\t2. Drop table\n\t\t3. Describe table\n\t\t4. Insert Into\n"<<endl;
        getline(cin, input);
    }
    cout << "\n-----------------------------------------------------------------" << endl;
    cout<<endl;
}
