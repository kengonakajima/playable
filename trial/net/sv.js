//


//require.paths.push('/usr/local/lib/node_modules');

var server = require('http').createServer(function(req, res){
  res.writeHead(200, {'Content-Type': 'text/html'});
  res.end('server connected');
});
server.listen(1337);

var io = require('socket.io').listen(server);
io.sockets.on('connection', function (socket) {
  socket.emit('info', { msg: 'welcome' });
  socket.on('msg', function (msg) {
    io.sockets.emit('msg', {msg: msg});
  });
  socket.on('disconnect', function(){
    socket.emit('info', {msg: 'bye'});
  });
  socket.on("error", function() {
    console.log("error");
  });
});

