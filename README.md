# pfm

Compilar código C++ de la raspberry:

Para productor
```
 g++ producer.cpp -lrf24-bcm -lrf24network -lrdkafka++ -lstdc++ -std=gnu++0x -ljsoncpp  -o producer
```
Para consumidor
```
 g++ consumer.cpp -lrf24-bcm -lrf24network -lrdkafka++ -lstdc++ -std=gnu++0x -ljsoncpp -fpermissive -o consumer
```
