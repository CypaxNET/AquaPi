<!DOCTYPE html>
<!--
File identification:      $Id: index.php 129 2019-04-01 18:20:49Z cypax $
Revision of last commit:  $Rev: 129 $
Author of last commit:    $Author: cypax $
Date of last commit:      $Date: 2019-04-01 20:20:49 +0200 (Mo, 01 Apr 2019) $
@file index.html
@brief Aquarium web application
@author Philipp Bank
-->
<html>
  
<head>
  <title>Aquarium</title>
  <meta charset="ISO-8859-1">
  <meta name="viewport" content="width=device-width, initial-scale=1">

  <link rel="shortcut icon" href="./ico/favicon.ico">

  <!-- jQuery CSS -->
  <link rel="stylesheet" href="jquery/jquery.mobile-1.4.5.css">

  <!-- Custom CSS -->
  <link rel="stylesheet" type="text/css" href="css/aquarium.css">
  <link rel="stylesheet" href="css/w3.css">

  <!-- jQuery includes -->
  <script src="jquery/jquery.js"></script>
  <script src="jquery/jquery.mobile-1.4.5.js"></script>

  <!-- jQuery plugins -->
  <script src="jquery/jquery-bigtext.js"></script>
  <script src="jquery/jquery.cookie.js"></script>

  <!-- Custom JS -->
  <script src="js/aquarium.js"></script>
  <script src="js/index.js"></script>
  
  <script type="text/javascript" language="javascript">
    $(document).ready(function(){
      console.log("custom code");
      setImageShowDuration(60000);
    });
  </script>

</head>

<body>

<!--
Problem mit chromium
<audio id="audio-0001" src="audio/drop.wav" preload="auto"></audio>
-->

<div data-role="page" class="aquarium-menupage swipe-page ui-responsive-panel" id="page_home" data-title="AquariumControl" data-theme="b">
  
  <div data-role="header" data-position="fixed">
    <h1 class="text-0505">Status</h1>
    <a href="#nav-panel" data-icon="bars" data-iconpos="notext">Menu</a>
    <a href="#add-language" data-icon="gear" data-iconpos="notext">Add</a>
  </div>
  
  <div data-role="footer"    data-position="fixed" data-theme="b">
    <p id="btn-feeding"      class="text-0014 click-button ui-btn ui-corner-all ui-shadow ui-icon-action ui-btn-inline ui-btn-icon-left ui-btn-b" data-rel="back">???</p>
    <p id="btn-maintainance" class="text-0015 click-button ui-btn ui-corner-all ui-shadow ui-icon-alert ui-btn-inline ui-btn-icon-left ui-btn-b" data-rel="back">???</p>
    <p id="btn-endoverwrite" class="text-0016 click-button ui-btn ui-corner-all ui-shadow ui-icon-check ui-btn-inline ui-btn-icon-left ui-btn-b" data-rel="back">???</p>
    <a id="btn_helpstatus" href="#page_statushelp" class="text-0504 click-button ui-btn ui-corner-all ui-shadow ui-icon-info ui-btn-inline ui-btn-icon-notext ui-btn-b" data-icon="info" data-rel="dialog" data-transition="pop">Help</a>
  </div>

  <div role="main" class="flex-container ui-content">
    <div class="flex-item" style="padding:0;min-width:45%">
      <table class="table-outlet-state" cellpadding="0" cellspacing="0" >
        <tr>
          <th align="left">Status</th>
          <th align="left">Name</th>
        </tr>
          <?php
          for ($i=0; $i<8; $i++)
          {
            echo "<tr>\r\n";
            echo "<td><img id=\"img-relay-state".$i."\" src=\"\" class=\"img-outlet-state img-outlet-state".$i."\"/><span class=\"s-overwrite-delay".$i."\" style=\"margin-left:0.5em;\"></span></td>\r\n";
            echo "<td><p class=\"text-004".$i."\"></p></td>\r\n";
            echo "</tr>\r\n";
          }
          ?>
      </table>
    </div>
    <div class="flex-item" style="background:#111;display:flex;flex-direction:column;flex-grow:1;flex-basis:40%">
      <div style="color:#22aadd;">
        <p class="" style="text-align:center">
          <span id="aquarium-value" class="aquarium-value" style="margin-left:0.2em;font-size:10vw;">??????</span><span class="aquarium-unit" style="margin-right:0.2em;margin-left:0.2em;font-size:10vw;">&deg;C</span>
        </p>
      </div>
      <div style="min-height:30%;top:0%;background:#111">
        <p class="" style="text-align:center">
          <span class="aquarium-status-datetime" style="font-size:5vw;">YYYY-MM-DD hh:mm:ss</span>
        </p>
      </div>
    </div>
  </div>

  <div data-role="panel" data-display="overlay" data-position="right" data-theme="a" id="add-language">
    <h4><span class="text-0501">Language</span></h4>
      <ul data-role="listview" data-inset="true">
        <li><a data-rel="close" class="languageSelector ui-btn" href="#"><img src="./ico/de.png" alt="" class="ui-li-icon"/>Deutsch</a></li>
        <li><a data-rel="close" class="languageSelector ui-btn" href="#"><img src="./ico/en.png" alt="" class="ui-li-icon"/>English</a></li>
        <li><a data-rel="close" class="languageSelector ui-btn" href="#"><img src="./ico/thl.png" alt="" class="ui-li-icon"/>thlIngan Hol</a></li>
      </ul>
  </div>

  <div data-role="panel" data-display="overlay" data-theme="a" id="nav-panel">
    <ul data-role="listview">
      <li data-icon="delete"><a href="#" data-rel="close">Close menu</a></li>
      <li data-icon="home"><a href="#page_home"       class="text-0505 click-button">Home</a></li>
      <li data-icon="comment"><a href="#page_info"     class="text-0502 click-button">Info</a></li>
      <li data-icon="calendar"><a href="#page_relays" class="text-0503 click-button">Relays</a></li>
      <li data-icon="camera"><a href="#page_camera"       class="text-0506 click-button">Camera</a></li>
    </ul>
  </div>
 
