var TabClass = Class.create();
TabClass.prototype = { 
    initialize: function(){
    },
    finish_switch: function() {
        new Effect.BlindDown($('content'))
    },
    switch_to: function(screen_name) {
        var me = this;
        new Effect.BlindUp($('content'), { afterFinish: function() { me.finish_switch() } })
        this.active_screen = screen_name;
    }


};

var tabs = new TabClass();

// Old Stuff

var last_data_fetch;
var refresh_timer;
var last_tick;
var mode;
var HTTPResponseHandler;
var previous_urls = new Array();
previous_urls.push("index.html");
var old_from_where = 'index.html';

function back_button()
{
    alert("would goto " + previous_urls.pop());
    // fetch_data(previous_urls.pop());
}

function fetch_data(from_where)
{
  previous_urls.push(old_from_where);
  if(refresh_timer){
    clearTimeout(refresh_timer);
  }
  mode = "replace";
  get_data(from_where);
  old_from_where = from_where;
}

function update_meters(jobs, krate)
{
  var id = $('jobs_rate');
  id.replaceChild(document.createTextNode(jobs), id.firstChild);

  id = $('k_rate');
  id.replaceChild(document.createTextNode(krate), id.firstChild);
}

function update_heading(new_heading)
{
  var h = $('heading');
  h.replaceChild(document.createTextNode(new_heading), h.firstChild);
}

function get_data(from_where) {
  if(http_busy){
      return false;
  }
  http_busy = 1;

  last_data_fetch = from_where;
  http.open("GET", from_where, true);
  http.onreadystatechange = handleHttpResponse;
  HTTPResponseHandler = data_response;
  http.send(null);
  return false;
}

function RequeueFetch() {
    if(last_data_fetch){
        get_data(last_data_fetch);
    }
}

function handleHttpResponse() {
  if(http_busy) {
    if (http.readyState == 4) {
        HTTPResponseHandler(http.responseText);
        http_busy = 0;
    }
  }
}

function data_response(data)
{
  var results = data.split("\n");
  var run_me = results.shift();
  eval(run_me);

  if(mode == "update"){
    updateResponseTable(results);
  }else{
    var thediv = $('content');
    var old = thediv.replaceChild(getResponseTable(results), thediv.firstChild);
    old = null;
  }

  refresh_timer = setTimeout('RequeueFetch()', 5000);
}

function getResponseTable(data) 
{
  var my_table = document.createElement('table');
  var my_tbody = document.createElement('tbody');
  my_tbody.id = "ResponseTable";
  my_table.appendChild(my_tbody);

  var header_classes = data.shift().split("|");
  data.each(function(row, index) {
    if(row){
      my_tbody.appendChild(getTableRow(row, header_classes));
    }
  });
  return my_table;
}

function updateResponseTable(data)
{
  var my_tbody = $('ResponseTable');
  var header_classes = data.shift().split("|");
  for(var i = 0; i < data.length; i++){
    if(data[i]){
        var row = getTableRow(data[i], header_classes);
        var old_row = $(row.id);
        if(old_row){
            my_tbody.replaceChild(row, old_row);
            old_row = null;
        }else{
            my_tbody.appendChild(row);
        }
    } 
  }
}

function getTableRow(data, header_classes) 
{
  var results = data.split("||");
  var row  = document.createElement('tr'); 
  row.id = results.shift();
  for(var j=0; j< results.length; j++){
    var cell = document.createElement('td');
    var a    = document.createElement('a'); 
    var sub_data = results[j].split("|");
    if(sub_data.length == 1) {
        a.appendChild(document.createTextNode(sub_data[0]));
    } else {
        a.appendChild(document.createTextNode(sub_data[1]));
        if(sub_data[0] && (sub_data[0] != " ")){
            //a.onclick = new Function(sub_data[0]);
            a.href = "javascript: " + sub_data[0];
        }
    }
    cell.appendChild(a); row.appendChild(cell); 
  }
  return row;
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

function ping_url(url) {
    /* runs a get on a url, but doesn't do anything with the response*/
    var req = getHTTPObject();
    req.open("GET", url, true);
    req.send(null);
}

function log_info(info)
{
  var el = $('jslog');
  var li = document.createElement('li');
  li.appendChild(document.createTextNode(info));
  el.appendChild(li);
}

function view_file(url)
{
  if(refresh_timer){
    clearTimeout(refresh_timer);
  }

  if(http_busy){
      return false;
  }

  http_busy = 1;
  last_data_fetch = 0;

  http.open("GET", url, true);
  HTTPResponseHandler = view_file_response;
  http.onreadystatechange = handleHttpResponse;
  http.send(null);
}

function view_file_response(data)
{
    var thediv = $('content');
    var pre = document.createElement('pre');
    pre.appendChild(document.createTextNode(data));
    var old = thediv.replaceChild(pre, thediv.firstChild);
    old = undef;
    return false;
}

var http = getHTTPObject(); // We create the HTTP Object
var http_busy = 0;

