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
	<a href='myprojects.php'>go back to your list of projects</a>
    </div>
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

<script>
// Immediately-invoked function expression
(function() {
    // Load the script
    var script = document.createElement("SCRIPT");
    script.src = 'https://ajax.googleapis.com/ajax/libs/jquery/1.7.1/jquery.min.js';
    script.type = 'text/javascript';
    script.onload = function() {
        var $ = window.jQuery;
        // Use $ here...
    };
    document.getElementsByTagName("head")[0].appendChild(script);
})();



var mainVideo = document.getElementById("mainVideo");
var secondaryVideo = document.getElementById("secondaryVideo");
var playbackVideo = document.getElementById("playbackVideo");
var canvas = document.getElementById("canvas");
var imageCanvas = document.createElement("canvas");
var cameraVideo = document.getElementById("cameraVideo");
var componentVideo = document.getElementById("componentVideo");
var controls = document.getElementById("controls");
var buttonConf1 = document.getElementById('buttonConf1');
var buttonConf2 = document.getElementById('buttonConf2');
var buttonConf3 = document.getElementById('buttonConf3');
var buttonConf4 = document.getElementById('buttonConf4');
var buttonPause = document.getElementById('buttonPause');
var buttonDownload = document.getElementById('buttonDownload');
var buttonChooseComponent = document.getElementById('buttonChooseComponent');
var buttonStopComponent = document.getElementById('buttonStopComponent');
var comp = document.getElementById('comp');
var textTitle = document.getElementById('textTitle');
var checkboxWatermark = document.getElementById('checkboxWatermark');
var checkboxTitle = document.getElementById('checkboxTitle');
var labelTitle = document.getElementById('labelTitle');



var watermarkImage = document.createElement("img");
watermarkImage.src = "kings/Watermark.png";
var introVideo = document.createElement("video");
introVideo.src = "kings/Intro.mp4";
introVideo.type = "video/mp4";
var outroVideo = document.createElement("video");
outroVideo.src = "kings/Outro.mp4";
outroVideo.type = "video/mp4";

var pixels = 1000000;
var framePerSecond = 20;
var fixedRatioMode = true;

var theTitleLines = ["Set title"];

cameraVideo.muted = true;
componentVideo.muted = true;
mainVideo.muted = true;
secondaryVideo.muted = true;

const controlsVideoMaxWidth = 150;
const controlsVideoMaxHeight = 170;
const totalBorder = 300;

var fixedRatio=1;
var mainRatio=1;
var mainWidthFactor=1;
var mainHeightFactor=1;
var playbackRatio = 1;
var secondaryRatio=1;
var secondaryWidthFactor=1/4;
var secondaryHeightFactor=1/4;
var factor = 1/4;
var factorList = [1/4,1/4,1/2,1/4,1/4]

var selectedState = 1;
var currentState = 0;

var recording = 0;
var paused = 1;
var playingback = 0;
var fadingDown = 0;
var fadingUp = 0;
var playingIntro = 0;
var playingOutro = 0;
var playingTitle = 0;
var comingBack = 0;
var stitching = 0;
var alpha=1;
var restart=0;

// 0: when (re-)starting, title must play, then video comes on from black
// 1: when (re-)starting, video comes on from black
// 2: when re-starting, video comes on with nice transition from previous image
var currentSequenceLocation=0;
var initialSequenceLocation=0;

var changeMain = 1;

var mediaRecorder;
var stopImages = [];

function tocss(x) {
	return x.toString() + "px";
}

function resize(whratio, wmax, hmax) {
	if (whratio < wmax/hmax) {
		var hnew = hmax;
		var wnew = hmax*whratio;
	} else {
		var hnew = wmax/whratio;
		var wnew = wmax;
	}
	return [wnew, hnew];
}

function resizecss(whratio, wmax, hmax) {
	const [wnew, hnew] = resize(whratio, wmax, hmax);
	return [tocss(wnew), tocss(hnew)];
}

