

var sol_img;

$(document).ready( function() {
  console.log("game..");

  //
  
  sol_img = new Image();
  sol_img.src = "sol16.png";

  //
  
  setInterval( loopfunc, 16 );
});

function loopfunc() {


  var canv = $("#gamemain")[0];
  

  var ctx = canv.getContext("2d");

  ctx.clearRect(0,0,640,480);
  ctx.imageSmoothingEnabled = false;
  ctx.webkitImageSmoothingEnabled = false;



  ctx.drawImage( sol_img, 50,50 + irange(0,100), 32,32 );

}




// client
$(function(){
  var socket = io.connect('http://127.0.0.1:1337/');
  socket.on('connect', function() {
    
    $("#log").html($("#log").html() + "<br />" + 'connected');
    
    socket.on('info', function (data) {
      $("#log").html($("#log").html() + "<br />" + data.msg);
    });
    
    socket.on('msg', function(data){
      var d = new Date();
      console.log( d, d.getMilliseconds() );  
      $("#log").html($("#log").html() + "<br />" + "<b>" + data.msg + "</b>");
    });
    
    socket.on("ch", function(data){
      print("name:", data.name );
    });
    
    $("#send").click(function(){
      var msg = $("#msg").val();
      if(!msg){
        alert("input your message");
        return;
      }
      var d = new Date();
      console.log( d, d.getMilliseconds() );
      socket.emit('msg', msg);
    });
  });
});
