<?php 
	session_start();
?>
<?php
if($_SESSION["login"] != "YES")
{
	echo "<script languge=javascript> alert('您没有登录，无权访问，请先登录！');
			location.href='../index.php'</script>";
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml"><!-- InstanceBegin template="/Templates/tmplet.dwt.php" codeOutsideHTMLIsLocked="true" -->
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>短信群发管理系统</title>
<link type="text/css" rel="stylesheet" href="style/style.css" />

<!-- InstanceBeginEditable name="EditRegion2" -->
<link type="text/css" rel="stylesheet" href="style/common.css" />
<script language="JavaScript" type="text/javascript">
<!--
function checkdata()
{
		if(myform.content.value == '')
		{
			alert ("短消息内容不能为空！");
			myform.focus();
			return false;
		}
}
-->
</script>
<!-- InstanceEndEditable -->
</head>
<body>
<div id="container">
  <div id="head"> </div>
  <div id="menu">
    <ul>
      <li id="first_li"><a href="group.php">发送短信</a></li>
      <li><a href="send_box.php">发件箱</a></li>
      <li><a href="recv_box.php">收件箱</a></li>
      <li><a href="mynumber.php">管理电话薄</a></li>
      <li><a href="autoreply.php">添加自动回复</a></li>
      <li><a href="autoreply_ctl.php">管理自动回复</a></li>
      <li><a href="toadmin.php">联系管理员</a></li>
      <li><a href="logout.php">注销</a></li>
    </ul>
  </div>
  <div id="crumbs">
	短信管理 &gt;&gt;&gt; 
	<!-- InstanceBeginEditable name="EditRegion3" --> 
	  联系管理员   
	  <!-- InstanceEndEditable -->  </div>
  <div id="main"> 
  <!-- InstanceBeginEditable name="EditRegion1" --> 
<?php 
	require_once('connecting.php'); 
	//登录账号
	$content=$_POST["content"];
	$addtime = date("Y-m-d H:i:s");
	if ( '' != $content ) {
		$query = "insert into sendmessage(numbersend,timesend,contentsend,flagsend) 
								values ('15926510635','$addtime','$content','0')";
		mysql_query( $query );
		mysql_close( $member );
		echo "<script languge=javascript> alert('发送成功！'); 
					location.href='send_box.php'</script>";
	}
?>
    <form  method="post" name="myform"  onsubmit="return checkdata();">
      <table class="i_table" align="center" cellpadding="1" cellspacing="1">
        <tr >
          <td height="46" class="head"  align="center"><span class="STYLE2">请您填写短信内容</span>&nbsp;</td>
        </tr>
<!--        <tr>
          <td width="121" height="54">对方手机号码： </td>
          <td width="329"><input type="text" name="name" va />
          </td>
        </tr>-->
        <tr class="b">
          <td><textarea name="content"  rows="7"  cols="45"></textarea></td>
        </tr>
        <tr class="b">
          <td  height="41" align="center"><div align="center">
              <input type="submit" name="namesubmit" value="发送" />
            </div></td>
        </tr>
      </table>
    </form>
  <!-- InstanceEndEditable --> </div>
  <div id="footer"><br />
    &copy;2010 　Designed By <a href="http://hi.baidu.com/rjz78"><cite>Don Ren</cite><img src="images/stone.gif" alt="石三" style="margin-left:20px; border:none;" /></a><br />
    <br />
  </div>
</div>
</body>
<!-- InstanceEnd --></html>
