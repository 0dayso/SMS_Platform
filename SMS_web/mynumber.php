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
<link type="text/css" rel="stylesheet" href="style/common.css" />
<script language="javascript" src="lib/jquery.js"></script>
<script language="JavaScript" type="text/javascript">
<!--
function AjaxDelete( table, id )
{
//$.get()方式：
	//alert("get id:("+id+")");
	$.post(
	  'ajax_delete.php',
	  { table:table, MsgId:id },
	  function (data) //回传函数
	  {
		var myjson='';
	  	$('#tr_tag'+id).html("waiting..."); 
		eval('myjson=' + data + ';');
		//alert("myjson.flag is :("+myjson.flag+")");
		//alert("myjson.id is :("+myjson.id+")");
		if( myjson.flag!=0 && myjson.flag!=0 ) {
			$('#tr_tag'+id).remove();
		} else {
			alert("对不起，操作不成功！");
		}
		/*$('#result').html("姓名:" + myjson.flag + "<br/>工作:" + myjson.job);*/
	  }
	);
};


function p_del() {
	var msg = "您真的确定要删除电话薄全部信息吗？\n\n请确认！";
	if (confirm(msg)==true){
		return true;
	}
	else{
		return false;
	}
}
function p_del_personal() {
	var msg = "您真的确定要删除该条电话薄信息吗？\n\n请确认！";
	if (confirm(msg)==true){
		return true;
	}
	else{
		return false;
	}
}
function gotoDetail(username)
{
	newWin=window.open("detail.php?username=" + username,"aa","status=no,menubar=no,left=0,top=0,height=220,width=398");
	newWin.focus();
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
	  电话薄管理   
	  <!-- InstanceEndEditable -->  </div>
  <div id="main"> 
  <!-- InstanceBeginEditable name="EditRegion1" --> 
<?php require_once('connecting.php'); ?>
<?php
$cur_page_name = $_SERVER["PHP_SELF"];
$SqlTable = "number";

$current_page = ( isset($_GET['page']) ) ? $_GET['page'] : 1;

$start_limit = ( $current_page - 1 ) * $page_items;

$query = "select * from `$SqlTable` ORDER BY `idnum` DESC";
$result = mysql_query( $query ) or die(mysql_error());
$sum_rows_all = mysql_num_rows($result);

$tatal_pages = ceil( $sum_rows_all/$page_items );

?>
<?php
		//首先判断用户时候已经登录。因为只有登录用户才能访问此页面。
		//如果用户提交了删除留言的ID,在删除此留言
		$delid = $_GET["delid"];
		if($delid != "")
		{
			mysql_query("delete from `$SqlTable` where idnum = ".$delid);
			echo "<script languge=javascript> alert('删除成功！');
						location.href='mynumber.php'</script>";
		} 
?>
<?php
		//首先判断用户时候已经登录。因为只有登录用户才能访问此页面。
		//如果用户提交了删除留言的ID,在删除此留言
		$delete = $_GET["delete"];
		if($delete != "")
		{
			mysql_query("delete from `$SqlTable` where flag=0");
			echo "<script languge=javascript> alert('删除成功！'); 
						location.href='mynumber.php'</script>";
		} 
?>

<?php
		//清空表
		$table = trim($_GET["table"]);
		if($table != "") {
			mysql_query( "TRUNCATE TABLE  `$table`;" ) or die(mysql_error());
			echo "<script languge=javascript> alert('执行成功！'); 
						location.href='$cur_page_name'</script>";
		} 
?>

  <table class=i_table cellSpacing=1 cellPadding=4 width="100%" align=center>
              <TR>
                <TD class=head colSpan=4>电话薄管理</TD>
              </TR>
            <TBODY id=cate_a0>
              <TR class=c>
                <td align="center">姓名</td>
                <td align="center">号码</td>
                <td align="center">操作</td>
              </tr>


<?php
	$query = "select * from `$SqlTable` 
				ORDER BY `idnum` DESC 
				LIMIT $start_limit , $page_items ";
	$result = mysql_query( $query );
	$sum_rows = mysql_num_rows($result);
	if($sum_rows < 1) {
		echo "<tr class='b'><td align='center' colspan='3'>没有存储号码！</td></tr>";
	} else {
		while( $info = mysql_fetch_array( $result ) )
		{
			if($info != "") {
?>
			   <TR class=b onMouseOver="this.className='right_over';" 
  onmouseout="this.className='b';" id="tr_tag<?=$info['idnum']?>">
                  <td align="center"><?=$info['name']?></td>
                  <td align="center"><strong><?=$info['num']?></strong>
				  </td>
                  <td align="center">
				  <a href="send.php?number=<?=$info['num']?>">
						<span class=a_button>发送</span>
					</a>&nbsp;
				  <a href="modify.php?id=<?=$info['idnum']?>">
						<span class=a_button>修改</span>
					</a>&nbsp;
						<span class=a_button 
						onclick="AjaxDelete('<?=$SqlTable?>', '<?=$info['idnum']?>')">
						删除</span>
				</td>
                </tr>
<?php 
			}
		}
	}
?>
				<TR class=c>
                <td align="center" colspan="4">
				  <table border="0" cellSpacing=0 cellPadding=0 width="100%" align=center>
					  <tr class=c>
						<td width="15%"> 总共&nbsp;<font color="black"><b>
						  <?=$sum_rows_all?>
						  </b></font>&nbsp;条记录 </td>
						<td width="20%" align="left">
							<a href="<?=$cur_page_name?>?table=<?=$SqlTable?>" onclick="p_del()">
							<span class=a_button>清空</span>
							</a>
						</td>
						<td width="65%" align="right" style="padding-right:2em;">
						<a href="<?=$cur_page_name?>"> 首页</a>|
						  <?php if ($current_page > 1) {  ?>
						  <a href="?page=<?=($current_page - 1)?>">
						  <?php } ?>
						  上一页</a>|
						  <?php if ($current_page < $tatal_pages) { ?>
						  <a href="?page=<?=($current_page + 1)?>">
						  <?php }  ?>
						  下一页</a>|
						  <?php if ($current_page < $tatal_pages) {  ?>
						  <a href="?page=<?=$tatal_pages?>">
						  <?php }  ?>
						  尾页</a></td>
					  </tr>
					</table>
					</td>
              </TR>
            </TBODY>
          </table>
<?php
	mysql_close($member);
?>		
</table> 
    <br />
    <br />
    <form  method="get" name="myform" onsubmit="return checkdata();" action="add.php">
      <table width="100%" class=i_table cellSpacing=1 cellPadding=4 >
        <tr>
          <td colspan="3" class=head>添加联系人</td>
        </tr>
        <tr class=c>
          <td>姓名：
            <input type="text" name="name"  />
          </td>
          <td>号码：
            <input type="text"  name="content" >
          </td>
          <td><input type="submit" name="namesubmit" value="添加" /></td>
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
