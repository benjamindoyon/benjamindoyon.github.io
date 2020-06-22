<?
session_start();
$this_directory = dirname(__FILE__);
$uname = $_SESSION['data']['mail'];
$ufold = str_replace(".","_",str_replace("@kcl.ac.uk","",$uname));
$mainfolder = $this_directory.'/'.$ufold;

if (isset($_GET['project'])) {
	$_SESSION['project'] = $_GET['project'];
	$_SESSION['pfolder'] = $mainfolder."/".$_GET['project'];
}
if (!isset($_SESSION['project'])) {
	header('Location: https://nms.kcl.ac.uk/nikolay.gromov/dhd/bdvideo/myprojects.php');
	exit;
}

//echo $mainfolder."/".$_SESSION['project']."/meta.txt";
$title = file_get_contents($mainfolder."/".$_SESSION['project']."/meta.txt");
$webm = $ufold."/".$_SESSION['project'];

//echo $title;
$_SESSION['ptitle'] = $title;
$time = $_SESSION['time'];
if ( time()-$time>60*60*24 ) {
	header('Location: https://nms.kcl.ac.uk/nikolay.gromov/dhd/login.php?f=bdvideo/myprojects.php');
	exit;
}
//die();
?>

<!DOCTYPE html>
<html lang="en">

<head>
<script src="jquery-3.5.1.min.js"></script>
<script src="//cdnapisec.kaltura.com/p/2368101/sp/236810100/embedIframeJs/uiconf_id/42864872/partner_id/2368101">
</script>
<script id="MathJax-script" async src="https://cdn.jsdelivr.net/npm/mathjax@3/es5/tex-mml-chtml.js"></script>
    <meta charset="utf-8">
    <script>window.MathJax = {
  tex: {
    inlineMath: [['$', '$'], ['\\(', '\\)']]
  }
};
</script>
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="BD-Studio online lectures made simpler">
    <meta name="author" content="Mathematics Department, Benjamin Doyon & Nikolay Gromov">

    <title>BD-Studio (<? echo $_SESSION['ptitle']; ?>)</title>

    <!-- Bootstrap Core CSS -->
    <link href="css/bootstrap.min.css" rel="stylesheet">

    <!-- Custom CSS -->
    <link href="css_old/stylish-portfolio.css" rel="stylesheet">

    <!-- Custom Fonts -->
    <link href="font-awesome/css/font-awesome.min.css" rel="stylesheet" type="text/css">

    <!-- HTML5 Shim and Respond.js IE8 support of HTML5 elements and media queries -->
    <!-- WARNING: Respond.js doesn't work if you view the page via file:// -->
    <!--[if lt IE 9]>
        <script src="https://oss.maxcdn.com/libs/html5shiv/3.7.0/html5shiv.js"></script>
        <script src="https://oss.maxcdn.com/libs/respond.js/1.4.2/respond.min.js"></script>
    <![endif]-->
<style>
    body {
  background: url('bg.jpg') no-repeat center center fixed;
  -webkit-background-size: cover;
  -moz-background-size: cover;
  background-size: cover;
  -o-background-size: cover;
}
.list-group{
    max-height: 300px;
    margin-bottom: 10px;
    overflow:scroll;
    -webkit-overflow-scrolling: touch;
}
#wrapper {
	position: absolute;
    right: 5%;
    left: 5%;
    margin-left: -50px;
	margin-right: -50px;

} 
#wrapper .container {max-width: 100%; display: block;}


.mcontainer {
  position: relative;
  text-align: center;
  color: white;
}

.mcentered {
  position: absolute;
  top: 50%;
  left: 50%;
  transform: translate(-50%, -50%);
}