function sizeAll() {
	var w,h;
	factor = factorList[currentState];
	if (fixedRatioMode) {
		[w,h] = resize(fixedRatio,window.innerWidth-totalBorder,window.innerHeight-controlsVideoMaxHeight-totalBorder);
		let [sw,sh] = resize(mainRatio,w,h);
		[mainVideo.style.width, mainVideo.style.height] = [tocss(sw),tocss(sh)];
		[mainWidthFactor, mainHeightFactor] = [sw/w, sh/h];
	} else {
		if (!playingback) [w,h] = resize(mainRatio,window.innerWidth-totalBorder,window.innerHeight-controlsVideoMaxHeight-totalBorder);
		else [w,h] = resize(playbackRatio,window.innerWidth-totalBorder,window.innerHeight-controlsVideoMaxHeight-totalBorder);
		[mainVideo.style.width, mainVideo.style.height] = [tocss(w), tocss(h)];
		[mainWidthFactor, mainHeightFactor] = [1,1];
	}
	[container.style.width, container.style.height] = [tocss(w+10), tocss(h+10)];
	[playbackVideo.style.width, playbackVideo.style.height] = [tocss(w), tocss(h)];
	[canvas.style.width, canvas.style.height] = [tocss(w), tocss(h)];
	let [sw,sh] = resize(secondaryRatio,w*factor,h*factor);
	[secondaryVideo.style.width, secondaryVideo.style.height] = [tocss(sw), tocss(sh)];
	[secondaryWidthFactor, secondaryHeightFactor] = [sw/w, sh/h];
}

function setMain(v) {
	if (!changeMain) return;
	mainVideo.srcObject = v.srcObject;
    if (v.srcObject) mainRatio = (v.videoWidth-v.clipLeft-v.clipRight)/(v.videoHeight-v.clipUp-v.clipDown);
    else mainRatio = 1;
    mainVideo.clipLeft = v.clipLeft;
    mainVideo.clipUp = v.clipUp;
   	mainVideo.clipWidth = v.videoWidth -v.clipLeft-v.clipRight;
    mainVideo.clipHeight = v.videoHeight -v.clipUp-v.clipDown;
 }

function setSecondary(v) {
	if (v && v.srcObject) {
		secondaryVideo.srcObject = v.srcObject;
    	secondaryRatio = (v.videoWidth-v.clipLeft-v.clipRight)/(v.videoHeight-v.clipUp-v.clipDown);
    	secondaryVideo.clipLeft = v.clipLeft;
    	secondaryVideo.clipUp = v.clipUp;
    	secondaryVideo.clipWidth = v.videoWidth -v.clipLeft-v.clipRight;
    	secondaryVideo.clipHeight = v.videoHeight -v.clipUp-v.clipDown;
 	} else {
		secondaryVideo.srcObject = null;
    	secondaryRatio = 1;
	}
}

function setButtons() {
	buttonConf1.style.backgroundColor = "#DDD";
	buttonConf2.style.backgroundColor = "#DDD";
	buttonConf3.style.backgroundColor = "#DDD";
	buttonConf4.style.backgroundColor = "#DDD";
	if (selectedState == 1) buttonConf1.style.backgroundColor = "#BBF";
	if (selectedState == 2) buttonConf2.style.backgroundColor = "#BBF";
	if (selectedState == 3) buttonConf3.style.backgroundColor = "#BBF";
	if (selectedState == 4) buttonConf4.style.backgroundColor = "#BBF";
}

function setState() {
	playingTitle = 0;
	if (selectedState == 1) {
		setMain(cameraVideo);
		setSecondary(null);
		currentState = 1;
	} else if (selectedState == 2) {
		setMain(cameraVideo);
		setSecondary(componentVideo);
		if (componentVideo.srcObject) currentState = 2;
		else currentState = 1;
	} else if (selectedState == 3) {
    	if (componentVideo.srcObject) {
    		setMain(componentVideo);
    		setSecondary(cameraVideo);
			currentState = 3;
    	} else {
			setMain(cameraVideo);
			setSecondary(null);
			currentState = 1;
    	}
	} else if (selectedState == 4) {
    	if (componentVideo.srcObject) {
    		setMain(componentVideo);
			currentState = 4;
    	} else {
    		setMain(cameraVideo);
			currentState = 1;
    	}
    	setSecondary(null);
	}
	sizeAll();
}

