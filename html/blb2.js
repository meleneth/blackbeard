var Tab = Class.create();
Tab.prototype = {
    initialize: function() {
    },

};

var TabManager = Class.create();
TabManager.prototype = { 
    initialize: function(){
        this.tabs = $H({});
        this.current_tab = $('default_tab');
    },
    finish_switch: function() {
        new Effect.BlindDown(this.current_tab)
    },
    add_tab: function(tab_name) {
        var new_tab = Builder.node("div", "No Info in Tab");
        new_tab.hide();
        this.tabs = this.tabs.merge($H((tab_name, new_tab)));
        $('tab_body').appendChild(new_tab);
        return new_tab;
    },
    div_for_tab: function(tab_name) {
        this.tabs.each(function(tab){
            if(tab_name == tab.name) {
                return tab.value;
            }
        });
        return this.add_tab(tab_name);
    },
    switch_to: function(screen_name) {
        if(this.current_tab) {
          var me = this;
          new Effect.BlindUp(this.current_tab, { afterFinish: function() { me.finish_switch() } })
          this.current_tab = this.div_for_tab(screen_name);
        } else {
          this.current_tab = this.div_for_tab(screen_name);
          this.finish_switch();
        }
    },
};

var tabs = new TabManager();

var UserInterface = Class.create();
UserInterface.prototype = {
    initialize: function(){
    },
    show_newsgroups: function(){
        tabs.switch_to('NewsGroups);
        var tag = tabs.div_for_tab('NewsGroups');
       fetch_data('/newsgroups', tag);
    }
};

var ui = new UserInterface();


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

function fetch_data(from_where, to_where)
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
  $('jobs_rate').replaceChild(document.createTextNode(jobs), $('jobs_rate').firstChild);
  $('k_rate').replaceChild(document.createTextNode(krate), $('k_rate').firstChild);
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
    var thediv = tabs.div_for_tab('NewsGroups');
    var old = thediv.replaceChild(getResponseTable(results), thediv.firstChild);
    old = null;
  }

  refresh_timer = setTimeout('RequeueFetch()', 5000);
}

function getResponseTable(data) 
{
  var my_table = Builder.node('table');
  var my_tbody = Builder.node('tbody', {id: "ResponseTable"} );
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
  var row  = Builder.node('tr', {id: results.shift()} ); 
  for(var j=0; j< results.length; j++){
    var cell = Builder.node('td');
    var a; 
    var sub_data = results[j].split("|");
    if(sub_data.length == 1) {
        a = Builder.node('a', sub_data[0]);
    } else {
        a = Builder.node('a', sub_data[1]);
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
  var li = Builder.node('li', info);
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
    var pre = Builder.node('pre');
    pre.appendChild(Builder.node(data));
    var old = thediv.replaceChild(pre, thediv.firstChild);
    old = undef;
    return false;
}

var http = getHTTPObject(); // We create the HTTP Object
var http_busy = 0;