#container {
	margin: 0px auto;
	width: 800px;
	height: 800px;
	position: relative;
	background-color: #555;
}
#controls {
	margin: 0px auto;
	width: 670px;
	height: 160px;
	position: relative;
	background-color: #FFF;
}
#mainVideo {
	width: 800px;
	height: 800px;
	position: absolute;
	visibility: hidden;
}
#secondaryVideo {
	width: 200px;
	height: 200px;
	position: absolute;
	bottom: 0;
	right: 0;
	visibility: hidden;
}
#canvas {
	width: 800px;
	height: 800px;
	left: 5px;
	top: 5px;
	background-color: black;
	position: absolute;
}
#playbackVideo {
	width: 500px;
	height: 375px;
	position: absolute;
	visibility: hidden;
}
#camera {
	width: 150px;
	height: 150px;
	position: absolute;
	left: 0;
	top: 0;
}
#cameraVideo {
	width: 150px;
	height: 150px;
	background-color: #444;
	position: absolute;
	visibility: hidden;
	left: 50%;
	top: 50%;
	transform: translate(-50%,-50%);
}
#configs {
	width: 170px;
	height: 150px;
	position: absolute;
	left: 0px;
	top: 0;
}
#buttonConf1 {
	width: 30px;
	height: 40px;
	position: absolute;
	left: 10px;
	top: 15px;
	border-radius: 5px;
	background-color: #DDD;
	font-size: 20px;
}
#imageConf1 {
	width: 40px;
	height: 40px;
	position: absolute;
	left: 40px;
	top: 15px;
	background-color: #DDD;
	font-size: 20px;
}
#buttonConf2 {
	width: 30px;
	height: 40px;
	position: absolute;
	left: 90px;
	top: 15px;
	border-radius: 5px;
	background-color: #DDD;
	font-size: 20px;
}
#imageConf2 {
	width: 40px;
	height: 40px;
	position: absolute;
	left: 120px;
	top: 15px;
	background-color: #DDD;
	font-size: 20px;
}
#buttonConf3 {
	width: 30px;
	height: 40px;
	position: absolute;
	left: 10px;
	bottom: 35px;
	border-radius: 5px;
	background-color: #DDD;
	font-size: 20px;
}
#imageConf3 {
	width: 40px;
	height: 40px;
	position: absolute;
	left: 40px;
	bottom: 35px;
	background-color: #DDD;
	font-size: 20px;
}
#buttonConf4 {
	width: 30px;
	height: 40px;
	position: absolute;
	left: 90px;
	bottom: 35px;
	background-color: #DDD;
	font-size: 20px;
}
#imageConf4 {
	width: 40px;
	height: 40px;
	position: absolute;
	left: 120px;
	bottom: 35px;
	border-radius: 5px;
	background-color: #DDD;
	font-size: 20px;
}
#confinstruction {
	width: 170px;
	height: 20px;
	position: absolute;
	left: 0px;
	bottom: 5px;
	font-size: 15px;
	font-family: sans-serif;
	text-align: center;
}
#rec {
	width: 150px;
	height: 150px;
	position: absolute;
	left: 170px;
	top: 0;
}
#light {
	width: 15px;
	height: 15px;
	position: absolute;
	left: 45px;
	top: 5px;
	border-radius: 15px;
	background-color: #000;
}
#time {
	position: absolute;
	left: 68px;
	top: 0px;
}
#buttonPause {
	width: 130px;
	height: 100px;
	position: absolute;
	left: 10px;
	bottom: 25px;
	border: 1px;
	border-radius: 25px;
	background-color: #dc3545;
	font-size: 15px;
    outline-width: 0;
}
#pauseinstruction {
	width: 150px;
	height: 20px;
	position: absolute;
	left: 0px;
	bottom: 5px;
	font-size: 15px;
	font-family: sans-serif;
	text-align: center;
}
#comp {
	width: 150px;
	height: 150px;
	position: absolute;
	left: 320px;
	top: 0;
}
#buttonChooseComponent {
	width: 130px;
	height: 130px;
	position: absolute;
	left: 10px;
	top: 10px;
	border-radius: 5px;
	background-color: #DDD;
	font-size: 15px;
}
#buttonStopComponent {
	width: 60px;
	height: 60px;
	position: absolute;
	left: 45px;
	bottom: 45px;
	border-radius: 5px;
	background-color: #DDD;
	font-size: 15px;
	visibility: hidden;
}
#componentVideo {
	width: 150px;
	height: 150px;
	position: absolute;
	left: 50%;
	top: 50%;
	transform: translate(-50%,-50%);
  	background-color: #444;
	visibility: hidden;
}
#intro {
	width: 180px;
	height: 150px;
	position: absolute;
	left: 480px;
	top: 0;
	font-family: sans-serif;
	font-size: 15px;
}
.buttonbg {
	background-color: #DDD;
}
</style>