function changeState(state) {
	if (playingTitle || playingIntro || playingOutro || fadingDown || fadingUp || playingback) return;
	if (!cameraVideo.srcObject) selectedState = 4;
	if (state && cameraVideo.srcObject) selectedState = state;
	if (!componentVideo.srcObject) virtualState = 1;
	else virtualState = selectedState;
	setButtons();
	if (currentState==1 && virtualState==2) changeMain=0;
	else if (currentState==2 && virtualState==1) changeMain=0;
	else if (currentState==3 && virtualState==4) changeMain=0;
	else if (currentState==4 && virtualState==3) changeMain=0;
	else changeMain = 1;
	if (currentState != virtualState) fadingDown = 1;
}

var lightInterval;
var recordedChunks = [];
var newChunkLength = 0;
var lastChunkLength = 0;
var recordedBlobList = [];
var countVideoChunk;

function gotoPlayback(index) {
	if (index > recordedBlobList.length-1 || index<0) return;
	if (playbackVideo.src) URL.revokeObjectURL(playbackVideo.src);
	playbackVideo.src = URL.createObjectURL(recordedBlobList[index]);
	countVideoChunk = index;
}

////////////////////// NEW
//var mp4Files = [];
$("#proc").hide();
$("#proc2").hide();

function updateTemporaryImage(source) {
	document.getElementById("tmpimage").src = source;
	console.log(source);
	console.log(document.getElementById("tmpimage"));
}
function updateTemporaryDuration(dur) {
	document.getElementById("tmpdur").innerHTML = dur+"s";
}

function uploadBlob(blob,fname){
	var reader = new FileReader();
	fnamemp4 = fname.replace('webm','mp4');
	console.log('tmpname',fnamemp4);
	var vids = '<div class="container"><div class="row"><div class="col-2 align-self-center">'
					+'<button type="button" class="btn btn-danger" onclick="removeVideoDelayed(\''+fnamemp4+'\');"><i class="fa fa-trash fa-2x text-white"></i></button></div>'
					+'<div class="col-6 align-self-center"><img id="tmpimage" width="180" height="100"></img></div>'
					+'<div class="col-2 align-self-center text-center"><div  id="tmpdur"></div><div class="mcentered text-danger"><br><br>processing...</div></div></div>';
	document.getElementById("proc").innerHTML = vids;
	$("#proc").show();
    reader.onload = function(event){
        var fd = new FormData();
        fd.append('fname', fname);
		fd.append('data', event.target.result);
		//updatePlayList();
        $.ajax({
            type: 'POST',
            url: 'save.php',
			data: fd,
			timeout: 0,
            processData: false,
            contentType: false
        }).done(function(data) {
			$("#proc").hide();
    		console.log("conversion:",data);
			updatePlayList();
        });
    };      
    // trigger the read from the reader...
    reader.readAsDataURL(blob);
}

function uploadBlobI(i) {
	console.log("uploading a blob");
	var seconds = new Date().getTime();
	if (i<recordedBlobList.length)
		uploadBlob(recordedBlobList[i],"video"+seconds+".webm");
}

function saveVideo() {
	for (let i=0; i<recordedBlobList.length; i++) {
		if (playbackVideo.src) URL.revokeObjectURL(playbackVideo.src);
		playbackVideo.src = URL.createObjectURL(recordedBlobList[i]);
		buttonDownload.href = playbackVideo.src;
		buttonDownload.download = "video"+i.toString()+".webm";
		buttonDownload.click();

		////////////////////// NEW
		uploadBlobI(i);
		///////////////////////////
	}
	if (playbackVideo.src) URL.revokeObjectURL(playbackVideo.src);
	playbackVideo.src = URL.createObjectURL(recordedBlobList[0]);
	countVideoChunk = 0;
}

function startLight() {
	var lightIntensity = 5;
	var lightChange = 1;
	lightInterval = window.setInterval(function() {
		lightIntensity+=lightChange;
		if (lightIntensity >= 15) lightChange = -1;
		if (lightIntensity <= 5) lightChange = 1;
		light.style.backgroundColor = "#" + lightIntensity.toString(16) + "00";
	},50);
}

function stopLight() {
	clearInterval(lightInterval);
	light.style.backgroundColor = "#000";
}

