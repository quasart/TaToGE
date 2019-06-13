
  /* Widgets */

  function new_dice(obj)
  {
    var inital_face = ""
    var attr_list = "";
    if (obj.List)
    {
      obj.NbSides = obj.List.length
      inital_face = obj.List[obj.List.length-1]
      attr_list = "data-list='"+JSON.stringify(obj.List)+"' "
    }
    else
    {
      inital_face += obj.NbSides;
    }

    if (inital_face.length > 16)
    {
      attr_list += "style='font-size: 0.5em;' "
    }
    else if (inital_face.length > 8)
    {
      attr_list += "style='font-size: 0.7em;' "
    }

    return "<button type=button class='dice btn btn-table widget-content' " +
      "id='" + obj.id + "' " +
      "onclick='roll_dice(this)' " +
      "data-nb-sides='" + obj.NbSides + "' " +
      attr_list +
      ">" + inital_face + "</button>";
  }

  function roll_dice(dice_id) {
    var dice = $(dice_id);
    dice.data("rollTime", "15");
    dice.addClass("rolling");

    if (!dice.data("interval-id")) // Check is not already rolling.
    {
      // Start rolling...
      var intervalId = setInterval( function() {
        var nb_faces = dice.data("nb-sides");
        var randvalue = Math.floor(Math.random()*nb_faces);
        var face_names = dice.data("list");
        if (face_names)
        {
          dice.html(face_names[randvalue]);
        }
        else
        {
          dice.html(randvalue+1);
        }

        var roll_time = dice.data("rollTime");
        roll_time--;
        dice.data("rollTime", roll_time);

        // Stop condition
        if (roll_time == 0) {
          clearInterval(dice.data("interval-id"));
          dice.data("interval-id", null);
          dice.removeClass("rolling");
        }
      }, 100);
      dice.data("interval-id", intervalId);
    }
  }


  function new_timer(obj)
  {
    return "<div class='timer progress widget-content' " +
      "id='" + obj.id + "' " +
      "onclick='start_timer(this)' " +
      "data-duration='" + obj.Duration + "'>" +
      "<div class='progress-bar' role='progressbar' ></div>" +
      "</div>";
  }

  function start_timer(timer_id)
  {
    var timer = $(timer_id);
    var chunk = timer.children("div")
    if (timer.data("interval-id")) {
      chunk.width(0)
      clearInterval(timer.data("interval-id"));
      timer.data("interval-id", null);
    }
    else
    {
      chunk.width("100%")
      timer.data("beginTime", Date.now())

      var duration_ms = timer.data("duration")*1000;
      var refresh_time_ms = duration_ms / 500;
      if (refresh_time_ms < 50) refresh_time_ms = 50;

      var intervalId = setInterval( function() {
        var millis = Date.now() - timer.data("beginTime");
        var value = (duration_ms - millis) / duration_ms;
        chunk.width(Math.floor(value*100) + "%");

        if (value <= 0)
        {
          clearInterval(timer.data("interval-id"));
          timer.data("interval-id", null);
        }
      }, refresh_time_ms );
      timer.data("interval-id", intervalId);
    }
  }


  function new_counter(obj)
  {
    var html = "<form class='counter widget-content' id='" + obj.id + "'>"
    html += "<button type=button class='btn btn-table' onclick='increment_counter(\"#" +obj.id+" span\",-10)'>-10</button>" 
    html += "<button type=button class='btn btn-table' onclick='increment_counter(\"#" +obj.id+" span\",-5)' >-5</button>"  
    html += "<button type=button class='btn btn-table' onclick='increment_counter(\"#" +obj.id+" span\",-1)' >-1</button>"  
    html += "<span>0</span>"
    html += "<button type=button class='btn btn-table' onclick='increment_counter(\"#" +obj.id+" span\",1)'  >+1</button>"  
    html += "<button type=button class='btn btn-table' onclick='increment_counter(\"#" +obj.id+" span\",5)'  >+5</button>"  
    html += "<button type=button class='btn btn-table' onclick='increment_counter(\"#" +obj.id+" span\",10)' >+10</button>" 
    html += "</form>";
    return html;
  }

  function increment_counter(counter,incr)
  {
      label = $(counter);
      val = parseInt(label.html());
      label.html(val+incr);
  }



