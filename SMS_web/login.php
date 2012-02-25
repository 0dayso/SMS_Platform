<?php 
	session_start();
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>短信群发管理系统</title>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<link type="text/css" rel="stylesheet" href="style/login.css">
</head>
<body>
<?php 
require_once('connecting.php'); 
		
$name = $_POST["user"];
$password = $_POST["passwd"];
//如果用户提交了登陆信息，在连接到数据库，与数据库中的信息相比较
if( $name == "" || $password == "" )
{
	echo "<script languge=javascript> alert('用户名和密码不能为空！');
					history.back(-1);</script>";
} else {
	$query = "select * from admin where username = '$name'";
	$result  = mysql_query($query,$member);
	if( mysql_num_rows($result) > 0) {
		$info = mysql_fetch_array( $result );
	
		if($info['userpass'] != $password)
		{
			echo "<script languge=javascript> alert('密码输入错误，请重新登录!');
					history.back(-1);</script>";
		}
		else
		{
			//用户名密码都正确，则注册一个session来标记其登录状态
			$_SESSION["login"] = "YES";
			echo "<script languge=javascript>location.href = 'recv_box.php';</script>";
		}
	} else {
		echo "<script languge=javascript> alert('没有此用户!');
					history.back(-1);</script>";
	}
}
mysql_close($member);
?>
</body>
</html>