// output blob duration
function getBlobDuration(blob) {
	return new Promise((res,rej)=> {
		var tempVideoEl = document.createElement('video');
		tempVideoEl.addEventListener('timeupdate', function() {
			res(tempVideoEl.duration);
			URL.revokeObjectURL(tempVideoEl.src);
		});
		tempVideoEl.addEventListener('loadeddata', function() {
			if (tempVideoEl.duration === Infinity) {
				tempVideoEl.currentTime = 300*60; // set to a very large time (in seconds) so it looks for end				
			} else {
				res(tempVideoEl.duration);
				URL.revokeObjectURL(tempVideoEl.src);
			}
 		});
		tempVideoEl.src = URL.createObjectURL(blob);
	});
}

// output first image of frame
function getFirstImage(blob) {
	if (!fixedRatioMode) {
		h = Math.sqrt(pixels / mainRatio);
		w = h * mainRatio;
		imageCanvas.width = w;
		imageCanvas.height = h;
	} else {
		var h = Math.sqrt(pixels / fixedRatio);
		var w = h * fixedRatio;
	}
	return new Promise((res,rej)=> {
		var tempVideoEl = document.createElement('video');
		tempVideoEl.addEventListener('canplay', function() {
			var imageContext = imageCanvas.getContext('2d');
			imageContext.clearRect(0,0,w,h);
			imageContext.drawImage(tempVideoEl,0,0);
			res(imageCanvas.toDataURL("image/png"));
			URL.revokeObjectURL(tempVideoEl.src);
		});
		tempVideoEl.src = URL.createObjectURL(blob);
	});
}

function stopEventRec(event) {
	recordedBlobList.push(new Blob(recordedChunks, {type: "video/webm"}));
	playbackRatio = mainRatio;
	uploadBlobI(recordedBlobList.length-1);
	gotoPlayback(recordedBlobList.length-1);
	checkboxTitle.checked = false;
	getBlobDuration(recordedBlobList[recordedBlobList.length-1]).then(dur=>{
		durationList.push(dur);console.log("duration: "+dur.toString());
		updateTemporaryDuration(dur);
	});
	getFirstImage(recordedBlobList[recordedBlobList.length-1]).then(source=>{
		stopImages.push(new Image());
		stopImages[stopImages.length-1].src = source;
		updateTemporaryImage(source);
	});
}

/*
function stopEventRec(event) {
	if (discarded) {
		recordedChunks.length = lastChunkLength;
		if (recordedChunks.length>0) {
			recordedBlobList.push(new Blob(recordedChunks, {type: "video/webm"}));
			uploadBlobI(recordedBlobList.length-1);
		}
	} else {
		recordedBlobList.push(new Blob(recordedChunks, {type: "video/webm"}));
		uploadBlobI(recordedBlobList.length-1);
		playbackRatio = mainRatio;
		playbackVideo.src = URL.createObjectURL(recordedBlobList[0]);
		countVideoChunk=0;
		playbackVideo.onended = function() {
			var source = playbackVideo.src;
			countVideoChunk++;
			if (countVideoChunk < recordedBlobList.length) {
				playbackVideo.src = URL.createObjectURL(recordedBlobList[countVideoChunk]);
				URL.revokeObjectURL(source);
				playbackVideo.play();
			} else {
				if (stitching) {
					mediaRecorder.stop();
					buttonStitch.style.backgroundColor = "#DDD";
					stitching = 0;
				} else {
					playbackVideo.src = URL.createObjectURL(recordedBlobList[0]);
					URL.revokeObjectURL(source);
					countVideoChunk = 0;
				}
			}
		};
	}
}*/
function dataEventRec(event) {
	recordedChunks.push(event.data);
	if (paused && !discarded) {
		lastChunkLength = newChunkLength;
		newChunkLength = recordedChunks.length;
	}
}
var canvasStream;
function setRecorder() {
//	var canvasStream = new MediaStream(canvas.captureStream(framePerSecond).getTracks().concat(cameraVideo.srcObject.getAudioTracks()));
//	var canvasStream = new MediaStream(canvas.captureStream(framePerSecond));
//	canvasStream.addTrack(cameraVideo.srcObject.getAudioTracks()[0]);
	canvasStream = new MediaStream([canvas.captureStream(framePerSecond).getTracks()[0],cameraVideo.srcObject.getAudioTracks()[0]]);
	var options = { mimeType: "video/webm" };
//	var options = { mimeType: "video/mp4" };
	recordedBlobList = [];
	durationList = [];
	mediaRecorder = new MediaRecorder(canvasStream,options);
	mediaRecorder.ondataavailable = dataEventRec;
	mediaRecorder.onstop = stopEventRec;
	mediaRecorder.onstart = event => {
		paused = 0;
		recordedChunks = [];
	}
}
/*
function setRecorder() {
//	var canvasStream = new MediaStream(canvas.captureStream(framePerSecond).getTracks().concat(cameraVideo.srcObject.getAudioTracks()));
//	var canvasStream = new MediaStream(canvas.captureStream(framePerSecond));
//	canvasStream.addTrack(cameraVideo.srcObject.getAudioTracks()[0]);
	var canvasStream = new MediaStream([canvas.captureStream(framePerSecond).getTracks()[0],cameraVideo.srcObject.getAudioTracks()[0]]);
	var options = { mimeType: "video/webm" };
//	var options = { mimeType: "video/mp4" };
	recordedBlobList = [];
	mediaRecorder = new MediaRecorder(canvasStream,options);
	mediaRecorder.ondataavailable = dataEventRec;
	mediaRecorder.onstop = stopEventRec;
	mediaRecorder.onstart = event => {
		paused = 0;
		discarded = false;
		recordedChunks = [];
		newChunkLength = 0;
		lastChunkLength = 0;
	}
	mediaRecorder.start();
	var lightIntensity = 5;
	var lightChange = 1;
	lightInterval = window.setInterval(function() {
		lightIntensity+=lightChange;
		if (lightIntensity >= 15) lightChange = -1;
		if (lightIntensity <= 5) lightChange = 1;
		light.style.backgroundColor = "#" + lightIntensity.toString(16) + "00";
	},50);
}*/



