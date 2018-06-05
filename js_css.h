String webSite, javaScript,css;
const int relays = 3;        // This sets the number of sliders you want
int relaysVal[relays];
void buildCss(){
  css = "<style>\n";
  css += "body {margin: 0;font-family: 'Source Sans Pro', 'Helvetica Neue', Helvetica, Arial, sans-serif;font-size: 14px;line-height: 1.42857143;color: #839496;background-color: #002b36;}\n";
  css += "html {font-family: sans-serif;-ms-text-size-adjust: 100%;-webkit-text-size-adjust: 100%;font-size: 10px;-webkit-tap-highlight-color: rgba(0, 0, 0, 0);}\n";
  css += ".relay{height:100px;width:170px;background-color: #586e75;border-color: #586e75;  -webkit-transition-duration: 0.3s;transition-duration: 0.3s;}\n";
  css += ".display-middle{position:absolute;top:100px;left:50%;transform:translate(-50%,0);-ms-transform:translate(-50%,0)}\n";
  css += ".box {display: inline-block;margin: 5px;font-weight: normal;text-align: center;vertical-align: middle;-ms-touch-action: manipulation;touch-action: manipulation;border: 1px solid transparent;padding: 10px 12px;padding-bottom: 0px;font-size: 20px;line-height: 1.42857143;border-radius: 4px;}\n";
  css += ".relay:hover {color: #ffffff;text-decoration: none;box-shadow: 0 5px 16px 0 rgba(0,0,0,0.8), 0 10px 50px 0 rgba(0,0,0,0.19);}\n";
  css += ".btn-primary {color: #ffffff;background-color: #2aa198;border-color: #2aa198;position:relative;font-size: 14px;width:100px;top:20px;cursor: pointer;padding: 6px;}\n";
  css += ".btn-primary:hover {color: #ffffff;background-color: #1f7972;border-color: #1d706a;}\n";
  css += ".btn-danger {color: #ffffff;background-color: #d33682;border-color: #d33682;position:relative;font-size: 14px;width:100px;top:20px;cursor: pointer;padding: 6px;}\n";
  css += ".btn-danger:hover {color: #ffffff;background-color: #b02669;border-color: #a72564;}\n";
  css += "</style>\n";
}
void buildJavascript() {
  javaScript = "<SCRIPT>\n";
  javaScript += "var websock;\n";
  javaScript += "function start() {\n";
  javaScript += "  websock = new WebSocket('ws://' + window.location.hostname + ':81/');\n";
  javaScript += "  websock.onopen = function(evt) { console.log('websock open'); };\n";
  javaScript += "  websock.onclose = function(evt) { console.log('websock close'); };\n";
  javaScript += "  websock.onmessage = function(evt) {\n";
  javaScript += "if (evt.data.charAt(0)=='#'){\n";
  javaScript += "var relayNum = parseInt(evt.data.substring(1,evt.data.indexOf(',')));\n";
  javaScript += "var relayVal = parseInt(evt.data.substring(evt.data.indexOf(',')+1));\n";
  javaScript += "if (relayVal==0) changeState(relayNum,'OFF');\n";
  javaScript += "if (relayVal==1) changeState(relayNum,'ON');\n";
  javaScript += "}\n";
  javaScript += "else{\n";
  javaScript += "concole.log('Unknown command');\n";
  javaScript += "}\n";
  javaScript += "};\n";
  javaScript += "  websock.onerror = function(evt) {console.log(evt);};}\n";
  javaScript += "start();\n";

  javaScript += "function changeState(id, state){\n";
  javaScript += "  var div = document.getElementById(id);\n";
  javaScript += "  if(state == 'OFF'){\n";
  javaScript += "    div.style.borderTopColor = '#F00';\n";
  javaScript += "    div.children[0].innerHTML = 'ON';\n";
  javaScript += "    div.children[0].className = 'box btn-primary';}\n";
  javaScript += "  else if(state == 'ON'){\n";
  javaScript += "    div.style.borderTopColor = '#0F0';\n";
  javaScript += "    div.children[0].innerHTML = 'OFF';\n";
  javaScript += "    div.children[0].className = 'box btn-danger';}}\n";

  javaScript += "function sendChangeRequest(id, state){\n";
  javaScript += "  websock.send('#'+String(id) +','+ String(state));\n";
  javaScript += "}\n";

  javaScript += "function createBox(name, id){\n";
  javaScript += "  var div = document.createElement('div');\n";
  javaScript += "  div.className = 'box relay';\n";
  javaScript += "  div.innerHTML = name;\n";
  javaScript += "  div.id = id;\n";
  javaScript += "  div.style.borderTopColor = '#0F0';\n";
  javaScript += "  var btn = document.createElement('div');\n";
  javaScript += "  btn.className = 'box btn btn-danger';\n";
  javaScript += "  btn.innerHTML = 'OFF';\n";
  javaScript += "  btn.addEventListener('click',function (){sendChangeRequest(id, this.innerHTML);});\n";
  javaScript += "  div.appendChild(btn);\n";
  javaScript += "  document.getElementById('middle-div').appendChild(div);}\n";

  javaScript += "for (i = 0; i <"+String(relays)+"; i++) {\n";
  javaScript += "  createBox(String(i+1) + 'th Lamp', i);}\n";
  javaScript += "changeState(0, 'OFF');\n";
  javaScript += "changeState(1, 'OFF');\n";
  javaScript += "changeState(2, 'OFF');\n";
  javaScript += "</SCRIPT>\n";
}

