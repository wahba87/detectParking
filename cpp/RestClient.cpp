//
//  RestClient.cpp
//  detectParking
//
//  Created by Ahmed Adel on 11/24/16.
//  Copyright © 2016 Ahmed Adel. All rights reserved.
//

#include "RestClient.hpp"
#include <iostream>
#include <stdio.h>
#include <curl/curl.h>
#include <pthread.h>
#include "ConfigLoad.h"


struct arg_struct {
    string bayId;
    bool free;
};

inline const char * const BoolToString(bool b)
{
    return b ? "true" : "false";
}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


void* RestClient::updateParkingInfoRunn(void* arguments){
    CURL *curl;
    CURLcode res;
    
    struct arg_struct *args = (struct arg_struct *)arguments;
    
    
    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);
    
    /* get a curl handle */
    curl = curl_easy_init();
    std::string readBuffer;
    if(curl) {
        
        /*Headers */
        struct curl_slist *chunk = NULL;
        
        /* Remove a header curl would otherwise add by itself */
        chunk = curl_slist_append(chunk, "Content-Type: application/x-www-form-urlencoded");
        
        /* Add a custom header */
        chunk = curl_slist_append(chunk, "Accept: application/json");
        
        /* set our custom set of headers */
        res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
        
        /* First set the URL that is about to receive our POST. This URL can
         just as well be a https:// URL if that is what should receive the
         data. */
        curl_easy_setopt(curl, CURLOPT_URL, (ConfigLoad::options["TW_Host"]+"Thingworx/Things/Group.Parking.Thing/Services/updateParking?appKey="+ConfigLoad::options["TW_AppKey"]).c_str());
        /* Now specify the POST data */
        
        string strArguments = "ParkingId=" + ConfigLoad::options["ParkingId"] + "&BayId="+args -> bayId+"&BayState="+BoolToString(args -> free);
        
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strArguments.c_str());
        
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); //only for https
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); //only for https
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); //remove this to disable verbose output
        
        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        
        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    delete args;
    pthread_exit(NULL);
}


void RestClient::updateParkingInfo(string bayId,bool free){
    
    arg_struct * args = new arg_struct;
    args->bayId = bayId;
    args->free = free;

    
    pthread_t t;
    pthread_create(&t, NULL, &updateParkingInfoRunn, args);
}