function pauseRecord() {
	mediaRecorder.stop();
	stopLight();
}

function playTitle() {
	playingTitle = 1;
	window.setTimeout(()=>{
		fadingDown = 1;
		currentSequenceLocation = 2;
	},2000);
	canvasStream.getAudioTracks()[0].enabled = false;
}

// this is not used anymore
function comeOn() {
// transitions are disabled
//	alpha=0;
//	fadingUp=1;
	currentSequenceLocation = 2;
}

// this is not used anymore
function comeBack() {
// transitions are disabled
//	comingBack=1;
//	changeMain=1;
//	alpha=0;
//	fadingUp=1;
	currentSequenceLocation = 2;
}

function restartRecord() {
	mediaRecorder.start();
	startLight();
}

/*
function discardLastSequence() {
	if (numberStopImages>=1) {
		stopImages[numberStopImages-1]=null;
		numberStopImages --;
	}
	if (recordedBlobList.length==1) {
		if (checkboxTitle.checked) currentSequenceLocation = 0;
		else currentSequenceLocation = 1;
	}
	if (recordedBlobList.length==2) {
		currentSequenceLocation = initialSequenceLocation+1;
	}
	recordedBlobList.pop();
	durationList.pop();
	gotoPlayback(recordedBlobList.length-1);
}
*/