</head>

<body>
    

<!-- Navigation -->
<nav class="navbar navbar-expand-lg navbar-light bg-light static-top mb-5 shadow">
  <div class="container">
    <a class="navbar-brand" href="#">BDVideo</a>
    <button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarResponsive" aria-controls="navbarResponsive" aria-expanded="false" aria-label="Toggle navigation">
          <span class="navbar-toggler-icon"></span>
        </button>
    <div class="collapse navbar-collapse" id="navbarResponsive">
      <ul class="navbar-nav ml-auto">
        <li class="nav-item">
          <a class="nav-link" title="log-out" href="https://nms.kcl.ac.uk/nikolay.gromov/dhd/login.php?f=dbvideo/"><? echo $uname; ?></a>
        </li>
                <li class="nav-item active">
          <a class="nav-link" href="index.php">BDVideo
                <span class="sr-only">(current)</span>
              </a>
        </li>
  
      </ul>
    </div>
  </div>
</nav>

<!-- Page Content -->
<header id="top" class="header">
<div>





	
  <div class="card border-0 shadow m-0" style="width: 100%; ">
    <div class="card-body p-0">
        
<div class="container">
  <div class="row">
      

<div class="col-sm-12">       
<p>


<div class="panel panel-primary" id="result_panel">
	<div class="panel-heading"><h2 class="panel-title"><? echo $_SESSION['ptitle']; ?></h2>
	<a href='myprojects.php'><i class="fa fa-arrow-left"></i> go back to your list of projects</a>
	<h3>How this works?</h3>
	1) Select one of 4 templates (web-cam only, or web-cam with shared screen/ipad in different compositions)<br>
	2) Press record button (big red one), record a part and press stop<br>
	<small class="text-muted">Feel free to record and stop as many times as you like. You can discard any segment of your recording in one click. You can also change the template at any time.</small><br>
	3) When you are finished press "Merge & Download" at the bottom of the page<br>
	<small class="text-muted">It will merge the segments you decided to keep and add King's intro and outro</small><br>
	4) As an option: <small class="text-muted"> you can add a slate (like a slide with a text on it) before each segment</small>
	</div>
	<div class="text-warning">For the current version: please use Chrome, and keep individual segments shorter than 1min</div>
    <div class="panel-body">
<br>




<div id="container" style="width: 1570px; height: 883.125px;">
	<video id="mainVideo" autoplay="true" style="width: 1177.5px; height: 883.125px;"></video>
    <video id="secondaryVideo" autoplay="true" style="width: 220.781px; height: 220.781px;"></video>
    <canvas id="canvas" style="width: 1570px; height: 883.125px;" width="1333" height="750"></canvas>
    <video id="playbackVideo" controls="controls" src="blob:https://nms.kcl.ac.uk/7c7c14ce-5e5f-5047-a5ed-543bd1f18654" style="width: 1570px; height: 883.125px;"></video>
</div><br>

<div id="controls" class="shadow">
	<div id="camera">
		<video id="cameraVideo" autoplay="true" style="width: 150px; height: 112.5px;"></video>
	</div>
	<div id="configs">
	  	<input id="buttonConf1" type="button" value="1" style="background-color: rgb(187, 187, 255);">
	  	<img id="imageConf1" src="buttons/button1.png" >
		<input id="buttonConf2" type="button" value="2" style="background-color: rgb(221, 221, 221);">
	  	<img id="imageConf2" src="buttons/button2.png" >
		<input id="buttonConf3" type="button" value="3" style="background-color: rgb(221, 221, 221);">
	  	<img id="imageConf3" src="buttons/button3.png" >
		<input id="buttonConf4" type="button" value="4" style="background-color: rgb(221, 221, 221);">
	  	<img id="imageConf4" src="buttons/button4.png" >
		<div id="confinstruction" class="text-muted">
			(press 1, 2, 3, 4)
		</div>
	</div>
	<div id="rec">
	    <div id="light" style="background-color: rgb(0, 0, 0);"></div><div id="time"><small></small></div>
		<input id="buttonPause" type="button" value="record">
		<div id="pauseinstruction" class="text-muted">
			(press space bar)
		</div>
		<a id="buttonDownload" style="visibility: hidden" href="blob:https://nms.kcl.ac.uk/e32c9fca-a324-7642-8040-b06841c181e9" download="video0.webm"> download </a>
	</div>
	<div id="comp">
		<input id="buttonChooseComponent" type="button" value="share window" style="visibility: hidden;">
	    <video id="componentVideo" autoplay="true" style="width: 150px; height: 150px; visibility: hidden;"></video>
		<input id="buttonStopComponent" type="button" value="stop" style="visibility: hidden;">
	</div>
	<div id="intro">
	<form>
  <div class="form-group"><br>
    <label for="textTitle">Add a slide with text</label>
    <input type="text" class="form-control" id="textTitle" aria-describedby="emailHelp" placeholder="Enter title">
  </div>
  <div class="form-check">
    <input type="checkbox" class="form-check-input" id="checkboxTitle">
    <label class="form-check-label" for="checkboxTitle">Display title at start</label>
  </div>
