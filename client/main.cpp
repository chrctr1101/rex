#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include "tools.hpp"

std::vector<server::IP> ip;

static bool check;
static std::string mock;

std::ifstream fileI ("ips", std::ios::out);
std::ofstream fileO ("ips", std::ios::app);

void init() {
    if(!fileO.is_open() || !fileI.is_open()) exit(1);
    std::string input;
    while (std::getline(fileI, input)) {
        std::string name, ip_;
        for (int i = 0; i < input.length(); i++) {
            if (input[i] == ',') {
                name = input.substr(0,i);
                ip_ = input.substr(i+1);
                break;
            }
        }
        server::IP temp{name, ip_};
        ip.push_back(temp);
    }
}

void out() {
    std::cout << "Enter your operation\n\n";
    std::cout << "[1] - Connect to server\n";
    std::cout << "[2] - Copy to server\n";
    std::cout << "[3] - Copy from server\n";
    std::cout << "[4] - Execute program on server\n";
    std::cout << "[5] - Settings\n";
    std::cout << "[6] - Check logs\n\n";
    std::cout << "[0] - Quit program" << std::endl;
}

int main () {


    while (true) {
        init();
        out();
        int var;
        std::cin >> var;

        switch(var) {
            case(0): { exit(0); }
            case(1): {
                std::cout << "Enter name of server:\n";

                for (int i = 0; i < ip.size(); i++) {
                    std::cout << ip[i] << std::endl;
                }

                std::string name_;
                std::cin.ignore();
                std::getline(std::cin, name_);

                bool found = false;

                for (int i = 0; i < ip.size(); i++) {
                    if (name_ == ip[i].name) {
                        found = true;
                        server::connect(ip[i]);
                        break;
                    }
                }

                if (!found) { std::cerr << "Name of server not found!" << std::endl; }

                break;
            }
            case(2): {
                std::cout << "Enter name of file/folder" << std::endl;
                std::string fileName;
                std::cin.ignore();
                std::getline(std::cin, fileName);

                std::cout << "Enter name of server:\n";

                for (int i = 0; i < ip.size(); i++) {
                    std::cout << ip[i] << std::endl;
                }

                std::string name_;
                std::cin.ignore();
                std::getline(std::cin, name_);

                int it;
                bool found = false;

                for (int i = 0; i < ip.size(); i++) {
                    if (name_ == ip[i].name) {
                        found = true;
                        it = i;
                        break;
                    }
                }

                if (!found) { std::cerr << "Name of server not found!" << std::endl; }

                std::string archName = fileName + ".tar.gz";

                server::packArch(archName, fileName);
                server::pushArch(ip[it], archName);

                break;
            }
            case(3): {
                std::cout << "Enter name of file/folder" << std::endl;
                std::string fileName;
                std::cin.ignore();
                std::getline(std::cin, fileName);

                std::cout << "Enter name of server:\n";

                for (int i = 0; i < ip.size(); i++) {
                    std::cout << ip[i] << std::endl;
                }

                std::string name_;
                std::cin.ignore();
                std::getline(std::cin, name_);

                int it;
                bool found = false;

                for (int i = 0; i < ip.size(); i++) {
                    if (name_ == ip[i].name) {
                        found = true;
                        it = i;
                        break;
                    }
                }

                if (!found) { std::cerr << "Name of server not found!" << std::endl; }

                server::pullArch(ip[it], fileName);
                server::unpackArch(fileName);

                break;
            }
            case(4): {
                std::cout << "Enter name of server:\n";

                for (int i = 0; i < ip.size(); i++) {
                    std::cout << ip[i] << std::endl;
                }
                std::cin.ignore();
                std::string name_;
                std::getline(std::cin, name_);

                bool found = false;
                int it;
                for (int i = 0; i < ip.size(); i++) {
                    if (name_ == ip[i].name) {
                        found = true;
                        it = i;
                        break;
                    }
                }

                if (!found) { std::cerr << "Name of server not found!" << std::endl; }

                std::cout << "What do you want to do?\n\n" <<
                "[F]ind file or directory\n" <<
                "[C]reate reserve copy to mounted USB device\n" <<
                "[R]eboot server\n" <<
                "[U]pdate server's kernel (not recomended)\n" <<
                "[P]ersonal command (form shell)\n" <<
                "[B]ack to menu\n\n";

                char ch;
                std::cin >> ch;
                if (toupper(ch) == 'F') {
                    std::string find;
                    std::cout << "What do you want to find?" << std::endl;
                    std::getline(std::cin, find);

                    server::remoteExec(ip[it], find, 'f');
                }
                else if (toupper(ch) == 'C') {
                    std::cout << "Creating reserve copy" << std::endl;

                    server::remoteExec(ip[it], mock, 'c');
                }
                else if (toupper(ch) == 'R') {
                    std::cout << "Rebooting server. Be sure that you save your important data" << std::endl;

                    server::remoteExec(ip[it], mock, 'r');
                }
                else if (toupper(ch) == 'U') {
                    std::cout << "Are you sure that you need to update server's system? - [y]/[N]" << std::endl;
                    char ans;
                    std::cin >> ans;
                    if (toupper(ans) == 'Y') {
                        server::remoteExec(ip[it], mock, 'u');
                    } 
                    else break;
                }
                else if (toupper(ch) == 'P') {
                    std::cout << "Enter your command: ";
                    std::string cmd;
                    std::cin.ignore();
                    std::getline(std::cin, cmd);

                    server::remoteExec(ip[it], cmd, 'p');
                }
                else if (toupper(ch) == 'B') break;
                else { std::cerr << "Unexpected symbol" << std::endl; }
                break;
            }
            case(5): {
                std::cout << 
                "What do you need?\n\n" <<
                "[A]dd new connection\n" << 
                "[D]elete connection\n" << 
                "[P]ing server\n" <<
                "[L]ist of connections\n" <<
                "[R]ecreate server functional scripts\n" << 
                "[B]ack\n" << std::endl;
                char ch;
                std::cin >> ch;

                if (toupper(ch) == 'A') {
                    std::cout << "Name of new connection: ";
                    std::string name_;
                    std::cin.ignore();
                    std::getline(std::cin, name_);
                    std::cout << "\nNew IP address: ";
                    std::string ip_;
                    std::getline(std::cin, ip_);

                    server::IP temp = {name_, ip_};

                    if(!server::pingServer(temp)) break;

                    ip.push_back(temp);

                    server::updateList(ip);
                }
                else if (toupper(ch) == 'D') {
                    std::cout << "Enter name of server: ";
                    std::cin.ignore();
                    std::string name_;
                    std::getline(std::cin, name_);
                    server::forgetServer(ip, name_);
                }
                else if (toupper(ch) == 'P') {
                    std::cout << "Name of server: ";
                    std::string name_;
                    std::cin.ignore();
                    std::getline(std::cin, name_);

                    int id;

                    for (int i = 0; i < ip.size(); i++) {
                        if (ip[i].name == name_) {
                            id = i;
                        }
                    }

                    check = server::pingServer(ip[id]);

                    if(!check) break;
                }
                else if (toupper(ch) == 'L') {
                    for (int i = 0; i < ip.size(); i++) {
                        std::cout << ip[i].name << ", " << ip[i].ip << std::endl;
                    }
                }
                else if (toupper(ch) == 'R') {
                    
                    std::cout << "Enter name of server:\n";

                    for (int i = 0; i < ip.size(); i++) {
                        std::cout << ip[i] << std::endl;
                    }

                    std::cin.ignore();
                    std::string name_;
                    std::getline(std::cin, name_);

                    bool found = false;
                    int it;
                    for (int i = 0; i < ip.size(); i++) {
                        if (name_ == ip[i].name) {
                            found = true;
                            it = i;
                            break;
                        }
                    }

                    if (!found) { std::cerr << "Name of server not found!" << std::endl; }

                    std::string tools = ".tools";
                    server::remoteExec(ip[it], mock, 'R');

                    break;
                }
                else if (toupper(ch) == 'B') {
                    break;
                }
                else { std::cerr << "Unexpected simbol" << std::endl; }
                break;
            }
            default: continue;
        }
    }
}
