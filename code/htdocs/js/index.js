/*!
* File identification:			$Id: index.js 116 2019-03-29 10:29:14Z cypax $
* Revision of last commit:	$Rev: 116 $
* Author of last commit:		$Author: cypax $
* Date of last commit:			$Date: 2019-03-29 11:29:14 +0100 (Fr, 29 Mrz 2019) $
* @file aquarium.js
* @brief Aquarium web application custom JavaScript.
* @author Philipp Bank
*/

function imageShowSetup() {
  imageShowSetup.duration = imageShowSetup.duration || 60000;
  imageShowSetup.imageIndex = imageShowSetup.imageIndex || 0;
  imageShowSetup.interval = imageShowSetup.interval || setInterval(imageShowNext, imageShowSetup.duration);
  imageShowSetup.mode = 'play';
  imageShowNext();
}


function imageShowNext() {
  var i;
  var x = document.getElementsByClassName("imageshow-slide");
  for (i = 0; i < x.length; i++) {
    x[i].style.display = "none";  
  }
  var y = document.getElementsByClassName("imageshow-text");
  for (i = 0; i < y.length; i++) {
    y[i].style.display = "none";  
  }        
  if (x.length > 0)
  {
    console.log("new image " + imageShowSetup.imageIndex);
    x[imageShowSetup.imageIndex].style.display = "block";  
    y[imageShowSetup.imageIndex].style.display = "block";
    imageShowSetup.imageIndex++;
    if (imageShowSetup.imageIndex >= x.length) {imageShowSetup.imageIndex = 0;}
  }
}

function imageShowPrev() {
  var i;
  var x = document.getElementsByClassName("imageshow-slide");
  for (i = 0; i < x.length; i++) {
    x[i].style.display = "none";  
  }
  var y = document.getElementsByClassName("imageshow-text");
  for (i = 0; i < y.length; i++) {
    y[i].style.display = "none";  
  }        
  if (x.length > 0)
  {
    console.log("new image " + imageShowSetup.imageIndex);
    x[imageShowSetup.imageIndex].style.display = "block";  
    y[imageShowSetup.imageIndex].style.display = "block";
    imageShowSetup.imageIndex--;
    if (imageShowSetup.imageIndex < 0) {imageShowSetup.imageIndex =  (x.length-1);}
  }
}


function setImageShowDuration(duration) {
  imageShowSetup.duration = duration;
  clearInterval(imageShowSetup.interval);
  imageShowSetup.interval = setInterval(imageShowNext, imageShowSetup.duration);
}


function stopImageShow() {
  clearInterval(imageShowSetup.interval);
  imageShowSetup.mode = 'stop';
}


function webcamUpdate() {
  d = new Date();
  var els = document.getElementsByClassName("img-webcam");
  Array.prototype.forEach.call(els, function(el) {
    el.src = "webcam.jpg?"+d.getTime(); // append time query string to force the browser to reload the image
  });
  setTimeout(webcamUpdate, 30000); // Change image every 30 seconds
}


