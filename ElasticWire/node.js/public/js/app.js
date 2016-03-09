$(function() {
  var socket = io();
  socket.on('datapacket', function(pkt){
    var packettable = $('#packettable tbody');
    packettable.append('<tr><td>' + intToHex(pkt.id, 4) + '</td><td>' + intToIP(pkt.src, 8) + '</td><td>' + intToIP(pkt.dst, 8) + '</td><td>' + intToHex(pkt.type, 2) + '</td><td>' + intToHex(pkt.seq, 2) + '</td><td>' + pkt.payload + '</td></tr>');
    packettable.scrollTop(packettable.prop('scrollHeight'));
  });
});

function intToIP(int) {
  return ((int >> 24) & 0xff) + '.' + ((int >> 16) & 0xff) + '.' + ((int >> 8) & 0xff) + '.' + (int & 0xff);
}

function IPToInt(string) {
  var d = dot.split('.');
  return ((((((+d[0])*256)+(+d[1]))*256)+(+d[2]))*256)+(+d[3]);
}

function intToHex(int, padding) {
  return '0x' + (pad((int).toString(16), padding)).toUpperCase();
}

function pad(string, padding) {
  return (Array(padding).join('0') + string).slice(-padding);
}
