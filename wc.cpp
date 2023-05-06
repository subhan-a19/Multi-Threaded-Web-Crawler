#include<iostream>
#include<cstdlib>
#include<fstream>
#include<queue>
#include<time.h>
#include<thread>
#include<pthread.h>
#include<chrono>
using namespace std;

void htmlDownloader(string bURL);
void urlParser();
void *crawler(void *arg);
queue<string> q,q2;

int main()
{
    string burl;
    cout<<"Enter the Base URL: ";
    cin>>burl;

    cout<<endl<<endl;
    q.push(burl);
    pthread_t workingThread;
    pthread_create(&workingThread,NULL,crawler,NULL);
    pthread_join(workingThread,NULL);
   
    return 0;
}

void htmlDownloader(string bURL)
{
    string baseURL= bURL;
    string down= "wget -q -O index.txt " + baseURL;
    system((const char*)down.c_str());
}
void urlParser()
{
    fstream reading;
    reading.open("index.txt");
    string temp = "xxxx";
    int ankerHrefCount = 0;
    int httpCount = 0;
    int place = 0;
    string url = "";

    while (!reading.eof())
    {
        reading >> temp;
        if (temp[0] == 'h' && temp[1] == 'r' && temp[2] == 'e' && temp[3] == 'f')
        {
            ankerHrefCount++;
            place = temp.find("http");

            if (temp[place + 4] == ':' || temp[place + 5] == ':')
            {
                if (place >= 0)
                {
                    while (temp[place] != '"' && place<temp.length() )
                    {
                        url += temp[place];
                        if (false)
                            cout << url << endl;
                        place++;
                    }
                    q.push(url);
                    std::this_thread::sleep_for(std::chrono::milliseconds(300));

                    cout << "\nTID: "<<pthread_self()<<" :: " <<url;
                    url = "";
                    httpCount++;
                }
            }
        }
    }
    reading.close();
}
void *crawler(void *arg)
{
    // queue<string> qq=static_cast<queue<string>*>(arg);
    // cout<<"----------------------------------"<<*arg.front()<<endl;
    string temp;
    while (!q.empty())
    {
        temp = q.front();
        htmlDownloader(temp);
        urlParser();
        cout << "\n--Queue Size: "<<q.size()<<" -- URL Crawled: "<<temp;
        q.pop();
    }
    pthread_exit(NULL);
}