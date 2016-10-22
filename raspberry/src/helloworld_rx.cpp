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

static bool run = true;

static void sigterm (int sig) {
  run = false;
}

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
	
        // Kafka Setup
        std::string errstr;
        std::string brokers = "52.19.54.104:9092";
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
                        std::string time = timestr(std::time(nullptr));

		        std::string jsonData = "{\"type\":\"" + type  + "\", \"value\":" + value + ", \"timestamp\":" + time + "}";
   	
			printf("Sending %s \n", jsonData.c_str());

                        RdKafka::ErrorCode resp =
                            producer->produce(topic, partition,
                               RdKafka::Producer::RK_MSG_COPY /* Copy payload */,
                               (char*)jsonData.c_str(), jsonData.length(),
                               NULL, NULL);
 
                       if (resp != RdKafka::ERR_NO_ERROR)
                            std::cerr << "% Produce failed: " <<
                            RdKafka::err2str(resp) << std::endl;
                       else
                           std::cerr << "% Produced message (" << jsonData.length()  << " bytes)" << std::endl;

                       producer->poll(0);
                 }
		  
		 delay(2000);
	}

        delete topic;
        delete producer;
	return 0;
}