/*
function continueStartSequence() {
	if (checkboxTitle.checked) {
		playingTitle = 1;
		window.setTimeout(()=>{fadingDown = 1;},2000);
	}
}

function startRecord() {
	if (checkboxIntro.checked) {
		introVideo.play();
		playingIntro = 1;
		if (!fixedRatioMode) {
			mainRatio = introVideo.videoWidth/introVideo.videoHeight;
			sizeAll();
		}
		introVideo.addEventListener('ended',function() {
			playingIntro=0;
			if (!fixedRatioMode) setState();
			continueStartSequence();
		});
	} else continueStartSequence();
	window.setTimeout(setRecorder,100);
}

function stopRecorder() {
	mediaRecorder.requestData();
	mediaRecorder.stop();
	clearInterval(lightInterval);
	light.style.backgroundColor = "#000";
}

function continueStopSequence() {
	stopRecorder();
	buttonRecord.value = "rec";
	recording = 0;
	for (let i=0; i++; i<numberStopImages) stopImages[i]=null;
	numberStopImages = 0;
	if (!fixedRatioMode) setState();
}

function stopRecord() {
	if (playingOutro) return;
	if (checkboxOutro.checked) {
		buttonRecord.value = "...";
		outroVideo.play();
		playingOutro = 1;
		if (!fixedRatioMode) {
			mainRatio = outroVideo.videoWidth/outroVideo.videoHeight;
			sizeAll();
		}
		outroVideo.addEventListener('ended',function() {
			continueStopSequence();
			playingOutro=0;
		});
	} else continueStopSequence();
	if (playingIntro) {
		playingIntro = 0;
		introVideo.pause();
		introVideo.currentTime=0;
	}
	if (playingTitle) playingTitle=0;
}

var discarded=false;
function pauseRecord() {
	discarded = false;
	mediaRecorder.requestData();
	mediaRecorder.pause();
	stopImages[numberStopImages] = new Image();
	stopImages[numberStopImages].src = imageCanvas.toDataURL("image/png");
	numberStopImages = numberStopImages+1;

// in the end not used as pause is disabled during intro and outro
	if (playingIntro) {
		introVideo.pause();
	}
	if (playingOutro) {
		outroVideo.pause();
	}
}

function restartRecord() {
// in the end not used as pause is disabled during intro and outro
	if (playingIntro) {
		introVideo.play();
	}
	if (playingOutro) {
		outroVideo.play();
	}

	if (!discarded) mediaRecorder.resume();
	else mediaRecorder.start();
}

function discardLastSequence() {
	discarded = true;
	stopImages[numberStopImages-1]=null;
	numberStopImages --;
	mediaRecorder.stop();
}
*/

// done at the start
function starting() {
	if (navigator.mediaDevices.getUserMedia) {
 	 	navigator.mediaDevices.getUserMedia({ audio: true, video: true })
    	.then(function (stream) {
    	  	cameraVideo.srcObject = stream;
    	  	cameraVideo.onloadedmetadata = (ev) => {
    	  		[cameraVideo.style.width, cameraVideo.style.height] = resizecss(cameraVideo.videoWidth/cameraVideo.videoHeight,controlsVideoMaxWidth,controlsVideoMaxHeight);
				changeState(1);
				setRecorder();
    	  	};
    	})
    	.catch(function (error) {
    		cameraVideo.srcObject = null;
    	  	changeState();
    	});
	} else {
		cameraVideo.srcObject = null;
    	changeState();
    }
    componentVideo.clipUp=0;
    componentVideo.clipDown=0;
    componentVideo.clipLeft=0;
    componentVideo.clipRight=0;
    cameraVideo.clipUp=0;
    cameraVideo.clipDown=0;
    cameraVideo.clipLeft=0;
    cameraVideo.clipRight=0;
    playbackVideo.src = null;
	playbackVideo.onended = function() {
		var source = playbackVideo.src;
		countVideoChunk++;
		if (countVideoChunk < recordedBlobList.length) {
			playbackVideo.src = URL.createObjectURL(recordedBlobList[countVideoChunk]);
			URL.revokeObjectURL(source);
			playbackVideo.play();
		} else {
			if (stitching) {
				stitching = 0;
			} else {
				playbackVideo.style.visibility = "hidden";
				playingback = 0;
				sizeAll();
			}
			gotoPlayback(recordedBlobList.length-1)
		}
	};
	context = canvas.getContext('2d');
	var h,w;
	recordInterval = window.setInterval(function() {
		// note: h * h * ratio = pixels
		if (!fixedRatioMode) {
			h = Math.sqrt(pixels / mainRatio);
			w = h * mainRatio;
			canvas.width = w;
			canvas.height = h;
		} else {
			var h = Math.sqrt(pixels / fixedRatio);
			var w = h * fixedRatio;
		}
		context.clearRect(0, 0, canvas.width, canvas.height);

			if (playingTitle) {
				if (fadingDown || fadingUp) {
					context.globalAlpha = alpha;
				}
				context.fillStyle = "white";
				context.fill();
				context.font = '48px sans-serif';
				context.textAlign = "center"; 
				let size = theTitleLines.length;
				theTitleLines.forEach((s,i)=>context.fillText(s,w/2,(h-size*48)/2+48*i));
			} else {
				if ((fadingDown || fadingUp) && changeMain) {
					context.globalAlpha = alpha;
				}
				let [sx,sy] = [(1-mainWidthFactor)*w/2, (1-mainHeightFactor)*h/2];
				if (mainVideo.srcObject) context.drawImage(mainVideo,mainVideo.clipLeft,mainVideo.clipUp,mainVideo.clipWidth,mainVideo.clipHeight,sx,sy,mainWidthFactor*w,mainHeightFactor*h);
				if ((fadingDown || fadingUp) && !changeMain) {
					context.globalAlpha = alpha;
				}
				if (secondaryVideo.srcObject) context.drawImage(secondaryVideo,secondaryVideo.clipLeft,secondaryVideo.clipUp,secondaryVideo.clipWidth,secondaryVideo.clipHeight,(1-secondaryWidthFactor)*w-5,5,secondaryWidthFactor*w,secondaryHeightFactor*h);
			}
			context.globalAlpha = 1;
			if (checkboxWatermark.checked) context.drawImage(watermarkImage,w-watermarkImage.width,0);
			if (restart) {
				restart = 0;
				mediaRecorder.start();
				canvasStream.getAudioTracks()[0].enabled = true;
			}

		if (fadingDown) {
			alpha = alpha - 0.1*0.25;
			if (alpha<=0) {
				if (!playingTitle) {
					alpha = 0; 
					fadingDown = 0;
					fadingUp = 1;
					setState();
				}
				if (playingTitle) {
					mediaRecorder.stop();
					alpha = 1;
					fadingDown = 0;
					playingTitle = 0;
					window.setTimeout(()=>restart = 1,50);
				}
			}
		}
		if (fadingUp) {
			alpha = alpha + 0.1*0.25;
			if (alpha>=1) {
				alpha = 1;
				fadingUp = 0;
				comingBack = 0;
				context.globalAlpha = 1;
			}
		}
	}, 10);
}

