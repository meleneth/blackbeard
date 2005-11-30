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
    var id = document.getElementById('jobs_rate');
    id.replaceChild(document.createTextNode(jobs), id.firstChild);

    id = document.getElementById('k_rate');
    id.replaceChild(document.createTextNode(krate), id.firstChild);
}

function update_heading(new_heading)
{
  var h = document.getElementById('heading');
  h.replaceChild(document.createTextNode(new_heading), h.firstChild);
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
      var results = http.responseText.split("\n");
      var run_me = results.shift();
      eval(run_me);

      var onclick_format = results.shift();
      var onclick_regex = /%s/;

      thediv = document.getElementById('content');
      thediv.replaceChild(getResponseTable(onclick_format, onclick_regex, results), thediv.firstChild);

      refresh_timer = setTimeout('RequeueFetch()', 5000);
      http_busy = 0;
    }
  }
  
}

function getResponseTable(onclick_format, onclick_regex, data) 
{
  var my_table = document.createElement('table');
  var my_tbody = document.createElement('tbody');
  my_table.appendChild(my_tbody);

  for(var i = 0; i < data.length; i++){
    if(data[i]){
      var myresults = data[i].split("||");
      var row  = document.createElement('tr'); 
        for(var j=0; j< myresults.length; j++){

          var cell = document.createElement('td');
          var a    = document.createElement('a'); 
          
          var sub_data = myresults[j].split("|");
          if(sub_data.length == 1) {
              a.appendChild(document.createTextNode(sub_data[0]));
          } else {
              a.appendChild(document.createTextNode(sub_data[1]));
              var my_func_text = onclick_format;
              my_func_text = my_func_text.replace(onclick_regex, sub_data[0]);
              a.onclick = new Function(my_func_text);
          }
          cell.appendChild(a); row.appendChild(cell); 
      }
      my_tbody.appendChild(row);
    } 
  }
  return my_table;
}

function getResponseUl(onclick_format, onclick_regex, data) 
{
  var my_ul = document.createElement('ul');
  for(var i = 0; i < data.length; i++){
    if(data[i]){
      var myresults = data[i].split("|");
      var list_item = document.createElement('li');
      var a = document.createElement('a');
      a.appendChild(document.createTextNode(myresults[1]));

      var my_func_text = onclick_format;
      my_func_text = my_func_text.replace(onclick_regex, myresults[0]);
      a.onclick = new Function(my_func_text);

      list_item.appendChild(a);
      my_ul.appendChild(list_item);
    } 
  }
  return my_ul;
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

