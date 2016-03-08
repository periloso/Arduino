$(function() {
  var socket = io();
  socket.on('datapacket', function(pkt){
    var packettable = $('#packettable tbody');
    packettable.append('<tr><td>' + intToHex(pkt.id, 4) + '</td><td>' + intToHex(pkt.src, 8) + '</td><td>' + intToHex(pkt.dst, 8) + '</td><td>' + intToHex(pkt.type, 2) + '</td><td>' + intToHex(pkt.seq, 2) + '</td><td>' + pkt.payload + '</td></tr>');
    packettable.scrollTop(packettable.prop('scrollHeight'));
  });
});

function intToHex(int, padding) {
  return '0x' + (pad((int).toString(16), padding)).toUpperCase();
}

function pad(string, padding) {
  return (Array(padding).join('0') + string).slice(-padding);
}
