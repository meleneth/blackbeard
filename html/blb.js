var http_busy = 0;
var onload_actions = new Array;

function debug_log(text) 
{
  $("debug_log").appendChild(Builder.node("li", [text]));
}

var Screen = Class.create();
Screen.prototype = {
  next_data_fetch: "",
  initialize: function(is_paged) {
    this.div = Builder.node("div");
    this.tbody = Builder.node("tbody");
    if(is_paged) { this.enable_paging(); }
    this.div.appendChild(Builder.node("table", [this.tbody]));
  },
  table_cell: function(cell_data) {
    var link;
    var sub_data = cell_data.split("|");
    if(sub_data.length == 1) {
        link = Builder.node('a', sub_data[0]);
    } else {
        link = Builder.node('a', sub_data[1]);
        if(sub_data[0] && (sub_data[0] != " ")){
            link.href = "javascript: " + sub_data[0];
        }
    }
    return Builder.node('td', [ link ]);
  },
  update_table_row: function(row, headers) {
    var cells = row.split("||");
    var rowid = cells.shift();
    var children = $A(this.tbody.childNodes);
    var row = children.find(function(row){ return row.id == rowid });
    var me = this;
    if(!row) {
        row = Builder.node("tr", {id: rowid});
        headers.map( function(h, i){
          row.appendChild(Builder.node("td", {class: h}, [me.table_cell(cells[i])] ));
        });
        this.tbody.appendChild(row);
        return;
    }
    cells.map(function(cell, cell_index){
        var new_cell = me.table_cell(cell);
        row.replaceChild(new_cell, row.children[cell_index]);
    });
  },
  update_url_data: function(url) {
    var screen = this;
    var req = new Ajax.Request( url, { 
      method: 'get', 
      onComplete: function(request){
        var data = request.responseText.split("\n");
        var run_me = data.shift();
        Try.these( function(){ eval(run_me);});
        var headers = data.shift();
        headers = headers.split("|");
        data.each(function(row) { if(!row) return; screen.update_table_row(row, headers);});
    }});
  },
  enable_paging: function() {
    var prev_link = Builder.node('a', ['<-']);
    var next_link = Builder.node('a', ['->']);

    prev_link.href="javascript: ul.screen.prev_page();";
    next_link.href="javascript: ul.screen.next_page();";

    var span = Builder.node('span', {style: "float: right;"});
    span.appendChild(next_link);
    var link_div = Builder.node('div', [ span, prev_link ]);

    this.div.appendChild(link_div);
    this.div.appendChild(Builder.node('br', {style: "clear: both;"}));
  }
};


var UserInterface = Class.create();
UserInterface.prototype = {
    initialize: function(){
    },
    open_screen_with_url_data: function(url){
      var screen = new Screen(0);
      this.change_screen(screen);
      screen.update_url_data(url);
    },
    open_screen_with_paged_url_data: function(url){
      var screen = new Screen(1);
      this.change_screen(screen);
      screen.update_url_data(url);
    },
    change_screen: function(new_screen){
      var old_screen = this.screen;
      this.screen = new_screen;
      if(old_screen) {
        $('screen').replaceChild(new_screen.div, old_screen.div);
      }else {
        $('screen').appendChild(new_screen.div);
      }
    }
};

var ui = new UserInterface();


// Old Stuff

var last_data_fetch;
var refresh_timer;
var previous_urls = new Array();
previous_urls.push("index.html");
var old_from_where = 'index.html';


function process_onload()
{
  onload_actions.map(function(f, i){ f() });
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

/* new PeriodicalExecuter(function(){ tabs.update_current_tab(); }, 5); */

