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

function random_n(min,max){
  return Math.floor(Math.random() * (max - min + 1)) + min;
}

let data = [0]
let labels = ['0s']
let label = 0

/*~~~~~~~~~~~~~~~~~~~~~~LINE CHART~~~~~~~~~~~~~~~~~~~~~~~*/
var ctx = document.getElementById('myChart').getContext('2d');
var myChart = new Chart(ctx, {
  type: 'line',
  data: {
    labels: labels,
    datasets: [{
      label: 'RC Car speed',
      data: data,
      backgroundColor: "rgba(153,255,51,0.6)"
    } //, {
    //   label: 'oranges',
    //   data: [2, 29, 5, 5, 2, 3, 10],
    //   backgroundColor: "rgba(255,153,0,0.6)"
    // }]
  ]
  }
});
async function addData(data){
  while (true) {
    await new Promise(r => setTimeout(r, 1000));
    //GET CURRENT SPEED FROM THE RC CAR
    data.push(random_n(0,40));
    //////////////////////////
    labels.push(label.toString()+'s')
    label = label + 1;
    console.log(myChart.data.datasets[0]);
    myChart.update()


  }
}
addData(data);
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
