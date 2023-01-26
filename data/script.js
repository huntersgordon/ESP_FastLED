 var gateway = `ws://${window.location.hostname}/ws`;
 var websocket;
 window.addEventListener('load', onLoad);
 function initWebSocket() {
   console.log('Trying to open a WebSocket connection...');
   websocket = new WebSocket(gateway);
   websocket.onopen    = onOpen;
   websocket.onclose   = onClose;
   websocket.onmessage = onMessage; // <-- add this line
 }
 function onOpen(event) {
   console.log('Connection opened');
 }
 function onClose(event) {
   console.log('Connection closed');
   setTimeout(initWebSocket, 2000);
 }
  function onLoad(event) {
   initWebSocket();
   initButton();
 }
 function initButton() {
   document.getElementById('button').addEventListener('click', toggle);
 }


var colorPicker = new iro.ColorPicker(".colorPicker", {
  layout: [
    {
      component: iro.ui.Wheel,
      options: {}
    }
  ],
  width: 200,
  color: "rgb(255, 0, 0)",
  borderWidth: 1,
  borderColor: "#fff" });

var colorPicker2 = new iro.ColorPicker(".colorPicker", {
  layout: [
    {
      component: iro.ui.Wheel,
      options: {}
    }
  ],
  width: 200,
  color: "rgb(255, 0, 0)",
  borderWidth: 1,
  borderColor: "#fff" });

  function rainbow() {
    websocket.send('rainbow');
  }
 function strobe() {
  websocket.send('strobe');
}

function fade() {
 var xhr = new XMLHttpRequest();
   websocket.send('fade');
}

colorPicker.on(["color:change"], function (color) {
  //Front:1 -> front ; Front:0 -> back
  websocket.send(JSON.stringify({
    front: 1,
    R: color.red,
    G: color.green,
    B: color.blue
  }));
});

colorPicker2.on(["color:change"], function (color) {
  websocket.send(JSON.stringify({
    front: 0,
    R: color.red.toString(),
    G: color.green.toString(),
    B: color.blue.toString()
  }));
  // var xhr = new XMLHttpRequest();
  //   xhr.open("GET", "/back?R="+color.red.toString()+"&G="+color.green.toString()+"&B="+color.blue.toString(), true);
  //   xhr.send();
});