function updateRelayState() {
  $.ajax({
    type:'get',
    url:'cgi-bin/aquaRequest.cgi?request=getStatus',
    success:function(data) {
      var array = data.split("|", 10);
      
      if (array.length < 10) {
        $(".img-outlet-state0").attr("src", "images/icons-svg/delete-red.svg");
        $(".img-outlet-state1").attr("src", "images/icons-svg/delete-red.svg");
        $(".img-outlet-state2").attr("src", "images/icons-svg/delete-red.svg");
        $(".img-outlet-state3").attr("src", "images/icons-svg/delete-red.svg");
        $(".img-outlet-state4").attr("src", "images/icons-svg/delete-red.svg");
        $(".img-outlet-state5").attr("src", "images/icons-svg/delete-red.svg");
        $(".img-outlet-state6").attr("src", "images/icons-svg/delete-red.svg");
        $(".img-outlet-state7").attr("src", "images/icons-svg/delete-red.svg");
        $('.aquarium-value').html("???");
        $('.aquarium-status-datetime').html("???");
      }
      else
      {
        var datetimeString = array[0];
        $('.aquarium-status-datetime').html(datetimeString);
        
        var temperatureString = array[1];
        $('.aquarium-value').html(temperatureString);
        
        for (i = 0; i < 8; i++) {
          var dayStr = array[i+2];
          var dayData = dayStr.split(",", 3);
          
          if ('ON' == dayData[0]) {
            $(".img-outlet-state"+i).attr("src", "images/power-on.svg");
          } else if ('OFF' == dayData[0]) {
            $(".img-outlet-state"+i).attr("src", "images/icons-svg/power-black.svg");
          } else {
            $(".img-outlet-state"+i).attr("src", "images/icons-svg/delete-red.svg");
          }
          
          if ('KEEP_ON' == dayData[1]) {
            $("#p-outlet-mode-on"+i).addClass("ui-btn-active");
            $("#p-outlet-mode-off"+i).removeClass("ui-btn-active");
            $("#p-outlet-mode-schedule"+i).removeClass("ui-btn-active");
          } else if ('KEEP_OFF' == dayData[1]) {
            $("#p-outlet-mode-on"+i).removeClass("ui-btn-active");
            $("#p-outlet-mode-off"+i).addClass("ui-btn-active");
            $("#p-outlet-mode-schedule"+i).removeClass("ui-btn-active");
          } else if ('SCHEDULE' == dayData[1]) {
            $("#p-outlet-mode-on"+i).removeClass("ui-btn-active");
            $("#p-outlet-mode-off"+i).removeClass("ui-btn-active");
            $("#p-outlet-mode-schedule"+i).addClass("ui-btn-active");
          } else {
            $("#p-outlet-mode-on"+i).removeClass("ui-btn-active");
            $("#p-outlet-mode-off"+i).removeClass("ui-btn-active");
            $("#p-outlet-mode-schedule"+i).removeClass("ui-btn-active");
          }
          
          if ("0" != dayData[2]) {
            var date = new Date(null);
            date.setSeconds(dayData[2]); // specify value for SECONDS here
            var result = date.toISOString().substr(11, 8);
            var els = document.getElementsByClassName("s-overwrite-delay"+i);
            Array.prototype.forEach.call(els, function(el) {
                el.innerHTML = result;
            });
          } else {
            var els =document.getElementsByClassName("s-overwrite-delay"+i);
            Array.prototype.forEach.call(els, function(el) {
                el.innerHTML = "";
            });
          }
        }
      }
    }
  });
}

// Pagecreate will fire for each of the pages in this demo
// but we only need to bind once so we use "one()"
$( document ).one( "pagecreate", ".swipe-page", function() {

  console.log("swipe-page created");

  // Handler for navigating to the next page
  function navnext( next ) {
    var animation = "slide";
    $.mobile.changePage(next, {transition: animation, allowSamePageTransition: false, pageReload: false });
  }
  // Handler for navigating to the previous page
  function navprev( prev ) {
    var animation = "slide";
    $.mobile.changePage(prev, {transition: animation, reverse: true, allowSamePageTransition: false, pageReload: false });
  }

  // slide home > info
  $(document).on("swipeleft", "#page_home", function(event) {
    var targetPage = $("#page_info");
    if (targetPage) {
      navnext(targetPage);
    }
  });

  // slide home < info
  $(document).on("swiperight", "#page_info", function(event) {
    var targetPage = $("#page_home");
    if (targetPage) {
      navprev(targetPage);
    }
  });

  // slide info > relays
  $(document).on("swipeleft", "#page_info", function(event) {
    var targetPage = $("#page_relays");
    if (targetPage) {
      navnext(targetPage);
    }
  });
  
  // slide info < relays
  $(document).on("swiperight", "#page_relays", function(event) {
    var targetPage = $("#page_info");
    if (targetPage) {
      navprev(targetPage);
    }
  });

  // slide relays > camera
  $(document).on("swipeleft", "#page_relays", function(event) {
    var targetPage = $("#page_camera");
    if (targetPage) {
      navnext(targetPage);
    }
  });

  // slide relays < camera
  $(document).on("swiperight", "#page_camera", function(event) {
    var targetPage = $("#page_relays");
    if (targetPage) {
      navprev(targetPage);
    }
  });

});  