</form>

<input id="checkboxWatermark" type="checkbox" style="visibility: hidden;">
<label style="visibility: hidden;"> watermark </label>

<?
//		<div style="width: 5px; height: 5px;"></div>
//		<input id="checkboxTitle" type="checkbox" checked="checked">
//		<label id="labelTitle"> display title at start </label>
//		<br>
//		<input id="checkboxWatermark" type="checkbox" style="visibility: hidden;">
//		<label style="visibility: hidden;"> watermark </label>
//		<br>
//		<textarea id="textTitle" rows="2" cols="28" style="z-index: 1; position: absolute; left: 10px; top: 35px; font-family: sans-serif; font-size: 12px;">Set title</textarea>
?>
	</div>
</div>

<br>

<div class="container">
<div class="row"><div class="col-6">
	<div class="alert alert-warning p-0 m-0" role="alert" id="proc">
		Processing... (you can continue recording)
	  </div>	
<ul class="list-group shadow" id="cont" style="max-height:400px;overflow: auto;"></ul>
</div>
<div class="col-6 align-self-center  shadow">
	<div class="row justify-content-center align-self-center">
		<div class="col-sm-5 d-flex justify-content-center m-0 p-1 align-self-center">
			<div id = "totalduration" class="text-info align-middle"></div>
		</div>
		<div class="col-sm-5 d-flex justify-content-center m-0 p-1 text-info align-self-center">
			<div class="input-group">
				<div class="input-group-prepend">
				  <label class="input-group-text" for="inputGroupSelect01">Speed</label>
				</div>
				<select class="custom-select" id="inputGroupSelect01" onchange="setSpeed(this.value)">
				  <option selected>2</option>
				  <option value="1.5">1.5</option>
				  <option value="1">1</option>
				</select>
			  </div>
		</div>
	</div>
	<div class="row">
		<div class="col-sm-12 d-flex justify-content-center m-2">
			<video  type="video/mp4" src="kings/Intro.mp4" id="myvideo" width="400" height="300" controls style="background:black"></video><br>
		</div>
	</div>
	<div class="row align-middle">
		<div class="col-sm-6 d-flex justify-content-center">
			<button type="button" class="btn btn-warning btn-block text-white" onclick="mergeAndWatch();"><i class="fa fa-camera fa-1x text-white"></i> Merge & Watch</button>
		</div>
		<div class="col-sm-6 justify-content-center">
			<button type="button" class="btn btn-success btn-block" onclick="mergeAndDownload();"><i class="fa fa-download fa-1x text-white"></i> Merge & Download</button>
		</div>
	</div>
	<div class="alert alert-warning" role="alert" id="proc2">
		Processing... please wait
	</div>	

</div>
</div>
</div>

<script src="bd.js"></script>


<script>





/*======================*/
var fnameToDelete = '';

function removeVideoDelayed(fname) {
	$("#proc").hide();
	console.log('tmpname2',fname);
	fnameToDelete = fname;
	console.log(fnameToDelete);
}

function removeVideo(fname) {
	$.post( "save.php", {deletefile : "", fn : fname})
		.done(function( data ) {
			//PHPdata = eval(data);
			//console.log( "Data Loaded: ",PHPdata );
			//alert(data);
			removeFromList(fname);
	});
}

