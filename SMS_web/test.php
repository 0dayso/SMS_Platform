<?php 
/*
 * A platform based on B/S mode with enough features to be as a SMS server.
 * <SMS_Platform> Copyright (C) <2012>  <Jiangzhe Ren>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Email : jiangzhe78@gmail.com
 */
?>
<?php 
	session_start();
?>
<?php
if($_SESSION["login"] != "YES") {
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
<script language="javascript" src="lib/maxlength.js"></script>
<script language="JavaScript" type="text/javascript">
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

$(function () {
	$('textarea.limited').maxlength({
		'feedback' : '.charsLeft', 'useInput' : true
	});
   
	$('input.limited').maxlength({
		'feedback' : '.charsLeft' 
	});
   
	$('textarea.wordLimited').maxlength({
		'words': true,
		'feedback': '.wordsLeft', 'useInput' : true
	});
});
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
	  test  
	  <!-- InstanceEndEditable -->  </div>
  <div id="main"> 
  <!-- InstanceBeginEditable name="EditRegion1" --> 


<form action="">
        <p>可以输入 <span class="charsLeft">20</span> 个字符</p>
        <textarea class="limited"></textarea>
        <input type="hidden" name="maxlength" value="20" />
  </form>
    
  <form action="">
        <p>可以输入 <span class="charsLeft">10</span> 个字符</p>
        <input maxlength="10" class="limited" />
  </form>

<?php

/*

require_once( "connecting.php" );

$tel = "12502321592651063523";

echo PhoneNumberDetect( $tel )."<br>";

function Name2Number( $name ) {
	$ResNumber = '';
	
	if ( $name != '' ) {
		$query = "select * from `number` where `name` LIKE '$name'";
		$result = mysql_query( $query );
		if( mysql_num_rows( $result ) > 0 ) {
			$info = mysql_fetch_array( $result );
			if($info != "") {
				$ResNumber = $info['num'];
			}
		}//else
	}
	return $ResNumber;
}
echo Name2Number( "任江哲" )."<br>";*/
?>   
  <!-- InstanceEndEditable --> </div>
  <div id="footer"><br />
    &copy;2010 　Designed By <a href="http://hi.baidu.com/rjz78"><cite>Don Ren</cite><img src="images/stone.gif" alt="石三" style="margin-left:20px; border:none;" /></a><br />
    <br />
  </div>
</div>
</body>
<!-- InstanceEnd --></html>
