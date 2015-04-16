/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <map>
#include <iostream>
#include <time.h>
#include <vector>

using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     bool userChoice = true;
    // srand(time(NULL));

     std::vector<string> card_vec;
     std::map<string,int> card_map;
     //std::map<string,int>::iterator it;   
   
     card_vec.push_back("2");
     card_vec.push_back("3");
     card_vec.push_back("4");   
     card_vec.push_back("5");
     card_vec.push_back("6");
     card_vec.push_back("7");
     card_vec.push_back("8");
     card_vec.push_back("9");
     card_vec.push_back("10");
     card_vec.push_back("J");
     card_vec.push_back("K");
     card_vec.push_back("Q");
     card_vec.push_back("A");


     card_map["2"] = 2;
     card_map["3"] = 3;
     card_map["4"] = 4;
     card_map["5"] = 5;
     card_map["6"] = 6;
     card_map["7"] = 7;
     card_map["8"] = 8;
     card_map["9"] = 9;
     card_map["10"] =10;
     card_map["J"] = 10;
     card_map["K"] = 10;
     card_map["Q"] = 10;
     card_map["A"] = 1; 

    // it = card_map.find("2"); 

     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     
     if (newsockfd < 0) 
          error("ERROR on accept");
     bzero(buffer,256);

     while((n = read(newsockfd,buffer,255))>0 && userChoice) 
     {
        //if (n < 0) error("ERROR reading from socket");
        //printf("Here is the message: %s\n",buffer);
        if(strcmp(buffer,"Yes"==0))
        {
            userChoice = true;
        }
        else if(strcmp(buffer,"No"==0))
        {
            userChoice = false;
        }
        if(userChoice)
        {

         srand(time(NULL));
         
         int randIndex = rand()%card_map.size();
          
         cout<< randIndex<<"\n";     
         cout<<buffer<<"\n";     
         
         std::map<string,int>::iterator it;
           
         it = card_map.find(card_vec.at(randIndex));

         char *temp; 
         
         cout<<it->first.c_str();
           
         temp = (char *)it->first.c_str();
         
         card_vec.erase(card_vec.begin()+randIndex);

         card_map.erase (it);
     
         n = write(newsockfd,temp,sizeof(temp));
       
         temp = NULL;
         if (n < 0) error("ERROR writing to socket");
        }
        else
        {

            n = write(sockfd,"Good bye!",strlen(buffer));
        }
      }

     //delete []temp;
     close(newsockfd);
     close(sockfd);
     
     return 0; 
}