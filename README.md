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

Ejecución código C++ de la raspberry:

Para productor
```
./producer
```

Para consumidor (test y topic1)
```
./consumer -b 192.168.1.105:9092 -g test topic1
```
