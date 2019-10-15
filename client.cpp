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
#define BUFF_SIZE 1024
using namespace std;
int client_count = 0;
int client_socket;
//downloading file in chunks
void *download_file(void *v)
{

    int *s = (int *)v;
    int i = *s;

    FILE *fp = fopen("peer.cpp", "r+");

    fseek(fp, 0, i * 512);
    int size = 512;

    //sending data
    char word[512];
    int n;
    send(client_socket, &size, sizeof(size), 0);
    while ((n = fread(word, sizeof(char), 512, fp)) > 0 && size > 0)
    {
        cout<<"BUffer"<<word<<endl;
        send(client_socket, word, n, 0);
        memset(word, '\0', sizeof(word));
        size = size - n;
    }

    fclose(fp);
    pthread_exit(0);
}

// int check_login(int sock)
// {
//     char ele[128];
//     int ip, port;
//     read(sock, &ip, sizeof(ip));
//     read(sock, &port, sizeof(port));
//     FILE *fp;
//     fp = fopen("client.txt", "a+");
//     sprintf(ele, "%d", ip);
//     fputs(ele, fp);
//     fputs(" ", fp);
//     sprintf(ele, "%d", port);
//     fputs(ele, fp);
//     fputs("\n", fp);
//     return 1;
// }
// // int create_login(int sock)
// {
//     //CHECK if client exists from server

//     //IF NOT create new
//     int ip = 4;
//     cout << "enter ip";
//     cin >> ip;

