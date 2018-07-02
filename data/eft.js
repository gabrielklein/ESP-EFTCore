$(document).ready(function() {

  // Load the module API
  $("#main").show();
  $("#head").show();
  $("#loading").hide();

  // Setup ajax parameters
  $.ajaxSetup({
    contentType: "application/json",
    dataType: "json"
  });


  function showPan(typ, name, description, fx) {

    $("#main #title").text(description);
    $("#main #desc").html("Sensor Type: " + typ + "<br />Sensor name: " + name);
    $("#main #fx").text("");

    var f = $("#fxmenu");
    f.html("");

    $("#fxcontenttitle").text("");
    $("#fxcontentfx").text("");
    $("#fxcontentaction").text("");
    $("#fxcontentresult").text("");
    $("#fxcontentcall").text("");


    $.each(fx, function(i, item) {
      var c = $("<button />");
      c.text(item.api);
      c.attr("class", "tablinks");
      c.click(function() {
        $("#fxcontenttitle").text(item.desc);
        var fx = $("#fxcontentfx");
        fx.text("");
        var someparam = false;
        $.each(item.params, function(i, item2) {
          someparam = true;
          fx.append(item2.param + ": ");
          var j = $("<input />");
          j.attr("id", "fxp_"+item2.param);
          switch (item2.type) {
            case 'number':
              j.attr("type", "number");
              break;
            case 'boolean':
              j.attr("type", "checkbox");
              //j.attr("checked", "oui");
              break;
            case 'string':
            default:
              j.attr("type", "text");
          }
          fx.append(j);
          fx.append(" (" + item2.type + ")");
          fx.append($("<br />"));
          var i = $("<i />");
          i.text(item2.desc);
          fx.append(i);
          fx.append($("<br />"));
          fx.append($("<br />"));
          //		"param": "{configurationVariable}",
          //		"type": "String",
          //		"desc": "Change a variable - change is effective at reboot"
        });

        $("#fxcontentaction").text("");

        var  callApi = "api/"+item.api;

        if (someparam) {
          var b = $("<button />");
          b.text("Call function");
          b.click(function() {
            var  callApiFx = callApi+"?unixTime="+Date.now();
            $.each(item.params, function(i, item2) {
              var x = $("#fxp_"+item2.param);
              var v = x.val();
              if (item2.type == 'boolean') {
                if ($("#fxp_"+item2.param).is(':checked')) {
                  v = "1";
                } else {
                  v = "0";
                }
              }
              callApiFx = callApiFx+"&"+encodeURIComponent(item2.param)+"="+encodeURIComponent(v);
            });
            $("#fxcontentcall").text("(Get) "+callApiFx);
            $("#fxcontentresult").text("");
            $("#fxcontenterror").text("");
            $.get(callApiFx, "", function(data) {
              var jsonPretty = JSON.stringify(data, null, '\t');
              $("#fxcontentresult").text(jsonPretty);
            }, "json").fail(function(err) {
              $("#fxcontenterror").text(err.statusText);
              //console.log(err);
            });
          });
          $("#fxcontentaction").append(b);
        }

        var c = $("<button />");
        c.text("Call function with no parameters");
				c.click(function() {
          $("#fxcontentresult").text("");
          $("#fxcontenterror").text("");
          $("#fxcontentcall").text("(Get) "+callApi);
          $.get(callApi, "", function(data) {
            var jsonPretty = JSON.stringify(data, null, '\t');
            $("#fxcontentresult").text(jsonPretty);
          }, "json").fail(function(err) {
            $("#fxcontenterror").text(err.statusText);
            //console.log(err);
          });
				});
        $("#fxcontentaction").append(c);
        $("#fxcontentresult").text("");
        $("#fxcontenterror").text("");
        return false;
      });
      f.append(c);
    });
  };

  // Add a pannel
  function addPan(typ, name, description, fx) {
    var a = $("<a />");
    a.click(function() {
      showPan(typ, name, description, fx);
      return false;
    });
    a.attr("href", "#");
    a.append(name);
    $("#head").append("[").append(a).append("] ");
  };

  // Get modules
  $.get("api/modules", "", function(data) {
    $("#head").text("");
    $.each(data.modules, function(i, item) {
      addPan(item.type, item.name, item.description, item.fx);
    });

  }, "json");





});
