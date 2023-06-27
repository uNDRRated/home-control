#include "../headers/includes.h"

const char index_html[] = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://ndrr.xyz/home-control/home-control.css" crossorigin="anonymous">
</head>

<body>
  <h2>ESP DS18B20 Server</h2>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="ds-labels">Temperature Celsius</span> 
    <span id="temperaturec">%TEMPERATUREC%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="ds-labels">Temperature Fahrenheit</span>
    <span id="temperaturef">%TEMPERATUREF%</span>
    <sup class="units">&deg;F</sup>
  </p>
  <form id="roomOnForm" onsubmit="sendRoomOnRequest(); return false;">
    <input type="submit" value="Turn Room On">
  </form>
  <form id="roomOffForm" onsubmit="sendRoomOffRequest(); return false;">
    <input type="submit" value="Turn Room Off">
  </form>
  <input type="color" id="colorPicker" onchange="sendColorRequest(this.value)">
</body>
<script>
function sendColorRequest(color) {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/setColor?color=" + encodeURIComponent(color), true);
  xhr.send();
}
function sendRoomOnRequest() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/roomOn", true);
  xhr.send();
}
function sendRoomOffRequest() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/roomOff", true);
  xhr.send();
}
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperaturec").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperaturec", true);
  xhttp.send();
}, 10000) ;
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperaturef").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperaturef", true);
  xhttp.send();
}, 10000) ;
</script>
</html>)rawliteral";