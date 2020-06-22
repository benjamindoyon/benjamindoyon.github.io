<?
set_time_limit(60*60);

session_start();
$this_directory = dirname(__FILE__);
$uname = $_SESSION['data']['mail'];
$ufold = str_replace(".","_",str_replace("@kcl.ac.uk","",$uname));
$mainfolder = $this_directory.'/'.$ufold;
$webm = $ufold."/".$_SESSION['project'];
//echo $webm;
//die();

if (!isset($_SESSION['project'])) {
	die();
}


$this_directory = dirname(__FILE__);
$vid_directory = "$this_directory/$webm";

function gettimeoffile($s) {
    return str_replace(str_replace('video','',$s),'.mp4','');
}

function sortByTime($a, $b) {
    return $b[1]-$a[1];
}

function sortByTimeInv($a, $b) {
    return ((int) gettimeoffile($b[0]))-((int) gettimeoffile($a[0]));
}

if (isset($_POST['getlist'])) {
    $res = array();
    $dirs = scandir($vid_directory);
    foreach ($dirs as $d) 
        if (strpos($d, '.mp4') !== false) {
            if ($d != '0.mp4' && $d != 'Intro.mp4' && $d != 'Outro.mp4' && $d != 'output.mp4') {
                $lastModifiedTimestamp = filemtime("$vid_directory/$d");
                //$output2 = shell_exec("$this_directory/ffmpeg4/ffmpeg/ffmpeg -i $vid_directory/$d 2>&1 | grep \"Duration\"");
                $res[] = array($d,$lastModifiedTimestamp);
            }
        }
    usort($res, 'sortByTime');    
    //echo $res;
    echo json_encode($res);
    die('');
}

if (isset($_POST['deletefile'])) {
    $filename = $_POST['fn'];
    $filename3 = explode(".",$_POST['fn'])[0].".jpg";

    if (file_exists("$vid_directory/$filename")) {
        chmod("$vid_directory/$filename", 0777);
        unlink("$vid_directory/$filename");
    }
    if (file_exists("$vid_directory/$filename3")) {
        chmod("$vid_directory/$filename3", 0777);
        unlink("$vid_directory/$filename3");
    }
    //echo file_exists("$vid_directory/$filename");
    echo 'deleted:';
    die($filename);
}

if (isset($_POST['duration'])) {
    $filename = "$vid_directory/".$_POST['fn'];
    $output = shell_exec("$this_directory/ffmpeg4/ffmpeg/ffmpeg -i $filename 2>&1 | grep -o -P \"(?<=Duration: ).*?(?=,)\"");
    echo $output;
    die();
}

if (isset($_POST['merge'])) {
    $res = array();
    $dirs = scandir($vid_directory);
    foreach ($dirs as $d) 
        if (strpos($d, '.mp4') !== false) {
            if ($d != '0.mp4' && $d != 'Intro.mp4' && $d != 'Outro.mp4' && $d != 'output.mp4') {
                $lastModifiedTimestamp = filemtime("$vid_directory/$d");
                $res[] = array($d,$lastModifiedTimestamp);
                $filename = "$vid_directory/$d";
                $filename2 = str_replace('.webm','.mp4',$filename);
                //$output = shell_exec("$this_directory/ffmpeg4/ffmpeg/ffmpeg -fflags +genpts -i $filename -r 24 $filename2 2>&1");
                //echo "<br><br>$output<br><br>";
            }
        }
    //echo "<br><br>";
    //print_r ($res);
    //usort($res, 'sortByTime'); 
    //echo "<br><br>";
    //print_r ($res);
    //echo "<br><br>";          
    usort($res, 'sortByTimeIvs'); 
    print_r ($res);
    //echo "<br><br>";       
    //$res = var_dump(array(array("Intro.mp4",0))+$res+array(array("Intro.mp4",0)));
    
    print_r($res);

    $mylist = "$vid_directory/mylist.txt";
    if (file_exists($mylist)) {
        chmod($mylist, 0777);
        unlink($mylist);
    }

    $s = "file '$this_directory/webm/Intro.mp4'\n";
    foreach ($res as $file) {
        $f = str_replace('.webm','.mp4',$file[0]);
        $s = $s."file '$vid_directory/$f'\n";
    }
    $s = $s."file '$this_directory/webm/Intro.mp4'";

    echo $s;

    echo $mylist;
    echo file_put_contents($mylist,$s);
    chmod($mylist, 0777);

    $output = "$vid_directory/output.mp4";
    $outputr = "$vid_directory/output.mp";  
    if (file_exists($output)) {
        chmod($output, 0777);
        unlink($output);
    }
    if (file_exists($outputr)) {
        chmod($outputr, 0777);
        unlink($outputr);
    }
   
    //$output = shell_exec("$this_directory/ffmpeg4/ffmpeg/ffmpeg -fflags +genpts -i $this_directory/kings/Intro.mp4 -r 24 $vid_directory/Intro.mp4 2>&1");
    //$output = shell_exec("$this_directory/ffmpeg4/ffmpeg/ffmpeg -fflags +genpts -i $this_directory/kings/Outro.mp4 -r 24 $vid_directory/Outro.mp4 2>&1");


    $output2 = shell_exec("$this_directory/ffmpeg4/ffmpeg/ffmpeg -f concat -safe 0 -i $mylist -c copy $output 2>&1");
    echo "$output2";

    chmod($output, 0777);

    copy($output,$outputr);

    chmod($outputr, 0777);
    
    die('');
}

