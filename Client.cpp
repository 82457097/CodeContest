/*Copyright [2020.11.04] <qianwanjun>*/
#include<unistd.h>
#include<getopt.h>
#include"Socket.h"
#include"wangbk.h"
#include"PlayerAction.h"
#include"AvoidDanger.h"

#define TOKEN_START_POS 5
#define TOKEN_LEN 2
#define OK_LEN 4

class Client {
public:
    bool ClientInit(int port, char* ipAddr) {
        m_socket.SockInitClient(port, ipAddr);
        if (!m_socket.SockConnect()) {
            cout << "Connect failed" << endl;
            cout << "Clinet initial failed" << endl;
            return false;
        }

        return true;
    }

    bool ClientSend(const char buf[], int len) {
        if (m_socket.SockSend(m_socket.sockfd, buf, len) < ERR_ZERO) {
            cout << "Send failed" << endl;
            return false;
        }
        cout << "Send buf: " << buf << "  Buflen: " << len << endl;

        return true;
    }

    bool ClientRecv() {
        if (m_socket.SockRecv(m_socket.sockfd, recvbuf, BUFFER_SIZE) < ERR_ZERO) {
            cout << "Recv failed" << endl;
            return false;
        }

        return true;
    }

    void ClientClose() {
        m_socket.SockClose();
    }

    bool SendHeartPac() {
        while (true) {
            ClientSend("(H)", strlen("(H)"));
            ClientRecv();
            cout << recvbuf << endl;
            string s_recvbuf = recvbuf;
            sleep(1);
            if (s_recvbuf.find("START") == 1 || s_recvbuf.find("START") == 5) {
                ParseStart();
                ClientSend("(READY)", strlen("(READY)"));
                break;
            } else {
                continue;
            }
        }

        return true;
    }

    bool RecvMap() {
        while ( true ) {
            ClientRecv();
            if (ParseMap()) {
                cout << recvbuf << endl;
                string s_recvbuf = recvbuf;

                if (s_recvbuf.find("GAMEOVER") == 1) {
                    cout << "Game over!" << endl;
                    break;
                }
                while ( true ) {
                    if (m_loc.y == -1) {
                        string sendMsg = "(" + m_token + "  )";
                        ClientSend(sendMsg.c_str(), strlen(sendMsg.c_str()));
                        break;
                    }
                    m_pos.x = m_loc.x;
                    m_pos.y = m_loc.y;
                    playerAct.Init(m_pos, enemyLoc);
                    playerAct.Run(mapArr);
                    string avoidCom = "1";
                    checkDanger.Init(mapArr, m_poi);
                    avoidCom = checkDanger.Run();
                    algorithm.Init(mapArr, m_mapSize, m_loc, mapArr[m_loc.x][m_loc.y]);
                    algorithm.Run(m_loc);
                    string sendMsg = "";
                    cout << avoidCom << endl;
                    if(avoidCom != "1") {
                        cout << "danger!" << endl;
                        sendMsg = "(" + m_token + avoidCom + ")";
                    } else {
                        sendMsg = "(" + m_token + algorithm.Decision() + " )";
                    }
                    ClientSend(sendMsg.c_str(), strlen(sendMsg.c_str()));

                    sleep(0.5);
                    break;
                }
            }
        }

        return true;
    }

    bool ParseStart() {
        string dstStr = recvbuf;
        int pos = dstStr.find(' ');
        string num = dstStr.substr(pos + 1, 1);
        m_num = atoi(num.c_str());
        string mapSize = dstStr.substr(pos + 3, 2);
        m_mapSize = atoi(mapSize.c_str());

        cout << "my number: " << m_num << endl;
        cout << "map size: " << m_mapSize << endl;

        return true;
    }

    bool ParseMap() {
        string s_recvbuf = recvbuf;
        if (s_recvbuf.find("OK") == 1) {
            cout << recvbuf << endl;
            sleep(0.5);
            return false;
        }

        if (s_recvbuf.find("MAP") == 1) {
            m_token = s_recvbuf.substr(TOKEN_START_POS, TOKEN_LEN);
        }

        GetScore(s_recvbuf);
        GetLocation(s_recvbuf);
        GetMap(s_recvbuf);

        return true;
    }

