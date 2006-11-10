var http_busy = 0;
var onload_actions = new Array;

function debug_log(text) 
{
  $("debug_log").appendChild(Builder.node("li", [text]));
}

function img_or_text(fragment)
{
  var matched = /^img: (.*)/.exec(fragment);
  if(matched) {
    return [ Builder.node("img", { src: matched[1] }) ];
  }
  return fragment;
}

var Pager = Class.create();
Pager.prototype = {
  initialize: function(page_size, item_no, num_items, url) {
    this.url = url;
    this.page_no = Math.floor(item_no / page_size);
    this.page_size = page_size;
    this.num_pages = Math.floor(num_items / page_size);
    this.first_visible = this.page_no * page_size;
    p = $('pager');
    p.replaceChild(this.get_pager_ui(), p.firstChild);
  },
  get_pager_ui: function() {
    pp = Builder.node('a', {href: this.get_url(this.page_no - 1)}, ['<-']);
    np = Builder.node('a', {href: this.get_url(this.page_no + 1)}, ['->']);
    br = Builder.node('br');
    first = Builder.node('a', {href: this.get_url(0)}, ['|<']);
    last = Builder.node('a', {href: this.get_url(this.num_pages)}, ['>|']);
    page_text = this.page_no + "/" + this.num_pages;

    return Builder.node("div", [pp, page_text, np, br, first, last]);
  },
  get_url: function(new_page_no) {
    item_no = new_page_no * this.page_size;
    return "javascript: ui.open_screen_with_paged_url_data('" + this.url + item_no + "')";
  }
}

var Screen = Class.create();
Screen.prototype = {
  next_data_fetch: "",
  initialize: function(is_paged) {
    this.div = Builder.node("div");
    this.tbody = Builder.node("tbody");
    this.div.appendChild(Builder.node("table", {style: 'clear: both'}, [this.tbody]));
  },
  table_cell: function(cell_data) {
    var link;
    var sub_data = cell_data.split("|");
    if(sub_data.length == 1) {
        link = Builder.node('a', img_or_text(sub_data[0]));
    } else {
        link = Builder.node('a', img_or_text(sub_data[1]));
        if(sub_data[0] && (sub_data[0] != " ")){
            link.href = "javascript: " + sub_data[0];
        }
    }
    return link;
  },
  update_table_row: function(row, headers) {
    var cells = row.split("||");
    var rowid = cells.shift();
    var children = $A(this.tbody.childNodes);
    var table_row = children.find(function(row){ return row.id == rowid });
    var me = this;
    if(!table_row) {
        table_row = Builder.node("tr", {id: rowid});
        headers.map( function(h, i){
          table_row.appendChild(Builder.node("td", {class: h}, [me.table_cell(cells[i])] ));
        });
        this.tbody.appendChild(table_row);
        return;
    }
    try {
      cells.map(function(cell, cell_index){
          var new_cell = Builder.node("td", {class: headers[cell_index]}, [me.table_cell(cell)]);
          table_row.replaceChild(new_cell, table_row.childNodes[cell_index]);
      });
    } catch(err) {
      debug_log("Error Found!!");
      debug_log(err.description);
      debug_log(row);
    }
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
  }
};

var UserInterface = Class.create();
UserInterface.prototype = {
    initialize: function(){
    },
    open_screen_with_url_data: function(url){
      this.last_url = null;
      var screen = new Screen(0);
      this.change_screen(screen);
      screen.update_url_data(url);
    },
    open_screen_with_paged_url_data: function(url){
      this.last_url = null;
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
    },
    update_meters: function(jobs, krate){
      $('jobs_rate').replaceChild(document.createTextNode(jobs), $('jobs_rate').firstChild);
      $('k_rate').replaceChild(document.createTextNode(krate), $('k_rate').firstChild);
    },
    update_heading: function(new_heading){
      var h = $('heading');
      h.replaceChild(document.createTextNode(new_heading), h.firstChild);
    },
    refresh_data: function(){
      if(this.last_url) {
        var url = this.last_url;
        this.last_url = null;
        this.screen.update_url_data(url);
      }
    },
    clear_debug_log: function(){
      var debug_node = $('debug_log');
      while(debug_node.hasChildNodes()){
        debug_node.removeChild(debug_node.firstChild);
      }
    },
    enable_search_screen: function(search_url, name){
      $('search_label').replaceChild(document.createTextNode(name), $('search_label').firstChild);
      this.search_url = search_url;
    },
    search_url: "/search?ngi=15",
    retsin: function(){
      this.open_screen_with_url_data(this.search_url + ";q=" +  $('find_me').value);
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

function ping_url(url) {
    /* runs a get on a url, but doesn't do anything with the response*/
    var pinger = new Ajax.Request( url, { method: 'get'});
}

function view_file_response(data)
{
    data = data.responseText;

    var screen = new Object();
    screen.div = Builder.node('pre', [data]);
    ui.change_screen(screen);
    ui.last_url = null;

}

function view_file(url)
{
  var req = new Ajax.Request( url, { method: 'get', onComplete: view_file_response });
}

new PeriodicalExecuter(function(){ ui.refresh_data(); }, 5);


