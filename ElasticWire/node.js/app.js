#!/usr/bin/env node
"use strict"


// HTTP section
let express = require('express');
let webapp = express();

webapp.get('/', function(req, res) {
  res.render(__dirname + '/views/index');
});

webapp.set('view engine', 'ejs');
webapp.use('/', express.static(__dirname + '/public'));
let http = require('http').Server(webapp);
let io = require('socket.io')(http);
http.listen(3000, function() {
  console.log('listening on *:3000');
});

io.on('connection', function(socket){
  console.log('a user connected');
  socket.on('disconnect', function(){
    console.log('user disconnected');
  });

  socket.on('message', function(msg){
    console.log('message: ' + msg);
  });
});


// Serial port section
let serialPort = require("serialport");
let SerialPort = serialPort.SerialPort;
let sp  = new SerialPort('/dev/tty.usbserial', {
  baudrate: 19200,
  parser: serialPort.parsers.raw
});


// Packet handling
let PacketHandler = require('./PacketHandler');
let buffer = new Buffer(0);

sp.on('open', function() {
  console.log('Port open.');
  sp.on('data', function(data) {
    buffer = Buffer.concat([buffer, new Buffer(data)]);

    // Try to decode it
    let decoded = PacketHandler.handle(buffer);
    if (!decoded.error) {   // Packet is incomplete, we need to keep adding stuff
      // console.log('new data available.');
      // console.log(decoded.packet);
      io.emit('datapacket', decoded.packet);
      // console.log('Packet decoded successfully.');
    } else {                // The packet wasn't complete or something
      if (decoded.error !== 'incomplete') {
        console.log("Don't know what the error is about.");
      }
    }
    buffer = decoded.buffer;
  });
});
