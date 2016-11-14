
var Kafka = require('no-kafka');
var consumer = new Kafka.SimpleConsumer({'connectionString': 'localhost:9092'});
var dweetClient = require("node-dweetio");
var dweetio = new dweetClient();
var sleep = require('sleep');

var dataHandler = function (messageSet, topic, partition) {
    messageSet.forEach(function (m) {
        var json = JSON.parse(m.message.value.toString('utf8'));

        dweetio.dweet_for(m.message.key.toString('utf8'), json, function(err, dweet){if(err != undefined) console.log(err)});
        sleep.sleep(5);
    });
};


return consumer.init().then(function () {
    return consumer.subscribe('test', dataHandler);
});

