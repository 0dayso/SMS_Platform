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
<script language="JavaScript" type="text/javascript">
<!--
function checkdata()
{
	if(myform.name.value == '')
	{	
		alert ("联系人名字不能为空，建议输入真实姓名！");
		myform.focus();
		return false;
	}
	if(myform.content.value == '')
	{
		alert ("联系号码不能为空！");
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
	  添加联系人   
	  <!-- InstanceEndEditable -->  </div>
  <div id="main"> 
  <!-- InstanceBeginEditable name="EditRegion1" --> 
<?php 
	require_once('connecting.php'); 
	
	$name = $_GET["name"];
	$content=$_GET["content"];
	
	$content = PhoneNumberDetect( $content );
	//登录账号
	if($name != "" && $content != "" ) 
	{
		if( $content ) {
			$query = "select * from `number` where num='$content'";
			$result = mysql_query( $query );
			if( mysql_num_rows($result) > 0 ) {
				echo "<script languge=javascript> alert('该号码已存在！'); 
						location.href='mynumber.php'</script>";
			} else {
				
				$query = "insert into number(idnum,name,num) values ('0','$name','$content')";
				mysql_query( $query );
				echo "<script languge=javascript> alert('添加成功！'); 
							location.href='mynumber.php'</script>";
			}
		} else {
			echo "<script languge=javascript> alert('号码输入有误，请重新输入！'); 
						location.href='add.php'</script>";
		}
	}
	mysql_close( $member );
?>
  	<form  method="GET" name="myform" onsubmit="return checkdata();" action="add.php">
		<table border="1" align="center"  cellspacing="1" border="1">
			<tr >
				<td height="46"  colspan="2" align="center"><span class="STYLE2">请您填写联系人和联系号码</span>
				</td>	
			</tr>
			<tr>
				<td width="121" height="40">联系人名字：	
				</td>
				<td width="329">
					<input type="text" name="name" value="<?=$name?>"  />		
				</td>
			</tr>
			 <tr>
				<td height="63">联系人号码：
				</td>
				<td><input type="text" name="content" value="<?=$content?>" > </td>
			</tr>
			<tr>
				<td  colspan="2" height="41">
				  <div align="center">
				  <input name="reset"  type="reset"  value="重置" />
				  <input type="submit" name="namesubmit" value="添加" />
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
