var fs = require("fs");
var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);


var text = fs.readFileSync("./new.pgm").toString('utf-8');
var textByLine = text.split("\n");

app.get('/', function(req, res){
  res.sendFile(__dirname + '/index.html');
});

app.get('/map.png', function(req, res){
  res.sendFile(__dirname + '/map.png');
});

io.on('connection', function(socket){
  socket.on('chat message', function(msg){
    console.log('message: ' + msg);
  });
});

io.on('connection', function(socket){
  console.log('a user connected');
        socket.on('disconnect', function(){
    console.log('user disconnected');
  });
});

// emitting data to web.
io.on('connection', function (socket) { // Notify for a new connection and pass the socket as parameter.
    console.log('new connection');
    setInterval(function () {
	socket.emit('textByLine', textByLine); // Emit on the opened socket.
}, 250);
});


http.listen(16888, function(){
  console.log('listening on *:16888');
});

//console.log(textByLine)