$(document).one("pageshow","body",function(){
  console.log("body shown");

  $("#btn-prev" ).on( "click", function() {
    console.log("prev clicked");
    stopImageShow();
    imageShowPrev();
    if ('play' === imageShowSetup.mode) {
      $("#btn-play").addClass("ui-btn-active");
      $("#btn-stop").removeClass("ui-btn-active");
    } else {
      $("#btn-play").removeClass("ui-btn-active");
      $("#btn-stop").addClass("ui-btn-active");
    }
  });  

  $("#btn-stop" ).on( "click", function() {
    console.log("stop clicked");
    stopImageShow();
    if ('play' === imageShowSetup.mode) {
      $("#btn-play").addClass("ui-btn-active");
      $("#btn-stop").removeClass("ui-btn-active");
    } else {
      $("#btn-play").removeClass("ui-btn-active");
      $("#btn-stop").addClass("ui-btn-active");
    }
  });  

  $("#btn-play" ).on( "click", function() {
    console.log("play clicked");
    imageShowSetup();
    if ('play' === imageShowSetup.mode) {
      $("#btn-play").addClass("ui-btn-active");
      $("#btn-stop").removeClass("ui-btn-active");
    } else {
      $("#btn-play").removeClass("ui-btn-active");
      $("#btn-stop").addClass("ui-btn-active");
    }
  });  

  $("#btn-next" ).on( "click", function() {
    console.log("next clicked");
    stopImageShow();
    imageShowNext();
    if ('play' === imageShowSetup.mode) {
      $("#btn-play").addClass("ui-btn-active");
      $("#btn-stop").removeClass("ui-btn-active");
    } else {
      $("#btn-play").removeClass("ui-btn-active");
      $("#btn-stop").addClass("ui-btn-active");
    }
  });  

  $(".p-outlet-mode-on" ).on( "click", function() {
    var contentPanelId = jQuery(this).attr("id");
    contentPanelId = contentPanelId.replace("p-outlet-mode-on", "");
    $.ajax({ type:'get',  url:'cgi-bin/aquaRequest.cgi?request=setRelayMode'+contentPanelId+'|KEEP_ON', success:function(data) { console.log("success: "+data); }  });    
  });  

  $(".p-outlet-mode-off" ).on( "click", function() {
    var contentPanelId = jQuery(this).attr("id");
    contentPanelId = contentPanelId.replace("p-outlet-mode-off", "");
    $.ajax({ type:'get',  url:'cgi-bin/aquaRequest.cgi?request=setRelayMode'+contentPanelId+'|KEEP_OFF', success:function(data) { console.log("success: "+data); }  });    
  });  

  $(".p-outlet-mode-schedule" ).on( "click", function() {
    var contentPanelId = jQuery(this).attr("id");
    contentPanelId = contentPanelId.replace("p-outlet-mode-schedule", "");
    $.ajax({ type:'get',  url:'cgi-bin/aquaRequest.cgi?request=setRelayMode'+contentPanelId+'|SCHEDULE', success:function(data) { console.log("success: "+data); }  });    
  });     
 
  // switch pumps off for 30 min, everything else back to normal mode
  $("#btn-feeding" ).on( "click", function() {
    console.log("clicked");
    $.ajax({ type:'get',  url:'cgi-bin/aquaRequest.cgi?request=setRelayOverwrite0|0,OFF', success:function(data) { console.log("success"); }  });      
    $.ajax({ type:'get',  url:'cgi-bin/aquaRequest.cgi?request=setRelayOverwrite1|1800,OFF', success:function(data) { console.log("success"); }  });      
    $.ajax({ type:'get',  url:'cgi-bin/aquaRequest.cgi?request=setRelayOverwrite2|1800,OFF', success:function(data) { console.log("success"); }  });      
    $.ajax({ type:'get',  url:'cgi-bin/aquaRequest.cgi?request=setRelayOverwrite3|0,OFF', success:function(data) { console.log("success"); }  });      
    $.ajax({ type:'get',  url:'cgi-bin/aquaRequest.cgi?request=setRelayOverwrite5|0,OFF', success:function(data) { console.log("success"); }  });      
    $.ajax({ type:'get',  url:'cgi-bin/aquaRequest.cgi?request=setRelayOverwrite6|0,OFF', success:function(data) { console.log("success"); }  });      
    $.ajax({ type:'get',  url:'cgi-bin/aquaRequest.cgi?request=setRelayOverwrite7|0,OFF', success:function(data) { console.log("success"); }  });      
  });  

  // switch everything except light off for 3 hours
  $("#btn-maintainance" ).on( "click", function() {
    $.ajax({ type:'get',  url:'cgi-bin/aquaRequest.cgi?request=setRelayOverwrite0|10800,OFF', success:function(data) { console.log("success"); }  });      
    $.ajax({ type:'get',  url:'cgi-bin/aquaRequest.cgi?request=setRelayOverwrite1|10800,OFF', success:function(data) { console.log("success"); }  });      
    $.ajax({ type:'get',  url:'cgi-bin/aquaRequest.cgi?request=setRelayOverwrite2|10800,OFF', success:function(data) { console.log("success"); }  });      
    $.ajax({ type:'get',  url:'cgi-bin/aquaRequest.cgi?request=setRelayOverwrite3|10800,OFF', success:function(data) { console.log("success"); }  });      
    $.ajax({ type:'get',  url:'cgi-bin/aquaRequest.cgi?request=setRelayOverwrite4|0,OFF',     success:function(data) { console.log("success"); }  });      
    $.ajax({ type:'get',  url:'cgi-bin/aquaRequest.cgi?request=setRelayOverwrite5|10800,OFF', success:function(data) { console.log("success"); }  });      
    $.ajax({ type:'get',  url:'cgi-bin/aquaRequest.cgi?request=setRelayOverwrite6|10800,OFF', success:function(data) { console.log("success"); }  });      
    $.ajax({ type:'get',  url:'cgi-bin/aquaRequest.cgi?request=setRelayOverwrite7|10800,OFF', success:function(data) { console.log("success"); }  });      
  });  

  // switch everything back to normal operation mode
  $("#btn-endoverwrite" ).on( "click", function() {
    $.ajax({ type:'get',  url:'cgi-bin/aquaRequest.cgi?request=setRelayOverwrite0|0,OFF', success:function(data) { console.log("success"); }  });      
    $.ajax({ type:'get',  url:'cgi-bin/aquaRequest.cgi?request=setRelayOverwrite1|0,OFF', success:function(data) { console.log("success"); }  });      
    $.ajax({ type:'get',  url:'cgi-bin/aquaRequest.cgi?request=setRelayOverwrite2|0,OFF', success:function(data) { console.log("success"); }  });      
    $.ajax({ type:'get',  url:'cgi-bin/aquaRequest.cgi?request=setRelayOverwrite3|0,OFF', success:function(data) { console.log("success"); }  });      
    $.ajax({ type:'get',  url:'cgi-bin/aquaRequest.cgi?request=setRelayOverwrite4|0,OFF', success:function(data) { console.log("success"); }  });      
    $.ajax({ type:'get',  url:'cgi-bin/aquaRequest.cgi?request=setRelayOverwrite5|0,OFF', success:function(data) { console.log("success"); }  });      
    $.ajax({ type:'get',  url:'cgi-bin/aquaRequest.cgi?request=setRelayOverwrite6|0,OFF', success:function(data) { console.log("success"); }  });      
    $.ajax({ type:'get',  url:'cgi-bin/aquaRequest.cgi?request=setRelayOverwrite7|0,OFF', success:function(data) { console.log("success"); }  });      
  });     
     
  $('.languageSelector').on("click",function(){
    setLanguage($(this).text());
  });

  Number.decPoint = '.';
  Number.thousand_sep = ',';

  Number.prototype.format = function(k, fixLength) {
    if(!k) k = 0;
    var neu = '';
    var sign = this < 0 ? '-' : '';
    // Runden
    var f = Math.pow(10, k);
    var zahl = Math.abs(this);
    zahl = '' + parseInt(zahl * f + .5 ) / f ;
    // Komma ermittlen
    var idx = zahl.indexOf('.');
    // fehlende Nullen einfügen
    if(fixLength && k) {
         zahl += (idx == -1 ? '.' : '' )
         + f.toString().substring(1);
    }
    // Nachkommastellen ermittlen
    idx = zahl.indexOf('.');
    if(idx == -1) idx = zahl.length;
    else neu = Number.decPoint + zahl.substr(idx + 1, k);
    // Tausendertrennzeichen
    while(idx > 0)
    {
        if(idx - 3 > 0)
        neu = Number.thousand_sep + zahl.substring( idx - 3, idx) + neu;
        else
        neu = zahl.substring(0, idx) + neu;
        idx -= 3;
    }
    return sign + neu;
  };
});


$(document).ready(function(){
  console.log("document ready");
  setLanguage($.cookie("language") == null ? "English" : $.cookie("language"));
  imageShowSetup();
  webcamUpdate();
  setInterval(updateRelayState, 1000);    
});