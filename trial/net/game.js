

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