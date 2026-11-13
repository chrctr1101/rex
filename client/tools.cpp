#ifndef TOOLS_H
#define TOOLS_H

#include "tools.hpp"
#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

static bool check;
std::ofstream fileOO ("ips", std::ios::app);
std::ifstream fileII ("ips", std::ios::out);

namespace server {

    static std::string exec;


    struct IP {
        std::string name;
        std::string ip;

        IP(std::string name_, std::string ip_) : name(name_), ip(ip_) {}
    };

    void connect(IP m) {
        exec = "ssh ";
        exec+=m.name;
        exec+="@";
        exec+=m.ip;

        system(exec.c_str());
        return;
    };

    bool packArch(std::string& name, std::string& file) {
        exec = "tar -czvf ";
        exec+=name;
        exec+=" ";
        exec+=file;
        check = system(exec.c_str());
        if (!check) { std::cerr << "Archive not created" << std::endl; return false; }
        std::cout << file << " was packed" << std::endl;
        return true;       
    };

    bool unpackArch(std::string& name) {
        exec = "tar -xzvf ";
        exec+=name;
        check = system(exec.c_str());
        if (!check) { std::cerr << "Archive not unpacked" << std::endl; return false; }
        std::cout << name << " was unpacked" << std::endl;
        return true;
    };

    bool pushArch(IP& ip, std::string& arch) {
        exec = "ls -la ";
        exec+=arch;

        std::cout << "Check to archive ..." << std::endl;

        exec = "scp -r ";
        exec+= arch;
        exec+=" ";
        exec+=ip.name;
        exec+="@";
        exec+=ip.ip;
        exec+=":~/";

        std::cout << "Trying to push on remote server..." << std::endl;

        system(exec.c_str());

        return true;
    };

    bool pullArch(IP& ip, std::string& arch) {
        exec = "ssh ";
        exec+=ip.name;
        exec+="@";
        exec+=ip.ip;
        std::string cmd = " 'ls ";
        cmd+=arch;
        cmd+="'";
        exec+=cmd;

        std::cout << "Check to archive ..." << std::endl; 

        system(exec.c_str());

        exec = "scp -r ";
        exec+=ip.name;
        exec+="@";
        exec+=ip.ip;
        exec+=":~/";
        exec+=arch;
        exec+=" ~/";

        std::cout << "Trying to push on remote server..." << std::endl;

        system(exec.c_str());

        std::cout << "Done!" << std::endl;

        return true;
    };

    bool remoteExec(IP& ip, std::string& cmd, char ch) {
        std::string exec_ = "ssh ";
        std::string execSudo = "ssh -t ";
        exec_+=ip.name + "@" + ip.ip;
        execSudo+=ip.name + "@" + ip.ip;

        switch(ch) {
            case('f'): {
                exec = exec_;
                exec+= " 'ls | grep -r ";
                exec+=cmd;
                exec+="'";

                check = system(exec.c_str());
                if (!check) { std::cerr << "Error in running command on server" << std::endl; return false; }
                return true;
                break;
            }
            case('c'): {
                exec = execSudo;
                exec+=" '~/.tools/reserve.sh'";
                system(exec.c_str());
                
                return true;
                break;
            }
            case('r'): {
                exec = execSudo;
                exec+= " '~/.tools/reboot.sh'";
                check = system(exec.c_str());

                if (!check) { std::cerr << "Error in rebooting server" << std::endl; return false; }
                return true;
            }
            case('u'): {
                std::cerr << "I hope you know what you do. Good Luck!" << std::endl;
                exec = execSudo;
                exec+= " '~/tools/update.sh'";
                check = system(exec.c_str());

                if (!check) { std::cerr << "Something went wrong when server was updating" << std::endl; return false; }
                return true;
                break;
            }
            case('p'): {
                exec = exec_;
                exec+= " '";
                exec+= cmd;
                exec+= "'";

                system(exec.c_str());

                return true;
                break;
            }
            case('R'): {
                system("mkdir -p ~/.tools/");
                system("cp ~/rex/server/* ~/.tools/");
                std::string tools = ".tools";
                std::string tools_tar = ".tools.tar.gz";
                packArch(tools_tar, tools);
                server::pushArch(ip, tools_tar);
                system("rm -r ~/.tools; rm ~/.tools.tar.gz");

                exec = exec_;
                exec+= " 'tar -xzf ~/.tools.tar.gz; chmod +x ~/.tools/*; rm ~/.tools.tar.gz;mkdir -p ~/important ~/logs;'";

                system(exec.c_str());
                
                return true;
                break;
            }
            default: return false;
        }
    }

    bool pingServer(IP &ip) {
        exec = "ping -c 4 ";
        exec+=ip.name;
        exec+="@";
        exec+=ip.ip;

        check = system(exec.c_str());
        if(!check) { std::cerr << "Server not available" << std::endl; return false; }

        return true;
    };

    void updateList(std::vector<IP>& ip) {
        system("rm ips; touch ips");
        for (int i = 0; i < ip.size(); i++) {
            fileOO << ip[i].name << ", " << ip[i].ip << "\n";
        }
    }

    void forgetServer(std::vector<IP>& ip, std::string name_) {
        for (int i = 0; i < ip.size(); i++) {
            if (ip[i].name == name_) {
                ip.erase(ip.begin() + i);
            }
        }
        return;
    }

    std::ostream& operator<<(std::ostream& os, const IP& obj) {
        os << obj.name << ", " << obj.ip;
        return os;
    }

}

#endif