</div>


<!-- statushelp page -->
<!-- ############################################################################################################################# -->
<div data-role="page" class="aquarium-menupage" id="page_statushelp">

	<div data-role="header" data-theme="b">
		<h1 class="text-0504">HELP</h1>
	</div><!-- /header -->

	<div role="main" class="ui-content" style="width:100%;background:#2a2a2a;color:#fff;position:absolute;" >
    <table>
      <tr><td><img src="images/power-on.svg" /></td>              <td class="text-0301">DEVICE IS ON</td></tr>
      <tr><td><img src="images/icons-svg/power-black.svg" /></td> <td class="text-0302">DEVICE IS OFF</td></tr>
      <tr><td><img src="images/overwrite.png" /></td>             <td class="text-0303">DEVICE IS IN OVERWRITE MODE</td></tr>      
      <tr><td><img src="images/icons-svg/delete-red.svg" /></td>  <td class="text-0307">DEVICE ERROR</td></tr>
    </table>
	</div><!-- /content -->
</div>

<div data-role="page" class="aquarium-menupage swipe-page ui-responsive-panel" id="page_info" data-title="AquariumControl" data-theme="b">
  
  <div data-role="header" data-position="fixed">
    <h1 class="text-0502">Images</h1>
    <a href="#nav-panel2" data-icon="bars" data-iconpos="notext">Menu</a>
    <a href="#add-language2" data-icon="gear" data-iconpos="notext">Add</a>   
  </div>
  
  <div role="main" class="flex-container ui-content">
      
      
      <div class="flex-item" style="margin:0;background:#000;flex-basis:55%;position:relative;">
<?php
        $myfile = fopen("imageshow.txt", "r");
        $lines = file("imageshow.txt", FILE_IGNORE_NEW_LINES);
        echo '<!-- read '.count($lines).' lines from imageshow.txt -->';
        foreach ($lines as $line)
        {
          $items= explode("|", $line, 2);
          if (2 == count($items))
          {
            echo '<img class="imageshow-slide" src="'.$items[0].'">';
          }
        }
?>
      </div>
      
      <div class="flex-item" style="background:#000;margin:0;flex-basis:45%;">
<?php
        $myfile = fopen("imageshow.txt", "r");
        $lines = file("imageshow.txt", FILE_IGNORE_NEW_LINES);
        
        foreach ($lines as $line)
        {
          $items= explode("|", $line, 2);
          if (2 == count($items))
          {
            echo '<div class="imageshow-text">'.$items[1].'</div>';
          }
        }
