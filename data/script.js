window.addEventListener("load",app);
let i = 0;
let topSpeed = 0;
document.getElementById("speedPlaceHolder").innerHTML = "0";

function resetTopSpeed() {
	topSpeed = 0;
	document.getElementById("speedPlaceHolder").innerHTML = "0";
}

function app() {
	var oldStrLen = 0,
		charsEachSec = [],
		getWPM = (wpm) => {
			let arrow = document.querySelector(".arrow"),
				display = [
					document.getElementById("h"),
					document.getElementById("t"),
					document.getElementById("o"),
				],
				strLen = document.querySelector("textarea").value.length,
			// // WARNING: For GET requests, body is set to null by browsers.
			// var data = "";
			//
			// var xhr = new XMLHttpRequest();
			// xhr.withCredentials = true;
			//
			// xhr.addEventListener("readystatechange", function() {
			//   if(this.readyState === 4) {
			//     wpm = this.responseText;
			//   }
			// });
			//
			// xhr.open("GET", "192.168.1.1/speed");
			// xhr.setRequestHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9");
			// xhr.setRequestHeader("Accept-Encoding", "gzip, deflate, br");
			// xhr.setRequestHeader("Accept-Language", "en-US,en;q=0.9,zh-CN;q=0.8,zh;q=0.7");
			//
			// xhr.send(data);

			// make old string length equal to newest one before calculating WPM again
			// set ceiling for and rotate arrow
			 maxWpm = 80,
				arrowWpm = wpm < maxWpm ? wpm : maxWpm;

			arrow.style.transform = "rotate(" + ((arrowWpm * 3) - 120) + "deg) translateY(-72%)";

			// make WPM string, clean digits, redisplay digits
			let wpmStr = wpm.toString();

			for (var d of display)
				d.className = "_";

			for (var i in wpmStr)
				display[display.length - 1 - i].className = "_" + wpmStr[wpmStr.length - 1 - i];
		};

	// runtime loop
	var run = () => {


		// WARNING: For GET requests, body is set to null by browsers.
		// WARNING: For GET requests, body is set to null by browsers.
		var data = "";

		var xhr = new XMLHttpRequest();
		xhr.withCredentials = true;

		xhr.addEventListener("readystatechange", function() {
		  if(this.readyState === 4) {
				var speed = parseInt(this.responseText);
				if (speed > topSpeed){
					topSpeed = speed;
					document.getElementById("speedPlaceHolder").innerHTML = topSpeed;
				}
		    getWPM(parseInt(this.responseText));
		  }
		});
		xhr.open("GET", "speed");
		xhr.send(data);
		setTimeout(run,50);
	};
	run();
};
