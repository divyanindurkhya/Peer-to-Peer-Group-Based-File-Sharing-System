#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <map>
#include <bits/stdc++.h>
#define BUFF_SIZE 1024
using namespace std;
// struct user
// {
//     char ip[16];
//     int port;
//     char userid[40];
//     char userpass[16];
//     int groupid;
//     char groupowner[20];
// } user_details[100];

// int initialize()
// {

//     char ele[1024];
//     FILE *fp;
//     int count = 0;
//     fp = fopen("tracker_info.txt", "r+");
//     int i = 0;
//     while (fgets(ele, sizeof(ele), fp) != NULL)
//     {
//         char *token = strtok(ele, " ");
//         strcat(user_details[i].userid, token);

//         while (token != NULL)
//         {
//             //printf("%s\n", token);
//             token = strtok(NULL, " ");
//             user_details[i].userpass;
//         }
//         count++;
//         i++;
//     }
//     fclose(fp);
//     return count;
// }

int main(int argc, char **argv)
{
    unordered_map<string, string> map_useridpass{};
    unordered_map<string, int> map_useridport{};
    unordered_map<string, vector<string>> map_grouplist{};
    //int count = initialize();
    int count = 0;
    //create server socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
        cout << "SOCKET ERROR\n";
    // specifying server address for the socket
    struct sockaddr_in serv_addr, newaddr;
    socklen_t newaddrsize;
    memset(&serv_addr, '\0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(4444);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    //bind the socket
    int b = bind(server_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (b < 0)
    {
        //cout << userid << ":" << userpass << "\n";
        cout << "Binding Error\n";
        exit(1);
    }
    cout << "Binding created\n";
    int client_socket;
    int *portx;
    char ele[1024];
    while (1)
    {
        if (listen(server_socket, 5) < 0)
            cout << "LISTENING ERROR\n";
        cout << "Listening created\n";
        client_socket = accept(server_socket, NULL, NULL);
        if (client_socket < 0)
        {
            cout << "CONNECTION FAILED\n";
            exit(1);
        }
        else
            cout << "CONNECTION SUCCESSFUL\n";
        //recv(client_socket,portx,sizeof(portx),0);
        //cout<<*portx<<endl;
        recv(client_socket, ele, sizeof(ele), 0);
        cout << ele << endl;
        send(client_socket, "gotit", 128, 0);
        if (strcmp(ele, "create_user") == 0)
        {
            cout << "USER CREATION:\n";
            char userid[128], userpass[128];

            recv(client_socket, userid, sizeof(userid), 0);
            recv(client_socket, userpass, sizeof(userpass), 0);
            cout << "CREDENTIALS::" << userid << "\n";
            int flag = 0, i;
            string s_a = userid;
            string s_b = userpass;

            if (map_useridpass.find(s_a) == map_useridpass.end())
            {
                FILE *fp;
                fp = fopen("tracker_info.txt", "a+");
                if (fp < 0)
                    cout << "FILE NOT OPENED";

                map_useridpass.insert({s_a, s_b});
                //  map_useridport.insert({s_a, *portx});
                cout << map_useridpass[s_a] << "\n";
                // cout << map_useridport[s_a] << "\n";
                count++;
                send(client_socket, "CLIENT ADDED IN TRACKER", 128, 0);
                fclose(fp);
                flag = 1;
            }

            if (flag == 0)
            {
                cout << "USER ALready exists!! change userid\n";
                flag = 1;
            }
        }
        else if (strcmp(ele, "login") == 0)
        {
            cout << "USER DATA:\n";
            char userid[128], userpass[128];
            recv(client_socket, userid, sizeof(userid), 0);
            recv(client_socket, userpass, sizeof(userpass), 0);
            int flag = 0, i;
            string s_a = userid;
            string s_b = userpass;

            if (map_useridpass.find(s_a) != map_useridpass.end() && map_useridpass[s_a] == s_b)
            {
                int type = 1;
                write(client_socket, &type, sizeof(type));

                //cout << "if\n";
            }

            // else if (map_useridpass[s_a] == s_b)
            // {
            //     cout << "else if\n";
            //     int type;
            //     type = 1;
            //     write(client_socket, &type, sizeof(type));
            // }
            else
            {
                //cout << "else\n";
                int type = 0;
                write(client_socket, &type, sizeof(type));
            }
        }
        else if (strcmp(ele, "create_group") == 0)
        {
            int flag=0;
            cout << "USER DATA:\n";
            char groupid[128], userid[128];

            recv(client_socket, groupid, sizeof(groupid), 0);
            recv(client_socket, userid, sizeof(userid), 0);
            cout << "HERE::" << userid << ":" << groupid << "\n";
            // int flag = 0, i;
            string s_b = userid;
            string s_a = groupid;

            if (map_grouplist.find(s_a) == map_grouplist.end())
            {
                

                map_grouplist[s_a].push_back(s_b);
               
                cout << map_grouplist[s_a][0]<< "\n";
                //cout << map_useridport[s_a] << "\n";
              
               // send(client_socket, "CLIENT ADDED IN TRACKER", 128, 0);
                
                flag = 1;
            }

            if (flag == 0)
            {
                cout << "Group id ALready exists!! change groupid\n";
                flag = 1;
            }
        }
        else if (strcmp(ele, "list_group") == 0)
        {
            int flag=0;
            cout << "USER DATA:\n";
            char groupid[128], userid[128];

            recv(client_socket, groupid, sizeof(groupid), 0);
            recv(client_socket, userid, sizeof(userid), 0);
            cout << "HERE::" << userid << ":" << groupid << "\n";
            // int flag = 0, i;
            string s_b = userid;
            string s_a = groupid;

            if (map_grouplist.find(s_a) == map_grouplist.end())
            {
                

                map_grouplist[s_a].push_back(s_b);
               
                cout << map_grouplist[s_a][0]<< "\n";
                //cout << map_useridport[s_a] << "\n";
              
               // send(client_socket, "CLIENT ADDED IN TRACKER", 128, 0);
                
                flag = 1;
            }

            if (flag == 0)
            {
                cout << "Group id ALready exists!! change groupid\n";
                flag = 1;
            }
        }
        else
        {
            cout << "NO DATA TRANSFER\n";
        }
        close(client_socket);
    }

    return 0;
}