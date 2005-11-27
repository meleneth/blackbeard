var last_data_fetch;
var refresh_timer;

function fetch_data(from_where)
{
    if(refresh_timer){
       clearTimeout(refresh_timer);
    }
   get_data(from_where);
}

function update_meters(jobs, krate)
{
    id = document.getElementById('jobs_rate');
    id.replaceChild(document.createTextNode(jobs), id.firstChild);

    id = document.getElementById('k_rate');
    id.replaceChild(document.createTextNode(krate), id.firstChild);
}

function get_data(from_where) {
  if(http_busy){
      return false;
  }
  last_data_fetch = from_where;
  http.open("GET", from_where, true);
  http.onreadystatechange = handleHttpResponse;
  http.send(null);
  http_busy = 1;
  return false;
    
}

function RequeueFetch() {
    get_data(last_data_fetch);
}

function handleHttpResponse() {
  if(http_busy) {
    if (http.readyState == 4) {
      // Split the comma delimited response into an array
      results = http.responseText.split("\n");
      eval(results[0]);
      onclick_format = results[1];
      onclick_regex = /%s/;

      thediv = document.getElementById('content');
      http_busy = 0;

      l = document.createElement('ul');
      for(var i = 2; i < results.length; i++){
        if(results[i]){
          myresults = results[i].split("|");
          item = document.createElement('li');
          link = document.createElement('a');
          x = document.createTextNode(myresults[1]);
          link.appendChild(x);
          my_func_text = "function() { " + onclick_format + "; return false; };";
          link.onclick = eval(my_func_text.replace(onclick_regex, myresults[0]));
          item.appendChild(link);
          l.appendChild(item);
        } 
      }
      thediv.replaceChild(l, thediv.firstChild);
      refresh_timer = setTimeout('RequeueFetch()', 5000);
    }
  }
  
}

function getHTTPObject() {
  var xmlhttp;
  /*@cc_on
  @if (@_jscript_version >= 5)
    try {
      xmlhttp = new ActiveXObject("Msxml2.XMLHTTP");
    } catch (e) {
      try {
        xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
      } catch (E) {
        xmlhttp = false;
      }
    }
  @else
  xmlhttp = false;
  @end @*/
  if (!xmlhttp && typeof XMLHttpRequest != 'undefined') {
    try {
      xmlhttp = new XMLHttpRequest();
    } catch (e) {
      xmlhttp = false;
    }
  }
  return xmlhttp;
}

var http = getHTTPObject(); // We create the HTTP Object
var http_busy = 0;

