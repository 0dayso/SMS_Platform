<?php /*
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
		alert ("接收方号码不能为空！");
		myform.focus();
		return false;
	}
	if(myform.number.value == '')
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
	  修改联系人信息   
	  <!-- InstanceEndEditable -->  </div>
  <div id="main"> 
  <!-- InstanceBeginEditable name="EditRegion1" --> 
<?php 
	require_once('connecting.php');
	
	$id = $_GET["id"]; 
	if( $id ) {
		$name = $_GET["name"];
		$number=$_GET["number"];
		//echo $name;
		//echo $number;
		
		$query = "select * from `number` where `idnum` LIKE '$id'";
		$result = mysql_query( $query );
		if( mysql_num_rows( $result ) > 0 ) {
			$info = mysql_fetch_array( $result );
			if($info != "") {
				$DB_Name = $info['name'];
				$DB_Num = $info['num'];
			}
		}//else
		
		$Detect_number = PhoneNumberDetect( $number );
		
		if( $name != "" && $Detect_number != "" ) 
		{
			if( $number ) {
				$query = "select * from `number` where num='$number' and `idnum` <> '$id'";
				//echo $query;
				$result = mysql_query( $query );
				if( mysql_num_rows( $result) > 0 ) {
					echo "<script languge=javascript> alert('该号码已存在！'); 
							location.href='mynumber.php'</script>";
				} else {
					$query = "update `number` set `name`='$name', `num`='$number' where idnum='$id'";
					$result = mysql_query( $query );
					if( $result ) {
						echo "<script languge=javascript> alert('修改成功！'); 
									location.href='mynumber.php'</script>";
					}
				}
			} else {
				echo "<script languge=javascript> alert('号码输入有误，请重新输入！'); 
							location.href='add.php'</script>";
			}
		}
	}
	mysql_close( $member );
?>

  	<form  method="GET" name="myform" onsubmit="return checkdata();" action="modify.php">
		<input type="hidden" name="id" value="<?=$id?>"  />	
		<table border="1" align="center"  cellspacing="1" border="1">
			<tr >
				<td height="46"  colspan="2" align="center"><span class="STYLE2">请您修改联系人和联系号码</span>
				</td>	
			</tr>
			<tr>
				<td width="121" height="40">联系人名字：	
				</td>
				<td width="329">
					<input type="text" name="name" value="<?=$DB_Name?>"  />		
				</td>
			</tr>
			 <tr>
				<td height="63">联系人号码：
				</td>
				<td><input type="text" name="number" value="<?=$DB_Num?>" > </td>
			</tr>
			<tr>
				<td  colspan="2" height="41">
				  <div align="center">
				  <input name="reset"  type="reset"  value="重置" />
				  <input type="submit" name="namesubmit" value="修改" />
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
