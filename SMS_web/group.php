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
<script language="JavaScript" type="text/javascript">
<!--
function checkdata()
{	
	
	if( myform.msgto.value == '' )
	{	
		alert ("接收方号码不能全部为空！");
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


function moveOption1(e1, e2){
 try{
  for(var i = 0; i < e1.options.length; i++){
  
   if( e1.options[i].selected ){
    var e = e1.options[i];
	e2.options.add(new Option(e.text, e.value));
    e1.remove(i);
    i = i - 1;
   }
  }
  document.myform.msgto.value=getvalue(document.myform.list2);
 }
 catch(e){}
}

function moveOption2(e1, e2){
 try{
  for(var i = 0; i < e1.options.length; i++){
   if(e1.options[i].selected ){
    var e = e1.options[i];
	e2.options.add(new Option(e.text, e.value));
    e1.remove(i);
    i = i - 1;
   }
  }
  document.myform.msgto.value=getvalue(document.myform.list2);
 }
 catch(e){}
}

function getvalue(geto){
	var allvalue = "";
	for(var i=0;i<geto.options.length;i++){
		allvalue +=geto.options[i].value + ",";
	}
	return allvalue;
}

function moveAllOption1(e1, e2){
 try{
  for(var i = 0;i < e1.options.length; i++){
   var e = e1.options[i];
   
   e2.options.add(new Option(e.text, e.value));
   e1.remove(i);
   i = i - 1;
  }
  document.myform.msgto.value=getvalue(document.myform.list2);  
 }
 catch(e){ 
 }
}


function moveAllOption2(e1, e2){
 try{
  for(var i = 0;i < e1.options.length; i++){
   var e = e1.options[i];
    e2.options.add(new Option(e.text, e.value));
   e1.remove(i);
   i = i - 1;
   
  }
  document.myform.msgto.value=getvalue(document.myform.list2);
  
 }
 catch(e){
  
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
	  发送短信   
	  <!-- InstanceEndEditable -->  </div>
  <div id="main"> 
  <!-- InstanceBeginEditable name="EditRegion1" -->
<?php require_once('connecting.php'); ?>
<?php
	//php代码
	$content=$_POST["content"];
	$msgto = $_POST["msgto"];
	
	if( $content != '' && $msgto != '' ) {	
		$addtime = date("Y-m-d H:i:s");
		//号码处理
		$msg2 = trim( $msgto, ',');
		$msg2 = explode( ',', $msg2 );
		if( $msg2 ) {
			foreach( $msg2 as $var ) {
				$query = "insert into sendmessage(numbersend,timesend,contentsend,flagsend) 
								values ('$var','$addtime','$content','0')";
				mysql_query( $query );
			}
			echo "<script languge=javascript> alert('记录成功，等待发送！'); 
					location.href='send_box.php'</script>;";
		} else {
			echo "<script languge=javascript> alert('号码不对，请核对！'); 
					history.back(-1);</script>;";
		}
	}
?>
    <form  method="post" name="myform"  onsubmit="return checkdata();">
      <div style=" padding-bottom:20px;">
	  <table border="0" width="100%" style="">
        <tr>
          <td bgcolor="#3399CC"   width="13%"><select  style="width:100%;" multiple name="list1" size="15" ondblclick="moveOption1(document.myform.list1, document.myform.list2)">
<?php 
	$query = "select * from `number`";
	$result = mysql_query( $query );
	if( mysql_num_rows( $result ) < 1 )
	{
		echo "<option> &nbsp;号码为空!></option>>";
	}
	else
	{
			$totalnum = mysql_num_rows($result);
			
			for( $i=0;$i <=$totalnum;$i++ )
			{
				$info = mysql_fetch_array( $result );
				if($info != "")
				{
					echo "<option value=".$info['num']."><font color=blue ><b>".$info['name']."</b></font></option><br>";
				}
			}
	}//else
	mysql_close($member);

?>
            </select>
          </td>
          <td width="10%" align="center"><input type="button" value="添加" onClick="moveOption1(document.myform.list1, document.myform.list2)">
            <br>
            <br>
            <input type="button" value="全选" onClick="moveAllOption1(document.myform.list1, document.myform.list2)">
            <br>
            <br>
            <input type="button" value="删除" onClick="moveOption2(document.myform.list2, document.myform.list1)">
            <br>
            <br>
            <input type="button" value="全删" onClick="moveAllOption2(document.myform.list2, document.myform.list1)">
          </td>
          <td bgcolor="#3399CC" width="13%"><select style="width:100%;" multiple name="list2" size="15" ondblclick="moveOption2(document.myform.list2, document.myform.list1)">
            </select>
          </td>
          <td><table  rules="none"  align="center"  cellpadding="1" cellspacing="10" border="0" >
              <tr bgcolor="">
                <td height="23"  align="left" style="font-size:120%;"> 请将短信内容限制在70字以内。当前字数：
				  <span id="status1" style="width:30px;font-weight:bold;text-align:right; font-size:150%; font-family:Georgia, 'Times New Roman', Times, serif">
				  </span></td>
              </tr>
              <tr>
                <td>
				<textarea name="content"  style="width:350px; height:120px; font-size:150%;" data-maxsize="70" data-output="status1" wrap="virtual"></textarea>
				</td>
              </tr>
              <tr>
                <td  height="24"><div align="center">
                    <input type="submit" name="namesubmit" value="发送" />
                  </div></td>
              </tr>
            </table></td>
        </tr>
      </table>
	  </div>
      <input type="hidden" name="msgto" value="" />
    </form>
	
    <!-- InstanceEndEditable --> </div>
  <div id="footer"><br />
    &copy;2010 　Designed By <a href="http://hi.baidu.com/rjz78"><cite>Don Ren</cite><img src="images/stone.gif" alt="石三" style="margin-left:15px; border:none;" /></a><br />
    <br />
  </div>
</div>
</body>
<!-- InstanceEnd --></html>