    bool GetScore(string s_recvbuf) {
        int pos = s_recvbuf.find("SCORE");
        string score = s_recvbuf.substr(pos + 9);

        string tmpStr;
        for (auto s : score) {
            if (s != ' ' && s != ']') {
                tmpStr += s;
            } else {
                scoreArr.push_back(tmpStr);
                tmpStr = "";
            }

            if (s == ']') {
                break;
            }
        }

        cout << "Score: " << scoreArr[m_num] << endl;

        scoreArr.clear();
        return true;
    }

    bool GetLocation(string s_recvbuf) {
        int pos = s_recvbuf.find("LOCATION");
        string location = s_recvbuf.substr(pos + 12);

        string tmpStr;
        for (auto s : location) {
            if (s != ' ' && s != ']') {
                tmpStr += s;
            } else {
                locationArr.push_back(tmpStr);
                tmpStr = "";
            }

            if (s == ']') {
                break;
            }
        }

        m_loc.x = atoi(locationArr[m_num].c_str()) / m_mapSize;
        m_loc.y = atoi(locationArr[m_num].c_str()) % m_mapSize;

        for (auto &v : locationArr) {
            Position tmpPos;
            tmpPos.x = atoi(v.c_str()) / m_mapSize;
            tmpPos.y = atoi(v.c_str()) % m_mapSize;
            if (tmpPos.x != m_loc.x && tmpPos.y != m_loc.y) {
                enemyLoc.push_back(tmpPos);
            }
        }

        cout << "Location: " << m_loc.x << "," << m_loc.y << endl;

        enemyLoc.clear();
        locationArr.clear();
        return true;
    }

    bool GetMap(string s_recvbuf) {
        mapArr = {};
        int pos = s_recvbuf.find("MAP");
        int mapSize = m_mapSize * m_mapSize;
        m_map = s_recvbuf.substr(pos + 7, mapSize);

        string tmpStr;
        int index = 0;
        for (auto s : m_map) {
            tmpStr += s;
            if (++index < m_mapSize) {
                continue;
            } else {
                mapArr.push_back(tmpStr);
                tmpStr = "";
                index = 0;
            }
        }
        int i = 0;
        for (auto v : mapArr) {
            cout << i++ << ": " << v << endl;
        }

        return true;
    }

private:
    Loc m_loc;
    Pos m_pos;
    Poi m_poi;
    string m_token;
    int m_num;
    int m_mapSize;
    string m_map;
    std::vector<string> locationArr;
    std::vector<Position> enemyLoc;
    std::vector<string> mapArr;
    std::vector<string> scoreArr;
    Socket m_socket;
    char recvbuf[BUFFER_SIZE];

    Algorithm algorithm;
    PlayerAction playerAct;
    CheckDanger checkDanger;
};

int main(int argc, char* const argv[]) {
    int opt;
    int digit_optind = 0;
    int option_index = 0;
    const char *string = ":i:p:";
    static struct option long_options[] = {
        {"ip", required_argument, NULL, 'i'},
        {"port", required_argument, NULL, 'p'},
        {NULL,     0,                 NULL, 0}
    };

    char* port;
    char* ipAddr;
    int index = 2;
    while (index--) {
        char tmp;
        tmp = getopt_long(argc, argv, string, long_options, &option_index);
        switch (tmp) {
            case 'i':
                ipAddr = optarg;
                cout << "Ip addr: " << ipAddr << endl;
                break;
            case 'p':
                port = optarg;
                cout << "Port: " << port << endl;
                break;
            default:
                break;
        }
    }

    Client client;
    const char key[] = KEY;

    if (!client.ClientInit(atoi(port), ipAddr)) {
        return ERR_RET;
    }

    client.ClientSend(key, strlen(key));
    client.ClientRecv();
    client.SendHeartPac();
    client.RecvMap();

    client.ClientClose();
    return 0;
}

