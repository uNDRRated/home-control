#include "../headers/includes.h"

const char index_html[] = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>

<body>
  <h2>HUZZAH SmartHomeController</h2>
  <div class="sensor-data">
    <span class="dht-labels">Temperature</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
    <span class="dht-labels">Humidity</span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">%</sup>
  </div>

  <div class="container light-control" style="display: flex; justify-content:center">
    <div class="living-room">
      <form id="roomLivingOn" onsubmit="sendRoomLivingOnRequest(); return false;">
        <input type="submit" value="Turn living room on">
      </form>
      <form id="roomLivingOff" onsubmit="sendRoomLivingOffRequest(); return false;">
        <input type="submit" value="Turn living room off">
      </form>

      <span>Color picker: Living Room</span>
      <input type="color" id="colorPickerLivingRoom" onchange="sendColorLivingRoomRequest(this.value)">
    </div>

    <div class="kitchen" style="padding-left: 1rem">
      <form id="roomKitchenOn" onsubmit="sendRoomKitchenOnRequest(); return false;">
        <input type="submit" value="Turn kitchen on">
      </form>
      <form id="roomKitchenOff" onsubmit="sendRoomKitchenOffRequest(); return false;">
        <input type="submit" value="Turn kitchen off">
      </form>

      <span>Color picker: Kitchen</span>
      <input type="color" id="colorPickerKitchen" onchange="sendColorKitchenRequest(this.value)">
    </div>
  </div>

  <div class="animations">
    <form id="setAnimation" onsubmit="sendAnimationRequest(this.elements.animation.value); return false;">
      <select name="animation" id="animationSelect">
        <option value="0">Static</option>
        <option value="1">Increment</option>
        <option value="2">Light up</option>
        <option value="3">Reverse increment</option>
      </select>
      <input type="submit" value="Set animation">
    </form>
  </div>

  <div class="eeprom-content">
    <form id="saveEEPROM" onsubmit="sendSaveEEPROMRequest(); return false;">
      <input type="submit" value="SAVE SETTINGS">
    </form>

    <form id="loadEEPROM" onsubmit="sendLoadEEPROMRequest(); return false;">
      <input type="submit" value="LOAD SETTINGS">
    </form>
  </div>
</body>
<style>
body {
  font-family: Arial, sans-serif;
  margin: 0;
  padding: 20px;
  background-color: #0e0e0e;
  color: white;
}

h2 {
  text-align: center;
}

#setAnimation {
  justify-content: center;
  align-items: center;
}

.sensor-data {
  text-align: center;
  margin-bottom: 20px;
}

.dht-labels {
  font-weight: bold;
  margin-right: 5px;
}

.units {
  font-size: 0.8em;
  margin-left: 2px;
}

.container {
  display: flex;
  justify-content: space-between;
  margin-bottom: 20px;
}

.living-room,
.kitchen {
  display: flex;
  flex-direction: column;
  align-items: center;
}

.living-room input[type="submit"],
.kitchen input[type="submit"],
#setAnimation input[type="submit"],
#saveEEPROM input[type="submit"],
#loadEEPROM input[type="submit"] {
  display: block;
  margin-bottom: 10px;
  padding: 8px;
  background-color: #4e4caf;
  color: white;
  border: none;
  text-align: center;
  text-decoration: none;
  font-size: 14px;
  cursor: pointer;
}

.living-room input[type="color"],
.kitchen input[type="color"] {
  width: 100%;
  height: 30px;
  margin-bottom: 10px;
  border: none;
  background-color: #4e4caf;
}

#setAnimation {
  display: flex;
  align-items: baseline;
}

#animationSelect {
  margin-right: 10px;
  padding: 5px;
}

.eeprom-content {
  text-align: center;
}

.eeprom-content input[type="submit"] {
  display: block;
  margin: 0 auto;
  padding: 8px;
  background-color: #2196F3;
  color: white;
  border: none;
  text-align: center;
  text-decoration: none;
  font-size: 14px;
  cursor: pointer;
}
</style>
<script>

window.addEventListener('load', restoreColor);

function sendColorKitchenRequest(color) {
  localStorage.setItem('colorKitchen', color);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/setColorKitchen?color=" + encodeURIComponent(color), true);
  xhr.send();
}

function sendColorLivingRoomRequest(color) {
  localStorage.setItem('colorLivingRoom', color);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/setColorLivingRoom?color=" + encodeURIComponent(color), true);
  xhr.send();
}

function restoreColor() {
  var colorKitchen = localStorage.getItem('colorKitchen');
  var colorLivingRoom = localStorage.getItem('colorLivingRoom');
  if (colorKitchen) {
    document.getElementById('colorPickerKitchen').value = color;
  }

  if (colorLivingRoom) {
    document.getElementById('colorPickerLivingRoom').value = color;
  }
}

var lastSelectedAnimation = localStorage.getItem('lastSelectedAnimation');
if (lastSelectedAnimation) {
  document.getElementById('animationSelect').value = lastSelectedAnimation;
}

function sendAnimationRequest(animation) {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/setAnimation?animation=" + encodeURIComponent(animation), true);
  xhr.send();

  localStorage.setItem('lastSelectedAnimation', animation);
}

function sendSaveEEPROMRequest() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/saveEEPROM", true);
  xhr.send();
}

function sendLoadEEPROMRequest() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/loadEEPROM", true);
  xhr.send();
}

function sendRoomLivingOnRequest() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/roomLivingOn", true);
  xhr.send();
}

function sendRoomLivingOffRequest() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/roomLivingOff", true);
  xhr.send();
}

function sendRoomKitchenOnRequest() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/roomKitchenOn", true);
  xhr.send();
}

function sendRoomKitchenOffRequest() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/roomKitchenOff", true);
  xhr.send();
}

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 10000 ) ;

</script>
</html>)rawliteral";