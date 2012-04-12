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
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>短信群发管理系统</title>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<link type="text/css" rel="stylesheet" href="style/login.css">
<script language="javascript" src="lib/jquery.js"></script>
<script language="javascript">
<!--
$(document).ready(function() {
	$('body').keydown(function(event) {  
		if(event.keyCode==13){  
			return checkdata();
		}  
	}); 
	$('#login_div').click(function() {
	   	return checkdata();
	});
});
 
function checkdata() {
	if(myform.user.value == '') {	
		alert ("用户名不能为空！");
		myform.focus();
		return false;
	}
	if(myform.passwd.value == '') {
		alert ("请输入密码！");
		myform.focus();
		return false;
	}
	$('#button_sunmit').trigger('click');
}
-->
</script>
</head>
<body background="images/backimg.jpg" bgcolor="#18537c" leftmargin="0" topmargin="0" marginwidth="0" marginheight="0" onLoad="document.myform.user.focus();">
<!-- ImageReady Slices (gui2_onling.psd) -->
<form action="login.php" method="post" name="myform" id="myform" onsubmit="return checkdata();">
<table id="Table_01" width="554" height="676" border="0" cellpadding="0" cellspacing="0" align="center" >
	<tr>
		<td colspan="7">
			<img src="images/gui2_onling_01.jpg" alt="" width="554" height="285" border="0" usemap="#Map2"></td>
	</tr>
	<tr>
		<td colspan="3" rowspan="4">
			<img src="images/gui2_onling_02.jpg" width="231" height="146" alt=""></td>
		<td height="40px" colspan="2" valign="bottom" background="images/gui2_onling_03.jpg">
      <input name="user" type="text" size="14" style="height:30px;  background:url(images/gui2_onling_03.jpg); border:1px solid #2f2d2d; font-size:18px; font-weight:bold; color:#FFFFFF; width:160px" tabindex="1"></td>
		<td colspan="2" rowspan="4">
			<img src="images/gui2_onling_04.jpg" width="143" height="146" alt=""></td>
	</tr>
	<tr>
		<td colspan="2">
			<img src="images/gui2_onling_05.jpg" width="180" height="7" alt=""></td>
	</tr>
	<tr>
		<td colspan="2" background="images/gui2_onling_06.jpg" height="40px" valign="bottom">
			 <input name="passwd" type="password" size="13" style="height:30px; background:url(images/gui2_onling_06.jpg);border:1px solid #2f2d2d; font-size:18px; font-weight:bold; color:#FFFFFF; width:160px" tabindex="2"></td>
	</tr>
	<tr>
		<td colspan="2">
			<img src="images/gui2_onling_07.jpg" width="180" height="59" alt=""></td>
	</tr>
	<tr>
		<td rowspan="2">
			<img src="images/gui2_onling_08.jpg" width="111" height="244" alt=""></td>
		<td id="reset">
			<a tabindex="4" href="#" onClick="javascript:document.myform.reset()" style="cursor:hand;"><div id="reset_div"> </div></a></td>
		<td colspan="2" rowspan="2">
			<img src="images/gui2_onling_10.jpg" width="152" height="244" alt=""></td>
		<td colspan="2" width="98px" height="40px" id="login" style="cursor:hand;"><input type="button" onclick="document.myform.submit()" id=button_sunmit style="display:none;" /><a href="#"  tabindex="3" style="cursor:hand;"><div id="login_div"></div></a>
	  </td>
		<td rowspan="2">
			<img src="images/gui2_onling_12.jpg" alt="" width="110" height="244" border="0" usemap="#Map"></td>
	</tr>
	<tr>
		<td>
			<img src="images/gui2_onling_13.jpg" width="83" height="204" alt=""></td>
		<td colspan="2">
			<img src="images/gui2_onling_14.jpg" width="98" height="204" alt=""></td>
	</tr>
	<tr>
		<td>
			<img src="images/spacer.gif" width="111" height="1" alt=""></td>
		<td>
			<img src="images/spacer.gif" width="83" height="1" alt=""></td>
		<td>
			<img src="images/spacer.gif" width="37" height="1" alt=""></td>
		<td>
			<img src="images/spacer.gif" width="115" height="1" alt=""></td>
		<td>
			<img src="images/spacer.gif" width="65" height="1" alt=""></td>
		<td>
			<img src="images/spacer.gif" width="33" height="1" alt=""></td>
		<td>
			<img src="images/spacer.gif" width="110" height="1" alt=""></td>
	</tr>
</table>
</form>
<!-- End ImageReady Slices -->

<map name="Map" id="Map"><area shape="rect" coords="35,162,97,222" href="http://hi.baidu.com/rjz78" target="_blank" />
</map>
<map name="Map2" id="Map2"><area onclick="alert('Welcome.\n\nThe System contains 1 errors.')" shape="rect" coords="109,71,444,113" href="#" /></map></body>
</html>
