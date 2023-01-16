#include <iostream>
#include <fstream>
#include <unistd.h>
#include <tins/tins.h>
#include <string> 
#include <ctype.h>

using namespace std;
using namespace Tins;

void echo_how();

int main(int argc, char * argv[]) {
	int channel = 1;
	if(argc < 3){
		echo_how();
		return -1;
	}

	vector<string> SSIDList;
    string strSSID;

	std::ifstream ifs(argv[2]);

	strSSID.resize(255);
    while(ifs.getline(&strSSID[0], 255))
        SSIDList.push_back(strSSID);

    ifs.close();

	while (true){
		RadioTap tap;

		Dot11::address_type ap="00:11:22:33:44:55";		 //Access Point Mac address
		Dot11::address_type unicast="ff:ff:ff:ff:ff:ff"; //Target Mac address [brodcast ff:ff:ff:ff:ff:ff]

		for(int i=0;i<SSIDList.size();i++){
			Dot11Beacon beacon;	//set beacon frame
			beacon.addr1(unicast);
			beacon.addr2(ap);	
			beacon.addr3(ap);				//set AP MAC
			beacon.ssid(SSIDList[i]);				//set Target Mac
			beacon.ds_parameter_set(channel);	//set channel
			
			beacon.supported_rates({ 1.0f, 5.5f, 11.0f }); //what? (only work C++11)
			beacon.timestamp(0);
			beacon.interval(0);
			beacon.seq_num(0);

			tap.inner_pdu(beacon);
			
			PacketSender sender(argv[1]);
			sender.send(tap);
			usleep(5000);
		}
	}
}

void echo_how(){
	puts("syntax : beacon-flood <interface> <ssid-list-file>\nsample : beacon-flood mon0 ssid-list.txt");
}