//     int port = 8000;
//     cout << "enter port";
//     cin >> port;
//     write(sock, &ip, sizeof(ip));
//     write(sock, &port, sizeof(port));
//     return 1;
// }
void *peer_communication(void *argc)
{
    int *portx = (int *)argc;
    int port = *portx;
    cout << port << endl;
    //create socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    // specifying address for the socket
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    //setting connection
    int connection_status = connect(client_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    //checking error in connection
    if (connection_status == -1)
        printf("There was an error making a connection to the remote socket\n\n");
    cout << "CONNECTION\n";
    FILE *fp;
    fp = fopen("TEST.txt", "a+");
    if(fp<0)
    cout<<"ERROR";
    char buffer[512];
    int file_size, n;
    recv(client_socket, &file_size, sizeof(file_size), 0);
    while ((n = recv(client_socket, buffer, sizeof(buffer), 0)) > 0)
    {
        cout<<"BUffer"<<buffer<<endl;
        
        fwrite(buffer, sizeof(char), n, fp);
        memset(buffer, '\0', sizeof(buffer));
        // fputs("\n",fp);
        file_size = file_size - n;
    }
    //memset( buffer , '\0', sizeof(buffer));
    printf("successfully received");

    fclose(fp);

    close(client_socket);
    pthread_exit(0);
}
void *create_login(void *argc)
{
    int *portx = (int *)argc;
    int port = *portx;
    cout << port << endl;
    //create socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    // specifying address for the socket
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    //setting connection
    int connection_status = connect(client_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    //checking error in connection
    if (connection_status == -1)
        printf("There was an error making a connection to the remote socket\n\n");
    cout << "CONNECTION\n";
    send(client_socket, "hi", 10, 0);
    // int check = create_login(client_socket);

    // if (check == 1)
    //     cout << "Client created successfully";
    //fclose(fp);

    close(client_socket);
    pthread_exit(0);
}
void *client(void *arg_c)
{
    int *portx = (int *)arg_c;
    int port = *portx;
    int valid_login;
    string user;
    //char userid[128], userpass[128], meg[128],groupid[128];
    //Thread id
    pthread_t peer[3];
    //Create Attributes
    pthread_attr_t attr;
    //Initializr attribute
    pthread_attr_init(&attr);
    //Create server and client thread
    int ip1 = 8000, ip2 = 8001, ip3 = 8002;
    //Suppose want to pass any variable then REPLACE NULL by &varname
    char command[128];
    //--------------COMMAND------
    //label:
    while (1)
    {
        cout << "Enter Command\n";
        cin.getline(command, 128);
        // char *token = strtok(command, " ");
        // char *comm[3];
        // Keep printing tokens while one of the
        // delimiters present in str[].

        int i = 0;
        char *token = strtok(command, " ");
        char *comm[3];
        while (token != NULL)
        {
            comm[i] = token;
            //cout << comm[i] << endl;
            token = strtok(NULL, " ");
            i++;
        }
        //cout<<"hereeee:"<<comm[0]<<":"<<comm[1]<<endl;

        // auto first_token = command.substr(0, command.find(' '));
        // cout<<first_token<<endl;
        //-------------------LOGIN-----------------
        if (strcmp(comm[0], "login") == 0)
        {
            cout << "login here:" << endl;
            // comm[0]="create_user";
            char userid[128], userpass[128], meg[128], ch[2];
            int trackerip = 4444;
            strcat(userid, comm[1]);
            strcat(userpass, comm[2]);
            int client_socket = socket(AF_INET, SOCK_STREAM, 0);
            // specifying address for the socket
            struct sockaddr_in serv_addr;
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_port = htons(trackerip);
            serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
            //setting connection
            // cout<<"SETTT"<<endl;
            int connection_status = connect(client_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
            //checking error in connection
            if (connection_status == -1)
                printf("There was an error making a connection to the remote socket\n\n");
            cout << "CONNECTION ESTABLISHED\n";
            // cout<<comm[0]<<endl;
            //send(client_socket, &port, sizeof((int)port), 0);
            send(client_socket, "login", sizeof((string)comm[0]), 0);
            recv(client_socket, meg, sizeof(meg), 0);
            //send(client_socket, &port, sizeof((int)port), 0);
            cout << meg << endl;
            if (strcmp(meg, "gotit") == 0)
            {
                user = userid;
                //send(client_socket, &port, sizeof(port), 0);
                send(client_socket, &userid, sizeof(userid), 0);
                send(client_socket, &userpass, sizeof(userpass), 0);
                //  recv(client_socket, ch, sizeof(ch), 0);

                read(client_socket, &valid_login, sizeof(valid_login));
                //close(client_socket);
                cout << valid_login << endl;
                if (valid_login == 1)
                {
                    close(client_socket);
                }
                else
                {
                    cout << "UNSUCCESSFUL LOGIN" << endl;
                    cout << "Enter login command again with correct details\n";
                    exit(1);
                }
            }
            close(client_socket);
        }
        else if (strcmp(comm[0], "create_group") == 0 && valid_login == 1)
        {
            // comm[0]="create_user";
            char groupid[128], meg[128];
            int trackerip = 4444;

            strcpy(groupid, comm[1]);
            char *userid = strdup(user.c_str());
            int client_socket = socket(AF_INET, SOCK_STREAM, 0);
            // specifying address for the socket
            struct sockaddr_in serv_addr;
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_port = htons(trackerip);
            serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
            //setting connection
            int connection_status = connect(client_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
            //checking error in connection
            if (connection_status == -1)
                printf("There was an error making a connection to the remote socket\n\n");
            cout << "CONNECTION\n";
            // cout<<comm[0]<<endl;
            send(client_socket, "create_group", sizeof((string)comm[0]), 0);
            recv(client_socket, meg, sizeof(meg), 0);
            if (strcmp(meg, "gotit") == 0)
            {
                cout << userid << endl;
                send(client_socket, groupid, sizeof((string)groupid), 0);
                send(client_socket, userid, sizeof((string)userid), 0);

                // recv(client_socket, meg, sizeof(meg), 0);
                // cout << meg << endl;

                //close(client_socket);
                //cout << userid << ":" << groupid << endl;
            }
            close(client_socket);
        }
        else if (strcmp(comm[0], "list_groups") == 0 && valid_login == 1) //list_groups
        {
            // comm[0]="create_user";
            char groupid[128], meg[128];
            int trackerip = 4444;

            strcpy(groupid, comm[1]);
            char *userid = strdup(user.c_str());
            int client_socket = socket(AF_INET, SOCK_STREAM, 0);
            // specifying address for the socket
            struct sockaddr_in serv_addr;
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_port = htons(trackerip);
            serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
            //setting connection
            int connection_status = connect(client_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
            //checking error in connection
            if (connection_status == -1)
                printf("There was an error making a connection to the remote socket\n\n");
            cout << "CONNECTION\n";
            // cout<<comm[0]<<endl;
            send(client_socket, "list_groups", sizeof((string)comm[0]), 0);
            recv(client_socket, meg, sizeof(meg), 0);
            if (strcmp(meg, "gotit") == 0)
            {
                cout << userid << endl;
                send(client_socket, groupid, sizeof((string)groupid), 0);
                send(client_socket, userid, sizeof((string)userid), 0);

                // recv(client_socket, meg, sizeof(meg), 0);
                // cout << meg << endl;

                //close(client_socket);
                //cout << userid << ":" << groupid << endl;
            }
            close(client_socket);
        }
        else if (strcmp(comm[0], "create_user") == 0)
        {
            char userid[128], userpass[128], meg[128];
            int trackerip = 4444;
            strcat(userid, comm[1]);
            strcat(userpass, comm[2]);
            int client_socket = socket(AF_INET, SOCK_STREAM, 0);
            // specifying address for the socket
            struct sockaddr_in serv_addr;
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_port = htons(trackerip);
            serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
            //setting connection
            int connection_status = connect(client_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
            //checking error in connection
            if (connection_status == -1)
                printf("There was an error making a connection to the remote socket\n\n");
            cout << "CONNECTION\n";
            // cout<<comm[0]<<endl;
            send(client_socket, "create_user", sizeof((string)comm[0]), 0);
            recv(client_socket, meg, sizeof(meg), 0);
            if (strcmp(meg, "gotit") == 0)
            {
                send(client_socket, &userid, sizeof(userid), 0);
                send(client_socket, &userpass, sizeof(userpass), 0);
                recv(client_socket, meg, sizeof(meg), 0);
                cout << meg << endl;

                //close(client_socket);
                cout << userid << ":" << userpass << endl;
            }
            close(client_socket);
        }

        else if (strcmp(comm[0], "download_file") == 0 )
        {
            int input;
            cout << "Enter input 1 to communicate with peer";
            cin >> input;
            if (input == 1)
            {
                pthread_create(&peer[0], &attr, peer_communication, &ip1);
                pthread_create(&peer[1], &attr, peer_communication, &ip2);
              pthread_create(&peer[2], &attr, peer_communication, &ip3);
                //main waits for all thread to terminate
                pthread_join(peer[0], NULL);
                pthread_join(peer[1], NULL);
               pthread_join(peer[2], NULL);
            }
        }
    }
    pthread_exit(0);
}
void *server(void *arg_s)
{
    int *portx = (int *)arg_s;
    int port = *portx;
    char str[128] = "hell";
    //create server socket
     pthread_attr_t attr;
    //Initializr attribute
    pthread_attr_init(&attr);
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
        cout << "SOCKET ERROR\n";
    // specifying server address for the socket
    struct sockaddr_in serv_addr, newaddr;
    socklen_t newaddrsize;
    memset(&serv_addr, '\0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    //bind the socket
    int b = bind(server_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (b < 0)
    {
        cout << "Binding Error at server\n";
        exit(1);
    }
    //cout<<"Binding created\n";
    if (listen(server_socket, 5) < 0)
        cout << "LISTENING ERROR\n";

    int client_socket;
    char ele[128];
    
        client_socket = accept(server_socket, (struct sockaddr *)&newaddr, &newaddrsize);
        if (client_socket < 0)
            cout << "CONNECTION FAILED\n";
        // int i=1;
        // // cout << "Enter 1 for download";
        // // cin >> i;
        // while (i--)
        // {
            // while (1)
            // {
            //     recv(client_socket, ele, 128, 0);

            //     cout << ele << "\n";
            //     send(client_socket, ele, 128, 0);
            // }

            pthread_t chunk[3];
            int i1 = 0;
            int i2 = 1;
            int i3 = 2;

            pthread_create(&chunk[0], &attr, download_file, &i1);
            pthread_create(&chunk[1], &attr, download_file, &i2);
            pthread_create(&chunk[2], &attr, download_file, &i3);

            //JOINING pthread
            pthread_join(chunk[0], NULL);
            pthread_join(chunk[1], NULL);
            pthread_join(chunk[2], NULL);
       // }
    

    //check_login(client_socket);

    close(client_socket);
    //     int client_socket;
    //     client_socket=accept(server_socket,NULL,NULL);

    // //gets(str);
    // send(client_socket,&str,sizeof(str),0);
    // //printf("SERVER MESSAGE:%s",str);

    close(server_socket);
    pthread_exit(0);
}
int main(int argc, char **argv)
{
    int port;
    port = atoi(argv[1]);
    //Thread id
    pthread_t peer[2];
    //Create Attributes
    pthread_attr_t attr;
    //Initializr attribute
    pthread_attr_init(&attr);
    //Create server and client thread
    //Suppose want to pass any variable then REPLACE NULL by &varname
    pthread_create(&peer[0], &attr, server, &port);
    pthread_create(&peer[1], &attr, client, &port);
    //main waits for all thread to terminate
    pthread_join(peer[0], NULL);
    pthread_join(peer[1], NULL);
    cout << "JOINED THREAD";
    return 0;
}
