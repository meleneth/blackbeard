var http_busy = 0;

function debug_log(text) 
{
  $("debug_log").appendChild(Builder.node("li", [text]));
}

var Screen = Class.create();
Screen.prototype = {
  next_data_fetch: "",
  initialize: function(name) {
    this.div = Builder.node("div");
    this.tbody = Builder.node("tbody");
    this.name = name;
    this.link = Builder.node("a", [name]);
    this.link.href = "javascript: tabs.switch_to('" + name + "')"
    $('tabs').appendChild(this.link); 

    this.div.appendChild(Builder.node("table", [this.tbody]));
    this.div.hide();
  },
  table_cell: function(cell_data) {
    var a; 
    var sub_data = cell_data.split("|");
    if(sub_data.length == 1) {
        a = Builder.node('a', sub_data[0]);
    } else {
        a = Builder.node('a', sub_data[1]);
        if(sub_data[0] && (sub_data[0] != " ")){
            //a.onclick = new Function(sub_data[0]);
            a.href = "javascript: " + sub_data[0];
        }
    }
    return a;

  },
  update_table_row: function(row, headers) {
    var cells = row.split("||");
    var rowid = cells.shift();
    var children = $A(this.tbody.childNodes);
    var row = children.find(function(row){ return row.id == rowid});
    var me = this;
    if(!row) {
        row = Builder.node("tr", {id: rowid});
        headers.map( function(h, i){
          row.appendChild(Builder.node("td", {class: h}, [me.table_cell(cells[i])] ));
        });
        this.tbody.appendChild(row);
        return;
    }
    var row_cells = row.childNodes;
    cells.map(function(cell, cell_index){
        var new_cell = me.table_cell(cell);
        var old_cell = row_cells[cell_index];
        old_cell.replaceChild(new_cell, old_cell.firstChild);
    });
  },
  update_url_data: function(url) {
    var tab = this;
    var req = new Ajax.Request( url, { 
      method: 'get', 
      onComplete: function(request){
        var data = request.responseText.split("\n");
        var run_me = data.shift();
        eval(run_me);
        var headers = data.shift();
        headers = headers.split("|");
        data.each(function(row) { if(!row) return; tab.update_table_row(row, headers);});
    }});
    var current_tabname = this.name;
    
  }
};

var ScreenManager = Class.create();
ScreenManager.prototype = { 
    initialize: function(){
        this.tabs = [];
        this.current_tab = null;
    },
    finish_switch: function() {
        new Effect.BlindDown(this.current_tab.div)
    },
    add_tab: function(tab_name) {
        var new_tab = new Tab(tab_name);
        this.tabs.push(new_tab);
        $('tab_body').appendChild(new_tab.div);
        return new_tab;
    },
    tab_for_name: function(tab_name) {
      var tab = this.tabs.find( function(tab){ return (tab.name == tab_name); });
      if(tab) return tab;
      return this.add_tab(tab_name);
    },
    div_for_tab: function(tab_name) {
        return this.tab_for_name(tab_name).div;
    },
    switch_to: function(screen_name) {
        if(this.current_tab) {
          var me = this;
          new Effect.BlindUp(this.current_tab.div, { afterFinish: function() { me.finish_switch() } })
          this.current_tab = this.tab_for_name(screen_name);
        } else {
          this.current_tab = this.tab_for_name(screen_name);
          this.finish_switch();
        }
    },
    update_current_tab: function(){
        var tab = tabs.current_tab;
        if(tab){
            if(tab.last_retrieve) {
                tab.update_url_data(tab.last_retrieve);
            }
        }
    }
};


var UserInterface = Class.create();
UserInterface.prototype = {
    initialize: function(){
    },
    open_screen_with_url_data: function(url){
      var screen = new Screen();
      screen.update_url_data(url);
    }

};

var ui = new UserInterface();


// Old Stuff

var last_data_fetch;
var refresh_timer;
var previous_urls = new Array();
previous_urls.push("index.html");
var old_from_where = 'index.html';


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
  var req = new Ajax.Request( url, { method: 'get', onComplete: view_file_response });
}


new PeriodicalExecuter(function(){ tabs.update_current_tab(); }, 5);

