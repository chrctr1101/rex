#ifndef TOOLS_H
#define TOOLS_H
#include <string>
#include <vector>
namespace server {

    struct IP {
        std::string name;
        std::string ip;

        IP(std::string name_, std::string ip_) : name(name_), ip(ip_) {}
    };

    void connect(IP m);

    bool packArch(std::string& name, std::string& file);
    bool unpackArch(std::string& name);

    bool pushArch(IP& ip, std::string& arch);
    bool pullArch(IP& ip, std::string& arch);

    bool remoteExec(IP& ip, std::string& cmd, char ch);

    bool pingServer(IP& ip);
    
    void updateList(std::vector<IP>& ip);

    void forgetServer(std::vector<IP>& ip, std::string name_);

    std::ostream& operator<<(std::ostream& os, const IP& obj);
}


#endif