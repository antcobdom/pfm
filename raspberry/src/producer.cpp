#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>
#include <iostream>
#include <ctime>
#include <stdio.h>
#include <time.h>
#include <string>

#include <sstream>
#include <librdkafka/rdkafkacpp.h>
#include <csignal>

#include <jsoncpp/json/value.h>

#include <jsoncpp/json/json.h>
#include <fstream>

static bool run = true;

static void sigterm (int sig) {
  run = false;
}

using namespace std;

std::string timestr(time_t t) {
   std::stringstream strm;
   strm << t;
   return strm.str();
}

// Setup for GPIO 22 CE and CE1 CSN with SPI Speed @ 8Mhz
RF24 radio(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ);  

RF24Network network(radio);

// Address of our node in Octal format
const uint16_t this_node = 00;

// Address of the other node in Octal format (01,021, etc)
const uint16_t other_node = 01;


struct payload_t {                  // Structure of our payload
  char type[4];
  float value = 0.00;
};

int main(int argc, char** argv) 
{
        // NRF24 Setup
	radio.begin();
	
	delay(5);
	network.begin(/*channel*/ 90, this_node);
	radio.printDetails();
	
	//Carga de fichero de mapeo
	std::ifstream types_file("types.json",std::ifstream::binary);
	Json::Value types;
	types_file >> types;
	cout<<types;        
	Json::FastWriter fastWriter;
	Json::Value myjson;


	// Kafka Setup
        std::string errstr;
        std::string brokers = "192.168.1.105:9092";
        std::string topic_str = "test";
        int32_t partition = RdKafka::Topic::PARTITION_UA;

        RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
        RdKafka::Conf *tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);
        conf->set("metadata.broker.list", brokers, errstr);
        signal(SIGINT, sigterm);
        signal(SIGTERM, sigterm);

        RdKafka::Producer *producer = RdKafka::Producer::create(conf, errstr);
        if (!producer) {
           std::cerr << "Failed to create producer: " << errstr << std::endl;
        }

        std::cout << "% Created producer " << producer->name() << std::endl;

        RdKafka::Topic *topic = RdKafka::Topic::create(producer, topic_str, tconf, errstr);
        if (!topic) {
           std::cerr << "Failed to create topic: " << errstr << std::endl;
         }

	std::time_t time_0 = time(nullptr);

	while(run)
	{
		  network.update();
  		  while ( network.available() ) {     
    			
		 	RF24NetworkHeader header;        
   			payload_t payload;
  			network.read(header,&payload,sizeof(payload));
                        std::string type(payload.type);
                        std::ostringstream ss;
                        ss << payload.value;
                        std::string value(ss.str());
                                              	
			
			myjson[types[type].asString()]=payload.value;
			myjson["timestamp"]=timestr(std::time(nullptr));

			
			cout<<myjson;
			printf("Diferencia de tiempo %d \n",(std::time(nullptr))-time_0); 
		        if ((std::time(nullptr))-time_0>3)
				{
					 time_0 = time(nullptr);
					//std::string jsonData = "{\"type\":\"" + type  + "\", \"value\":" + value + ", \"timestamp\":" + time + "}";
                        		std::string key = "raspberry-casa";
   					std::string jsonData_0 = fastWriter.write(myjson);
					std::string jsonData=jsonData_0.substr(0, jsonData_0.size()-1);
					
					printf("Sending %s \n", &jsonData);
	
        		                RdKafka::ErrorCode resp = producer->produce(topic, partition,RdKafka::Producer::RK_MSG_COPY /* Copy payload */,(char*)jsonData.c_str(), jsonData.length(),key.c_str(), key.length(), NULL);
 
  			                if (resp != RdKafka::ERR_NO_ERROR)
		                            std::cerr << "% Produce failed: " <<
                		            RdKafka::err2str(resp) << std::endl;
                       			else
                           			std::cout << "% Produced message (" << jsonData.length()  << " bytes)" << std::endl;

                       			producer->poll(0);
					//limpio
				}
                 }
		  
		 delay(2000);
	}

        delete topic;
        delete producer;
	return 0;
}