function getDuration(fname) {
	$.post( "save.php", {duration : "", fn : fname})
		.done(function( data ) {
			console.log(fname, data);
			var sec = time2sec(("bb"+data).replace("bb00:","").replace("\n","").replace(" ",""));
			document.getElementById('t'+fname).innerHTML = sec+"s";
			totalDuration();
	});
}


function removeFromList(fname) {
	//alert(fname);
	//videoList = [];
	var elmnt = document.getElementById('cont');
	var children = elmnt.children;
	for (var i = 0; i < children.length; i++) {
		var tableChild = children[i];
		if( tableChild.innerHTML.includes(fname)) {
			elmnt.removeChild(tableChild);
			tableChild = null;
		} 
	}
	updatePlayList();
}

var videoList = [];

function duration(i) {
	alert(document.getElementById(arrayMp4[i][0]).duration);
}

function buildList(arrayMp4) {
	videoList = [];
	var elmnt = document.getElementById('cont');
	s = '';
	var html = elmnt.innerHTML;
	var seconds = new Date().getTime();
	var j = 0;
	for (var i=arrayMp4.length-1;i>=0;i--) {
		console.log(fnameToDelete,arrayMp4[i][0]);
		videoList.push('<? echo $webm; ?>/'+arrayMp4[arrayMp4.length-1-i][0]);
		if (fnameToDelete == arrayMp4[i][0])
			removeVideo(fnameToDelete);
		if (! html.includes(arrayMp4[i][0]) && fnameToDelete != arrayMp4[i][0]) {
			var vids = '<div class="alert alert-info p-0 m-0" role="alert" id="alert'+arrayMp4[i][0]+'"  onclick="goToVideo(\''+arrayMp4[i][0]+'\');"><div class="container"><div class="row"><div class="col-2 align-self-center">'
					+'<button type="button" class="btn btn-danger" onclick="removeVideo(\''+arrayMp4[i][0]+'\');"><i class="fa fa-trash fa-2x text-white"></i></button></div>'
					//+'<div class="col-6 align-self-center"><video id="'+arrayMp4[i][0]+'" width="160" height="120" controls="controls"><source src="webm/'+arrayMp4[i][0]+'" type="video/mp4"></video></div>'
					+'<div class="col-6 align-self-center"><image id="'+arrayMp4[i][0]+'" width="180" height="100" src="<? echo $webm; ?>/'+arrayMp4[i][0].replace('mp4','jpg')+'" ></div>'
					+'<div class="col-2 align-self-center text-center" id="t'+arrayMp4[i][0]+'"></div></div></div>';
			//s = '<li id = "line'+i+'" class=\"list-group-item\"><small style=\'width:100px\'>'+arrayMp4[i][0]+'</small>'+vids+'</li>';
			var li = document.createElement("li");
			li.innerHTML = vids;
			elmnt.insertBefore(li, elmnt.firstChild);	
			getDuration(arrayMp4[i][0]);
		//	document.getElementById(arrayMp4[i][0]).addEventListener('loadeddata', function() {
		///	   document.getElementById('t'+this.id).innerText = this.duration+'s';
		//	   totalDuration();
		//	}, false);
		}		
	}
	playAll();

	j = arrayMp4.length-1;
	//elmnt.innerHTML = s;
	scrollTo('line'+j);
}

function getVidId(i) {
	return videoList[i].replace("<? echo $webm; ?>/","").replace(".","\\.");
}

function highlightList(k,scrolQ) {
	for (var i=0;i<videoList.length;i++) {
		$("#alert"+getVidId(i)).removeClass("alert-primary");	
		$("#alert"+getVidId(i)).addClass("alert-info");	
	}
	$("#alert"+getVidId(k)).removeClass("alert-info");	
	$("#alert"+getVidId(k)).addClass("alert-primary");	
	if (scrolQ) {
		scrollTo('alert'+getVidId(k));
	}
}

function time2sec(str0) {
	var ar = str0.split('.');
	str = ar[0];
	ms = ar[1];
    var p = str.split(':'),
        s = 0, m = 1;

    while (p.length > 0) {
        s += m * parseInt(p.pop(), 10);
        m *= 60;
    }

    return s+'.'+ms;
}