if (document.readyState === 'loading') {  // Loading hasn't finished yet
	document.addEventListener('DOMContentLoaded', starting);
} else {  // `DOMContentLoaded` has already fired
	starting();
}

introVideo.addEventListener("loadedmetadata", function() {
	if (!fixedRatioMode) return;
	fixedRatio = introVideo.videoWidth / introVideo.videoHeight;
	sizeAll();
	var h = Math.sqrt(pixels / fixedRatio);
	var w = h * fixedRatio;
	canvas.width = w;
	canvas.height = h;
	imageCanvas.width = w;
	imageCanvas.height = h;
});

document.addEventListener('keydown', (event) => {
	if (document.activeElement === textTitle) return;
	const keyName = event.key;
	if (keyName == "1") document.getElementById('buttonConf1').click();
	if (keyName == "2") document.getElementById('buttonConf2').click();
	if (keyName == "3") document.getElementById('buttonConf3').click();
	if (keyName == "4") document.getElementById('buttonConf4').click();
	if (keyName == " ") {event.preventDefault();document.getElementById('buttonPause').click();}
//	if (keyName == "-") document.getElementById('buttonDiscard').click();
});

window.addEventListener("resize",sizeAll);

document.getElementById('buttonConf1').addEventListener('click', () => changeState(1));

document.getElementById('buttonConf2').addEventListener('click', function() { 
	changeState(2); 
	if (componentVideo.style.visibility!="visible")
		showShareDialog();
});

document.getElementById('buttonConf3').addEventListener('click', function() { 
	changeState(3); 
	if (componentVideo.style.visibility!="visible")
		showShareDialog();
});

document.getElementById('buttonConf4').addEventListener('click', function() { 
	changeState(4); 
	if (componentVideo.style.visibility!="visible")
		showShareDialog();
});
/*
buttonDiscard.addEventListener('click', function() {
	if (!paused || recordedBlobList.length==0) return;
	discardLastSequence();
});
*/

/*
// replay the last sequence only
buttonPlayback.addEventListener('click', function() {
	if (fadingDown || fadingUp || recording || !paused || recordedBlobList.length==0 || stitching) return;
	if (playingback) {
		playbackVideo.style.visibility = "hidden";
		buttonPlayback.style.backgroundColor = "#DDD";
		buttonPlayback.value = "replay sequence";
		playingback = 0;
		playbackVideo.currentTime = 0;
		gotoPlayback(recordedBlobList.length-1);
		sizeAll();
	} else {
		playbackVideo.style.visibility = "visible";
		buttonPlayback.style.backgroundColor = "#BBF";
		buttonPlayback.value = "stop";
		playingback = 1;
		playbackVideo.play();
		sizeAll();
	}
});
*/