?>
      </div>
      
  </div>

  <div data-role="footer"  data-position="fixed" data-theme="b">
    <p id="btn-prev"      class="click-button ui-btn ui-corner-all ui-shadow ui-icon-carat-l ui-btn-inline ui-btn-icon-notext ui-btn-b">PREV</p>
    <p id="btn-stop"      class="click-button ui-btn ui-corner-all ui-shadow ui-icon-stop ui-btn-inline ui-btn-icon-notext ui-btn-b">STOP</p>
    <p id="btn-play"      class="click-button ui-btn ui-corner-all ui-shadow ui-icon-play ui-btn-inline ui-btn-icon-notext ui-btn-b ui-btn-active">PLAY</p>
    <p id="btn-next"      class="click-button ui-btn ui-corner-all ui-shadow ui-icon-carat-r ui-btn-inline ui-btn-icon-notext ui-btn-b">NEXT</p>
  </div>

  <div data-role="panel" data-display="overlay" data-position="right" data-theme="a" id="add-language2">
    <h4><span class="text-0501">Language</span></h4>
      <ul data-role="listview" data-inset="true">
        <li><a data-rel="close" class="languageSelector ui-btn" href="#"><img src="./ico/de.png" alt="" class="ui-li-icon"/>Deutsch</a></li>
        <li><a data-rel="close" class="languageSelector ui-btn" href="#"><img src="./ico/en.png" alt="" class="ui-li-icon"/>English</a></li>
        <li><a data-rel="close" class="languageSelector ui-btn" href="#"><img src="./ico/thl.png" alt="" class="ui-li-icon"/>thlIngan Hol</a></li>
      </ul>
  </div>
 
  <div data-role="panel" data-display="overlay" data-theme="a" id="nav-panel2">
    <ul data-role="listview">
      <li data-icon="delete"><a href="#" data-rel="close">Close menu</a></li>
      <li data-icon="home"><a href="#page_home"       class="text-0505 click-button">Home</a></li>
      <li data-icon="comment"><a href="#page_info"     class="text-0502 click-button">Info</a></li>
      <li data-icon="calendar"><a href="#page_relays" class="text-0503 click-button">Relays</a></li>
      <li data-icon="camera"><a href="#page_camera"       class="text-0506 click-button">Camera</a></li>
    </ul>
  </div>
 
</div>

<div data-role="page" class="aquarium-menupage swipe-page ui-responsive-panel" id="page_relays" data-title="AquariumControl" data-theme="b">
  
  <div data-role="header" data-position="fixed">
    <h1 class="text-0503">Images</h1>
    <a href="#nav-panel3" data-icon="bars" data-iconpos="notext">Menu</a>
    <a href="#add-language3" data-icon="gear" data-iconpos="notext">Add</a>     
  </div>
  
  <div role="main" class="ui-content">
   <table class="table-outlet-state" cellpadding="0" cellspacing="0" >
    <tr>
      <th align="left">Status</th>
      <th align="left">Name</th>
      <th align="left">Mode</th>
    </tr>
    <?php
    for ($i=0; $i<8; $i++)
    {
      echo "<tr>\r\n";
      echo "<td><img id=\"img-outlet-state".$i."\" src=\"\" class=\"img-outlet-state img-outlet-state".$i."\"/><span class=\"s-overwrite-delay".$i."\" style=\"margin-left:0.5em;\"></span></td>\r\n";
      echo "<td><a rel=\"external\" href=\"./schedule".$i.".html\" data-role=\"button\" class=\"text-004".$i." ui-btn ui-corner-all ui-mini ui-btn-inline\" style=\"width:80%;text-align:left;\">OUTLET_".$i."</a></td>\r\n";
      
      echo "<td><div data-role=\"controlgroup\" data-type=\"horizontal\">\r\n";
      echo "<p id=\"p-outlet-mode-on".$i."\" class=\"p-outlet-mode-on ui-btn ui-corner-all ui-icon-power ui-btn-icon-notext\">ON</p>\r\n";
      echo "<p id=\"p-outlet-mode-off".$i."\" class=\"p-outlet-mode-off ui-btn ui-corner-all ui-icon-forbidden ui-btn-icon-notext\">OFF</p>\r\n";
      echo "<p id=\"p-outlet-mode-schedule".$i."\" class=\"p-outlet-mode-schedule ui-btn ui-corner-all ui-icon-clock ui-btn-icon-notext\">SCHEDULE</p>\r\n";
      echo "</div></td>";

      echo "</tr>\r\n";
    }
    ?>
  </table> 
    
  </div>

  <div data-role="footer" data-position="fixed" data-theme="b">
    <a id="btn_help" href="#page_relayhelp" class="text-0504 click-button ui-btn ui-corner-all ui-shadow ui-icon-info ui-btn-inline ui-btn-icon-notext ui-btn-b" data-icon="info" data-rel="dialog" data-transition="pop">Help</a>
  </div>

  <div data-role="panel" data-display="overlay" data-position="right" data-theme="a" id="add-language3">
    <h4><span class="text-0501">Language</span></h4>
      <ul data-role="listview" data-inset="true">
        <li><a data-rel="close" class="languageSelector ui-btn" href="#"><img src="./ico/de.png" alt="" class="ui-li-icon"/>Deutsch</a></li>
        <li><a data-rel="close" class="languageSelector ui-btn" href="#"><img src="./ico/en.png" alt="" class="ui-li-icon"/>English</a></li>
        <li><a data-rel="close" class="languageSelector ui-btn" href="#"><img src="./ico/thl.png" alt="" class="ui-li-icon"/>thlIngan Hol</a></li>
      </ul>
  </div>
 
   <div data-role="panel" data-display="overlay" data-theme="a" id="nav-panel3">
    <ul data-role="listview">
      <li data-icon="delete"><a href="#" data-rel="close">Close menu</a></li>
      <li data-icon="home"><a href="#page_home"       class="text-0505 click-button">Home</a></li>
      <li data-icon="comment"><a href="#page_info"     class="text-0502 click-button">Info</a></li>
      <li data-icon="calendar"><a href="#page_relays" class="text-0503 click-button">Relays</a></li>
      <li data-icon="camera"><a href="#page_camera"       class="text-0506 click-button">Camera</a></li>
    </ul>
  </div>
 