function sec2time(timeInSeconds) {
    var pad = function(num, size) { return ('000' + num).slice(size * -1); },
    time = parseFloat(timeInSeconds).toFixed(3),
    hours = Math.floor(time / 60 / 60),
    minutes = Math.floor(time / 60) % 60,
    seconds = Math.floor(time - minutes * 60),
    milliseconds = time.slice(-3);

    return pad(hours, 2) + ':' + pad(minutes, 2) + ':' + pad(seconds, 2) + '<small>.' + pad(milliseconds, 3)+'</small>';
}

function sec2time2(timeInSeconds) {
    var pad = function(num, size) { return ('000' + num).slice(size * -1); },
    time = parseFloat(timeInSeconds).toFixed(3),
    hours = Math.floor(time / 60 / 60),
    minutes = Math.floor(time / 60) % 60,
    seconds = Math.floor(time - minutes * 60),
    milliseconds = time.slice(-3);

    return pad(minutes, 2) + ':' + pad(seconds, 2) ;
}



function totalDuration() {
	var d = 0;
	for (var i=0;i<videoList.length;i++) {
		var r = parseFloat($("#t"+getVidId(i)).text().replace('s',''));
		if (r>0) 
			d = d+r;
	}
	$("#totalduration").html('total duration: '+sec2time(d));
}


function scrollTo(idname) {
	var elmnt = document.getElementById(idname.replace('\\','') );
	if (elmnt != null) {
		//if (!elementInViewport(elmnt))
		elmnt.scrollIntoView({ behavior: 'smooth', block: 'center' });
	}
}

function updatePlayList() {
	$.post( "save.php", {getlist : ""})
		.done(function( data ) {
			PHPdata = eval(data);
			//buildDataArray(data);
			console.log( "Data Loaded: ",PHPdata );
			buildList(PHPdata);			
		});
}

function mergeAndDownload() {
	$("#proc2").show();
	$.post( "save.php", {merge : ""})
		.done(function( data ) {
			console.log("merging",data);
			var anchor = document.createElement('a');
			var file_path = '<? echo $webm; ?>/output.mp';
			anchor.href = file_path;
			//anchor.target = '_blank';
			anchor.download = 'output.mp4';
			$("#proc2").hide();
			anchor.click();
		});
}

function mergeAndWatch() {
	$("#proc2").show();
	$.post( "save.php", {merge : ""})
		.done(function( data ) {
			console.log("merging",data);
			var anchor = document.createElement('a');
			var file_path = '<? echo $webm; ?>/output.mp4';
			anchor.href = file_path;
			anchor.target = '_blank';
			//anchor.download = 'output.mp4';
			$("#proc2").hide();
			anchor.click();
		});
}


updatePlayList();
var speed = 2;

function setSpeed(val) {
	speed = val;
	playAll();
}

var activeVideo = 0;

function goToVideo(fname) {
	var myvid = document.getElementById('myvideo');
	for(var i=0;i<videoList.length;i++) {
		if (videoList[i] == "<? echo $webm; ?>/"+fname) {
			activeVideo = i;
			myvid.src = videoList[activeVideo];
			myvid.defaultPlaybackRate = speed;
			myvid.play();		
			highlightList(activeVideo,true);
		}
	}
}

function playAll() {
	var myvid = document.getElementById('myvideo');
	activeVideo = videoList.length-1;

	myvid.src = videoList[activeVideo];
	myvid.defaultPlaybackRate = speed;

	myvid.addEventListener('play', function(e) {
		highlightList(activeVideo,true);
	});


	myvid.addEventListener('ended', function(e) {
		activeVideo = (--activeVideo);
		if (activeVideo<0) 
			activeVideo = videoList.length-1;	
		highlightList(activeVideo,true);
		myvid.src = videoList[activeVideo];
		console.log(videoList);
		myvid.play();		
	});

	myvid.addEventListener('error', function(e) {
		activeVideo = 0;
		//myvid.src = videoList[activeVideo];
		myvid.pause();
	});

}
</script>








</div></div>
</header>
</body>
</html>