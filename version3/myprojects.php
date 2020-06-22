<?
session_start();
$uname = $_SESSION['data']['mail'];
$time = $_SESSION['time'];
if ( time()-$time>60*60*24 ) {
	header('Location: https://nms.kcl.ac.uk/nikolay.gromov/dhd/login.php?f=bdvideo/myprojects.php');
	exit;
}
$ufold = str_replace(".","_",str_replace("@kcl.ac.uk","",$uname));
$this_directory = dirname(__FILE__);

if (isset($_GET['title'])) {
    $pname = time();
    $pfolder = $this_directory.'/'.$ufold.'/'.$pname;
    echo $pfolder;
    if (!file_exists($pfolder)) {
        mkdir($pfolder , 0777, true);
        file_put_contents($pfolder."/meta.txt", $_GET['title']);
        chmod($pfolder, 0777);
        chmod($pfolder."/meta.txt", 0777);    
        header('Location: https://nms.kcl.ac.uk/nikolay.gromov/dhd/bdvideo/myprojects.php');
    }    
}


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
    <meta name="description" content="DB-Studio your projects">
    <meta name="author" content="Mathematics Department, Benjamin Doyon & Nikolay Gromov">

    <title>DB-Studio: your projects</title>

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
    </style>

</head>

<body>
    

<!-- Navigation -->
<nav class="navbar navbar-expand-lg navbar-light bg-light static-top mb-5 shadow">
  <div class="container">
    <a class="navbar-brand" href="#">DB-Studio</a>
    <button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarResponsive" aria-controls="navbarResponsive" aria-expanded="false" aria-label="Toggle navigation">
          <span class="navbar-toggler-icon"></span>
        </button>
    <div class="collapse navbar-collapse" id="navbarResponsive">
      <ul class="navbar-nav ml-auto">
        <li class="nav-item">
          <a class="nav-link" title="log-out" href="https://nms.kcl.ac.uk/nikolay.gromov/dhd/login.php?f=captions/"><? echo $uname; ?></a>
        </li>
                <li class="nav-item active">
          <a class="nav-link" href="">DB-Studio
                <span class="sr-only">(current)</span>
              </a>
        </li>
  
      </ul>
    </div>
  </div>
</nav>

<!-- Page Content -->
<div class="container" style="width:80%">
<div class="row">    
  <div class="card border-0 shadow my-5">
    <div class="card-body p-5">
        
<div class="container">
  <div class="row">
      

<div class="col-sm-12">       
<p>

<div class="panel panel-primary" id="result_panel">
    <div class="panel-heading"><h2 class="panel-title">Your projects:</h2>
    </div>
    <div class="panel-body">
<? 

if (!file_exists($this_directory.'/'.$ufold)) {
    mkdir($this_directory.'/'.$ufold, 0777, true);
}

$mainfolder = $this_directory.'/'.$ufold;
chmod($mainfolder, 0777);

$dirs = scandir($mainfolder);
foreach ($dirs as $d) { 
    if (strpos($d, '.') === false) {
        $res[] = $d;
        $title = file_get_contents($mainfolder."/".$d."/meta.txt");
        echo "<h4><a href='index.php?project=$d'>".$title."</a></h4>";
        chmod($mainfolder."/".$d, 0777);
        chmod($mainfolder."/".$d."/meta.txt", 0777);
    }
}
//usort($res, 'sortByTime');    



?>

<h2>Create new project:</h2>
<form action="myprojects.php">
  <div class="form-group">
    <label for="exampleInputEmail1">To create a new project fill this form:</label>
    <input name="title" type="text" class="form-control" id="exampleInputEmail1" aria-describedby="emailHelp" placeholder="Project title">
    <small id="emailHelp" class="form-text text-muted">Project title, for internal use.</small>
  </div>
  <button type="submit" class="btn btn-primary">Submit</button>
</form>


    
  </div></div>
    </div>
</body>
</div>
</html>