</div>


<!-- relayhelp page -->
<!-- ############################################################################################################################# -->
<div data-role="page" class="aquarium-menupage" id="page_relayhelp" data-theme="b">

  <div data-role="header" data-theme="b">
    <h1 class="text-0504">HELP</h1>
  </div><!-- /header -->

  <div role="main" class="ui-content" style="width:100%;background:#2a2a2a;color:#fff;position:absolute;" >
    <table>
      <tr><td><img src="images/power-on.svg" /></td>              <td class="text-0301">DEVICE IS ON</td></tr>
      <tr><td><img src="images/icons-svg/power-black.svg" /></td> <td class="text-0302">DEVICE IS OFF</td></tr>
      <tr><td><img src="images/alwayson.png" /></td>              <td class="text-0304">DEVICE IS ALWAYS ON</td></tr>
      <tr><td><img src="images/alwaysoff.png" /></td>             <td class="text-0305">DEVICE IS ALWAYS OFF</td></tr>
      <tr><td><img src="images/scheduled.png" /></td>             <td class="text-0306">DEVICE IS SCHEDULED</td></tr>
      <tr><td><img src="images/overwrite.png" /></td>             <td class="text-0303">DEVICE IS IN OVERWRITE MODE</td></tr>
      <tr><td><img src="images/icons-svg/delete-red.svg" /></td>  <td class="text-0307">DEVICE ERROR</td></tr>
    </table>
  </div><!-- /content -->

</div><!-- /relayhelp page -->


<div data-role="page" class="aquarium-menupage swipe-page ui-responsive-panel" id="page_camera" data-title="AquariumControl" data-theme="b">
  
  <div data-role="header" data-position="fixed">
    <h1 class="text-0506">Camera</h1>
    <a href="#nav-panel4" data-icon="bars" data-iconpos="notext">Menu</a>
    <a href="#add-language4" data-icon="gear" data-iconpos="notext">Add</a>     
  </div>
  
  <div role="main" class="ui-content" style="background:#000;padding:0;">
    <img class="img-webcam" src="webcam.jpg" />
  </div>

  <div data-role="footer" data-position="fixed" data-theme="b">
    <span id="aquarium-value" class="aquarium-value" style="color:#22aadd;margin-left:1em">??????</span><span class="aquarium-unit" style="color:#22aadd;margin-right:0.2em;margin-left:0.2em;">&deg;C</span>
    <span class="aquarium-status-datetime" style="color:#999;float:right;margin-right:1em;">YYYY-MM-DD hh:mm:ss</span>
  </div>

  <div data-role="panel" data-display="overlay" data-position="right" data-theme="a" id="add-language4">
    <h4><span class="text-0501">Language</span></h4>
      <ul data-role="listview" data-inset="true">
        <li><a data-rel="close" class="languageSelector ui-btn" href="#"><img src="./ico/de.png" alt="" class="ui-li-icon"/>Deutsch</a></li>
        <li><a data-rel="close" class="languageSelector ui-btn" href="#"><img src="./ico/en.png" alt="" class="ui-li-icon"/>English</a></li>
        <li><a data-rel="close" class="languageSelector ui-btn" href="#"><img src="./ico/thl.png" alt="" class="ui-li-icon"/>thlIngan Hol</a></li>
      </ul>
  </div>
 
   <div data-role="panel" data-display="overlay" data-theme="a" id="nav-panel4">
    <ul data-role="listview">
      <li data-icon="delete"><a href="#" data-rel="close">Close menu</a></li>
      <li data-icon="home"><a href="#page_home"       class="text-0505 click-button">Home</a></li>
      <li data-icon="comment"><a href="#page_info"     class="text-0502 click-button">Info</a></li>
      <li data-icon="calendar"><a href="#page_relays" class="text-0503 click-button">Relays</a></li>
      <li data-icon="camera"><a href="#page_camera"       class="text-0506 click-button">Camera</a></li>
    </ul>
  </div>
 
</div>



</html>
