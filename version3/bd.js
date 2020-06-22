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

// pixels only used in not-fixedRatioMode
var pixels = 1000000;
var framePerSecond = 20;
// for now only fixedRatioMode has been checked
var fixedRatioMode = true;

// this replaces pixels, and is standard HD 720p
var targetWidth = 1280;
var targetHeight = 720;

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
var factorList = [1/4,1/4,1/3,1/4,1/4]

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
var takeFirstImage=0;

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
var bunchingInterval;
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

// change made here


/*
// output first image of frame - not used anymore, first image taken from canvas
function getFirstImage(blob) {
	if (!fixedRatioMode) {
		h = Math.sqrt(pixels / mainRatio);
		w = h * mainRatio;
		imageCanvas.width = w;
		imageCanvas.height = h;
	} else {
		var h = targetHeight;
		var w = targetWidth;
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
}*/

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
	updateTemporaryImage(stopImages[stopImages.length-1].src);
/*
	getFirstImage(recordedBlobList[recordedBlobList.length-1]).then(source=>{
		stopImages.push(new Image());
		stopImages[stopImages.length-1].src = source;
		updateTemporaryImage(source);
	});*/
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
		takeFirstImage = 1;
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
	clearInterval(bunchingInterval);
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
	bunchingInterval = window.setInterval(function() {
		mediaRecorder.stop();
		window.setTimeout(mediaRecorder.start,10);
	},60000)
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
	if (fixedRatioMode) {
		fixedRatio = targetWidth/targetHeight;
		sizeAll();
		canvas.width = targetWidth;
		canvas.height = targetHeight;
		imageCanvas.width = targetWidth;
		imageCanvas.height = targetHeight;
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
	var h,w,imageContext;
	recordInterval = window.setInterval(function() {
		// note: h * h * ratio = pixels
		if (!fixedRatioMode) {
			h = Math.sqrt(pixels / mainRatio);
			w = h * mainRatio;
			canvas.width = w;
			canvas.height = h;
		} else {
			h = targetHeight;
			w = targetWidth;
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
			if (takeFirstImage) {
				takeFirstImage = 0;
				imageContext = imageCanvas.getContext('2d');
				imageContext.clearRect(0,0,w,h);
				imageContext.drawImage(canvas,0,0);
				stopImages.push(new Image());
				stopImages[stopImages.length-1].src = imageCanvas.toDataURL("image/png");
			}
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

/* // ratio is now set to HD without need for the introvideo
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
*/

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


