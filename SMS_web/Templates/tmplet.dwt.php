<?php 
	session_start();
?>
<?php
if($_SESSION["login"] != "YES") {
	echo "<script languge=javascript> alert('您没有登录，无权访问，请先登录！');
			location.href='index.php'</script>";
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>短信群发管理系统</title>
<link type="text/css" rel="stylesheet" href="../style/style.css" />
<!-- TemplateInfo codeOutsideHTMLIsLocked="true" -->
<!-- TemplateBeginEditable name="EditRegion2" -->
<script language="javascript">
<!--
function checkdata() {
	if(myform.name.value == '') {	
		alert ("接收方号码不能为空！");
		myform.focus();
		return false;
	}
	if(myform.content.value == '') {
		alert ("短消息内容不能为空！");
		myform.focus();
		return false;
	}
}
-->
</script>
<!-- TemplateEndEditable -->
</head>
<body>
<div id="container">
  <div id="head"></div>
  <div id="menu">
    <ul>
      <li id="first_li"><a href="../group.php">发送短信</a></li>
      <li><a href="../send_box.php">发件箱</a></li>
      <li><a href="../recv_box.php">收件箱</a></li>
      <li><a href="../mynumber.php">管理电话薄</a></li>
      <li><a href="../autoreply.php">添加自动回复</a></li>
      <li><a href="../autoreply_ctl.php">管理自动回复</a></li>
      <li><a href="../toadmin.php">联系管理员</a></li>
      <li><a href="../logout.php">注销</a></li>
    </ul>
  </div>
  <div id="crumbs">
	短信管理 &gt;&gt;&gt; 
	<!-- TemplateBeginEditable name="EditRegion3" --> 
	EditRegion3	
	<!-- TemplateEndEditable -->  </div>
  <div id="main"> 
  <!-- TemplateBeginEditable name="EditRegion1" --> 
  EditRegion1   
  <!-- TemplateEndEditable --> </div>
  <div id="footer"><br />
    &copy;2010 　Designed By <a href="http://hi.baidu.com/rjz78"><cite>Don Ren</cite><img src="../images/stone.gif" alt="石三" style="margin-left:15px; border:none;" /></a><br />
    <br />
  </div>
</div>
</body>
</html>
