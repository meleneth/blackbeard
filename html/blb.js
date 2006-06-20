var http_busy = 0;

function debug_log(text) 
{
  $("debug_log").appendChild(Builder.node("li", [text]));
}

var Tab = Class.create();
Tab.prototype = {
  initialize: function(name) {
    this.name = name;
    this.div = Builder.node("div");
    this.tbody = Builder.node("tbody");
    var link = "javascript: tabs.switch_to(\"" + name + "\");";
    this.link = Builder.node("a", [name]);
    this.link.href = link;
    this.next_data_fetch="";
    $('tabs').appendChild(this.link); 

    this.div.appendChild(Builder.node("table", [this.tbody]));
    this.div.hide();
  },
  update_data: function(data) {
  },
  update_url_data: function(url) {
    var req = new Ajax.Request( url, { method: 'get', onComplete: function(data){
      
    }});
    var current_tabname = this.name;
    
  }
};

var TabManager = Class.create();
TabManager.prototype = { 
    initialize: function(){
        this.tabs = [];
        this.current_tab = null;
    },
    finish_switch: function() {
        new Effect.BlindDown(this.current_tab)
    },
    add_tab: function(tab_name) {
        var new_tab = new Tab(tab_name);
        this.tabs.push(new_tab);
        $('tab_body').appendChild(new_tab.div);
        return new_tab;
    },
    tab_for_name: function(tab_name) {
      for(var i = 0; i < this.tabs.length; i++){
          if(tab_name == this.tabs[i].name) {
              return this.tabs[i];
          }
      }
      return this.add_tab(tab_name);
    },
    div_for_tab: function(tab_name) {
        return this.tab_for_name(tab_name).div;
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
        tabs.switch_to("NewsGroups");
        var tag = tabs.div_for_tab('NewsGroups');
       fetch_data('/newsgroups', tag);
    },
    open_tab_with_url_data: function(tab_name, url){
      var tab = tabs.tab_for_name(tab_name);
      tab.update_url_data(url);
      tabs.switch_to(tab_name);
    }
};

var ui = new UserInterface();


// Old Stuff

var last_data_fetch;
var refresh_timer;
var mode;
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

function data_response(data)
{
  data = data.responseText;
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

  http_busy = null;
  refresh_timer = setTimeout('RequeueFetch()', 5000);
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

function ping_url(url) {
    /* runs a get on a url, but doesn't do anything with the response*/
    var pinger = new Ajax.Request( url, { method: 'get'});
}


function view_file_response(data)
{
    data = data.responseText;
    var thediv = $('content');
    var pre = Builder.node('pre');
    pre.appendChild(Builder.node(data));
    var old = thediv.replaceChild(pre, thediv.firstChild);
    old = undef;
    return false;
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

  var req = new Ajax.Request( url, { method: 'get', onComplete: view_file_response });
}



