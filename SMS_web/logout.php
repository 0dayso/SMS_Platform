<?php
	session_start();
?>
<!--文件logout.php: 注销登录页面-->
<?php
	
	$_SESSION["login"] = "" ;
	$_SESSION["viplogin"] = "" ;
	echo "<script languge=javascript> location.href='index.php'; </script>";
?>