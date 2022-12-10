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
    var xhr = new XMLHttpRequest();
      xhr.open("GET", "/rainbow", true);
      xhr.send();
  }
 function strobe() {
  var xhr = new XMLHttpRequest();
    xhr.open("GET", "/strobe", true);
    xhr.send();
}

function fade() {
 var xhr = new XMLHttpRequest();
   xhr.open("GET", "/fade", true);
   xhr.send();
}

colorPicker.on(["color:change"], function (color) {
  var xhr = new XMLHttpRequest();
    xhr.open("GET", "/front?R="+color.red.toString()+"&G="+color.green.toString()+"&B="+color.blue.toString(), true);
    xhr.send();
});

colorPicker2.on(["color:change"], function (color) {
  var xhr = new XMLHttpRequest();
    xhr.open("GET", "/back?R="+color.red.toString()+"&G="+color.green.toString()+"&B="+color.blue.toString(), true);
    xhr.send();
});
