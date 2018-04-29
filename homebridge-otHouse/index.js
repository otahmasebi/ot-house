var Service, Characteristic;
var request = require('sync-request');
var WebSocket = require('ws');
var url 
const ws = new WebSocket('ws://ot.local:81');

module.exports = function (homebridge) {
    Service = homebridge.hap.Service;
    Characteristic = homebridge.hap.Characteristic;
    homebridge.registerAccessory("homebridge-Switch", "OT-SWITCH", OT_SWITCH);
}


function OT_SWITCH(log, config) {
    this.log = log;

    // url info
this.lightNum = config["lightNum"] || '0';
    this.name = config["name"];

    this.state = true;
    var that = this;


ws.reconnectInterval = 1800; // try to reconnect after 3 seconds
ws.on('close', function ws_close(){
	a.log('websocket closed');
that.log('trying to connect again after 3 seconds');
});

ws.on('message', function ws_incoming(data) {
if (data.charAt(0)=='#'){
var relayNum = parseInt(data.substring(1,data.indexOf(',')));
var relayVal = parseInt(data.substring(data.indexOf(',')+1));
if (relayVal==0 && relayNum == that.lightNum) {that.state == false;}
if (relayVal==1 && relayNum == that.lightNum) {that.state == true;}}
});
ws.on('open', function open() {
	that.log('websocket opened');
});

}
OT_SWITCH.prototype = {
    getStatusState: function (callback) {
this.log("getting state");
        callback(null, this.state);
    },

    setPowerState: function(powerOn, callback) {

	state = "";
   if (powerOn) {
ws.send("#"+this.lightNum+",ON");
state = "ON";
        this.log("Setting power state to on");
    } else {
ws.send("#"+this.lightNum+",OFF");
state = "OFF";
        this.log("Setting power state to off");
    }
			callback();
    },

    getServices: function () {
        var informationService = new Service.AccessoryInformation();

        informationService
                .setCharacteristic(Characteristic.Manufacturer, "otahmasebi.com")
                .setCharacteristic(Characteristic.Model, "OT_house")
                .setCharacteristic(Characteristic.SerialNumber, "001");
        switchService = new Service.Switch(this.name);
            switchService
                .getCharacteristic(Characteristic.On)
                .on('get', this.getStatusState.bind(this))
                .on('set', this.setPowerState.bind(this));
        return [switchService];
    }
};
