//
//  RestClient.hpp
//  detectParking
//
//  Created by Ahmed Adel on 11/24/16.
//  Copyright © 2016 Ahmed Adel. All rights reserved.
//

#ifndef RestClient_hpp
#define RestClient_hpp

#include <string>


using namespace std;


#endif /* RestClient_hpp */

class RestClient {
public:
    static void updateParkingInfo(string bayId, bool free);
    static void* updateParkingInfoRunn(void* arguments);
    
};
