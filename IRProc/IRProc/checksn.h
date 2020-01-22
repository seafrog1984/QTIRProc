#include <atlbase.h>
#include <atlconv.h>
#include "iphlpapi.h"

#include <fstream>
#include <iostream>  
#include <stdlib.h>  
#include "string"

#include<atltime.h>



#pragma comment ( lib, "Iphlpapi.lib")



void getMac(char *mac);

int CheckSN(char *mac,char*sn);
int CheckTime(char *sn);

