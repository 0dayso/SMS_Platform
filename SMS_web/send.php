<?php 
	session_start();
?>
<?php
if($_SESSION["login"] != "YES")
{
	echo "<script languge=javascript> alert('您没有登录，无权访问，请先登录！');
			location.href='index.php'</script>";
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml"><!-- InstanceBegin template="/Templates/tmplet.dwt.php" codeOutsideHTMLIsLocked="true" -->
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>短信群发管理系统</title>
<link type="text/css" rel="stylesheet" href="style/style.css" />

<!-- InstanceBeginEditable name="EditRegion2" -->
<script type="text/javascript" src="lib/jquery.js"></script>
<script type="text/javascript" src="lib/maxlength.js"></script>
<script language="javascript" type="text/javascript">
	<!--
		function checkdata()
		{
				if(myform.name.value == '')
				{	
					alert ("接收方号码不能为空！");
					myform.focus();
					return false;
				}
				if(myform.content.value == '')
				{
					alert ("短消息内容不能为空！");
					myform.focus();
					return false;
				}
		}
	-->
</script>

<?php
	$number = $_GET['number'];
	$recv_id = $_GET['recv_id'];
	
	if( $recv_id ) {
		require_once('connecting.php');
		$sql = "select * from `receivemessage` where `idreceive` ='$recv_id'";
		$result = mysql_query( $sql ) or die(mysql_error());
		if( mysql_num_rows($result) < 1 ) {
			echo "<script languge=javascript> alert('不能回复！'); 
						location.href='recv_box.php'</script>";
		mysql_close();
	}
	$info = mysql_fetch_array( $result );
}
?>

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
		  <?php
		  if( $recv_id ) {
		  	echo "回复短信";
		  } else {
		  	echo "发送短信";
		  }
		  ?>
	  <!-- InstanceEndEditable -->  </div>
  <div id="main"> 
  <!-- InstanceBeginEditable name="EditRegion1" -->

    <form  method="post" name="myform"  onsubmit="return checkdata();" action="group.php">
      <table align="center" cellpadding="1" cellspacing="1" border="1">
        <tr >
          <td height="46"  colspan="2" align="center"><span class="STYLE2">
		  <?php
		  if( $info ) {
		  	echo '【'.Number2Name( $info['numreceive'] )."】 发送内容：<font color='#A5CBDE'><strong>".$info['contentreceive']."</strong></font>";
		  } else {
		  	echo "请您填写联系号码和短信内容";
		  }
		  ?>
		  </span>&nbsp;</td>
        </tr>
        <tr>
          <td width="121" height="54">对方手机号码： </td>
          <td width="329"><strong><?=$number?></strong><input type="text" name="msgto" value="<?=$number?>" <?=$number ? " style='display:none;'" : ""?> />
          </td>
        </tr>
        <tr>
          <td height="98">
<?php
	if( $recv_id ) {
		echo "回复内容：";
	} else {
		echo "发送内容：";
	}
?>
		  </td>
          <td>
		  	  <textarea name="content"  style="width:350px; height:120px; font-size:150%;" data-maxsize="70" data-output="status1" wrap="virtual"></textarea><br />
  请将短信内容限制在70字以内。当前字数：<span id="status1" style="width:30px;font-weight:bold;text-align:right; font-size:150%;"></span><br />
		  </td>
        </tr>
        <tr>
          <td  colspan="2" height="41" align="center"><div align="center">
              <input type="button" onclick="history.back(-1);"  value="返回" />&nbsp;&nbsp;
              <input type="submit" name="namesubmit" value="发送" />
            </div></td>
        </tr>
      </table>
    </form>

    <!-- InstanceEndEditable --> </div>
  <div id="footer"><br />
    &copy;2010 　Designed By <a href="http://hi.baidu.com/rjz78"><cite>Don Ren</cite><img src="images/stone.gif" alt="石三" style="margin-left:15px; border:none;" /></a><br />
    <br />
  </div>
</div>
</body>
<!-- InstanceEnd --></html>