var recordingTimeStart = 0;
var refreshIntervalId = setInterval(updateTimer, 1000);

function updateTimer( )
{
	if (recordingTimeStart>0) {
		var sec = new Date().getTime();
		//console.log(sec - recordingTimeStart);
		document.getElementById("time").innerHTML="<small><font color=gray class='text-secondary'>"+sec2time2((sec - recordingTimeStart)/1000)+"</font></small>"
	} else {
		document.getElementById("time").innerHTML="";
	}
}

// start / stop recording
buttonPause.addEventListener('click', function() {
	if (!(fadingDown || fadingUp || playingTitle || playingIntro || playingOutro)) {
		if (!paused) {
			pauseRecord();
			window.setTimeout(() => {
				paused=1;
			},50);
			buttonPause.value = "record";
			buttonPause.style.backgroundColor = "#dc3545";
			recordingTimeStart = 0;
		}
		else {
			paused=0;
			if (checkboxTitle.checked) playTitle();
/*
			if (recordedBlobList.length==0) {
				initialSequenceLocation = currentSequenceLocation;
			}
			if (currentSequenceLocation == 0) {
				if (checkboxTitle.checked) playTitle();
				else {
					currentSequenceLocation = 1;
					initialSequenceLocation = currentSequenceLocation;
				}
			}
			if (currentSequenceLocation == 1) {
				comeOn();
			}
			if (currentSequenceLocation == 2) {
				comeBack();
			}*/
			window.setTimeout(restartRecord,50);
			buttonPause.style.backgroundColor = "#ffc107";
			buttonPause.value = "stop"
			recordingTimeStart = new Date().getTime();
		}
	}	
});

// set title when focus out of title box
textTitle.addEventListener("focus", function() {
//	textTitle.style.visibility="hidden";
//	buttonTitle.value="set";
	checkboxTitle.checked= true;
});

textTitle.addEventListener("focusout", function() {
//	textTitle.style.visibility="hidden";
//	buttonTitle.value="set";
	theTitleLines = textTitle.value.split('\n');
});

function showShareDialog() {
	if (playingTitle || playingIntro || playingOutro || fadingDown || fadingUp || playingback) return;
	if (navigator.mediaDevices.getDisplayMedia) {
 	 	navigator.mediaDevices.getDisplayMedia({ video: true })
    	.then(function (stream) {
    	  	componentVideo.srcObject = stream;
    	  	componentVideo.onloadedmetadata = (ev) => {
    	  		[componentVideo.style.width, componentVideo.style.height] = resizecss(componentVideo.videoWidth/componentVideo.videoHeight,controlsVideoMaxWidth,controlsVideoMaxHeight);
    	  		if (selectedState==1) changeState(3);
    	  		else changeState();
    	  		buttonChooseComponent.style.visibility="hidden";
    	  		componentVideo.style.visibility="visible";
    	  	};
    	})
    	.catch(function (error) {
    	});
	}
}

buttonChooseComponent.addEventListener('click', showShareDialog);

comp.addEventListener('mouseover', function() {
	if (playingback) return;
	if (componentVideo.srcObject) buttonStopComponent.style.visibility = "visible";
});

comp.addEventListener('mouseout', function() {
	if (playingback) return;
	buttonStopComponent.style.visibility = "hidden";
});

buttonStopComponent.addEventListener('click', stopCapture);
function stopCapture(evt) {
	if (playingTitle || playingIntro || playingOutro || fadingDown || fadingUp || playingback) return;
	componentVideo.style.visibility = "hidden";
    buttonChooseComponent.style.visibility="visible";
	buttonStopComponent.style.visibility = "hidden";
	let tracks = componentVideo.srcObject.getTracks();
	tracks.forEach(track => track.stop());
	componentVideo.srcObject = null;
	componentVideo.style.width = tocss(controlsVideoMaxWidth);
	componentVideo.style.height = tocss(controlsVideoMaxHeight);
	changeState();
}

function function1() {
  var ul = document.getElementById("list");
  var li = document.createElement("li");
  li.appendChild(document.createTextNode("Element 4"));
}





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