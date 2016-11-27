'use strict'

const Telegram = require('telegram-node-bot')
const TelegramBaseController = Telegram.TelegramBaseController
const TextCommand = Telegram.TextCommand
const tg = new Telegram.Telegram('273793514:AAGQ8L2qXixlS6EazGeTJZ2ta8hmNldwB80')
var dweetClient = require("node-dweetio");
var dweetio = new dweetClient();

var kafka = require('kafka-node'),
    HighLevelProducer = kafka.HighLevelProducer,
    client = new kafka.Client(),
    producer = new HighLevelProducer(client);

class QueryController extends TelegramBaseController {
    /**
     * @param {Scope} $
     */
    queryHandler($) {
        dweetio.get_latest_dweet_for("raspberry-casa", function(err, dweet){
            if(err == null) {
              var dweet = dweet[0]; // Dweet is always an array of 1

              console.log(dweet.thing); // The generated name
              console.log(dweet.content); // The content of the dweet
              console.log(dweet.created); // The create date of the dweet
              $.runMenu({
                  message: "Selecciona la medida a mostar!",
                  oneTimeKeyboard: true,
                  options: {
                      parse_mode: 'Markdown'
                  },
                  layout: 2,
                  'temperatura': () =>
                      {
                           $.sendMessage(dweet.content.temperatura)
                      },
                  'humedad': () =>
                      {
                           $.sendMessage(dweet.content.humedad)
                      },
                  'lluvia': () =>
                      {
                           $.sendMessage(dweet.content.lluvia)
                      }
              })
            } else {
              console.log(err);
            }
        });
    }

    get routes() {
        return {
            'QueryCommand': 'queryHandler'
        }
    }
}

const form = {
    dev: {
        q: 'What is the device that you want to modify?',
        error: 'sorry, wrong input',
        validator: (message, callback) => {
            if(message.text) {
                callback(true, message.text) //you must pass the result also
                return
            }

            callback(false)
        }
    },
    sensor: {
        q: 'What is the sensor that you want to modify?',
        error: 'sorry, wrong input',
        validator: (message, callback) => {
            if(message.text) {
                callback(true, message.text) //you must pass the result also
                return
            }

            callback(false)
        }
    },
    status: {
        q: "What is the new status's sensor?",
        error: 'Sorry, valid status: ON/OFF',
        validator: (message, callback) => {
            if(message.text &&
                (message.text.toLowerCase() === 'on' ||
                 message.text.toLowerCase() === 'off')) {
                   callback(true, message.text.toUpperCase()) //you must pass the result also
                   return
            }

            callback(false)
        }
    }
}

class LedController extends TelegramBaseController {
    /**
     * @param {Scope} $
     */
    ledHandler($) {
      $.runForm(form, (result) => {
          var data = {DEV: result.dev, SENSOR: result.sensor, STATE: result.status}
          var payload = [{ topic: 'actions', messages: JSON.stringify(data) }];
          producer.send(payload, function (err, data) {
              if(err != null) {
                $.sendMessage("Your request wasn't processed correctly :(");
                console.log(err);
              }
              else {
                $.sendMessage('Your request was processed correctly :)');
              }
          });
      })

    }

    get routes() {
        return {
          'LedCommand': 'ledHandler'
        }
    }
}

tg.router
  .when(new TextCommand('/query', 'QueryCommand'), new QueryController())
  .when(new TextCommand('/led', 'LedCommand'),   new LedController())