if (isset($_POST['fname'])) {
    $data = substr($_POST['data'], strpos($_POST['data'], ",") + 1);
    $decodedData = base64_decode($data);
    $filename = "$this_directory/$webm/".$_POST['fname'];
    echo "fname:::".$_POST['fname'];
    $name0 = explode(".",$_POST['fname'])[0];
    $filename2 = "$this_directory/$webm/$name0.mp4";
    $filename3 = "$this_directory/$webm/$name0.jpg";
    //echo $filename2;
    if (file_exists($filename)) {
        chmod($filename, 0777);
        unlink($filename);
    }

    if (file_exists($filename2)) {
        chmod($filename2, 0777);
        unlink($filename2);
    }

    if (file_exists($filename3)) {
        chmod($filename3, 0777);
        unlink($filename3);
    }


    $fp = fopen($filename, 'wb');
    fwrite($fp, $decodedData);
    fclose($fp);
    chmod($filename, 0777);

    //ob_flush();
    //flush();

    echo $filename."\n";
    echo $filename2."\n";
    echo $filename3."\n";


    //$output = shell_exec("$this_directory/ffmpeg1/ffmpeg/ffmpeg -fflags +genpts -i $filename -r 24 $filename2 2>&1");
    //echo "<pre>$output</pre>";
    //$output = shell_exec("$this_directory/ffmpeg2/ffmpeg/ffmpeg -fflags +genpts -i $filename -r 24 $filename2 2>&1");
    //echo "<pre>$output</pre>";
    //$output = shell_exec("$this_directory/ffmpeg3/ffmpeg/ffmpeg -fflags +genpts -i $filename -r 24 $filename2 2>&1");
    //echo "<pre>$output</pre>";
   $output = shell_exec("$this_directory/ffmpeg4/ffmpeg/ffmpeg -fflags +genpts -i $filename -r 24 $filename2 2>&1");
    //$output = shell_exec("$this_directory/ffmpeg4/ffmpeg/ffmpeg -i $filename -vcodec libx264  $filename2 2>&1");
    $output .= shell_exec("$this_directory/ffmpeg4/ffmpeg/ffmpeg -i $filename -vframes 1 -f image2 $filename3 2>&1");

    //$output .= shell_exec("$this_directory/ffmpeg4/ffmpeg/ffmpeg -i $vid_directory/video1592666246937.mp4 -vframes 1 -f image2 $vid_directory/video1592666246937.jpg 2>&1");
    //$output .= shell_exec("$this_directory/ffmpeg4/ffmpeg/ffmpeg -i $vid_directory/video1592666188985.mp4 -vframes 1 -f image2 $vid_directory/video1592666188985.jpg 2>&1");
    //$output .= shell_exec("$this_directory/ffmpeg4/ffmpeg/ffmpeg -i $vid_directory/video1592665376973.mp4 -vframes 1 -f image2 $vid_directory/video1592665376973.jpg 2>&1");
    //$output .= shell_exec("$this_directory/ffmpeg4/ffmpeg/ffmpeg -i $vid_directory/video1592665329074.mp4 -vframes 1 -f image2 $vid_directory/video1592665329074.jpg 2>&1");
    //$output .= shell_exec("$this_directory/ffmpeg4/ffmpeg/ffmpeg -i $vid_directory/video1592665293420.mp4 -vframes 1 -f image2 $vid_directory/video1592665293420.jpg 2>&1");
    //$output .= shell_exec("$this_directory/ffmpeg4/ffmpeg/ffmpeg -i $vid_directory/video1592664736541.mp4 -vframes 1 -f image2 $vid_directory/video1592664736541.jpg 2>&1");
    //$output .= shell_exec("$this_directory/ffmpeg4/ffmpeg/ffmpeg -i $vid_directory/video1592664523055.mp4 -vframes 1 -f image2 $vid_directory/video1592664523055.jpg 2>&1");
    //$output .= shell_exec("$this_directory/ffmpeg4/ffmpeg/ffmpeg -i $vid_directory/video1592664347385.mp4 -vframes 1 -f image2 $vid_directory/video1592664347385.jpg 2>&1");
    //$output .= shell_exec("$this_directory/ffmpeg4/ffmpeg/ffmpeg -i $vid_directory/video1592664245603.mp4 -vframes 1 -f image2 $vid_directory/video1592664245603.jpg 2>&1");


    //echo "<pre>$output</pre>";
    //$output = shell_exec("$this_directory/ffmpeg5/ffmpeg/ffmpeg -fflags +genpts -i $filename -r 24 $filename2 2>&1");
    echo "<pre>$output</pre>";

    if (file_exists($filename)) {
        chmod($filename, 0777);
        unlink($filename);
    }
}


?> 