/* Table management */

  var EQUIPMENT_ID = 0;

  function create_widget(json_obj)
  {
    if (Array.isArray(json_obj))
    {
       html = ""
       json_obj.forEach(function(element) {
          html += create_widget(element);
       });
       return html;
    }

    json_obj.id = "equipment"+EQUIPMENT_ID;
    EQUIPMENT_ID++;

    html = '<div class="widget">';

    var label_content = ""
    //if (json_obj.Icon) label_content += "<img src='images/"+ json_obj.Icon +".png' />";
    if (json_obj.Name) label_content += json_obj.Name;
    html += '<div class="label label-over">' + label_content + '</div>';

    switch (json_obj.Type)
    {
      case "Dice":      html += new_dice(json_obj); break;
      case "Timer":     html += new_timer(json_obj); break;
      case "Counter":   html += new_counter(json_obj); break;
      case "Label":     html += "<div class='widget-content'>"+json_obj.Text+"</div>"; break;
      case "Space":     html += "<div class='widget-content'></div>"; break;
      default:
        console.log("Invalid widget type: "+json_obj.Type)
    }
    html += '<button class="control btn btn-info" onclick="move_up_widget(this)" aria-label="Delete">&uarr;</button>';
    html += '<button class="control btn btn-danger" onclick="delete_widget(this)" aria-label="Delete">&times;</button>';

    html += '</div>'
    return html;

  }

  function delete_widget(node)
  {
    $(node).parents(".widget").remove();
    apply_responsive();
  }

  // TODO should use https://github.com/SortableJS/Sortable instead
  function move_up_widget(node)
  {
    var w = $(node).parents(".widget");
    var previous = w.prev();
    if (previous) w.insertBefore(previous);
  }

  function add_equipment(input)
  {
    var json_obj = JSON.parse( $(input).val().replace(/'/g, '&apos;') )
    var html = create_widget(json_obj);
    $("#ttgeTable").append(html)
    apply_responsive();
    apply_admin_mode();
  }

  function clear_table()
  {
    $("#ttgeTable").html("")
  }

  function touch_table()
  {
      var timer = setTimeout(function(){
          var checkbox = $("input[name=admin_mode]");
          checkbox.prop("checked", !checkbox.prop("checked"));
          checkbox.trigger("change");
        },900);
      $(this).data("timer", timer);
  }

  function untouch_table()
  {
    clearTimeout( $(this).data("timer") );
  }

  function apply_admin_mode() {
    var is_admin_mode = $("input[name=admin_mode]").is(':checked')
    if (is_admin_mode)
    {
      $(".control").removeClass("control-hidden")
    }
    else
    {
      $(".control").addClass("control-hidden")
    }
  }

  function add_radio(name, value)
  {
    var html = "<label class='form-check-label'><input type='radio' name='type' " +
      "value='" + JSON.stringify(value) + "'> " +
      name + "</label><br>"
    $("#AddEquipment").append(html)
  }


  function apply_responsive() {
    var widgets = $('#ttgeTable > *');
    if (!widgets.length) return;

    var winH = $(window).height();
    var widgetH = winH / widgets.length * (0.9 - 0.2/widgets.length)
    var marginH = 2+0.05*widgetH
    widgets.css('height', (widgetH-marginH) + 'px');
    widgets.css('margin-bottom', marginH + 'px');
    widgets.css('width', (12*widgetH) + 'px');

    var fontsize = Math.min( 0.5*widgetH, 0.2*widgets.width() );
    widgets.css('font-size', fontsize + 'px');
  }

  $(function() {

    $(window).resize(apply_responsive);
    $("#ttgeTable").on("mousedown touchstart",touch_table);
    $("#ttgeTable").on("mouseup mouseleave touchend touchmove touchcancel",untouch_table);

    apply_admin_mode();

    add_radio("Counter", {Type:"Counter"});
    add_radio("6-sided dice", {Type:"Dice", NbSides:6, Name:"d6"});
    add_radio("8-sided dice", {Type:"Dice", NbSides:8, Name:"d8"});
    add_radio("20-sided dice", {Type:"Dice", NbSides:20, Name:"d20"});
    add_radio("Fudge dice", {Type:"Dice", List:["-"," ","+"], Name:"dF"});
    add_radio("Poker dice", {Type:"Dice", List:["9","10","Jack","Queen","King","Ace"], Name:"dP"});
    add_radio("30-second sandtimer", {Type:"Timer", Duration:30, Name:"30s"});
    add_radio("1-minute sandtimer", {Type:"Timer", Duration:60, Name:"1min"});

    var magic_list = [ "It is certain",
                       "It is decidedly so",
                       "Without a doubt",
                       "Yes - definitely",
                       "You may rely on it",
                       "As I see it, yes",
                       "Most likely",
                       "Outlook good",
                       "Yes",
                       "Signs point to yes",
                       "Don’t count on it",
                       "My reply is no",
                       "My sources say no",
                       "Outlook not so good",
                       "Very doubtful",
                       "Reply hazy, try again",
                       "Ask again later",
                       "Better not tell you now",
                       "Cannot predict now",
                       "Concentrate and ask again"];
    add_radio("Magic 8 Ball", {Type:"Dice", List:magic_list, Name:"8Ball"});
    //add_radio("Space", {type:"Space"});
  });



