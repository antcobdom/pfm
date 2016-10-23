'use strict'

const Telegram = require('telegram-node-bot')
const TelegramBaseController = Telegram.TelegramBaseController
const TextCommand = Telegram.TextCommand
const tg = new Telegram.Telegram('273793514:AAGQ8L2qXixlS6EazGeTJZ2ta8hmNldwB80')
var dweetClient = require("node-dweetio");
var dweetio = new dweetClient();

class PingController extends TelegramBaseController {
    /**
     * @param {Scope} $
     */
    queryHandler($) {
        dweetio.get_latest_dweet_for("raspberry-casa", function(err, dweet){

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
          if(dweet.content.type == 'T')
             $.sendMessage(dweet.content.value)
          else
             $.sendMessage("No disponible")
        }, 
    'humedad': () => 
        {
          if(dweet.content.type == 'H')
             $.sendMessage(dweet.content.value)
          else
             $.sendMessage("No disponible")
        }
})
        });
    }

    get routes() {
        return {
            'QueryCommand': 'queryHandler'
        }
    }
}

tg.router
    .when(
        new TextCommand('query', 'QueryCommand'),
        new PingController()
    )
