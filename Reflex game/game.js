 function game() {
      if(boxes==0) {
        time.innerHTML="Game Over";
        if(points > highscore){                 //keeps track of your highest score so player can strive to do better. Max score possible is 10,000
          highscore=Math.round(points);
          record.innerHTML="High Score: "+highscore;
        }
        boxes=10;
        points=0;
        numofboxes.innerHTML=boxes+" boxes left";
        start.disabled=false;                         //enables the player to play again without having to refresh the browser
      }
      else {
      clock=setInterval(changeClock,100);
      var draw= box.getContext("2d");
      x= Math.floor(Math.random()*451); //Random x,y location of the box within the canvas
      y= Math.floor(Math.random()*451);
      boxdim=Math.floor((Math.random()*36) + 15); // randomly generates the boxes dimension
      draw.fillRect(x, y, boxdim, boxdim)
      start.disabled=true;                        // disables start button once it is pressed
    }
};

function changeClock() {  //keeps track of how long it takes for the player to click on the box.
    var time= document.getElementById('Timer')
    time.innerHTML = "Time "+count.toFixed(1);
    count += 0.1;
};

function clear() {
    box.getContext("2d").clearRect(0,0, box.width, box.height);
};

function getPosition(el) { //Gets the position of the mouse no matter the size of the window.
  var xPosition = 0;
  var yPosition = 0;

  while (el) {
    xPosition += (el.offsetLeft - el.scrollLeft + el.clientLeft);
    yPosition += (el.offsetTop - el.scrollTop + el.clientTop);
    el = el.offsetParent;
  }
  return {
    x: xPosition,
    y: yPosition
  };
}

function getClickPosition(e) {
    var Canv = document.querySelector("#myCanvas");
    var ParentPosition = getPosition(e.currentTarget);
    var xposition=  e.clientX - ParentPosition.x - (Canv.clientWidth/500);
    var yposition= e.clientY - ParentPosition.y - (Canv.clientHeight/500);
    if(xposition >=x && yposition >=y && xposition <= x+boxdim && yposition <= y+boxdim){   //checks if click was inside the box
        clearTimeout(clock);
        total();
        boxes-=1;
        count=0;                                                                          //sets timer back to zero
        numofboxes.innerHTML=boxes+" boxes left";
        clear();   //After the box is clicked, the board is cleared and a new box is generated.
        game();
      }
};

function total() {            //determines score based on time. The faster you click on the box, the more points you earn.
    var multiplier=0;
    if(count <= 0.5){
      multiplier=500;
      points+=2*multiplier;
      score.innerHTML="Score "+Math.round(points);
    }
    else if (count <=0.8) {
      multiplier=400;
      points+=1.6*multiplier;
      score.innerHTML="Score "+Math.round(points);
    }
    else if (count <= 1.0 ) {
      multiplier=300;
      points+=1.2*multiplier;
      score.innerHTML="Score "+Math.round(points);
    }
    else {
      multiplier=200;
      points+=multiplier;
      score.innerHTML="Score "+Math.round(points);
    }
};
var highscore=0;
var record= document.getElementById('highscore');
var points=0;
var count=0;
var boxes=10;
var numofboxes= document.getElementById('remainder');
var box= document.getElementById('myCanvas');
var start= document.getElementById('initialize');
var time= document.getElementById('Timer')
var score= document.getElementById('score');

box.addEventListener('click',getClickPosition,false);
start.addEventListener('click', game, false);
