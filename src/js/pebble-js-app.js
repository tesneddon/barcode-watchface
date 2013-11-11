/*

   ....

*/

Pebble.addEventListener("ready", function() {
    console.log("redy called!");
});

Pebble.addEventListener("showConfiguration", function() {
    console.log("showing configuration");
    Pebble.openURL('github...');
});

Pebble.addEventListener("webviewclosed", function(e) {
    console.log("configuration closed");
    var options = JSON.parse(decodeURIComponent(e.response));
    console.log("Options = " + JSON.stringify(options));
});
